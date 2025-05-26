#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "GameObject.hpp"

void GameObject::FlipTexture() {
	if (HasTexture) {
		HasTexture = false;
	}
	else {
		HasTexture = true;
	}
}

void GameObject::SetTexture(const char* imagepath) { 
	HasTexture = true; // sets texture for the game object
	this->IMAGEPATH = imagepath;
	SURFACE = IMG_Load(IMAGEPATH);
}

void GameObject::RenderGameObject(SDL_Renderer* renderer) {
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

std::string GameObject::GetName() {
	return this->NAME;
}

bool GameObject::GetCanDamage() { // getters
	return this->CanDamage;
}

bool GameObject::GetCanCollect() {
	return this->CanCollect;
}

int GameObject::GetX() {
	return this->X;
}

int GameObject::GetY() {
	return this->Y;
}

int GameObject::GetGameObjectWidth() {
	return this->WIDTH;
}

int GameObject::GetGameObjectHeight() {
	return this->HEIGHT;
}

SDL_Rect& GameObject::GetRect() {
	return this->rect;
}

bool GameObject::CheckCollidable() {
	if (CanCollide) {
		return true;
	}
	else {
		return false;
	}
}


int GameObject::GetXU() {
	return this->XUrange;
}

int GameObject::GetXL() {
	return this->XLrange;
}

int GameObject::GetYU() {
	return this->YUrange;
}

int GameObject::GetYL() {
	return this->YLrange;
}

std::string GameObject::CheckBoundary(int PlayerY, int PlayerX, int PlayerWidth, int PlayerHeight) {

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