#ifndef ENEMY
#define ENEMY
#include <iostream>
#include <SDL.h>

class Enemy {
private:
	int HP = 100;
	std::string NAME = "";
	SDL_Rect rect = {};
	SDL_Surface* SURFACE = nullptr;
	SDL_Texture* texture = nullptr;
	int X, Y, WIDTH, HEIGHT;
public:

	Enemy(std::string name, int x, int y, int width, int height) :
		NAME(name), X(x), Y(y), WIDTH(width), HEIGHT(height) 
	{
		rect = { X, Y, WIDTH, HEIGHT };
	}

	~Enemy() {
		if (SURFACE) {
			SDL_FreeSurface(SURFACE);
		}
		if (texture) {
			SDL_DestroyTexture(texture);
		}
	}

	void RenderEnemy(SDL_Renderer* renderer) {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // RGB
		SDL_RenderFillRect(renderer, &rect); // draws the rectangle
		if (SURFACE) {
			texture = SDL_CreateTextureFromSurface(renderer, SURFACE);
			SDL_FreeSurface(SURFACE); // frees the surface after texture has been
			SURFACE = nullptr;
		}
	}

	int GetEnemyHP() {
		return this->HP;
	}

	void DamageEnemy(int damage) {
		this->HP -= damage;
	}

	SDL_Rect& GetRect() {
		return this->rect;
	}
};

#endif 