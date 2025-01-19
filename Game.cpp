#include <iostream>
#include <SDL_image.h>
#include <SDL.h>
#include "Game.hpp"
#include "Player.hpp"
#include "Level.hpp"

void Game::UserInput(SDL_Event& e, bool& running, const Uint8* keyboardState, SDL_Renderer* renderer) {
    if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
        running = false;
    }





    if (e.type == SDL_KEYDOWN) {
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

bool Game::MakeLevel(int ID, std::string levelname, const char* backgroundimagepath) {
    try {
        auto level = std::make_unique<Level>(ID, levelname, backgroundimagepath);
        Levels.push_back(std::move(level));
        return true;
    } 
    catch (const std::exception& e) {
        std::cerr << "Cannot make new level: " << e.what() << std::endl;
        return false;
    }
}

Player* Game::GetPlayer(int i) {
    return Players[i].get();
}

Level* Game::GetLevel(int i) {
    return Levels[i].get();
}


bool Game::LoadAssets(SDL_Renderer* renderer) {
    int playerstartX = 190, playerstartY = 390, playerspeed = 8;



    if (!MakePlayer("Ethan", playerstartX, playerstartY, "Images/PlayerDown.png", playerspeed)) { // x, y and speed for ints
        std::cerr << "Ethan not created!" << std::endl;
        return false;
    }
    if (!MakeLevel(1, "LevelOne", "Images/RoadBackground.png")) {
        std::cerr << "Couldn't create Level one!" << std::endl;
        return false;
    }
	return true;
}