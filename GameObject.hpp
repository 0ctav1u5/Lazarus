#ifndef GAME_OBJECT
#define GAME_OBJECT
#include <iostream>
#include <SDL.h>


class GameObject {
private:
	SDL_Rect rect;
	int X, Y, WIDTH, HEIGHT;
	bool CanCollide;
	bool CanDamage;
	bool CanCollect;
	bool Visible;
public:

	GameObject(int x, int y, int width, int height, bool cancollide, bool candamage, bool cancollect,
		bool visible)
		: X(x), Y(y), WIDTH(width), HEIGHT(height), CanCollide(cancollide), CanDamage(candamage), 
		CanCollect(cancollect), Visible(visible)
	{
		rect = { X, Y, WIDTH, HEIGHT };
	}



	void RenderGameObject(SDL_Renderer* renderer) {
		int Visibility = 0;
		if (this->Visible) {
			Visibility = 255;
		}
		else {
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
			Visibility = 0;
		}
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, Visibility); // RGB
		SDL_RenderFillRect(renderer, &rect);
	}

	bool GetCanDamage() { // getters
		return this->CanDamage;
	}

	bool GetCanCollect() {
		return this->CanCollect;
	}

	int GetX() {
		return this->X;
	}

	int GetY(){
		return this->Y;
	}

	int GetGameObjectWidth() {
		return this->WIDTH;
	}

	int GetGameObjectHeight() {
		return this->HEIGHT;
	}

	bool CheckCollidable() {
		if (CanCollide) {
			return true;
		}
		else {
			return false;
		}
	}

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
