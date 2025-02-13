#ifndef GAME_OBJECT
#define GAME_OBJECT
#include <iostream>
#include <SDL.h>

// TODO: Add unique ID for each object so we can destroy after collecting 
class GameObject {
private:
	std::string NAME = "";
	SDL_Rect rect = {};
	SDL_Surface* SURFACE = nullptr;
	SDL_Texture* texture = nullptr;
	const char* IMAGEPATH = "";
	int X, Y, WIDTH, HEIGHT;
	bool CanCollide;
	bool CanDamage; // damage zone is in the centre point line and right of the rect
	bool CanCollect;
	bool Visible;
	bool HasTexture = false;
public:

	GameObject(std::string name, int x, int y, int width, int height, bool cancollide, bool candamage, bool cancollect,
		bool visible)
		: NAME(name), X(x), Y(y), WIDTH(width), HEIGHT(height), CanCollide(cancollide), CanDamage(candamage), 
		CanCollect(cancollect), Visible(visible)
	{
		rect = { X, Y, WIDTH, HEIGHT };
	}

	~GameObject() {
		if (SURFACE) {
			SDL_FreeSurface(SURFACE);
		}
		if (texture) {
			SDL_DestroyTexture(texture);
		}
	}

	void FlipTexture() { // check rendergameobject
		if (HasTexture) {
			HasTexture = false;
		}
		else {
			HasTexture = true;
		}
	}

	void SetTexture(const char* imagepath) {
		HasTexture = true;
		this->IMAGEPATH = imagepath;
		SURFACE = IMG_Load(IMAGEPATH);
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
		SDL_RenderFillRect(renderer, &rect); // draws the rectangle

		if (HasTexture) {
			if (!texture && SURFACE) {
				texture = SDL_CreateTextureFromSurface(renderer, SURFACE);
				SDL_FreeSurface(SURFACE); // frees the surface after texture has been
				SURFACE = nullptr; // created
			}
			if (texture) { // if texture does exist then execute this condition
				SDL_RenderCopy(renderer, texture, nullptr, &rect);
				return; // prevents drawing over the texture with the black rectangle
			}
		}
		else { // TODO: have this be for collectible items instead of clicking save button
			if (SURFACE) {
				SDL_FreeSurface(SURFACE);
			}
			if (texture) {
				SDL_DestroyTexture(texture);
			}
		}
	}

	std::string GetName() {
		return this->NAME;
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
