#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <string>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "render.h"
#include "map.h"

class EventHandler
{
public:
	EventHandler();
	~EventHandler();

    void interpretEvents(Window & window, sf::Time elapsedTime, Map & worldmap);
	
private:
	Window window;
	Render render;
	sf::Event event;
	sf::Vector2i mousePosition;
    void eventKeyboard(Window & window, Map & worldmap);
    void eventMouseClick(Window & window, Map & worldmap);
    void pollRealTime(Window & window, sf::Time elapsedTime, Map & worldmap);
};

#endif