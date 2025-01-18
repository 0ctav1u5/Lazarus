#include <iostream>
#include <SDL_image.h>
#include <SDL.h>
#include "Game.hpp"
#include "Player.hpp"
// C:\\Users\\jed12\\Desktop\\example.png


void Game::DrawBackground(SDL_Renderer* renderer) {
    const char* imagePath = "Images/RoadBackground.png";
    SDL_Surface* surface = IMG_Load(imagePath);

    if (!surface) {
        std::cerr << "IMG_Load Error: " << IMG_GetError() << std::endl;
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);  

    if (!texture) {
        std::cerr << "SDL_CreateTexture Error: " << SDL_GetError() << std::endl;
        return;
    }
    SDL_RenderCopy(renderer, texture, NULL, NULL);            
    SDL_DestroyTexture(texture);  
}



void Game::UserInput(SDL_Event& e, bool& running, const Uint8* keyboardState, SDL_Renderer* renderer) {
    if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
        running = false;
    }


    if (keyboardState[SDL_SCANCODE_LEFT]) { 
        Players[0].get()->Move(-1, 0); // directions
        Players[0].get()->SetDirectionGraphic(1);
    }

    if (keyboardState[SDL_SCANCODE_RIGHT]) {
        Players[0].get()->Move(1, 0);
        Players[0].get()->SetDirectionGraphic(2);
    }

    if (keyboardState[SDL_SCANCODE_UP]) {
        Players[0].get()->Move(0, -1);
        Players[0].get()->SetDirectionGraphic(3);
    }

    if (keyboardState[SDL_SCANCODE_DOWN]) {
        Players[0].get()->Move(0, 1);
        Players[0].get()->SetDirectionGraphic(4);
    }

}
 

// const std::string NAME;
// const char* PLAYERMODELPATH = nullptr;

bool Game::MakePlayer(std::string name, int posx, int posy, const char* imagepath, int speed) {
    try {
        auto player = std::make_unique<Player>(name, posx, posy, imagepath, speed);
        Players.push_back(std::move(player));  
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Cannot make new player: " << e.what() << std::endl;
        return false;
    }
}

bool Game::MakeLevel() {
    return true;
}

Player* Game::GetPlayer(int i) {
    return Players[i].get();
}



bool Game::LoadAssets(SDL_Renderer* renderer) {
    // load main character
    if (!MakePlayer("Atom", 150, 250, "Images/PlayerDown.png", 10)) { // x, y and speed for ints
        std::cerr << "Atom not created!" << std::endl;
        return false;
    }
	return true;
}