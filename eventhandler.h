#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <string>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "render.h"
#include "worldmap.h"

class EventHandler
{
public:
	EventHandler();
	~EventHandler();

    void interpretEvents(Window & window, sf::Time elapsedTime, Worldmap & worldmap);
	
private:
	Window window;
	Render render;
	sf::Event event;
	sf::Vector2i mousePosition;
    void eventKeyboard(Window & window, Worldmap & worldmap);
    void eventMouseClick(Window & window, Worldmap & worldmap);
    void pollRealTime(Window & window, sf::Time elapsedTime, Worldmap & worldmap);
};

#endif