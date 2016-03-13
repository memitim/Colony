#include "..\headers\game.h"

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
    Map worldmap = Map(128, 128, 120);
    worldmap.initMapArray();
	render.prepGraphics(window);
	int currentFramerate = config.readSetting<int>("framerate");
	int currentFramelimit = config.readSetting<int>("framelimit");
	while (window.isOpen())
	{
		//Capture current time
		sf::Time elapsedTime = timer.getElapsedTime();
		// Framerate limiting
		if(currentFramelimit == 1)
		{
			if(elapsedTime.asMilliseconds() > 1000 / (currentFramerate))
			{
				//Event handling
				eventHandler.interpretEvents(window, elapsedTime, worldmap);

				// Render the scene
				render.drawScreen(window, worldmap);
				timer.restart();
			}
		}
		else
		{
			timer.restart();
			eventHandler.interpretEvents(window, elapsedTime, worldmap);
			render.drawScreen(window, worldmap);
		}
	}
	active = false;
	return active;
}

