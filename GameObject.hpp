#ifndef GAME_OBJECT
#define GAME_OBJECT
#include <iostream>
#include <SDL.h>


class GameObject {
private:
	SDL_Rect rect;
	int X, Y, WIDTH, HEIGHT;
	bool CanCollide;
public:

	GameObject(int x, int y, int width, int height, bool cancollide) : X(x), Y(y), WIDTH(width), 
		HEIGHT(height), CanCollide(cancollide)
	{
		rect = { X, Y, WIDTH, HEIGHT };
	}


	void RenderGameObject(SDL_Renderer* renderer) {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // RGB
		SDL_RenderFillRect(renderer, &rect);
	}

	bool CheckCollidable() {
		if (CanCollide) {
			return true;
		}
		else {
			return false;
		}
	}


	// TODO: set 4 ranges, currently using PlayerY and PlayerX to decide Player point of collision
	// needs to be ranges though so that way, all sides of rect will execute code on impact
	std::string CheckBoundary(int PlayerY, int PlayerX, int PlayerWidth, int PlayerHeight) {

		if (PlayerY + PlayerHeight == Y && PlayerX + PlayerWidth >= X && PlayerX <= X + WIDTH) { // TOP
			return "top";
		}
		if (PlayerY == Y + HEIGHT && (PlayerX + PlayerWidth >= X && PlayerX <= X + WIDTH)) {
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

#endif 
