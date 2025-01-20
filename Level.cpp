#include "Level.hpp"

void Level::RenderLevel(SDL_Renderer* renderer) { // this will be used to render the level
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















