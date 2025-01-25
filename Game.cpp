#include <iostream>
#include <SDL_image.h>
#include <SDL.h>
#include "Game.hpp"
#include "Player.hpp"
#include "Level.hpp"

const signed int LEFT_BOUNDARY = -30;
const int RIGHT_BOUNDARY = 530;
const signed int UPPER_BOUNDARY = -4;
const int LOWER_BOUNDARY = 400;

void Game::HandleEvents(SDL_Event& e, bool& running) {
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
            running = false;
        }
    }
}

void Game::UserInput(bool& running, const Uint8* keyboardState) {
    int PlayerX = Players[0]->GetX(), PlayerY = Players[0]->GetY();
    int PlayerWidth = Players[0]->GetPlayerWidth(), PlayerHeight = Players[0]->GetPlayerHeight();

    int xLeft = -1, xRight = 1, yDown = 1, yUp = -1;

    bool blockbottom = false, blocktop = false, blockleft = false, blockright = false;


    if (GameObjects[0]->CheckBoundary(PlayerY, PlayerX, PlayerWidth, PlayerHeight) == "top") {
        blockbottom = true;
    }
    if (GameObjects[0]->CheckBoundary(PlayerY, PlayerX, PlayerWidth, PlayerHeight) == "bottom") {
        blocktop = true;
    }

    if (GameObjects[0]->CheckBoundary(PlayerY, PlayerX, PlayerWidth, PlayerHeight) == "left") {
        blockright = true;
    }

    if (GameObjects[0]->CheckBoundary(PlayerY, PlayerX, PlayerWidth, PlayerHeight) == "right") {
        blockleft = true;
    }

    if (keyboardState[SDL_SCANCODE_LEFT]) {
        if (PlayerX != LEFT_BOUNDARY && blockleft == false) {
            PlayerMove(xLeft, 0); // directions
            Players[0].get()->SetDirectionGraphic(1);
        }
        blockright = true;
        blocktop = true;
        blockbottom = true;
    }
    if (keyboardState[SDL_SCANCODE_RIGHT]) {
        if (PlayerX <= 436 && PlayerX != RIGHT_BOUNDARY && blockright == false) {
            PlayerMove(xRight, 0);
            Players[0].get()->SetDirectionGraphic(2);
        }
        blockleft = true;
        blocktop = true;
        blockbottom = true;
    }

    if (keyboardState[SDL_SCANCODE_UP]) { // there will be a checklevel function call here
        // check level will set the boundaries for where the exit is
        if (PlayerY >= UPPER_BOUNDARY && blocktop == false) {
            PlayerMove(0, yUp);
            Players[0].get()->SetDirectionGraphic(3); // walk through anywhere below that point
        }
        else if (Players[0]->GetX() > 96 && Players[0]->GetX() < 306 && blocktop == false) { // can walk through top
            PlayerMove(0, yUp);
            Players[0].get()->SetDirectionGraphic(3);
        }
        blockright = true;
        blockleft = true;
        blockbottom = true;
    }

    if (keyboardState[SDL_SCANCODE_DOWN]) {

        if (PlayerY < LOWER_BOUNDARY && blockbottom == false) { // The player graphic is 87 pixels in height
            PlayerMove(0, yDown);
            Players[0].get()->SetDirectionGraphic(4);
        }
        blockright = true;
        blocktop = true;
        blockleft = true;
    }
    if (keyboardState[SDLK_ESCAPE]) {
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

bool Game::MakeLevel(int ID, std::string levelname, const char* backgroundimagepath) {
    try {
        auto level = std::make_shared<Level>(ID, levelname, backgroundimagepath);
        Levels.push_back(std::move(level));
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Cannot make new level: " << e.what() << std::endl;
        return false;
    }
}

bool Game::MakeGameObject(int x, int y, int width, int height) {
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

std::shared_ptr<GameObject> Game::GetGameObject(int i) {

    if (GameObjects[i] != nullptr && i < GameObjects.size()) {
        return GameObjects[i];
    }
    std::cerr << "Game Objects out of bounds!" << std::endl;
    return nullptr;
}

bool Game::LoadAssets(SDL_Renderer* renderer) {
    const int PLAYERSTARTX = 190, PLAYERSTARTY = 390, PLAYERSPEED = 2;
    const int ObjectX = 150, ObjectY = 250, ObjectWidth = 100, ObjectHeight = 100;

    if (!MakePlayer("Ethan", PLAYERSTARTX, PLAYERSTARTY, "Images/PlayerDown.png", PLAYERSPEED)) { // x, y and speed for ints
        std::cerr << "Ethan not created!" << std::endl;
        return false;
    }
    if (!MakeLevel(1, "LevelOne", "Images/RoadBackground.png")) {
        std::cerr << "Couldn't create Level one!" << std::endl;
        return false;
    }
    if (!MakeGameObject(ObjectX, ObjectY, ObjectWidth, ObjectHeight)) {
        std::cerr << "Couldn't create Level one!" << std::endl;
        return false;
    }

    return true;
}