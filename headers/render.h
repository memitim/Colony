#ifndef RENDER_H
#define RENDER_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <math.h>
#include <iomanip>
#include <sstream>
#include <vector>
#include "window.h"
#include "worldmap.h"

class Render
	{
		
	public:
		Render();
		~Render();

		void prepGraphics(Window & window);
		void leftClickScreen(Window & window, sf::Vector2i mousePosition, Worldmap & worldmap);
		void checkHover(Window & window, sf::Vector2i mousePosition);
		void releaseSelectedControl(Window & window, sf::Vector2i mousePosition, Worldmap & worldmap);
		void drawScreen(Window & window, Worldmap & worldmap);

		void drawText(Window & window, sf::Vector2i mousePosition);
        void panDown(Worldmap & worldmap);
        void panUp();
        void panLeft();
        void panRight(Worldmap & worldmap);
        void changeDepth(sf::Event event, Worldmap & worldmap);
		
	private:

		static const int numTiles = 14;
		static const int numTileAnimations = 2;
		static const int numTilesControls = 10;

		static const int mapPaneHeight = 35;
		static const int mapPaneWidth = 38;
		static const int textureDim = 32;
		


		// Pixels between viewports
		static const int paneBufferX = 20;
		static const int paneBufferY = 20;
		static const int leftBuffer = 15;
		static const int topBuffer = 30;

		// Viewports
		static sf::View mainMapView;
		static sf::View miniMapView;
		static sf::View controlsView;

		static std::vector< std::vector < sf::Sprite > > spriteTiles;
		static std::vector< std::vector < sf::Texture > > texture;
		static int currentDepth;
		static sf::Vector2i currCorner;
		static bool isControlSelected;
		static int selectedControl;
		
		
		static int panSpeed;
		static sf::RectangleShape plainTile;
		static sf::RectangleShape outlineTile;
		static sf::RectangleShape hoverOutlineTile;
		static sf::Text infoText;
		
		void loadTextures();
		void digHole(Window & window, sf::Vector2i mousePosition, Worldmap & worldmap);
		void createTileOutline();
		void drawMap(Window & window, Worldmap & worldmap);
        void setSelectedTile(Window & window, sf::Vector2i mousePosition, Worldmap & worldmap);

		static void setSelectedControl(Window & window, sf::Vector2i mousePosition);
		
        void setTile(sf::Vector2i newTile, Worldmap & worldmap);
	};


		
#endif