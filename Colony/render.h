#ifndef RENDER_H
#define RENDER_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <iomanip>
#include <sstream>

class Render
	{
		
	public:
		Render();
		~Render();

		static const int mapHeight = 64;
		static const int mapWidth = 64;
		static const int mapDepth = 10;

		static const int numTiles = 6;

		static const int mapPaneHeight = 35;
		static const int mapPaneWidth = 37;
		static const int textureDim = 32;
		static const int bufferTiles = 8;
		static const int windowHeight = 1200;
		static const int windowWidth = 1600;

		static sf::Sprite spriteMinimap;
		static sf::Vector2f scaleMinimap;
		static int currentDepth;
		static sf::Vector2i currCorner;
		static bool isControlSelected;
		static int selectedControl;
		static int mapArray[mapHeight][mapWidth][mapDepth];
		static int panSpeed;
		static sf::RectangleShape outlineTile;
		static sf::View mainMapView;
		static sf::RectangleShape hoverOutlineTile;

		static void Render::prepGraphics(void);
		static void loadTextures(int numTiles);
		static void initMapArray();
		static void drawMap(sf::RenderWindow & window, int currentDepth);
		static void drawScreen(sf::RenderWindow & window);
		static void setTile(sf::Vector2i newTile) {mapArray[newTile.x + 1][newTile.y + 1][currentDepth] = Render::selectedControl;}
		static void changeDepth(sf::Event event) {if ((event.key.code == sf::Keyboard::LBracket) && (currentDepth > 0)) currentDepth--; else if ((event.key.code == sf::Keyboard::RBracket) && (currentDepth < mapDepth - 1)) currentDepth++;}
		static void setSelectedTile(sf::Vector2i mousePosition) {sf::Vector2i newTile((mousePosition.x - 10 + currCorner.x) / textureDim - 1, (mousePosition.y - 30 + currCorner.y) / textureDim);	Render::setTile(newTile);}
		static void releaseSelectedControl(void) {if(isControlSelected == true) isControlSelected = false;}
		static void setSelectedControl(sf::Vector2i mousePosition) {isControlSelected = true; Render::selectedControl = (mousePosition.x - ((mapPaneWidth * textureDim * 1.f) + 20.f)) / textureDim;}
		static void leftClickScreen(sf::Vector2i mousePosition);
		static void Render::checkHover(sf::RenderWindow & window, sf::Vector2i mousePosition);
		static void Render::panLeft(void) {if(currCorner.x > panSpeed)	currCorner.x = currCorner.x - panSpeed;}
		static void Render::panRight(void) {if(currCorner.x < ((mapWidth * textureDim) - (mapPaneWidth * textureDim)) - panSpeed - 32) currCorner.x = currCorner.x + panSpeed;}
		static void Render::panUp(void) {if(currCorner.y > panSpeed)	currCorner.y = currCorner.y - panSpeed;}
		static void Render::panDown(void) {if(currCorner.y < ((mapHeight * textureDim) - (mapPaneHeight * textureDim)) - panSpeed - 32) currCorner.y = currCorner.y + panSpeed;}
		static void Render::createTileOutline(void);
		
	private:

	protected:

	};

#endif