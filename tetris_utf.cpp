#include "Game.h"
#include "Windows.h"

int main(int argc, char* argv[])
{
	SetConsoleOutputCP(CP_UTF8);
	Game* game = new Game();

	game->run();

	return 0;
}

