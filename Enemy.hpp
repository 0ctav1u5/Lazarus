#ifndef ENEMY
#define ENEMY
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <random>


// TODO: Update this class to have the square follow the player


class Enemy {
private:
	int HP = 80;
	std::string NAME = "";
	const char* IMAGEPATH;
	SDL_Rect rect = {};
	SDL_Surface* SURFACE = nullptr;
	SDL_Texture* texture = nullptr;
	int SPEED = 1;
	int X, Y, WIDTH, HEIGHT;
	bool STATIONARY = false;
public:

	Enemy(std::string name, int x, int y, int width, int height, const char* imagepath) :
		NAME(name), X(x), Y(y), WIDTH(width), HEIGHT(height), IMAGEPATH(imagepath)
	{
		rect = { X, Y, WIDTH, HEIGHT };
		SURFACE = IMG_Load(IMAGEPATH);
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
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // RGB

		// updaterect function
		SDL_RenderFillRect(renderer, &rect); // draws the rectangle
		SDL_RenderCopy(renderer, texture, NULL, &rect); // draws texture
		if (SURFACE) {
			texture = SDL_CreateTextureFromSurface(renderer, SURFACE);
			SDL_FreeSurface(SURFACE); // frees the surface after texture has been assigned
			SURFACE = nullptr;
		}
	}

	// this includes following player as the natural state of the enemy is to follow the player
	// TODO: Make Enemy movement more random
	void MoveEnemy(int PlayerX, int PlayerY) {

		if (PlayerX > rect.x) {
			rect.x += 1 * SPEED;
		}
		else if (PlayerX < rect.x) {
			rect.x -= 1 * SPEED;
		}
		if (PlayerY > rect.y) {
			rect.y += 1 * SPEED;
		}
		else if (PlayerY < rect.y) {
			rect.y -= 1 * SPEED;
		}
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

	void SetEnemySpeed(int speed) {
		this->SPEED = speed;
	}

	void SetEnemyStationary(bool stationary) {
		this->STATIONARY = stationary;
	}

	SDL_Rect& GetRect() {
		return this->rect;
	}

	int GetX() {
		return this->X;
	}

	int GetY() {
		return this->Y;
	}

	int GetWidth() {
		return this->WIDTH;
	}

	int GetHeight() {
		return this->HEIGHT;
	}

	bool& GetStationary() {
		return this->STATIONARY;
	}
};

#endif 