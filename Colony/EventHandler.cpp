#include "EventHandler.h"

sf::Event EventHandler::event;
sf::Vector2i EventHandler::mousePosition;

EventHandler::EventHandler()
{

}

EventHandler::~EventHandler()
{

}

// Main event interpreter function
void EventHandler::interpretEvents(sf::RenderWindow & window)
{
	while (window.pollEvent(event))
    {
		switch (event.type)
		{
		case sf::Event::Closed:
			window.close();
			break;
		case sf::Event::KeyPressed:
			EventHandler::eventKeyboard(window);
			break;
		case sf::Event::MouseButtonPressed:
			EventHandler::eventMouseClick(window);
			break;
		}
	}
}

// Process keyboard events
void EventHandler::eventKeyboard(sf::RenderWindow & window)
{
	switch (event.key.code)
	{
	case sf::Keyboard::LBracket:
		Render::changeDepth(event);
		break;
	case sf::Keyboard::RBracket:
		Render::changeDepth(event);
		break;
	case sf::Keyboard::Escape:
		window.close();
		break;
	}
}

// Process mouse clicks
void EventHandler::eventMouseClick(sf::RenderWindow & window)
{
	switch (event.mouseButton.button)
	{
	case sf::Mouse::Left:
		// Select control for placement
		Render::leftClickScreen(window, mousePosition);
		break;
	case sf::Mouse::Right:
		Render::releaseSelectedControl();
		break;
	}
}

// Process real-time input
void EventHandler::pollRealTime(sf::RenderWindow & window, sf::Time ElapsedTime)
{
	mousePosition = sf::Mouse::getPosition(window);
	// Real-time keyboard handling
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F10))
	{
		// Get framerate
		std::cout << "Framerate: " << std::fixed << std::setprecision(0) << 1.f / ElapsedTime.asMicroseconds() * 1000000 << std::endl;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		// Get framerate
		Render::panLeft();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		// Get framerate
		Render::panRight();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		// Get framerate
		Render::panUp();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		// Get framerate
		Render::panDown();
	}
	// Real-time mouse handling
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		// get global mouse position
	}
	// Check for hover events
	Render::checkHover(window, mousePosition);
}