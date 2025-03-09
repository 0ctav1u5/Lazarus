#include <iostream>
#include <SDL.h>
#include "Barrier.hpp"

void Barrier::RenderBarrier(SDL_Renderer* renderer) {
	int Visibility = 0;
	if (Visible) {
		Visibility = 255;

	}
	else {
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		Visibility = 0;
	}
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, Visibility); // RGB
	SDL_RenderFillRect(renderer, &BARRIER_RECT);
}

void Barrier::SetInvisible() {
	Visible = false;
}

// this checks the ranges of the rects
std::string Barrier::CheckBoundary(int PlayerY, int PlayerX, int PlayerWidth, int PlayerHeight) {

	if (PlayerY + PlayerHeight == Y && PlayerX + PlayerWidth >= X && PlayerX <= X + WIDTH) { // TOP
		return "top";
	}
	if (PlayerY == Y + HEIGHT && (PlayerX + PlayerWidth >= X && PlayerX <= X + WIDTH)) {
		return "bottom";
	}

	if (PlayerX + PlayerWidth == X && (PlayerY + PlayerHeight >= Y && PlayerY <= Y + HEIGHT)) {
		return "left";
	}

	if (PlayerX == X + WIDTH && (PlayerY + PlayerHeight >= Y && PlayerY <= Y + HEIGHT)) {
		return "right";
	}

	return "";
}


void Barrier::MoveBarrierY(int y) {
	Y += y;
	BARRIER_RECT.y += Y;
}