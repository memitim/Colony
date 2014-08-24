#include "EventHandler.h"

EventHandler::EventHandler()
{

}

EventHandler::~EventHandler()
{

}

// Main event interpreter function
void EventHandler::interpretEvents(Window & window, sf::Time elapsedTime)
{
	this->pollRealTime(window, elapsedTime);
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			window.close();
			break;
		case sf::Event::KeyPressed:
			this->eventKeyboard(window);
			break;
		case sf::Event::MouseButtonPressed:
			this->eventMouseClick(window);
			break;
		}
	}
}

// Process keyboard events
void EventHandler::eventKeyboard(Window & window)
{
	switch (event.key.code)
	{
	case sf::Keyboard::LBracket:
		break;
	case sf::Keyboard::RBracket:
		break;
	case sf::Keyboard::Escape:
		window.close();
		break;
	case sf::Keyboard::F12:
		break;
	case sf::Keyboard::F11:
		break;
	}
}

// Process mouse clicks
void EventHandler::eventMouseClick(Window & window)
{
	switch (event.mouseButton.button)
	{
	case sf::Mouse::Left:
		// Left click action
		break;
	case sf::Mouse::Right:
		break;
	}
}

// Process real-time input
void EventHandler::pollRealTime(Window & window, sf::Time elapsedTime)
{
	this->mousePosition = sf::Mouse::getPosition(window);
	// Real-time keyboard handling
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F10))
	{
		// Get framerate
		std::cout << "Framerate: " << std::fixed << std::setprecision(0) << 1.f / elapsedTime.asMilliseconds() * 1000 << std::endl;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		
	}
	// Real-time mouse handling
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		// Left click
	}

}