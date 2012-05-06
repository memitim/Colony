#ifndef RENDER_H
#define RENDER_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <iomanip>
#include <sstream>

const int mapHeight = 64;
const int mapWidth = 64;
const int mapDepth = 10;
const float minimapScale = 0.25f;

const int numSprites = 3;

const float controlY = 500.f;
const int mapPaneHeight = 36;
const int mapPaneWidth = 37;
const int textureDim = 32;

class Render
	{
		
	public:
		Render();
		~Render();
		static void Render::prepGraphics(void);
		static void loadTextures(int numSprites);
		static void initMapArray();
		static void drawMap(sf::RenderWindow & window, int currentDepth);
		static void drawScreen(sf::RenderWindow & window);
		static void setTile(sf::Vector2i newTile);
		static void changeDepth(sf::Event event);
		static void scrollMap(sf::Event event);
		static void setSelectedTile(sf::Vector2i mousePos);
		static void releaseSelectedControl(void);
		static void setSelectedControl(sf::Vector2i mousePos);
		static void leftClickScreen(sf::Vector2i mousePos);
		static void Render::panLeft(void);
		static void Render::panRight(void);
		static void Render::panUp(void);
		static void Render::panDown(void);
	private:

	protected:

	};

#endif