#include "Level.hpp"
#include "GameObject.hpp"
#include "Barrier.hpp"
#include <vector>

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

	// destroys enemy if enemies HP is less than or equal to 0, does this for all enemies
	// iterator goes through the vector of shared pointers
	// if the shared pointer points to an object with enemy HP lower than 0
	// then dereference the pointer, meaning remove the object being pointed to
	for (auto it = Enemies.begin(); it != Enemies.end(); ) {
		if ((*it)->GetEnemyHP() <= 0) {
			it = Enemies.erase(it); 
		}
		else {
			++it; // go to the next element in the array if there's no iteration
		}
	}

	// renders gameobjects, barriers and enemies if the vector sizes are above 0

	if (GameObjects.size() > 0) {
		for (auto& gameobject : GameObjects) {
			gameobject->RenderGameObject(renderer);
		}
	}
	if (Barriers.size() > 0) {
		for (auto& barrier : Barriers) {
			barrier->RenderBarrier(renderer);
		}
	}
	if (Enemies.size() > 0) {
		for (auto& enemy : Enemies) {
			enemy->RenderEnemy(renderer);
		}
	}
}

std::vector<std::shared_ptr<GameObject>>& Level::GetGameObjectVector() {
	return this->GameObjects;
}

std::vector<std::shared_ptr<Barrier>>& Level::GetBarrierVector() {
	return this->Barriers;
}

std::shared_ptr<Enemy> Level::GetEnemy(int i) {
	if (i >= 0 && i < Enemies.size()) {
		return Enemies[i];
	}
	std::cerr << "Enemy is out of bounds: " << i << std::endl;
	return nullptr;
}

int Level::GetEnemiesSize() {
	return this->Enemies.size();
}

// returns address to enemies vector
std::vector<std::shared_ptr<Enemy>>& Level::GetEnemiesVector() {
	return this->Enemies;
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

bool Level::MakeEnemy(std::string name, int x, int y, int width, int height, const char* enemy) {
	try {
		auto object = std::make_shared<Enemy>(name, x, y, width, height, enemy);
		Enemies.push_back(object);
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Cannot make new enemy: " << e.what() << std::endl;
		return false;
	}
}

bool Level::MakeGameObject(std::string name, int x, int y, int width, int height, bool cancollide,
	bool candamage, bool cancollect, bool visible, int xurange, int xlrange, int yurange, int ylrange) {
	try {
		auto object = std::make_shared<GameObject>(name, x, y, width, height, cancollide, candamage, 
		cancollect, visible, xurange, xlrange, yurange, ylrange);
		GameObjects.push_back(object);
		return true;
	}

	catch (const std::exception& e) {
		std::cerr << "Cannot make new object: " << e.what() << std::endl;
		return false;
	}
}

void Level::RemoveGameObject(int i) { // now call this
		GameObjects.erase(GameObjects.begin() + i);
}

std::shared_ptr<Barrier> Level::GetBarrier(int i) {
	if (i >= 0 && i < Barriers.size()) {
		return Barriers[i];
	}
	std::cerr << "Barrier index out of bounds: " << i << std::endl;
	return nullptr;
}

size_t Level::GetBarriersCount() const {
	return Barriers.size();
}

bool Level::MakeBarrier(int x, int y, int width, int height) {
	try {
		auto object = std::make_shared<Barrier>(x, y, width, height);
		Barriers.push_back(object);
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














