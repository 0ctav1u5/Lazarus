#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "GameEngine.hpp"
#include "Game.hpp"
#include "Player.hpp"

// START OF DEV BRANCH


bool GameEngine::Initialise() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        Cleanup("Error. Failed to initialise!");
        return false;
    }

    window = SDL_CreateWindow(m_WINDOW_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        m_WINDOW_WIDTH, m_WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        Cleanup("Error. Failed to create a window!");
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        Cleanup("Error. Graphics could not be rendered!");
        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    if (TTF_Init() == -1) {
        std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
        return false;
    }

    game = std::make_unique<Game>();
    if (!game->LoadAssets(renderer, LevelID)) {
        Cleanup("Error. Failed to load game assets!");
        return false;
    }

    return true;
}

void GameEngine::GameLoop() {
    bool running = true;
    SDL_Event e;
    const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);
    
    const int FPS = 70; // increase to improve framerate
    const int frameDelay = 1000 / FPS;  // maximum amount of time per frame

    Uint32 frameStart;
    int frameTime;


    while (running) {
        frameStart = SDL_GetTicks(); // start of frame
        game->HandleEvents(e, running, renderer);
        game->UserInput(running, keyboardState, LevelID);
        game->CheckPlayerStatus(LevelID, running, renderer);
        

        // CheckLevelID(); uncomment when debugging

        SDL_RenderClear(renderer);
        game->GetLevel(LevelID)->RenderLevel(renderer);
        game->GetPlayer(0)->RenderPlayer(renderer); 
        game->GetPlayer(0)->RenderPlayerHP(renderer);
        game->ChangeLevel(LevelID);
        SDL_RenderPresent(renderer);
        game->DisplayMessages(renderer);
        frameTime = SDL_GetTicks() - frameStart;  // time taken for this frame to have been rendered
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);  // delay to maintain a consistent fps
        }
    }
    std::cout << "Game Over!" << std::endl;
}

void GameEngine::CheckLevelID() {
    if (LevelID >= 0 && LevelID < game->GetLevelsCount()) {
        std::cout << "Current LevelID (index): " << LevelID
            << ", InstanceID: " << game->GetLevel(LevelID)->GetInstanceID() << std::endl;
    }
    else {
        std::cerr << "Invalid LevelID: " << LevelID << std::endl;
        LevelID = 0; // reset to first level
    }
}

void GameEngine::Cleanup(const std::string& errormsg) {
    std::cerr << errormsg << std::endl;
}