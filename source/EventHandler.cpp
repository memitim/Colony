#include "..\headers\EventHandler.h"

EventHandler::EventHandler()
{

}

EventHandler::~EventHandler()
{

}

// Main event interpreter function
void EventHandler::interpretEvents(Window & window, sf::Time elapsedTime, Worldmap & worldmap)
{
	this->pollRealTime(window, elapsedTime, worldmap);
	while (window.pollEvent(event))
    {
		switch (event.type)
		{
		case sf::Event::Closed:
			window.close();
			break;
		case sf::Event::KeyPressed:
			this->eventKeyboard(window, worldmap);
			break;
		case sf::Event::MouseButtonPressed:
			this->eventMouseClick(window, worldmap);
			break;
		}
	}
}

// Process keyboard events
void EventHandler::eventKeyboard(Window & window, Worldmap & worldmap)
{
	switch (event.key.code)
	{
	case sf::Keyboard::LBracket:
		render.changeDepth(event, worldmap);
		break;
	case sf::Keyboard::RBracket:
		render.changeDepth(event, worldmap);
		break;
	case sf::Keyboard::Escape:
		window.close();
		break;
	case sf::Keyboard::F12:
		worldmap.saveMap();
		break;
	case sf::Keyboard::F11:
		worldmap.initMapArray();
		break;
	}
}

// Process mouse clicks
void EventHandler::eventMouseClick(Window & window, Worldmap & worldmap)
{
	switch (event.mouseButton.button)
	{
	case sf::Mouse::Left:
		// Select control for placement
		render.leftClickScreen(window, mousePosition, worldmap);
		break;
	case sf::Mouse::Right:
		render.releaseSelectedControl(window, mousePosition, worldmap);
		break;
	}
}

// Process real-time input
void EventHandler::pollRealTime(Window & window, sf::Time elapsedTime, Worldmap & worldmap)
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
		// Pan left
		render.panLeft();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		// Pan right
		render.panRight(worldmap);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		// Pan up
		render.panUp();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		// Pan down
		render.panDown(worldmap);
	}
	// Real-time mouse handling
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		// Left click
	}
	// Check for hover events
	render.checkHover(window, mousePosition);

	render.drawText(window, mousePosition);
}