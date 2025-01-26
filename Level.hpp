#ifndef LEVEL
#define LEVEL
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "GameObject.hpp"

class Level {
private:
	static int LevelID;
	std::string LevelName = "";
	const char* BackgroundImage; 
	SDL_Surface* SURFACE = nullptr; 
	SDL_Texture* BackgroundTexture = nullptr; 
	std::vector<std::shared_ptr<GameObject>> GameObjects; // Game Objects should always be in levels

public:

	Level(std::string levelname, const char* backgroundimage) : 
	LevelName(levelname), BackgroundImage(backgroundimage){
		if (LevelID > 0) {
			LevelID++;
		}
	}

	size_t GetGameObjectsCount() const;
	void RenderLevel(SDL_Renderer* renderer);
	bool MakeGameObject(int x, int y, int width, int height);
	std::shared_ptr<GameObject> GetGameObject(int i);
	int GetLevelID();
};
#endif 
