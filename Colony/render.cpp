#include "render.h"

int Render::windowHeight = 1200;
int Render::windowWidth = 1600;
std::vector<std::vector <sf::Sprite> > Render::spriteTiles;
int Render::panSpeed = 4;
static float miniMapScale = 0.33f;
sf::Sprite Render::spriteMinimap;
sf::Vector2f Render::scaleMinimap;
int Render::currentDepth;
sf::Vector2i Render::currCorner(0, 0);
bool Render::isControlSelected;
int Render::selectedControl;
std::vector< std::vector< std::vector< int > > > Render::mapArray;
sf::RectangleShape Render::outlineTile;
sf::View Render::mainMapView;
sf::View Render::miniMapView;
sf::View Render::controlsView;
sf::RectangleShape Render::hoverOutlineTile;
double Render::hRatio;
double Render::vRatio;

Render::Render()
{

}

Render::~Render()
{

}

void Render::prepGraphics(sf::RenderWindow & window)
{
	Render::loadTextures(numTiles);
	Render::initMapArray();
	Render::createTileOutline();
	Render::hRatio = (mapPaneWidth * textureDim * 1.f) / window.getSize().x;
	Render::vRatio = (mapPaneHeight * textureDim * 1.f) / window.getSize().y;
	
	mainMapView.reset(sf::FloatRect(32.f,
	32.f,
	mapPaneWidth * textureDim * 1.f - 32.f, 
	mapPaneHeight * textureDim * 1.f - 32.f));

	mainMapView.setViewport(sf::FloatRect(15.f / window.getSize().x, 
		30.f / window.getSize().y, 
		hRatio, 
		vRatio));
	miniMapView = mainMapView;
	miniMapView.setViewport(sf::FloatRect(((mapPaneWidth * textureDim * 1.f) + 30.f) / window.getSize().x, 
		30.f / window.getSize().y, 
		mapPaneWidth * textureDim * miniMapScale / window.getSize().x * 1.f, 
		mapPaneHeight * textureDim * miniMapScale / window.getSize().y * 1.f));

		controlsView.reset(sf::FloatRect(0.f,0.f, window.getSize().x - (mapPaneWidth * textureDim * 1.f - 20), 
		window.getSize().y - (mapPaneHeight * miniMapScale * textureDim * 1.f + 20)));

	controlsView.setViewport(sf::FloatRect(((mapPaneWidth * textureDim * 1.f) + 30.f) / window.getSize().x, 
		(mapPaneHeight * textureDim * miniMapScale * 1.f + 50.f) / window.getSize().y, 
		1.f - ((mapPaneWidth * textureDim * 1.f - 30) / window.getSize().x * 1.f), 
		1.f - ((mapPaneHeight * miniMapScale * textureDim * 1.f - 50) / window.getSize().y * 1.f)));
}

void Render::loadTextures(int numTiles)
{
	spriteTiles.resize(numTiles);
	for(int ns=0;ns<numTiles;++ns)
	{
		spriteTiles[ns].resize(numTileAnimations);
	}
	
	// Load the sprite image from a file
	std::string textureSource;
	sf::Texture *texture = new sf::Texture[numTiles];
	for(int a=0;a<numTiles;a++)
	{
		textureSource = "texture" + (static_cast<std::ostringstream*>( &(std::ostringstream() << a + 1) )->str()) + ".png";
		if (!texture[a].loadFromFile(textureSource))
		{
			// Need error handler
		}
		spriteTiles[a][0].setTexture(texture[a]);
	}
}

void Render::initMapArray()
{
	mapArray.resize(mapHeight);
	for(int mh=0;mh<mapHeight;++mh)
	{
		mapArray[mh].resize(mapWidth);
		for(int mw=0;mw<mapWidth;++mw)
			mapArray[mh][mw].resize(mapDepth);
	}
}

void Render::drawMap(sf::RenderWindow & window)
{
	for (int x = 0 + (int)(currCorner.x / textureDim); x < mapPaneWidth + 2 + (int)(currCorner.x / textureDim); x++)
	{
		for (int y = 0 + (int)(currCorner.y / textureDim); y < mapPaneHeight + 2 + (int)(currCorner.y / textureDim); y++)
		{
			spriteTiles[mapArray[x][y][currentDepth]][0].setPosition(textureDim * x, 
				textureDim * y);

			window.setView(mainMapView);
			window.draw(spriteTiles[mapArray[x][y][currentDepth]][0]);

			window.setView(miniMapView);
			window.draw(spriteTiles[mapArray[x][y][currentDepth]][0]);
		}
	}
	window.setView(mainMapView);
	window.draw(hoverOutlineTile);
	window.setView(controlsView);
	// Draw controls
	for (int controls = 0; controls < numTiles; controls++)
	{
		int row = controls / 12;
		spriteTiles[controls][0].setPosition(textureDim * controls - (12 * row * textureDim), row * textureDim + row);
		outlineTile.setPosition(spriteTiles[controls][0].getPosition());
		window.draw(spriteTiles[controls][0]);
		window.draw(outlineTile);
	}

	window.setView(window.getDefaultView());
}

void Render::drawScreen(sf::RenderWindow & window)
{
	window.clear();	
	drawMap(window); 
	window.display();
}

void Render::leftClickScreen(sf::RenderWindow & window, sf::Vector2i mousePosition)
{
	if(mousePosition.x > (mapPaneWidth * textureDim * 1.f) + 25.f 
		&& mousePosition.x < ((mapPaneWidth * textureDim * 1.f) + 25.f) + (12 * textureDim)
		&& mousePosition.y > mapPaneHeight * textureDim * miniMapScale * 1.f + 50.f 
		&& mousePosition.y < (mapPaneHeight * textureDim * miniMapScale * 1.f + 50.f) + textureDim + (textureDim * (int)(numTiles / 12)))
	{
		Render::setSelectedControl(window, mousePosition);
	}

	// Place control on selected tile
	if(isControlSelected == true && mousePosition.x > 15 && mousePosition.x < 15 + (mapPaneWidth * 32)
		&& mousePosition.y > 30 && mousePosition.y < 30 + (mapPaneHeight * textureDim))
	{
		Render::setSelectedTile(window, mousePosition);
	}
}

void Render::createTileOutline(void)
{
	outlineTile.setSize(sf::Vector2f(textureDim,textureDim));
	outlineTile.setOutlineColor(sf::Color::Black);
	outlineTile.setOutlineThickness(1);
	outlineTile.setFillColor(sf::Color::Transparent);
	hoverOutlineTile = outlineTile;
	hoverOutlineTile.setOutlineColor(sf::Color(48,48,48,128));
}

void Render::checkHover(sf::RenderWindow & window, sf::Vector2i mousePosition)
{
	if(mousePosition.x > 15 && mousePosition.x < 15 + (mapPaneWidth * textureDim)
		&& mousePosition.y > 30 && mousePosition.y < 30 + (mapPaneHeight * textureDim))
	{
		window.setView(mainMapView);
		float roundX = (currCorner.x % textureDim) - 0.5f;
		float roundY = (currCorner.y % textureDim) - 0.5f;
		sf::Vector2f convertHoverTile = window.convertCoords(sf::Vector2i(mousePosition.x, mousePosition.y));	
		hoverOutlineTile.setPosition(textureDim * (int)(convertHoverTile.x / textureDim), textureDim * (int)(convertHoverTile.y / textureDim));
		window.setView(window.getDefaultView());
	}
	else
		hoverOutlineTile.setPosition (0.f,0.f);
}

void Render::setSelectedControl(sf::RenderWindow & window, sf::Vector2i mousePosition)
{
	isControlSelected = true;
	window.setView(controlsView);
	int row = numTiles / 12;
	sf::Vector2f convertSelectedControl = window.convertCoords(sf::Vector2i(mousePosition.x, mousePosition.y));
	int select = (int)(convertSelectedControl.x / textureDim) + (12 * (int)(convertSelectedControl.y / textureDim));
	if(select < numTiles)
		Render::selectedControl = select;
	window.setView(window.getDefaultView());
}

void Render::panLeft(void)
{
	if(currCorner.x > 0)
	{
		currCorner.x = currCorner.x - panSpeed; 
		mainMapView.move(-panSpeed,0);
		miniMapView.move(-panSpeed,0);
	}
}

void Render::panRight(void) 
{
	if(currCorner.x < ((mapWidth * textureDim) - (mapPaneWidth * textureDim)) - panSpeed - 32)
	{
		currCorner.x = currCorner.x + panSpeed; 
		mainMapView.move(panSpeed,0);
		miniMapView.move(panSpeed,0);
	}
}

void Render::panUp(void) 
{
	if(currCorner.y > 0)	
	{
		currCorner.y = currCorner.y - panSpeed; 
		mainMapView.move(0,-panSpeed);
		miniMapView.move(0,-panSpeed);
	}
}

void Render::panDown(void) 
{
	if(currCorner.y < ((mapHeight * textureDim) - (mapPaneHeight * textureDim)) - panSpeed - 32)
	{
		currCorner.y = currCorner.y + panSpeed; 
		mainMapView.move(0,panSpeed);
		miniMapView.move(0,panSpeed);
	}
}

void Render::setSelectedTile(sf::RenderWindow & window, sf::Vector2i mousePosition)
{
	window.setView(mainMapView);
	sf::Vector2f convertSelectedTile = window.convertCoords(sf::Vector2i(mousePosition.x, mousePosition.y));
	Render::setTile(sf::Vector2i((int)(convertSelectedTile.x / textureDim), (int)(convertSelectedTile.y / textureDim)));
	window.setView(window.getDefaultView());
}