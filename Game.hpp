#ifndef GAME
#define GAME
#include <SDL.h>
#include <iostream>
#include <vector>
#include <map>
#include "Player.hpp"
#include "Level.hpp"



class Game {
private:
	std::vector<std::unique_ptr<Player>> Players;
	std::vector<std::unique_ptr<Level>> Levels;
public:

	Game() {}
	bool LoadAssets(SDL_Renderer* renderer);
	void DrawBackground(SDL_Renderer* renderer);
	bool MakePlayer(std::string name, int posx, int posy, const char* imagepath, int speed);
	bool MakeLevel(int ID, std::string levelname, const char* backgroundimagepath);
	void UserInput(SDL_Event& e, bool& running, const Uint8* keyboardState, SDL_Renderer* renderer);
	Player* GetPlayer(int i);
	Level* GetLevel(int i);

};
#endif