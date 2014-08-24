#ifndef GAME_H
#define GAME_H

#include "Config.h"
#include "EventHandler.h"
#include "Render.h"
#include "window.h"

class Game
{
public:
	Game::Game();
	Game::~Game();

	bool active;
	Config config;
private:

	Window window;
	EventHandler eventHandler;
	Render render;


	bool loop();
};

#endif