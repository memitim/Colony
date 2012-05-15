#include "game.h"

bool Game::active = true;

Game::Game()
{
	Config config;
	EventHandler eventHandler;
	Render render;

	loop();
}

Game::~Game()
{


}

bool Game::loop()
{
	active = true;
	Window window = Window(sf::VideoMode(config.readSetting<int>("width"),config.readSetting<int>("height")), config);
	render.prepGraphics(window);
	while (window.isOpen())
	{
		//Capture current time
		sf::Clock timer;
		sf::Time elapsedTime = timer.getElapsedTime();
		
//		if(elapsedTime.asMilliseconds() > 1000 / 60)
		{
        //Event handling
        eventHandler.interpretEvents(window);
		eventHandler.pollRealTime(window, elapsedTime);

		// Render the scene
		drawScreen(window);
//		timer.restart();
		}
	}
	active = false;
	return active;
}

// Render all graphics
void Game::drawScreen(Window & window)
{
	window.clear();	
	render.drawMap(window); 
	window.display();
}