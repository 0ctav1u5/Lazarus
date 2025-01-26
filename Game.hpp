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
	bool MakeLevel(std::string levelname, const char* backgroundimagepath);
	void UserInput(bool& running, const Uint8* keyboardState);
	void HandleEvents(SDL_Event& e, bool& running);
	std::shared_ptr<Player> GetPlayer(int i);
	std::shared_ptr<Level> GetLevel(int i);
	size_t GetLevelsCount() const;
	void PlayerMove(int x, int y);
	void CollisionChecker(int levelnum, int playerY, int playerX, int playerWidth, int playerHeight,
	bool& blockBottom, bool& blockTop, bool& blockRight, bool& blockLeft);

};
#endif