#include "game.h"

int main()
{
	bool gameSession = true;
	Game game;

	while(gameSession)
    {
		gameSession = game.active;
	}

    return 0;
}



