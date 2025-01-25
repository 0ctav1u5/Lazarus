#ifndef GAME
#define GAME
#include <SDL.h>
#include <iostream>
#include <vector>
#include <map>
#include "Player.hpp"
#include "Level.hpp"
#include "GameObject.hpp"



class Game {
private:
	std::vector<std::shared_ptr<Player>> Players;
	std::vector<std::shared_ptr<Level>> Levels;
public:

	Game() {}
	bool LoadAssets(SDL_Renderer* renderer);
	bool MakePlayer(std::string name, int posx, int posy, const char* imagepath, int speed);
	bool MakeLevel(int ID, std::string levelname, const char* backgroundimagepath);
	void UserInput(bool& running, const Uint8* keyboardState);
	void HandleEvents(SDL_Event& e, bool& running);
	std::shared_ptr<Player> GetPlayer(int i);
	std::shared_ptr<Level> GetLevel(int i);
	void PlayerMove(int x, int y);

};
#endif