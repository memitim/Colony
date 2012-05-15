#ifndef RENDER_H
#define RENDER_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <iomanip>
#include <sstream>
#include <vector>
#include "window.h"

class Render
	{
		
	public:
		Render();
		~Render();

		void prepGraphics(Window & window);
		
		static void leftClickScreen(Window & window, sf::Vector2i mousePosition);
		static void changeDepth(sf::Event event) {if ((event.key.code == sf::Keyboard::LBracket) && (currentDepth > 0)) currentDepth--; else if ((event.key.code == sf::Keyboard::RBracket) && (currentDepth < mapDepth - 1)) currentDepth++;}
		static void setSelectedControl(Window & window, sf::Vector2i mousePosition);
		static void checkHover(Window & window, sf::Vector2i mousePosition);
		static void panLeft();
		static void panRight();
		static void panUp();
		static void panDown();
		static void releaseSelectedControl() {if(isControlSelected == true) isControlSelected = false;}
		static void drawMap(Window & window);
		
	private:

		static const int numTiles = 14;
		static const int numTileAnimations = 2;
		static const int numTilesControls = 10;

		static const int mapPaneHeight = 35;
		static const int mapPaneWidth = 38;
		static const int textureDim = 32;
		
		static int windowHeight;
		static int windowWidth;

		// Dimensions of the map
		static int mapHeight;
		static int mapWidth;
		static int mapDepth;
		static int tileProperties;

		// Pixels between viewports
		static int paneBufferX;
		static int paneBufferY;
		static int leftBuffer;
		static int topBuffer;

		// Viewports
		static sf::View mainMapView;
		static sf::View miniMapView;
		static sf::View controlsView;

		static std::vector< std::vector < sf::Sprite > > spriteTiles;
		static sf::Sprite spriteMinimap;
		static sf::Vector2f scaleMinimap;
		static int currentDepth;
		static sf::Vector2i currCorner;
		static bool isControlSelected;
		static int selectedControl;
		static std::vector< std::vector< std::vector< std::vector< int > > > > mapArray;
		
		static int panSpeed;
		static sf::RectangleShape outlineTile;
		static sf::RectangleShape hoverOutlineTile;

		static sf::Clock clock;
		static sf::Time elapsedTime;
		static int frameCounter;

		static void loadTextures(int numTiles);
		static void initMapArray();
		
		static void setTile(sf::Vector2i newTile) {mapArray[newTile.x][newTile.y][currentDepth][0] = Render::selectedControl;}
		static void setSelectedTile(Window & window, sf::Vector2i mousePosition);
		static void createTileOutline();

	};

#endif