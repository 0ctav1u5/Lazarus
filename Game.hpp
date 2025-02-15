#ifndef GAME
#define GAME
#include <SDL.h>
#include <iostream>
#include <vector>
#include <map>
#include "Player.hpp"
#include "Level.hpp"
#include "GameObject.hpp"
#include "Message.hpp"

class Game {
private:
	std::vector<std::shared_ptr<Player>> Players;
	std::vector<std::shared_ptr<Level>> Levels;
	std::vector<std::shared_ptr<Message>> Messages; 
public:

	Game() {}

	~Game() {
		
	}
	void PauseMenu(SDL_Renderer* renderer, bool& running);
	bool LoadAssets(SDL_Renderer* renderer, int& LevelID);
	bool MakePlayer(std::string name, int posx, int posy, const char* imagepath, int speed);
	bool MakeLevel(std::string levelname, const char* backgroundimagepath, int& LevelID,
	int lb, int rb, int ub, int lwb);
	void ChangeLevel(int& LevelID);
	void UserInput(bool& running, const Uint8* keyboardState, int& LevelID);
	void HandleEvents(SDL_Event& e, bool& running, SDL_Renderer* renderer);
	std::shared_ptr<Player> GetPlayer(int i);
	std::shared_ptr<Level> GetLevel(int i);
	size_t GetLevelsCount() const;
	void PlayerMove(int x, int y);
	void GameObjectCollisionChecker(int levelnum, int playerY, int playerX, int playerWidth, int playerHeight,
	bool& blockBottom, bool& blockTop, bool& blockRight, bool& blockLeft);
	void CheckPlayerStatus(int& LevelID, bool& running, SDL_Renderer* renderer);
	void Level2(int& LevelID);
	void Level3(int& LevelID);
	void Level4(int& LevelID);
	bool MakeMessage(std::string message, int x, int y, int w, int h);
	void DisplayMessages(SDL_Renderer* renderer);

};

struct ObjectProperties {
	int ObjectX;
	int ObjectY;
	int ObjectWidth;
	int ObjectHeight;
};

#endif