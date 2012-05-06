#include "render.h"

sf::Sprite *sprite = new sf::Sprite[numSprites];
sf::Sprite spriteMinimap;
sf::Vector2f scaleMinimap;
int currentDepth;
sf::Vector2i currCorner;
bool isControlSelected;
int selectedControl;
int mapArray[mapHeight][mapWidth][mapDepth];

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
	spriteMinimap = *sprite;
	// Render window
	int xOffset = currCorner.x / textureDim;
//std::cout << xOffset << std::endl;
	int yOffset = currCorner.y / textureDim;
	for (int x = 0; x < mapPaneWidth; x++) //loop through the height of the map
	{
		for (int y = 0; y < mapPaneHeight; y++) //loop through the width of the map
		{
			spriteMinimap = sprite[mapArray[x + xOffset][y + yOffset][currentDepth]];
			
			sprite[mapArray[x + xOffset][y + yOffset][currentDepth]].setPosition(10.f + (textureDim * x - (currCorner.x % textureDim)), 
				30.f + (textureDim * y - (currCorner.y % textureDim)));
//std::cout << currCorner.x % textureDim << std::endl;
			window.draw(sprite[mapArray[x + xOffset][y + yOffset][currentDepth]]);

			spriteMinimap.setPosition((window.getSize().x - 10 - (textureDim * minimapScale) * mapPaneWidth) + 
				((textureDim * minimapScale) * x - ((currCorner.x % textureDim) * minimapScale)), 
				30.f + ((textureDim * minimapScale) * y - (currCorner.y % textureDim) * minimapScale));
			scaleMinimap = sprite[mapArray[x + xOffset][y + yOffset][currentDepth]].getScale();
			spriteMinimap.setScale(scaleMinimap.x * minimapScale, scaleMinimap.y * minimapScale);
			window.draw(spriteMinimap);
		}
	}

	// Create outline for controls
	sf::RectangleShape outline;
	outline.setSize(sf::Vector2f(textureDim,textureDim));
	outline.setOutlineColor(sf::Color::Black);
	outline.setOutlineThickness(1);
	outline.setFillColor(sf::Color::Transparent);

	// Draw controls
	for (int controls = 0; controls < 3; controls++)
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
	mapArray[newTile.x][newTile.y][currentDepth] = selectedControl;
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

void Render::scrollMap(sf::Event event)
{
//	switch (event.key.code)
//	{
//	case sf::Keyboard::Left:
//		// Pan map left
//		if(currCorner.x > 8)
//		currCorner.x = currCorner.x - 8;
//		break;
//	case sf::Keyboard::Right:
//		// Pan map right
//		if(currCorner.x < ((mapWidth * textureDim) - (mapPaneWidth * textureDim)) - 8)
//		currCorner.x = currCorner.x + 8;
//		break;
//	case sf::Keyboard::Up:
//		// Pan map up
//		if(currCorner.y > 8)
//		currCorner.y = currCorner.y - 8;
//		break;
//	case sf::Keyboard::Down:
//		// Pan map down
//		if(currCorner.y < ((mapHeight * textureDim) - (mapPaneHeight * textureDim)) - 8)
//		currCorner.y = currCorner.y - 8;
//		break;
//	}
}

void Render::panLeft()
{
	if(currCorner.x > 8)
		currCorner.x--;
}

void Render::panRight()
{
	if(currCorner.x < ((mapWidth * textureDim) - (mapPaneWidth * textureDim)))
		currCorner.x++;
}

void Render::panUp()
{
	if(currCorner.y > 0)
		currCorner.y--;
}

void Render::panDown()
{
	if(currCorner.y < ((mapHeight * textureDim) - (mapPaneHeight * textureDim)))
		currCorner.y++;
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