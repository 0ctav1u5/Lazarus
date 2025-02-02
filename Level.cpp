#include "Level.hpp"
#include "GameObject.hpp"

void Level::RenderLevel(SDL_Renderer* renderer) { // this will be used to render the level
	SDL_Surface* surface = IMG_Load(BackgroundImage);
	if (!surface) {
		std::cerr << "IMG_Load Error: " << IMG_GetError() << std::endl;
		return;
	}

	BackgroundTexture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	if (!BackgroundTexture) {
		std::cerr << "SDL_CreateTexture Error: " << SDL_GetError() << std::endl;
		return;
	}
	SDL_RenderCopy(renderer, BackgroundTexture, NULL, NULL);
	SDL_DestroyTexture(BackgroundTexture);

	// this is where all the game objects are rendered for the level
	for (auto& gameobject : GameObjects) {
		gameobject->RenderGameObject(renderer);
	}
}

std::shared_ptr<GameObject> Level::GetGameObject(int i) {
	if (i >= 0 && i < GameObjects.size()) {
		return GameObjects[i];
	}
	std::cerr << "Game Object index out of bounds: " << i << std::endl;
	return nullptr;
}

size_t Level::GetGameObjectsCount() const {
	return GameObjects.size();
}

bool Level::MakeGameObject(int x, int y, int width, int height, bool cancollide,
	bool candamage, bool cancollect, bool visible) {
	try {
		auto object = std::make_shared<GameObject>(x, y, width, height, cancollide, candamage, 
		cancollect, visible);
		GameObjects.push_back(object);
		return true;
	}

	catch (const std::exception& e) {
		std::cerr << "Cannot make new object: " << e.what() << std::endl;
		return false;
	}
}

int Level::GetLeftBoundary() {
	return this->LEFT_BOUNDARY;
}

int Level::GetRightBoundary() {
	return this->RIGHT_BOUNDARY;
}

int Level::GetUpperBoundary() {
	return this->UPPER_BOUNDARY;
}

int Level::GetLowerBoundary() {
	return this->LOWER_BOUNDARY;
}



int Level::GetInstanceID() const {
	return InstanceID; // The unique identifier for this Level instance
}














