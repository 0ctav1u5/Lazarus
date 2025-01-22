#include <iostream>
#include <SDL.h>
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

    // assets loaded here
    game = std::make_unique<Game>();
    if (!game->LoadAssets(renderer)) {
        Cleanup("Error. Failed to load game assets!");
        return false;
    }
    return true;
}


void GameEngine::GameLoop() {
    bool running = true;
    SDL_Event e;
    const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);

    // levelnum 1, 2, 3 can be passed into userinput which a switch statement can use to decide
    // boundaries

    while (running) {
        game->HandleEvents(e, running);
        game->UserInput(running, keyboardState);
        

        // Rendering needs to be changed for different levels
        // For instance, a conditional statement which says, render level 1, 2...etc
        // through getlevel(0-...)
        SDL_RenderClear(renderer);
        game->GetLevel(0)->RenderLevel(renderer); // renderers get drawn on top of eachother
        game->GetPlayer(0)->RenderPlayer(renderer); // order matters
        game->GetGameObject(0)->RenderGameObject(renderer);
        
        SDL_RenderPresent(renderer);
    }
}


void GameEngine::Cleanup(const std::string& errormsg) {
    std::cerr << errormsg << std::endl;
}