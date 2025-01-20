#ifndef LEVEL
#define LEVEL
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

class Level {
private:
	int LevelID;
	std::string LevelName;
	const char* BackgroundImage; 
	SDL_Surface* SURFACE = nullptr; 
	SDL_Texture* BackgroundTexture = nullptr; 

public:

	Level(int num, std::string levelname, const char* backgroundimage) : LevelID(num),
		LevelName(levelname), BackgroundImage(backgroundimage){}


	void RenderLevel(SDL_Renderer* renderer);
};
#endif 
