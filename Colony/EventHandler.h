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

	static sf::Event EventHandler::event;
	static sf::Vector2i EventHandler::mousePosition;

	// Class functions
	static void EventHandler::eventKeyboard(sf::RenderWindow & window);
	static void EventHandler::eventMouseClick(sf::RenderWindow & window);
	static void EventHandler::interpretEvents(sf::RenderWindow & window);
	static void EventHandler::pollRealTime(sf::RenderWindow & window, sf::Time ElapsedTime);
private:

protected:

};

#endif