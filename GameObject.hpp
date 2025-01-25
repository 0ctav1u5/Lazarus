#ifndef GAME_OBJECT
#define GAME_OBJECT
#include <iostream>
#include <SDL.h>


class GameObject {
private:
	SDL_Rect rect;
	int X, Y, WIDTH, HEIGHT;
public:


	GameObject(int x, int y, int width, int height) : X(x), Y(y), WIDTH(width), HEIGHT(height) {
		rect = { X, Y, WIDTH, HEIGHT };
	}


	void RenderGameObject(SDL_Renderer* renderer) {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // RGB
		SDL_RenderFillRect(renderer, &rect);
	}



	// TODO: set 4 ranges, currently using PlayerY and PlayerX to decide Player point of collision
	// needs to be ranges though so that way, all sides of rect will execute code on impact
	std::string CheckBoundary(int PlayerY, int PlayerX, int PlayerWidth, int PlayerHeight) {

		if (PlayerY + PlayerHeight == Y && PlayerX + PlayerWidth >= X && PlayerX <= X + WIDTH) { // TOP
			return "top";
		}
		if (PlayerY == Y + HEIGHT && (PlayerX + PlayerWidth >= X && PlayerX <= X + PlayerWidth)) {
			return "bottom";
		}

		if (PlayerX + PlayerWidth == X && (PlayerY + PlayerHeight >= Y && PlayerY <= Y + WIDTH)) {
			return "left";
		}

		if (PlayerX == X + WIDTH && (PlayerY + PlayerHeight >= Y && PlayerY <= Y + WIDTH)) {
			return "right";
		}

		return "";
	}
};




/*

if (PlayerY + 100 == Y && PlayerX + 100 >= X && PlayerX <= X + 50) { // TOP
			return "top";
		}
		if (PlayerY == Y + HEIGHT && (PlayerX + 100 >= X && PlayerX + 50 <= X + 100)) {
			return "bottom";
		}

		if (PlayerX + 100 == X && (PlayerY + 100 >= Y && PlayerY <= Y + 50)) {
			return "left";
		}

		if (PlayerX == X + 50 && (PlayerY + 100 >= Y && PlayerY <= Y + 50)) {
			return "right";
		}

		return "";












*/



















#endif 
