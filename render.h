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
#include "loadShaders.h"

class Render
{

public:
	Render();
	~Render();

	void drawScreen(Window & window, sf::Time elapsedTime);
	void Render::drawTests(Window & window, sf::Time elapsedTime);

private:


	
};

#endif