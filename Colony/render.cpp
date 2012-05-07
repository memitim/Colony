#include "render.h"

sf::Sprite *spriteTiles = new sf::Sprite[Render::numTiles];
int Render::panSpeed = 4;
static const float miniMapScale = 0.33f;
sf::Sprite Render::spriteMinimap;
sf::Vector2f Render::scaleMinimap;
int Render::currentDepth;
sf::Vector2i Render::currCorner(panSpeed, panSpeed);
bool Render::isControlSelected;
int Render::selectedControl;
int Render::mapArray[mapHeight][mapWidth][mapDepth];
sf::RectangleShape Render::outlineTile;
sf::View Render::mainMapView;
sf::RectangleShape Render::hoverOutlineTile;

Render::Render()
{

}

Render::~Render()
{

}

void Render::prepGraphics()
{
	Render::loadTextures(numTiles);
	Render::createTileOutline();
}

void Render::loadTextures(int numTiles)
{
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
		spriteTiles[a].setTexture(texture[a]);
	}
}

void Render::initMapArray()
{
	for(int md=0;md<mapDepth;md++)
		for(int mw=0;mw<mapDepth;mw++)
			for(int mh=0;mh<mapDepth;mh++)
				mapArray[mh][mw][md] = 0;
}

void Render::drawMap(sf::RenderWindow & window, int currentDepth)
{
	mainMapView.reset(sf::FloatRect(32.f,
		32.f,
		mapPaneWidth * textureDim * 1.f - 32.f, 
		mapPaneHeight * textureDim * 1.f - 32.f));

	sf::View miniMapView = mainMapView;
	mainMapView.setViewport(sf::FloatRect(10.f / windowWidth, 
		30.f / windowHeight, 
		(mapPaneWidth * textureDim * 1.f) / windowWidth, 
		(mapPaneHeight * textureDim * 1.f) / windowHeight));

	miniMapView.setViewport(sf::FloatRect(((mapPaneWidth * textureDim * 1.f) + 20.f) / windowWidth, 
		30.f / windowHeight, 
		mapPaneWidth * textureDim * miniMapScale / windowWidth * 1.f, 
		mapPaneHeight * textureDim * miniMapScale / windowHeight * 1.f));
	
	sf::Vector2f test((mapPaneWidth * textureDim * 1.000f), ((mapPaneWidth * textureDim - 64) * 1.000f));
	;
	
	// Draw maps
	int xOffset = currCorner.x / textureDim;
	int yOffset = currCorner.y / textureDim;
	for (int x = 0; x < mapPaneWidth + 2; x++) //loop through the height of the map
	{
		for (int y = 0; y < mapPaneHeight + 2; y++) //loop through the width of the map
		{
			spriteTiles[mapArray[x + xOffset][y + yOffset][currentDepth]].setPosition((textureDim * x) + (-currCorner.x % textureDim) + panSpeed, 
				(textureDim * y) + (-currCorner.y % textureDim));
//			std::cout << ((mainMapView.getCenter().x - (x * textureDim) + (-currCorner.x % textureDim)) / mainMapView.getCenter().x) * textureDim << std::endl;
			window.setView(mainMapView);
			window.draw(spriteTiles[mapArray[x + xOffset][y + yOffset][currentDepth]]);

			window.setView(miniMapView);
			window.draw(spriteTiles[mapArray[x + xOffset][y + yOffset][currentDepth]]);
		}
	}
	window.setView(mainMapView);
	window.draw(hoverOutlineTile);
	window.setView(window.getDefaultView());

	// Draw controls
	sf::View controlsView;
	controlsView.reset(sf::FloatRect(0.f,0.f, windowWidth - (mapPaneWidth * textureDim * 1.f - 20), 
		windowHeight - (mapPaneHeight * miniMapScale * textureDim * 1.f + 20)));

	controlsView.setViewport(sf::FloatRect(((mapPaneWidth * textureDim * 1.f) + 20.f) / windowWidth, 
		(mapPaneHeight * textureDim * miniMapScale * 1.f + 40.f) / windowHeight, 
		1.f - ((mapPaneWidth * textureDim * 1.f - 30) / windowWidth * 1.f), 
		1.f - ((mapPaneHeight * miniMapScale * textureDim * 1.f - 50) / windowHeight * 1.f)));
	
	for (int controls = 0; controls < numTiles; controls++)
	{
		spriteTiles[controls].setPosition(textureDim * controls, 0.f);
		outlineTile.setPosition(textureDim * controls, 0.f);
		window.setView(controlsView);
		window.draw(spriteTiles[controls]);
		window.draw(outlineTile);
	}

	window.setView(window.getDefaultView());
}

void Render::drawScreen(sf::RenderWindow & window)
{
	window.clear();	
	drawMap(window,currentDepth); 
	window.display();
}

void Render::leftClickScreen(sf::Vector2i mousePosition)
{
	if(mousePosition.x > (mapPaneWidth * textureDim * 1.f) + 20.f 
		&& mousePosition.x < ((mapPaneWidth * textureDim * 1.f) + 20.f) + (numTiles * textureDim)
		&& mousePosition.y > mapPaneHeight * textureDim * miniMapScale * 1.f + 40.f 
		&& mousePosition.y < (mapPaneHeight * textureDim * miniMapScale * 1.f + 40.f) + textureDim)
	{
		Render::setSelectedControl(mousePosition);
	}

	// Place control on selected tile
	if(isControlSelected == true && mousePosition.x > 10 && mousePosition.x < 10 + (mapPaneWidth * 32)
		&& mousePosition.y > 30 && mousePosition.y < 30 + (mapPaneHeight * textureDim))
	{
		Render::setSelectedTile(mousePosition);
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
	if(mousePosition.x > 10 && mousePosition.x < 10 + (mapPaneWidth * 32)
		&& mousePosition.y > 30 && mousePosition.y < 30 + (mapPaneHeight * textureDim))
	{
		hoverOutlineTile.setPosition((textureDim * (mousePosition.x / textureDim)) + (-currCorner.x % textureDim) + panSpeed, 
			(textureDim * (mousePosition.y / textureDim)) + (-currCorner.y % textureDim) * 1.f);
	}
	else
		hoverOutlineTile.setPosition (0.f,0.f);
}