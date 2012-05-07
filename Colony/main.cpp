#include "settings.h"
#include "EventHandler.h"
#include "timer.h"

int main()
{
	// Create the window
	sf::RenderWindow window(sf::VideoMode(resolution.x, resolution.y), title);
//	window.setFramerateLimit(60);
	// Load the visuals
	Render::prepGraphics();
	
	// Main loop
    while (window.isOpen())
    {
		//Capture current time
		ElapsedTime = timer.getElapsedTime();
		timer.restart();

        //Event handling
        EventHandler::interpretEvents(window);
		EventHandler::pollRealTime(window, ElapsedTime);

		// Render the scene
		Render::drawScreen(window);

		runTimeStep();
	}

    return 0;
}



