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

	
	void Render::drawTests(Window & window, sf::Time elapsedTime);
	void Render::drawScreen(Window & window, sf::Time elapsedTime);

private:

	static glm::vec3 position;
	static float horizontalAngle;
	static float verticalAngle;
	static float speed;
	static float mouseSpeed;
	
	
};

#endif