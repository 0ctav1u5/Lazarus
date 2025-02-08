#include <iostream>
#include <SDL.h>
#ifndef BARRIER
#define BARRIER

// the barrier class is for customisable rects
class Barrier {
private:
	SDL_Rect BARRIER_RECT = {};
	int X, Y, WIDTH, HEIGHT;
public:


	Barrier(int x, int y, int width, int height) : X(x), Y(y), WIDTH(width), HEIGHT(height) {
		BARRIER_RECT = { X, Y, WIDTH, HEIGHT };
	}

	void RenderBarrier(SDL_Renderer* renderer) {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // RGB
		SDL_RenderFillRect(renderer, &BARRIER_RECT);
	}



	// this checks the ranges of the rects
	std::string CheckBoundary(int PlayerY, int PlayerX, int PlayerWidth, int PlayerHeight) {

		if (PlayerY + PlayerHeight == Y && PlayerX + PlayerWidth >= X && PlayerX <= X + WIDTH) { // TOP
			return "top";
		}
		if (PlayerY == Y + HEIGHT && (PlayerX + PlayerWidth >= X && PlayerX <= X + WIDTH)) {
			return "bottom";
		}

		if (PlayerX + PlayerWidth == X && (PlayerY + PlayerHeight >= Y && PlayerY <= Y + HEIGHT)) {
			return "left";
		}

		if (PlayerX == X + WIDTH && (PlayerY + PlayerHeight >= Y && PlayerY <= Y + WIDTH)) {
			return "right";
		}

		return "";
	}
};

#endif