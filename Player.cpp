#include <iostream>
#include "Player.hpp"



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

void Player::Move(int changeX, int changeY) {
	PosX += changeX * SPEED;
	PosY += changeY * SPEED;
}

void Player::SetDirectionGraphic(int direction) {
	this->DIRECTION = direction;
}

void Player::DamagePlayer(int damage) {
	this->HP -= damage;
	std::cout << "Player HP: " << this->HP << std::endl;
}

void Player::RenderPlayer(SDL_Renderer* renderer) {
	if (!texture) {
		texture = SDL_CreateTextureFromSurface(renderer, SURFACE);
	}

	const char* newImagePath = nullptr;

	switch (DIRECTION) {
	case 1: // Left
		newImagePath = "Images/PlayerLeft.png";
		break;
	case 2: // Right
		newImagePath = "Images/PlayerRight.png";
		break;
	case 3: // Up
		newImagePath = "Images/PlayerUp.png";
		break;
	case 4: // Down
		newImagePath = "Images/PlayerDown.png";
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
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); 
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // RGB make last digit 0 for transparency
	SDL_RenderFillRect(renderer, &PlayerPos);
	SDL_RenderCopy(renderer, texture, NULL, &PlayerPos);
}
