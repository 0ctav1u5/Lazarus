#ifndef GAME_ENGINE
#define GAME_ENGINE
#include "Game.hpp"


class GameEngine {
private:
	const int m_WINDOW_HEIGHT = 500;
	const int m_WINDOW_WIDTH = 500;
	const char* m_WINDOW_NAME = "Lazarus";
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	std::unique_ptr<Game> game;
	int LevelID = 0;

public:
	bool Initialise();
	void Cleanup(const std::string& errormsg);
	void GameLoop();
	void CheckLevelID();


	GameEngine() {}


	~GameEngine() {
		std::cout << "Game Engine Destroyed!" << std::endl;
		if (renderer != nullptr) {
			SDL_DestroyRenderer(renderer);
		}
		if (window != nullptr) {
			SDL_DestroyWindow(window);
		}
		TTF_Quit();
		SDL_Quit(); // on destruction of object, quit SDL subsystem
	}

};

#endif 
