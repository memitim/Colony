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

	static void EventHandler::interpretEvents(Window & window);
	static void EventHandler::pollRealTime(Window & window, sf::Time ElapsedTime);
private:
	static sf::Event EventHandler::event;
	static sf::Vector2i EventHandler::mousePosition;
	static void EventHandler::eventKeyboard(Window & window);
	static void EventHandler::eventMouseClick(Window & window);

};

#endif