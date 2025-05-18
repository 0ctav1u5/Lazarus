#ifndef PLAYER
#define PLAYER
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>


class Player {
private:
	int HP = 100;
	std::string NAME;
	int PosX, PosY, SPEED, DIRECTION = 4;
	const int PlayerRectWidth = 100, PlayerRectHeight = 100;
	const char* IMAGEPATH;
	SDL_Surface* SURFACE = nullptr;
	SDL_Rect PlayerPos = {};
	SDL_Texture* texture = nullptr;
	bool DEBUG_MODE = false; // for the programmer only
	bool guncollected = false;

public:

	Player(std::string& name, int posx, int posy, const char* imagepath, int speed) : NAME(name), 
		PosX(posx), PosY(posy), IMAGEPATH(imagepath), SPEED(speed) {
		SURFACE = IMG_Load(IMAGEPATH);
	}
	void SetX(int x);
	void SetY(int y);
	int GetX();
	int GetY();
	int GetHP();
	int GetPlayerWidth();
	int GetPlayerHeight();
	SDL_Rect& GetRect();
	void RenderPlayer(SDL_Renderer* renderer);
	void SetDirectionGraphic(int direction);
	void Move(int changeX, int changeY);
	void DamagePlayer(int damage);
	void RenderPlayerHP(SDL_Renderer* renderer);
	void GunCollected();
	int GetDirection();
};
#endif