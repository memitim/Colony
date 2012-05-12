#include "render.h"

// Default map dimensions
int Render::mapHeight = 128;
int Render::mapWidth = 128;
int Render::mapDepth = 20;
// Tile properties:
// 0: Sprite assigned to tile
// 1: Current animation frame of sprite
int Render::tileProperties = 2;

// Default window dimensions
int Render::windowHeight = 1200;
int Render::windowWidth = 1600;

// Default map panning speed
int Render::panSpeed = 16;

// Default minimap scale compared to main map
static float miniMapScale = 0.27f;

// Default buffer between viewports
int Render::paneBufferX = 20;
int Render::paneBufferY = 20;
int Render::leftBuffer = 15;
int Render::topBuffer = 30;

// Initialize viewports
sf::View Render::mainMapView;
sf::View Render::miniMapView;
sf::View Render::controlsView;

sf::Clock Render::clock;
sf::Time Render::elapsedTime;
int Render::frameCounter;

// Other static variable definitions
sf::Sprite Render::spriteMinimap;
sf::Vector2f Render::scaleMinimap;
int Render::currentDepth;
sf::Vector2i Render::currCorner(0, 0);
bool Render::isControlSelected;
int Render::selectedControl;
std::vector< std::vector <std::vector< std::vector< int > > > > Render::mapArray;
sf::RectangleShape Render::outlineTile;
sf::RectangleShape Render::hoverOutlineTile;
std::vector<std::vector <sf::Sprite> > Render::spriteTiles;

Render::Render()
{

}

Render::~Render()
{

}

// One-time setup of render variables
void Render::prepGraphics(sf::RenderWindow & window)
{
	Render::loadTextures(numTiles);
	Render::initMapArray();
	Render::createTileOutline();
	
	// Set up main map view and viewport
	mainMapView.reset(sf::FloatRect((float)textureDim,
	(float)textureDim,
	(float)mapPaneWidth * (float)textureDim - (float)textureDim, 
	(float)mapPaneHeight * (float)textureDim - (float)textureDim));
	mainMapView.setViewport(sf::FloatRect((float)leftBuffer / (float)window.getSize().x, 
		(float)topBuffer / (float)window.getSize().y, 
		((float)mapPaneWidth * (float)textureDim) / (float)window.getSize().x, 
		((float)mapPaneHeight * (float)textureDim) / (float)window.getSize().y));

	// Set up minimap view and viewport
	miniMapView = mainMapView;
	miniMapView.setViewport(sf::FloatRect((((float)mapPaneWidth * (float)textureDim * 1.f) + (float)paneBufferX + (float)leftBuffer) / (float)window.getSize().x, 
		(float)topBuffer / (float)window.getSize().y, 
		(float)mapPaneWidth * (float)textureDim * miniMapScale / (float)window.getSize().x * 1.f, 
		(float)mapPaneHeight * (float)textureDim * miniMapScale / (float)window.getSize().y * 1.f));

	// Set up controls view and viewport
	controlsView.reset(sf::FloatRect(0.f,0.f, (float)numTilesControls * (float)textureDim, 
		(floor((float)numTiles / (float)numTilesControls + 1.f) * (float)textureDim) * 1.00f));
	controlsView.setViewport(sf::FloatRect((((float)mapPaneWidth * (float)textureDim * 1.f) + (float)paneBufferX + (float)leftBuffer) / (float)window.getSize().x, 
		((float)mapPaneHeight * (float)textureDim * miniMapScale * 1.f + (float)topBuffer + (float)paneBufferY) / (float)window.getSize().y, 
		((float)numTilesControls * (float)textureDim) / (float)window.getSize().x * 1.f, 
		(floor((float)numTiles / (float)numTilesControls + 1.f) * (float)textureDim) * 1.00f / (float)window.getSize().y));
}

// Create the outlines used for tiles
void Render::createTileOutline(void)
{
	outlineTile.setSize(sf::Vector2f(textureDim,textureDim));
	outlineTile.setOutlineColor(sf::Color::Black);
	outlineTile.setOutlineThickness(1);
	outlineTile.setFillColor(sf::Color::Transparent);
	hoverOutlineTile = outlineTile;
	hoverOutlineTile.setOutlineColor(sf::Color(96,96,96,128));
}

// Load textures from files and create sprites
void Render::loadTextures(int numTiles)
{
	spriteTiles.resize(numTiles);
	for(int ns=0;ns<numTiles;++ns)
	{
		spriteTiles[ns].resize(numTileAnimations);
	}
	
	// Load the sprite image from a file
	std::string textureSource;
	sf::Texture (*texture)[numTileAnimations] = new sf::Texture[numTiles][numTileAnimations];
	for(int a=0;a<numTiles;a++)
	{
		for(int b=0;b<numTileAnimations;++b)
		{
			textureSource = "texture" + (static_cast<std::ostringstream*>( &(std::ostringstream() << a + 1) )->str()) + "-" + (static_cast<std::ostringstream*>( &(std::ostringstream() << b) )->str()) +".png";
			std::cout << textureSource << std::endl;
			if (!texture[a][b].loadFromFile(textureSource))
			{
				// Need error handler
				std::cout << "OH CRAP! Texture load issue!" << std::endl;
			}
			spriteTiles[a][b].setTexture(texture[a][b]);
		}
	}
}

// Map vector initialization
void Render::initMapArray()
{
	mapArray.resize(mapHeight);
	for(int mh=0;mh<mapHeight;++mh)
	{
		mapArray[mh].resize(mapWidth);
		for(int mw=0;mw<mapWidth;++mw)
		{
			mapArray[mh][mw].resize(mapDepth);
			for(int md=0;md<mapDepth;++md)
			{
				mapArray[mh][mw][md].resize(tileProperties);
				mapArray[mh][mw][md][1] = (rand()%2);
//				std::cout << mapArray[mh][mw][md][1] << std::endl;
			}
		}
	}
}

// Write the screen elements to their respective views
void Render::drawMap(sf::RenderWindow & window)
{
	// Limit animations to two frames per second regardless of framerate
	bool animate = false;
	if(frameCounter < 1.f / clock.getElapsedTime().asMilliseconds() * 1000 / 0.2f)
	{
		frameCounter++;
	}
	else
	{
		animate = true;
		clock.restart();
		frameCounter = 0;
	}

	// Draw the main map and minimap
	for (int x = 0 + (int)(currCorner.x / textureDim); x < mapPaneWidth + 2 + (int)(currCorner.x / textureDim); x++)
	{
		for (int y = 0 + (int)(currCorner.y / textureDim); y < mapPaneHeight + 2 + (int)(currCorner.y / textureDim); y++)
		{
			int currentAnimationTile = mapArray[x][y][currentDepth][1];
			spriteTiles[mapArray[x][y][currentDepth][0]][currentAnimationTile].setPosition(textureDim * x, 
				textureDim * y);
			window.setView(mainMapView);
			window.draw(spriteTiles[mapArray[x][y][currentDepth][0]][currentAnimationTile]);

			spriteTiles[mapArray[x][y][currentDepth][0]][0].setPosition(textureDim * x, 
				textureDim * y);
			window.setView(miniMapView);
			window.draw(spriteTiles[mapArray[x][y][currentDepth][0]][0]);

			// Toggle the tile animation if sufficient frames have passed
			if (animate == true)
			{
				// Rotate to the next animation frame for the tile
				if (currentAnimationTile == numTileAnimations - 1)
				{
					mapArray[x][y][currentDepth][1] = 0;
				}
				else
				{
					mapArray[x][y][currentDepth][1] = currentAnimationTile + 1;
				}
			}
		}
	}
	
	// Draw the hover tile
	window.setView(mainMapView);
	window.draw(hoverOutlineTile);
	
	// Draw the controls
	window.setView(controlsView);
	for (int controls = 0; controls < numTiles; controls++)
	{
		int row = controls / numTilesControls;
		spriteTiles[controls][0].setPosition(textureDim * controls - (numTilesControls * row * textureDim), row * textureDim + row);
		outlineTile.setPosition(spriteTiles[controls][0].getPosition());
		window.draw(spriteTiles[controls][0]);
		window.draw(outlineTile);
	}

	window.setView(window.getDefaultView());
}

// Render all graphics
void Render::drawScreen(sf::RenderWindow & window)
{
	window.clear();	
	drawMap(window); 
	window.display();
}

// Process left-clicks that reach the main screen
void Render::leftClickScreen(sf::RenderWindow & window, sf::Vector2i mousePosition)
{
	// Test for clicks in the controls viewport
	if(mousePosition.x > controlsView.getViewport().left * window.getSize().x
		&& mousePosition.x < (controlsView.getViewport().left + controlsView.getViewport().width) * window.getSize().x
		&& mousePosition.y > controlsView.getViewport().top * window.getSize().y
		&& mousePosition.y < (controlsView.getViewport().top + controlsView.getViewport().height) * window.getSize().y)
	{
		Render::setSelectedControl(window, mousePosition);
	}
	
	// Test for clicks in the main map and if a control is selected, then place control on selected tile
	if(isControlSelected == true && mousePosition.x > leftBuffer && mousePosition.x < leftBuffer + (mapPaneWidth * textureDim)
		&& mousePosition.y > topBuffer && mousePosition.y < topBuffer + (mapPaneHeight * textureDim))
	{
		Render::setSelectedTile(window, mousePosition);
	}
}

// Set the location of the hover outline
void Render::checkHover(sf::RenderWindow & window, sf::Vector2i mousePosition)
{
	if(mousePosition.x > leftBuffer && mousePosition.x < leftBuffer + (mapPaneWidth * textureDim)
		&& mousePosition.y > topBuffer && mousePosition.y < topBuffer + (mapPaneHeight * textureDim))
	{
		window.setView(mainMapView);
		sf::Vector2f convertHoverTile = window.convertCoords(sf::Vector2i(mousePosition.x, mousePosition.y));	
		hoverOutlineTile.setPosition(textureDim * floor(convertHoverTile.x / (float)textureDim), textureDim * floor(convertHoverTile.y / (float)textureDim));
		window.setView(window.getDefaultView());
	}
	else
		hoverOutlineTile.setPosition (0.f,0.f);
}

// Set the chosen control if control viewport clicked
void Render::setSelectedControl(sf::RenderWindow & window, sf::Vector2i mousePosition)
{
	isControlSelected = true;
	window.setView(controlsView);
	int row = numTiles / numTilesControls;
	sf::Vector2f convertSelectedControl = window.convertCoords(sf::Vector2i(mousePosition.x, mousePosition.y));
	int select = (int)(convertSelectedControl.x / textureDim) + (numTilesControls * (int)(convertSelectedControl.y / textureDim));
	if(select < numTiles)
		Render::selectedControl = select;
	window.setView(window.getDefaultView());
}

// Process screen scrolling keypresses
void Render::panLeft()
{
	if(currCorner.x > 0)
	{
		currCorner.x = currCorner.x - panSpeed; 
		mainMapView.move(-panSpeed,0);
		miniMapView.move(-panSpeed,0);
	}
}
void Render::panRight() 
{
	if(currCorner.x < ((mapWidth * textureDim) - (mapPaneWidth * textureDim)) - panSpeed - textureDim)
	{
		currCorner.x = currCorner.x + panSpeed; 
		mainMapView.move(panSpeed,0);
		miniMapView.move(panSpeed,0);
	}
}
void Render::panUp() 
{
	if(currCorner.y > 0)	
	{
		currCorner.y = currCorner.y - panSpeed; 
		mainMapView.move(0,-panSpeed);
		miniMapView.move(0,-panSpeed);
	}
}
void Render::panDown() 
{
	if(currCorner.y < ((mapHeight * textureDim) - (mapPaneHeight * textureDim)) - panSpeed - textureDim)
	{
		currCorner.y = currCorner.y + panSpeed; 
		mainMapView.move(0,panSpeed);
		miniMapView.move(0,panSpeed);
	}
}

// Set the sprite of the main map tile clicked to the control currently selected
void Render::setSelectedTile(sf::RenderWindow & window, sf::Vector2i mousePosition)
{
	window.setView(mainMapView);
	sf::Vector2f convertSelectedTile = window.convertCoords(sf::Vector2i(mousePosition.x, mousePosition.y));
	Render::setTile(sf::Vector2i(floor(convertSelectedTile.x / (float)textureDim), floor(convertSelectedTile.y / (float)textureDim)));
	window.setView(window.getDefaultView());
}