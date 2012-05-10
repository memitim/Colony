#ifndef RENDER_H
#define RENDER_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <iomanip>
#include <sstream>
#include <vector>

class Render
	{
		
	public:
		Render();
		~Render();

		static const int mapHeight = 64;
		static const int mapWidth = 64;
		static const int mapDepth = 10;

		static const int numTiles = 14;
		static const int numTileAnimations = 1;

		static const int mapPaneHeight = 35;
		static const int mapPaneWidth = 35;
		static const int textureDim = 32;
		static const int bufferTiles = 8;
		
		static int windowHeight;
		static int windowWidth;

		static std::vector< std::vector < sf::Sprite > > spriteTiles;
		static sf::Sprite spriteMinimap;
		static sf::Vector2f scaleMinimap;
		static int currentDepth;
		static sf::Vector2i currCorner;
		static bool isControlSelected;
		static int selectedControl;
		static std::vector< std::vector< std::vector< int > > > mapArray;
		
		static int panSpeed;
		static sf::RectangleShape outlineTile;
		static sf::View mainMapView;
		static sf::RectangleShape hoverOutlineTile;
		static double hRatio;
		static double vRatio;

		static void Render::prepGraphics(sf::RenderWindow & window);
		static void loadTextures(int numTiles);
		static void initMapArray();
		static void drawMap(sf::RenderWindow & window, int currentDepth);
		static void drawScreen(sf::RenderWindow & window);
		static void setTile(sf::Vector2i newTile) {mapArray[newTile.x + 1][newTile.y + 1][currentDepth] = Render::selectedControl;}
		static void changeDepth(sf::Event event) {if ((event.key.code == sf::Keyboard::LBracket) && (currentDepth > 0)) currentDepth--; else if ((event.key.code == sf::Keyboard::RBracket) && (currentDepth < mapDepth - 1)) currentDepth++;}
		static void setSelectedTile(sf::Vector2i mousePosition) {sf::Vector2i newTile((mousePosition.x - 15 + currCorner.x) / textureDim - (mousePosition.x / (mapPaneWidth * textureDim)), (mousePosition.y - 30 + currCorner.y + (mousePosition.y /320)) / textureDim); Render::setTile(newTile);}
		static void releaseSelectedControl(void) {if(isControlSelected == true) isControlSelected = false;}
		static void setSelectedControl(sf::Vector2i mousePosition);
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