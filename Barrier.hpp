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

	void RenderBarrier(SDL_Renderer* renderer);
	std::string CheckBoundary(int PlayerY, int PlayerX, int PlayerWidth, int PlayerHeight);
};

#endif