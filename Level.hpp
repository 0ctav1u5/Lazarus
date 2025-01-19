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



	// the first level will be in the load assets function
	// this function will load the level into the class memory from a map or even a vector


	void RenderLevel(SDL_Renderer* renderer) { // this will be used to render the level
		SDL_Surface* surface = IMG_Load(BackgroundImage);
		if (!surface) {
			std::cerr << "IMG_Load Error: " << IMG_GetError() << std::endl;
			return;
		}

		BackgroundTexture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
		if (!BackgroundTexture) {
			std::cerr << "SDL_CreateTexture Error: " << SDL_GetError() << std::endl;
			return;
		}
		SDL_RenderCopy(renderer, BackgroundTexture, NULL, NULL);
		SDL_DestroyTexture(BackgroundTexture);
	}
};
#endif 
