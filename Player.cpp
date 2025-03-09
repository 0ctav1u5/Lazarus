#include <iostream>
#include <string>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL.h>
#include "Player.hpp"

void Player::GunCollected() {
	this->guncollected = true;
}

int Player::GetX() {
	return this->PosX;
}

int Player::GetHP() {
	return this->HP;
}

int Player::GetY() {
	return this->PosY;
}

int Player::GetPlayerWidth() {
	return this->PlayerRectWidth;
}

int Player::GetPlayerHeight() {
	return this->PlayerRectHeight;
}

int Player::GetDirection() {
	return this->DIRECTION;
}

SDL_Rect& Player::GetRect() {
	return this->PlayerPos;
}

void Player::Move(int changeX, int changeY) {
	PosX += changeX * SPEED;
	PosY += changeY * SPEED;
}

void Player::SetDirectionGraphic(int direction) {
	this->DIRECTION = direction;
}

void Player::DamagePlayer(int damage) {
	this->HP -= damage;
}


// WARNING: The rectangle size is 100x100, but the textures are 64x64
// this means that everything will be scaled up, and therefore slight differences
// in flipped images will be magnified too. this will be fixed at one point
void Player::RenderPlayer(SDL_Renderer* renderer) {
	if (!texture) {
		texture = SDL_CreateTextureFromSurface(renderer, SURFACE);
	}

	const char* newImagePath = nullptr;

	switch (DIRECTION) {
	case 1: // Left
		if (guncollected) {
			newImagePath = "Images/PlayerGunLeft.png";
			// bools for left
		}
		else {
			newImagePath = "Images/PlayerLeft.png";
		}
		break;
	case 2: // Right
		if (guncollected) {
			newImagePath = "Images/PlayerGunRight.png";
		}
		else {
			newImagePath = "Images/PlayerRight.png";
		}
		break;
	case 3: // Up
		if (guncollected) {
			newImagePath = "Images/PlayerGunUp.png";
		}
		else {
			newImagePath = "Images/PlayerUp.png";
		}
		break;
	case 4: // Down
		if (guncollected) {
			newImagePath = "Images/PlayerGunDown.png";
		}
		else {
			newImagePath = "Images/PlayerDown.png";
		}
		break;
	default:
		std::cerr << "Invalid Direction!" << std::endl;
		exit(1);
	}

	SDL_Surface* newSurface = IMG_Load(newImagePath);
	if (!newSurface) {
		std::cerr << "IMG_Load Error: " << IMG_GetError() << std::endl;
		exit(1);
	}

	// surface must be freed so a new surface can be loaded in
	if (SURFACE) {
		SDL_FreeSurface(SURFACE);
	}
	SURFACE = newSurface;

	// needed here because the old texture must be destroyed
	if (texture) {
		SDL_DestroyTexture(texture);
	}

	// need here because we have to update the surface
	texture = SDL_CreateTextureFromSurface(renderer, SURFACE);
	if (!texture) {
		std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
		return;
	}
	PlayerPos = { PosX, PosY, PlayerRectWidth, PlayerRectHeight };
	if (DEBUG_MODE) {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	}
	else {
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	}
	 // RGB make last digit 0 for transparency
	SDL_RenderFillRect(renderer, &PlayerPos);
	SDL_RenderCopy(renderer, texture, NULL, &PlayerPos);
}


void Player::RenderPlayerHP(SDL_Renderer* renderer) {
	TTF_Font* font = TTF_OpenFont("Fonts/Vipnagorgialla Rg.otf", 24);

	int localHP = this->HP;
	std::string HPstring = std::to_string(localHP);
	const char* cstr = HPstring.c_str();

	SDL_Color textColour = { 255, 0, 0 }; // red

	SDL_Texture* HPtexture = nullptr;
	SDL_Surface* HPSurface = TTF_RenderText_Solid(font, cstr, textColour); // font, text, colour

	if (HPSurface) {
		HPtexture = SDL_CreateTextureFromSurface(renderer, HPSurface);
		SDL_FreeSurface(HPSurface);
	}
	else {
		std::cerr << "Text render error: " << TTF_GetError() << std::endl;
		return;
	}

	// rectangle size determines text size
	SDL_Rect HealthBar = { 10, 440, 50, 40 }; // size of rectangle
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // colour of rectangle
	SDL_RenderFillRect(renderer, &HealthBar);
	SDL_RenderCopy(renderer, HPtexture, nullptr, &HealthBar);
}