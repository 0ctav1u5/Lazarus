#ifndef LEVEL
#define LEVEL
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "GameObject.hpp"

class Level {
private:
    static int LevelIDCounter; // static for counting each level
    int InstanceID;            // unique ID
    std::string LevelName = "";
    const char* BackgroundImage;
    SDL_Surface* SURFACE = nullptr;
    SDL_Texture* BackgroundTexture = nullptr;
    std::vector<std::shared_ptr<GameObject>> GameObjects; 

public:
    Level(std::string levelname, const char* backgroundimage)
        : LevelName(levelname), BackgroundImage(backgroundimage) {
        InstanceID = LevelIDCounter++; 
    }

    size_t GetGameObjectsCount() const;
    void RenderLevel(SDL_Renderer* renderer);
    bool MakeGameObject(int x, int y, int width, int height);
    std::shared_ptr<GameObject> GetGameObject(int i);
    int GetInstanceID() const; 
};
#endif 
