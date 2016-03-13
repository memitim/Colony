#ifndef WINDOW_H
#define WINDOW_H

#include <SFML/Graphics.hpp>
#include "Config.h"

class Window : public sf::RenderWindow
{
public:
	Window();
	Window(sf::VideoMode videoMode, Config &config);
	~Window();
private:

};

#endif