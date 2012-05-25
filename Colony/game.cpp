#include "game.h"

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
	sf::Clock timer;
	active = true;
	Window window = Window(sf::VideoMode(config.readSetting<int>("width"),config.readSetting<int>("height")), config);
	render.prepGraphics(window);
	while (window.isOpen())
	{
		//Capture current time
		sf::Time elapsedTime = timer.getElapsedTime();
		// Framerate limiting
//		std::cout << config.readSetting<int>("framelimit") << std::endl;
		if(config.readSetting<int>("framelimit") == 1)
		{
			if(elapsedTime.asMilliseconds() > 1000 / (config.readSetting<int>("framerate")))
			{
				//Event handling
				eventHandler.interpretEvents(window, elapsedTime);

				// Render the scene
				render.drawScreen(window);
				timer.restart();
			}
		}
		else
		{
			timer.restart();
			eventHandler.interpretEvents(window, elapsedTime);
			render.drawScreen(window);
		}
	}
	active = false;
	return active;
}

