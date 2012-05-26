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
		void leftClickScreen(Window & window, sf::Vector2i mousePosition);
		void changeDepth(sf::Event event);
		void checkHover(Window & window, sf::Vector2i mousePosition);
		void Render::panLeft();
		void Render::panRight();
		void Render::panUp();
		void Render::panDown();
		void releaseSelectedControl() {if(this->isControlSelected == true) {this->isControlSelected = false;this->hoverOutlineTile = this->outlineTile;this->hoverOutlineTile.setOutlineColor(sf::Color(48,48,48,192));}}
		void drawScreen(Window & window);
		
	private:

		static const int numTiles = 14;
		static const int numTileAnimations = 2;
		static const int numTilesControls = 10;

		static const int mapPaneHeight = 35;
		static const int mapPaneWidth = 38;
		static const int textureDim = 32;
		
		// Dimensions of the map
		static int mapHeight;
		static int mapWidth;
		static int mapDepth;
		// Number of tile properties
		static int tileProperties;

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
		static int currentDepth;
		static sf::Vector2i currCorner;
		static bool isControlSelected;
		static int selectedControl;
		static std::vector< std::vector< std::vector< std::vector< int > > > > mapArray;
		
		static int panSpeed;
		static sf::RectangleShape plainTile;
		static sf::RectangleShape outlineTile;
		static sf::RectangleShape hoverOutlineTile;
		static sf::Texture (*texture)[2];
		
		void Render::saveMap();
		void loadTextures();
		void initMapArray();
		void createTileOutline();
		void drawMap(Window & window);
		void setSelectedTile(Window & window, sf::Vector2i mousePosition);

		static void setSelectedControl(Window & window, sf::Vector2i mousePosition);
		
		inline void setTile(sf::Vector2i newTile) {mapArray[newTile.x][newTile.y][currentDepth][0] = selectedControl;}
	};

		inline void Render::changeDepth(sf::Event event) {if ((event.key.code == sf::Keyboard::LBracket) && (this->currentDepth > 0)) 
			this->currentDepth--; else if ((event.key.code == sf::Keyboard::RBracket) && (this->currentDepth < this->mapDepth - 1)) this->currentDepth++;}		
		inline void Render::panLeft() {if(this->currCorner.x > 0)	{this->currCorner.x = this->currCorner.x - this->panSpeed; this->mainMapView.move(float(-panSpeed),0.f);
			this->miniMapView.move(float(-this->panSpeed),0.f);}}
		inline void Render::panRight() {if(this->currCorner.x < ((this->mapWidth * this->textureDim) - (this->mapPaneWidth * this->textureDim)) - this->panSpeed - this->textureDim) 
			{this->currCorner.x = this->currCorner.x + this->panSpeed; this->mainMapView.move(float(panSpeed),0.f); this->miniMapView.move(float(this->panSpeed),0.f);}}
		inline void Render::panUp() {if(this->currCorner.y > 0) {this->currCorner.y = this->currCorner.y - this->panSpeed; this->mainMapView.move(0.f,float(-this->panSpeed));
			this->miniMapView.move(0.f,float(-this->panSpeed));}}
		inline void Render::panDown() {if(this->currCorner.y < ((this->mapHeight * this->textureDim) - (this->mapPaneHeight * this->textureDim)) - this->panSpeed - this->textureDim)
			{this->currCorner.y = this->currCorner.y + this->panSpeed; this->mainMapView.move(0.f,float(this->panSpeed)); this->miniMapView.move(0.f,float(this->panSpeed));}}
		
#endif