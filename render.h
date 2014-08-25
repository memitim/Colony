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
#include "modelImport.h"
#include "mesh.h"

class Render
{

public:
	Render();
	~Render();

	
	void Render::drawTests(Window & window, sf::Time elapsedTime, Mesh *mesh);
	void Render::drawScreen(Window & window, sf::Time elapsedTime, Mesh *mesh);
	void Render::initOpenGL();

private:

	static glm::vec3 position;
	static float horizontalAngle;
	static float verticalAngle;
	static float speed;
	static float mouseSpeed;
	
	
};

#endif