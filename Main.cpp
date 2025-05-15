#define _CRTDBG_MAP_ALLOC
#include <SDL.h>
#include <SDL_image.h> // images
#include <SDL_ttf.h> // text
#include <SDL_mixer.h>
#include <iostream>
#include <memory>
#include <Windows.h>
#include <limits>
#include <chrono>
#include <cstdlib>
#include <crtdbg.h>
#include "GameEngine.hpp"


// note to self: char** WILL NOT WORK!!!
int main(int argc, char* argv[]) {
	std::unique_ptr<GameEngine> Engine = std::make_unique<GameEngine>();
	if (!Engine->Initialise()) {
		std::cerr << "Game Engine could not be initialised!" << std::endl;
		return -1;
	}
	Engine->GameLoop();
	_CrtDumpMemoryLeaks();
	return 0;
}
