#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "OverlayObject.hpp"


void OverlayObject::SetTexture(const char* imagepath) {
	this->IMAGEPATH = imagepath;
	SURFACE = IMG_Load(IMAGEPATH);
	if (!SURFACE) {
		std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
		return;
	}
}

void OverlayObject::RenderOverlayObject(SDL_Renderer* renderer) {
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // RGB Opaqueness
		SDL_RenderFillRect(renderer, &rect); // draws the rectangle

		texture = SDL_CreateTextureFromSurface(renderer, SURFACE);
		if (texture) {
			SDL_RenderCopy(renderer, texture, nullptr, &rect);
		}
}

void OverlayObject::Remove() {
	this->exists = false;
}


