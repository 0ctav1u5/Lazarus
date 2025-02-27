#ifndef LEVEL
#define LEVEL
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <memory>
#include "GameObject.hpp"
#include "Barrier.hpp"
#include "Enemy.hpp"

class Level {
private:
    static int LevelIDCounter; // static for counting each level
    int InstanceID = 0;            // unique ID
    std::string LevelName = "";
    const char* BackgroundImage;
    SDL_Surface* SURFACE = nullptr;
    SDL_Texture* BackgroundTexture = nullptr;
    int LEFT_BOUNDARY = 0;
    int RIGHT_BOUNDARY = 0;
    int UPPER_BOUNDARY = 0;
    int LOWER_BOUNDARY = 0;


    std::vector<std::shared_ptr<GameObject>> GameObjects; 
    std::vector<std::shared_ptr<Barrier>> Barriers;
    std::vector<std::shared_ptr<Enemy>> Enemies;

public:

    Level(std::string levelname, const char* backgroundimage, int lb, int rb, int ub, int lwb)
        : LevelName(levelname), BackgroundImage(backgroundimage), LEFT_BOUNDARY(lb),
        RIGHT_BOUNDARY(rb), UPPER_BOUNDARY(ub), LOWER_BOUNDARY(lwb)
    {
        InstanceID = LevelIDCounter++; // this increments everytime a Level Object is created
        std::cout << "Level " << InstanceID << " created!" << std::endl;
    }

    size_t GetGameObjectsCount() const;
    void RenderLevel(SDL_Renderer* renderer);
    bool MakeEnemy(std::string name, int x, int y, int width, int height, const char* enemy);
    bool MakeGameObject(std::string name, int x, int y, int width, int height, bool cancollide,
    bool candamage, bool cancollect, bool visible);
    void RemoveGameObject(int i);
    std::shared_ptr<GameObject> GetGameObject(int i);
    std::shared_ptr<Enemy> GetEnemy(int i);
    std::vector<std::shared_ptr<Enemy>>& GetEnemiesVector();
    std::shared_ptr<Barrier> GetBarrier(int i);
    size_t GetBarriersCount() const;
    bool MakeBarrier(int x, int y, int width, int height);
    int GetInstanceID() const; 
    int GetRightBoundary();
    int GetLeftBoundary();
    int GetUpperBoundary();
    int GetLowerBoundary();
    std::vector<std::shared_ptr<GameObject>>& GetGameObjectVector();
    int GetEnemiesSize();
};
#endif 
