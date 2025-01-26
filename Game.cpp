#include <iostream>
#include <SDL_image.h>
#include <SDL.h>
#include "Game.hpp"
#include "Player.hpp"
#include "Level.hpp"


int Level::LevelID = 0; // initialising the levelID

const signed int LEFT_BOUNDARY = -30;
const int RIGHT_BOUNDARY = 530;
const signed int UPPER_BOUNDARY = -4;
const int LOWER_BOUNDARY = 410;

void Game::HandleEvents(SDL_Event& e, bool& running) {
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
            running = false;
        }
    }
}

void Game::UserInput(bool& running, const Uint8* keyboardState) {
    int playerX = Players[0]->GetX(), playerY = Players[0]->GetY();
    int playerWidth = Players[0]->GetPlayerWidth(), playerHeight = Players[0]->GetPlayerHeight();

    bool blockBottom = false, blockTop = false, blockLeft = false, blockRight = false;


    int LevelNum = GetLevelsCount() - 1; // tells us the position of each level in the array


    CollisionChecker(LevelNum, playerY, playerX, playerWidth, playerHeight, blockBottom, blockTop, blockRight, blockLeft);
    

    if (keyboardState[SDL_SCANCODE_LEFT]) {
        if (!blockLeft && playerX > LEFT_BOUNDARY) { 
            PlayerMove(-1, 0); // Move left
            Players[0]->SetDirectionGraphic(1); 
        }
    }

    if (keyboardState[SDL_SCANCODE_RIGHT]) {
        if (!blockRight && (playerX + playerWidth < RIGHT_BOUNDARY)) {
            PlayerMove(1, 0); // Move right
            Players[0]->SetDirectionGraphic(2); 
        }
    }

    if (keyboardState[SDL_SCANCODE_UP]) {
        if (!blockTop && (playerY >= UPPER_BOUNDARY || (playerX > 96 && playerX < 306))) {
            PlayerMove(0, -1); // Move up
            Players[0]->SetDirectionGraphic(3); 
        }
    }

    if (keyboardState[SDL_SCANCODE_DOWN]) {
        if (!blockBottom && playerY < LOWER_BOUNDARY) {
            PlayerMove(0, 1); // Move down
            Players[0]->SetDirectionGraphic(4); 
        }
    }

    // Quit game
    if (keyboardState[SDL_SCANCODE_ESCAPE]) {
        SDL_Quit();
    }
}

void Game::PlayerMove(int x, int y) {
    Players[0].get()->Move(x, y);
}

bool Game::MakePlayer(std::string name, int posx, int posy, const char* imagepath, int speed) {
    try {
        auto player = std::make_shared<Player>(name, posx, posy, imagepath, speed);
        Players.push_back(std::move(player)); // pushing it onto the vector needs transfer of ownership
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Cannot make new player: " << e.what() << std::endl;
        return false;
    }
}

bool Game::MakeLevel(std::string levelname, const char* backgroundimagepath) {
    try {
        auto level = std::make_shared<Level>(levelname, backgroundimagepath);
        Levels.push_back(std::move(level));
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Cannot make new level: " << e.what() << std::endl;
        return false;
    }
}

std::shared_ptr<Player> Game::GetPlayer(int i) {

    if (Players[i] != nullptr && i < Players.size()) {
        return Players[i];
    }
    std::cerr << "Player out of bounds!" << std::endl;
    return nullptr;
}

std::shared_ptr<Level> Game::GetLevel(int i) {

    if (Levels[i] != nullptr && i < Levels.size()) {
        return Levels[i];
    }
    std::cerr << "Level out of bounds!" << std::endl;
    return nullptr;
}

size_t Game::GetLevelsCount() const {
    return Levels.size();
}

bool Game::LoadAssets(SDL_Renderer* renderer) {
    const int PLAYERSTARTX = 190, PLAYERSTARTY = 390, PLAYERSPEED = 2;
    const int ObjectX = -10, ObjectY = -100, ObjectWidth = 100, ObjectHeight = 100;

    if (!MakePlayer("Ethan", PLAYERSTARTX, PLAYERSTARTY, "Images/PlayerDown.png", PLAYERSPEED)) { // x, y and speed for ints
        std::cerr << "Ethan not created!" << std::endl;
        return false;
    }
    // 0 is level 1, as it's the first in the vector
    if (!MakeLevel("LevelOne", "Images/RoadBackground.png")) {
        std::cerr << "Couldn't create Level one!" << std::endl;
        return false;
    }
    if (!Levels[0]->MakeGameObject(ObjectX, ObjectY, ObjectWidth, ObjectHeight)) {
        std::cerr << "Couldn't create Game Object!" << std::endl;
        return false;
    }

    if (!Levels[0]->MakeGameObject(400, ObjectY, ObjectWidth, ObjectHeight)) {
        std::cerr << "Couldn't create Game Object!" << std::endl;
        return false;
    }

    return true;
}


void Game::CollisionChecker(int levelnum, int playerY, int playerX, int playerWidth, int playerHeight,
    bool& blockBottom, bool& blockTop, bool& blockRight, bool& blockLeft) {


    

    for (size_t i = 0; i < Levels[levelnum]->GetGameObjectsCount(); ++i) {
        auto gameObject = Levels[levelnum]->GetGameObject(i); // get all game objects in level[i]
        if (!gameObject) {
            continue;
        }

        std::string boundaryCheck = gameObject->CheckBoundary(playerY, playerX, playerWidth, playerHeight);

        if (boundaryCheck == "top") {
            blockBottom = true;
        }
        if (boundaryCheck == "bottom") {
            blockTop = true;
        }
        if (boundaryCheck == "left") {
            blockRight = true;
        }
        if (boundaryCheck == "right") {
            blockLeft = true;
        }
    }
}