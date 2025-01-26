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

	// game objects for rendering
	for (auto& gameobject : GameObjects) {
		gameobject->RenderGameObject(renderer);
	}
}

std::shared_ptr<GameObject> Level::GetGameObject(int i) {
	if (GameObjects[i] != nullptr && i < GameObjects.size()) {
		return GameObjects[i];
	}
	std::cerr << "Game Objects out of bounds!" << std::endl;
	return nullptr;
}

size_t Level::GetGameObjectsCount() const {
	return GameObjects.size();
}

bool Level::MakeGameObject(int x, int y, int width, int height) {
	try {
		auto object = std::make_shared<GameObject>(x, y, width, height);
		GameObjects.push_back(std::move(object));
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Cannot make new object: " << e.what() << std::endl;
		return false;
	}
}














