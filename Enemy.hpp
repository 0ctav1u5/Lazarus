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

	void RenderEnemy(SDL_Renderer* renderer);
	void MoveEnemy(int PlayerX, int PlayerY);
	std::string GetEnemyName();
	int GetEnemyHP();
	void DamageEnemy(int damage);
	void SetEnemySpeed(int speed);
	void SetEnemyStationary(bool stationary);
	SDL_Rect& GetRect();
	int GetX();
	int GetY();
	int GetWidth();
	int GetHeight();
	bool& GetStationary();
};

#endif 