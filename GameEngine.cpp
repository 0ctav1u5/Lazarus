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

    while (running) {
        // both of these must be drawn to the screen at all times
        SDL_RenderClear(renderer);
        game->GetLevel(0)->RenderLevel(renderer);
        game->GetPlayer(0)->RenderPlayer(renderer);
        SDL_RenderPresent(renderer);
        while (SDL_PollEvent(&e) != 0) {
            game->UserInput(e, running, keyboardState, renderer);
        }
    }
}


void GameEngine::Cleanup(const std::string& errormsg) {
    std::cerr << errormsg << std::endl;
}