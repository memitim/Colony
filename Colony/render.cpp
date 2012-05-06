#include "render.h"

sf::Sprite *sprite = new sf::Sprite[numSprites];
sf::Sprite spriteMinimap;
sf::Vector2f scaleMinimap;
int currentDepth;
sf::Vector2i currCorner;
bool isControlSelected;
int selectedControl;
int mapArray[mapHeight][mapWidth][mapDepth];
int panSpeed = 4;
int windowHeight = 1200;
int windowWidth = 1600;

Render::Render()
{

}

Render::~Render()
{

}

void Render::prepGraphics()
{
	Render::loadTextures(numSprites);
}

void Render::loadTextures(int numSprites)
{
	// Load the sprite image from a file
	
	std::string textureSource;
	sf::Texture *texture = new sf::Texture[numSprites];
	for(int a=0;a<numSprites;a++)
	{
		textureSource = "texture" + (static_cast<std::ostringstream*>( &(std::ostringstream() << a + 1) )->str()) + ".png";
		if (!texture[a].loadFromFile(textureSource))
		{
			// Need error handler
		}
		sprite[a].setTexture(texture[a]);
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
	sf::View mainMapView;
	mainMapView.reset(sf::FloatRect(32.f,32.f,mapPaneWidth * textureDim * 1.f - 32.f, mapPaneHeight * textureDim * 1.f - 32.f));
	mainMapView.setViewport(sf::FloatRect(10.f/ windowWidth, 
		30.f / windowHeight, 
		mapPaneWidth * textureDim * 1.f / windowWidth, 
		mapPaneHeight * textureDim * 1.f / windowHeight));

	sf::View miniMapView = mainMapView;
	miniMapView.setViewport(sf::FloatRect((window.getSize().x - 10.f - textureDim * miniMapScale * mapPaneWidth) / windowWidth, 
		30.f / windowHeight, 
		mapPaneWidth * textureDim * miniMapScale / windowWidth * 1.f, 
		mapPaneHeight * textureDim * miniMapScale / windowHeight * 1.f));
	
	// Render window
	int xOffset = currCorner.x / textureDim;
	int yOffset = currCorner.y / textureDim;
	for (int x = 0; x < mapPaneWidth + 2; x++) //loop through the height of the map
	{
		for (int y = 0; y < mapPaneHeight + 2; y++) //loop through the width of the map
		{
			sprite[mapArray[x + xOffset][y + yOffset][currentDepth]].setPosition((textureDim * x) + (-currCorner.x % textureDim), 
				(textureDim * y) + (-currCorner.y % textureDim));
			
			window.setView(mainMapView);
			window.draw(sprite[mapArray[x + xOffset][y + yOffset][currentDepth]]);

			window.setView(miniMapView);
			window.draw(sprite[mapArray[x + xOffset][y + yOffset][currentDepth]]);
		}
	}

	window.setView(window.getDefaultView());
	// Create outline for controls
	sf::RectangleShape outline;
	outline.setSize(sf::Vector2f(textureDim,textureDim));
	outline.setOutlineColor(sf::Color::Black);
	outline.setOutlineThickness(1);
	outline.setFillColor(sf::Color::Transparent);

	// Draw controls
	for (int controls = 0; controls < numSprites; controls++)
	{
		sprite[controls].setPosition(10.f + (textureDim * mapPaneWidth) + 30 + (textureDim * controls), controlY);
		outline.setPosition(10.f + (textureDim * mapPaneWidth) + 30 + (textureDim * controls), controlY);
		window.draw(sprite[controls]);
		window.draw(outline);
	}
}

void Render::drawScreen(sf::RenderWindow & window)
{
	window.clear();
	drawMap(window,currentDepth);
	window.display();
}

void Render::setTile(sf::Vector2i newTile)
{
	mapArray[newTile.x + 1][newTile.y + 1][currentDepth] = selectedControl;
}

void Render::changeDepth(sf::Event event)
{
	if ((event.key.code == sf::Keyboard::LBracket) && (currentDepth > 0))
	// Up one level
		currentDepth--;
	else if ((event.key.code == sf::Keyboard::RBracket) && (currentDepth < mapDepth - 1))
	// Down one level
		currentDepth++;
}

void Render::panLeft()
{
	if(currCorner.x > panSpeed)
		currCorner.x = currCorner.x - panSpeed;
}

void Render::panRight()
{
	if(currCorner.x < ((mapWidth * textureDim) - (mapPaneWidth * textureDim)) - panSpeed - 32)
		currCorner.x = currCorner.x + panSpeed;
}

void Render::panUp()
{
	if(currCorner.y > panSpeed)
		currCorner.y = currCorner.y - panSpeed;
}

void Render::panDown()
{
	if(currCorner.y < ((mapHeight * textureDim) - (mapPaneHeight * textureDim)) - panSpeed - 32)
		currCorner.y = currCorner.y + panSpeed;
}

void Render::setSelectedTile(sf::Vector2i mousePos)
{
	sf::Vector2i newTile((mousePos.x - 10 + currCorner.x) / textureDim, (mousePos.y - 30 + currCorner.y) / textureDim);
			Render::setTile(newTile);
}

void Render::releaseSelectedControl(void)
{
	if(isControlSelected == true)
	isControlSelected = false;
}

void Render::setSelectedControl(sf::Vector2i mousePos)
{
	isControlSelected = true;
	selectedControl = (mousePos.x - (10.f + (textureDim * mapPaneWidth) + 30)) / textureDim;
}

void Render::leftClickScreen(sf::Vector2i mousePos)
{
	if(mousePos.x > 10 + (textureDim * mapPaneWidth) + 30 && mousePos.x < 10 + (textureDim * mapPaneWidth) + 30 + (numSprites * textureDim)
			&& mousePos.y > controlY && mousePos.y < controlY + textureDim)
	{
		Render::setSelectedControl(mousePos);
	}

	// Place control on selected tile
	if(isControlSelected == true && mousePos.x > 10 && mousePos.x < 10 + (mapPaneWidth * 32)
		&& mousePos.y > 30 && mousePos.y < 30 + (mapPaneHeight * textureDim))
	{
		Render::setSelectedTile(mousePos);
	}
}