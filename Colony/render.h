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
		static sf::View miniMapView;
		static sf::View controlsView;
		static sf::RectangleShape hoverOutlineTile;
		static double hRatio;
		static double vRatio;

		static void Render::prepGraphics(sf::RenderWindow & window);
		static void loadTextures(int numTiles);
		static void initMapArray();
		static void drawMap(sf::RenderWindow & window);
		static void drawScreen(sf::RenderWindow & window);
		static void setTile(sf::Vector2i newTile) {mapArray[newTile.x][newTile.y][currentDepth] = Render::selectedControl;}
		static void changeDepth(sf::Event event) {if ((event.key.code == sf::Keyboard::LBracket) && (currentDepth > 0)) currentDepth--; else if ((event.key.code == sf::Keyboard::RBracket) && (currentDepth < mapDepth - 1)) currentDepth++;}
		static void setSelectedTile(sf::RenderWindow & window, sf::Vector2i mousePosition);
		static void releaseSelectedControl(void) {if(isControlSelected == true) isControlSelected = false;}
		static void setSelectedControl(sf::RenderWindow & window, sf::Vector2i mousePosition);
		static void leftClickScreen(sf::RenderWindow & window, sf::Vector2i mousePosition);
		static void Render::checkHover(sf::RenderWindow & window, sf::Vector2i mousePosition);
		static void panLeft(void);
		static void panRight(void);
		static void panUp(void);
		static void panDown(void);
		static void createTileOutline(void);
		
	private:

	protected:

	};

#endif