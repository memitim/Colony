#include "settings.h"
#include "EventHandler.h"

int main()
{
	// Create the window
	sf::RenderWindow window(sf::VideoMode(resolution.x, resolution.y), title);
//	window.setFramerateLimit(60);
	// Load the visuals
	Render::prepGraphics(window);
	
	// Main loop
    while (window.isOpen())
    {
		//Capture current time
		elapsedTime = timer.getElapsedTime();
		
		if(elapsedTime.asMilliseconds() > 1000 / frameRate)
		{
        //Event handling
        EventHandler::interpretEvents(window);
		EventHandler::pollRealTime(window, elapsedTime);

		// Render the scene
		
		timer.restart();
		}
		Render::drawScreen(window);

	}

    return 0;
}



