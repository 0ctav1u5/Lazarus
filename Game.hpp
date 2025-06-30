#ifndef GAME
#define GAME
#include <SDL.h>
#include <iostream>
#include <vector>
#include <map>
#include <SDL_mixer.h>
#include "Player.hpp"
#include "Level.hpp"
#include "GameObject.hpp"
#include "Message.hpp"
#include "Bullet.hpp"
#include "OverlayObject.hpp"

class Game {
private:
	std::vector<std::shared_ptr<Player>> Players;
	std::vector<std::shared_ptr<Level>> Levels;
	std::vector<std::shared_ptr<Message>> Messages; 
	std::vector<std::shared_ptr<Bullet>> Bullets;
	std::vector<std::shared_ptr<OverlayObject>> OverlayObjects;
	Mix_Chunk* BulletSound = nullptr;
	bool LevelLoader = false;

public:
	Game() {}

	void CheckAudio();
	void PauseMenu(SDL_Renderer* renderer, bool& running);
	bool LoadAssets(SDL_Renderer* renderer, int& LevelID);
	bool MakePlayer(std::string name, int posx, int posy, const char* imagepath, int speed);
	bool MakeLevel(std::string levelname, const char* backgroundimagepath, int& LevelID,
	int lb, int rb, int ub, int lwb, SDL_Renderer* renderer);
	bool MakeBullet(int speed, int damage, int startx, int starty);
	bool MakeOverlayObject(int x, int y, int width, int height);
	void RemoveOverlayObject();
	void ChangeLevel(int& LevelID, SDL_Renderer* renderer);
	void UserInput(bool& running, const Uint8* keyboardState, int& LevelID, SDL_Renderer* renderer);
	void HandleEvents(SDL_Event& e, bool& running, SDL_Renderer* renderer, int& LevelID);
	void LoadLevel(SDL_Event& e, SDL_Renderer* renderer, int& LevelID);
	std::shared_ptr<Player> GetPlayer(int i);
	std::shared_ptr<Level> GetLevel(int i);
	std::shared_ptr<Bullet> GetBullet(int i);
	size_t GetLevelsCount() const;
	void PlayerMove(int x, int y);
	void GameObjectCollisionChecker(int levelnum, int playerY, int playerX, int playerWidth, int playerHeight,
	bool& blockBottom, bool& blockTop, bool& blockRight, bool& blockLeft);
	void CheckPlayerStatus(int& LevelID, bool& running, SDL_Renderer* renderer);
	void Level2(int& LevelID, SDL_Renderer* renderer);
	void Level3(int& LevelID, SDL_Renderer* renderer);
	void Level4(int& LevelID, SDL_Renderer* renderer);
	void Level5(int& LevelID, SDL_Renderer* renderer);
	void Level6(int& LevelID, SDL_Renderer* renderer);
	void Level7(int& LevelID, SDL_Renderer* renderer);
	void Level8(int& LevelID, SDL_Renderer* renderer);
	void Level9(int& LevelID, SDL_Renderer* renderer);
	void Level10p1(int& LevelID, SDL_Renderer* renderer);
	void Level10p2(int& LevelID, SDL_Renderer* renderer);
	void Level11(int& LevelID, SDL_Renderer* renderer);
	void Level12(int& LevelID, SDL_Renderer* renderer);
	void Level13(int& LevelID, SDL_Renderer* renderer);
	void EndMessage();
	bool MakeMessage(std::string message, int x, int y, int w, int h);
	void DisplayMessages(SDL_Renderer* renderer);
	size_t GetBulletsSize();
	std::vector<std::shared_ptr<Bullet>> GetBullets();
	std::shared_ptr<OverlayObject> GetOverlayObject(int i);
	int GetOverlayObjectSize();
};

struct ObjectProperties {
	int ObjectX;
	int ObjectY;
	int ObjectWidth;
	int ObjectHeight;
};

#endif