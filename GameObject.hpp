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
	bool CheckBoundary(int PlayerY, int PlayerX) {
		if (PlayerY == Y && (PlayerX >= X && PlayerX <= X + WIDTH)) { // TOP
			std::cout << "TOP!" << std::endl;
			return true;
		} 

		if (PlayerY == Y + HEIGHT && (PlayerX >= X && PlayerX <= X + WIDTH)) {
			std::cout << "BOTTOM!" << std::endl;
			return true;
		}

		if (PlayerX == X && (PlayerY >= Y && PlayerY <= Y + HEIGHT)) {
			std::cout << "LEFT" << std::endl;
			return true;
		}

		if (PlayerX == X + WIDTH && (PlayerY >= Y && PlayerY <= Y + HEIGHT)) {
			std::cout << "RIGHT" << std::endl;
			return true;
		}
		return false;
	}


};
























#endif 
