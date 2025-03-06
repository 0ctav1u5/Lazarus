#include <iostream>
#include <SDL.h>
#include "Enemy.hpp"

void Enemy::RenderEnemy(SDL_Renderer* renderer) {
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
void Enemy::MoveEnemy(int PlayerX, int PlayerY) {

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

std::string Enemy::GetEnemyName() {
	return this->NAME;
}

int Enemy::GetEnemyHP() {
	return this->HP;
}

void Enemy::DamageEnemy(int damage) {
	this->HP -= damage;
}

void Enemy::SetEnemySpeed(int speed) {
	this->SPEED = speed;
}

void Enemy::SetEnemyStationary(bool stationary) {
	this->STATIONARY = stationary;
}

SDL_Rect& Enemy::GetRect() {
	return this->rect;
}

int Enemy::GetX() {
	return this->X;
}

int Enemy::GetY() {
	return this->Y;
}

int Enemy::GetWidth() {
	return this->WIDTH;
}

int Enemy::GetHeight() {
	return this->HEIGHT;
}

bool& Enemy::GetStationary() {
	return this->STATIONARY;
}


