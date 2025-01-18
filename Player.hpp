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
	const char* IMAGEPATH;
	SDL_Surface* SURFACE = nullptr;
	SDL_Rect PlayerPos = {};
	SDL_Texture* texture = nullptr;

public:

	Player(std::string& name, int posx, int posy, const char* imagepath, int speed) : NAME(name), 
		PosX(posx), PosY(posy), IMAGEPATH(imagepath), SPEED(speed) {
		SURFACE = IMG_Load(IMAGEPATH);
	}

	~Player() {

	}

	void RenderPlayer(SDL_Renderer* renderer);
	void SetDirectionGraphic(int direction);
	void Move(int changeX, int changeY);
	void PrintStats();
};
#endif