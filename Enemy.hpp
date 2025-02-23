#ifndef ENEMY
#define ENEMY
#include <iostream>
#include <SDL.h>


// TODO: Update this class to have the square follow the player


class Enemy {
private:
	int HP = 100;
	std::string NAME = "";
	SDL_Rect rect = {};
	SDL_Surface* SURFACE = nullptr;
	SDL_Texture* texture = nullptr;
	int SPEED = 0;
	bool following = false;
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

		// updaterect function
		SDL_RenderFillRect(renderer, &rect); // draws the rectangle
		if (SURFACE) {
			texture = SDL_CreateTextureFromSurface(renderer, SURFACE);
			SDL_FreeSurface(SURFACE); // frees the surface after texture has been assigned
			SURFACE = nullptr;
		}
	}


	void MoveEnemy(int x, int y) {

	}

	void FollowPlayer() {
		this->following = true;
	}

	std::string GetEnemyName() {
		return this->NAME;
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