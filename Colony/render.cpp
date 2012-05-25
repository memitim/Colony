#include "render.h"

// Default map dimensions
int Render::mapHeight = 128;
int Render::mapWidth = 128;
int Render::mapDepth = 20;

// Tile properties:
// 0: Sprite assigned to tile
// 1: Current animation frame of sprite
int Render::tileProperties = 2;

// Default map panning speed
int Render::panSpeed = 16;

// Default minimap scale compared to main map
static float miniMapScale = 0.27f;

// Initialize viewports
sf::View Render::mainMapView;
sf::View Render::miniMapView;
sf::View Render::controlsView;

// Other static variable definitions

int Render::currentDepth = 0;
sf::Vector2i Render::currCorner(0, 0);
bool Render::isControlSelected = false;
int Render::selectedControl;
std::vector< std::vector <std::vector< std::vector< int > > > > Render::mapArray;
sf::RectangleShape Render::plainTile;
sf::RectangleShape Render::outlineTile;
sf::RectangleShape Render::hoverOutlineTile;
std::vector<std::vector <sf::Sprite> > Render::spriteTiles;

sf::Texture (*Render::texture)[2] = new sf::Texture[numTiles][numTileAnimations];

Render::Render()
{

}

Render::~Render()
{

}

// One-time setup of render variables
void Render::prepGraphics(Window & window)
{
	this->loadTextures();
	this->initMapArray();
	this->createTileOutline();
	// Set up main map view and viewport
	this->mainMapView.reset(sf::FloatRect((float)textureDim,
	(float)textureDim,
	(float)mapPaneWidth * (float)textureDim - (float)textureDim, 
	(float)mapPaneHeight * (float)textureDim - (float)textureDim));
	this->mainMapView.setViewport(sf::FloatRect((float)leftBuffer / (float)window.getSize().x, 
		(float)topBuffer / (float)window.getSize().y, 
		((float)mapPaneWidth * (float)textureDim) / (float)window.getSize().x, 
		((float)mapPaneHeight * (float)textureDim) / (float)window.getSize().y));

	// Set up minimap view and viewport
	this->miniMapView = mainMapView;
	this->miniMapView.setViewport(sf::FloatRect((((float)mapPaneWidth * (float)textureDim * 1.f) + (float)paneBufferX + (float)leftBuffer) / (float)window.getSize().x, 
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
void Render::createTileOutline()
{
	this->plainTile.setSize(sf::Vector2f(float(this->textureDim),float(this->textureDim)));
	this->outlineTile.setSize(sf::Vector2f(float(this->textureDim),float(this->textureDim)));
	this->outlineTile.setOutlineColor(sf::Color::Black);
	this->outlineTile.setOutlineThickness(1);
	this->outlineTile.setFillColor(sf::Color::Transparent);
	this->hoverOutlineTile = this->outlineTile;
	this->hoverOutlineTile.setOutlineColor(sf::Color(48,48,48,192));
}

// Load textures from files and create sprites
void Render::loadTextures()
{
	this->spriteTiles.resize(this->numTiles);
	for(int ns=0;ns<this->numTiles;++ns)
	{
		this->spriteTiles[ns].resize(this->numTileAnimations);
	}
	
	// Load the sprite image from a file
	std::string textureSource;
//	sf::Texture (*texture)[this->numTileAnimations] = new sf::Texture[this->numTiles][this->numTileAnimations];
	for(int a=0;a<this->numTiles;a++)
	{
		for(int b=0;b<this->numTileAnimations;++b)
		{
			textureSource = "texture" + (static_cast<std::ostringstream*>( &(std::ostringstream() << a + 1) )->str()) + "-" + (static_cast<std::ostringstream*>( &(std::ostringstream() << b) )->str()) +".png";
			if (!texture[a][b].loadFromFile(textureSource))
			{
				// Need error handler
				std::cout << "OH CRAP! Texture load issue!" << std::endl;
			}
			this->spriteTiles[a][b].setTexture(texture[a][b]);
		}
	}
}

// Map vector initialization
void Render::initMapArray()
{
	this->mapArray.resize(this->mapHeight);
	for(int mh=0;mh<mapHeight;++mh)
	{
		this->mapArray[mh].resize(this->mapWidth);
		for(int mw=0;mw<mapWidth;++mw)
		{
			this->mapArray[mh][mw].resize(this->mapDepth);
			for(int md=0;md<mapDepth;++md)
			{
				this->mapArray[mh][mw][md].resize(this->tileProperties);
				this->mapArray[mh][mw][md][1] = (rand()%2);
			}
		}
	}
}

// Write the screen elements to their respective views
void Render::drawMap(Window & window)
{
	static int frameCounter = 0;
	static sf::Clock clock;
	// Limit animations to <some> frames per second regardless of framerate
	// TODO: Nail down a consistent equation of coverting FPS to that constant on the end
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
	for (int x = 0 + (int)(this->currCorner.x / this->textureDim); x < this->mapPaneWidth + 2 + (int)(this->currCorner.x / this->textureDim); x++)
	{
		for (int y = 0 + (int)(this->currCorner.y / this->textureDim); y < this->mapPaneHeight + 2 + (int)(this->currCorner.y / this->textureDim); y++)
		{
			int currentAnimationTile = this->mapArray[x][y][this->currentDepth][1];
			this->spriteTiles[this->mapArray[x][y][this->currentDepth][0]][currentAnimationTile].setPosition(float(this->textureDim * x), 
				float(textureDim * y));
			window.setView(this->mainMapView);
			window.draw(this->spriteTiles[this->mapArray[x][y][this->currentDepth][0]][currentAnimationTile]);

			spriteTiles[this->mapArray[x][y][this->currentDepth][0]][0].setPosition(float(this->textureDim * x), 
				float(this->textureDim * y));
			window.setView(this->miniMapView);
			window.draw(this->spriteTiles[this->mapArray[x][y][this->currentDepth][0]][0]);

			// Toggle the tile animation if sufficient frames have passed
			if (animate == true)
			{
				// Rotate to the next animation frame for the tile
				if (currentAnimationTile == this->numTileAnimations - 1)
				{
					this->mapArray[x][y][this->currentDepth][1] = 0;
				}
				else
				{
					this->mapArray[x][y][this->currentDepth][1] = currentAnimationTile + 1;
				}
			}
		}
	}
	
	// Draw the hover tile
	window.setView(this->mainMapView);
	window.draw(this->hoverOutlineTile);
	
	// Draw the controls
	window.setView(this->controlsView);
	for (int controls = 0; controls < this->numTiles; controls++)
	{
		int row = controls / this->numTilesControls;
		this->spriteTiles[controls][0].setPosition(float(textureDim * controls - (this->numTilesControls * row * this->textureDim)), float(row * this->textureDim + row));
		this->outlineTile.setPosition(this->spriteTiles[controls][0].getPosition());
		window.draw(this->spriteTiles[controls][0]);
		window.draw(this->outlineTile);
	}

	window.setView(window.getDefaultView());
}

// Process left-clicks that reach the main screen
void Render::leftClickScreen(Window & window, sf::Vector2i mousePosition)
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
void Render::checkHover(Window & window, sf::Vector2i mousePosition)
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
void Render::setSelectedControl(Window & window, sf::Vector2i mousePosition)
{
	isControlSelected = true;
	window.setView(controlsView);
	int row = numTiles / numTilesControls;
	sf::Vector2f convertSelectedControl = window.convertCoords(sf::Vector2i(mousePosition.x, mousePosition.y));
	int select = (int)(convertSelectedControl.x / textureDim) + (numTilesControls * (int)(convertSelectedControl.y / textureDim));
	if(select < numTiles)
	{
		selectedControl = select;
		hoverOutlineTile = plainTile;
		hoverOutlineTile.setTexture(&texture[selectedControl][0]);
	}
	window.setView(window.getDefaultView());
}

// Process screen scrolling keypresses


// Set the sprite of the main map tile clicked to the control currently selected
void Render::setSelectedTile(Window & window, sf::Vector2i mousePosition)
{
	window.setView(mainMapView);
	sf::Vector2f convertSelectedTile = window.convertCoords(sf::Vector2i(mousePosition.x, mousePosition.y));
	this->setTile(sf::Vector2i(floor(convertSelectedTile.x / textureDim), floor(convertSelectedTile.y / textureDim)));
	window.setView(window.getDefaultView());
}

// Render all graphics
void Render::drawScreen(Window & window)
{
	window.clear();	
	drawMap(window); 
	window.display();
}