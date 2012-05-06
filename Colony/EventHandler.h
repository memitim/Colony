#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <string>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "render.h"

class EventHandler
{
public:
	EventHandler();
	~EventHandler();

	static void EventHandler::eventKeyboard(void);
	static void EventHandler::eventMouseClick(sf::RenderWindow & window);
	static void EventHandler::interpretEvents(sf::RenderWindow & window);
	static void EventHandler::pollRealTime(sf::Time ElapsedTime);
private:

protected:

};

#endif