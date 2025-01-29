#include <iostream>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL.h>
#include "Game.hpp"
#include "Player.hpp"
#include "Level.hpp"


int Level::LevelIDCounter = 0;

const signed int LEFT_BOUNDARY = -30;
const int RIGHT_BOUNDARY = 530;
const signed int UPPER_BOUNDARY = -4;
const int LOWER_BOUNDARY = 410;

void Game::HandleEvents(SDL_Event& e, bool& running, SDL_Renderer* renderer) {
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
            running = false;
        }

        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
            PauseMenu(renderer);
        }
    }
}

void Game::PauseMenu(SDL_Renderer* renderer) {
    // Load font
    TTF_Font* font = TTF_OpenFont("Fonts/Pixel Game.otf", 24);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return;  // Exit the function if font fails to load
    }
    else {
        std::cout << "Font loaded successfully!" << std::endl;
    }

    bool pauseloop = true;
    SDL_Event p;

    SDL_Texture* textTexture = nullptr;
    SDL_Color textColour = { 255, 255, 255 }; // white color for text
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Paused", textColour); // font, text, colour
    if (textSurface) {
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FreeSurface(textSurface);  
    }
    else {
        std::cerr << "Text render error: " << TTF_GetError() << std::endl;
        return; // Return early if there's an error creating the text texture
    }

    while (pauseloop) {
        // Handle events
        while (SDL_PollEvent(&p)) {
            if (p.type == SDL_KEYDOWN && p.key.keysym.sym == SDLK_SPACE) {
                pauseloop = false;  // Exit the pause menu when space is pressed
            }
        }

        // clears buffer and adds overlay in alpha values
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); 
        // sets draw colour for new overlay
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);  // r, g, b, opaqueness
        SDL_Rect rect = { 0, 0, 500, 500 }; // rectangle for text x, y, width, height
        SDL_RenderFillRect(renderer, &rect); // render the rectangle

        SDL_Rect textRect = { 140, 200, 200, 50 };  // position of rectangle of text
        SDL_RenderCopy(renderer, textTexture, nullptr, &textRect); // renders texture and rect
        // together

        SDL_RenderPresent(renderer);
    } // end of while loop


    SDL_DestroyTexture(textTexture);  // destroy the texture after it's been used
    TTF_CloseFont(font);  // for closing font when function ends
}

void Game::UserInput(bool& running, const Uint8* keyboardState) {
    int playerX = Players[0]->GetX(), playerY = Players[0]->GetY();
    int playerWidth = Players[0]->GetPlayerWidth(), playerHeight = Players[0]->GetPlayerHeight();

    bool blockBottom = false, blockTop = false, blockLeft = false, blockRight = false;

    // LevelNum used for collisionchecker of objects
    int LevelNum = GetLevelsCount() - 1; // tells us the position of each level in the array


    CollisionChecker(LevelNum, playerY, playerX, playerWidth, playerHeight, blockBottom, blockTop, blockRight, blockLeft);
    

    if (keyboardState[SDL_SCANCODE_LEFT]) {
        if (!blockLeft && playerX > LEFT_BOUNDARY) { 
            PlayerMove(-1, 0); // Move left
            Players[0]->SetDirectionGraphic(1); 
        }
    }

    // left up
    // left down

    if (keyboardState[SDL_SCANCODE_RIGHT]) {
        if (!blockRight && (playerX + playerWidth < RIGHT_BOUNDARY)) {
            PlayerMove(1, 0); // Move right
            Players[0]->SetDirectionGraphic(2); 
        }
    }

    // right up
    // right down

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
}


void Game::ChangeLevel(SDL_Renderer* renderer, int& LevelID) {
    const int ObjectX = -10, ObjectY = -100, ObjectWidth = 100, ObjectHeight = 100;
        if (Players[0]->GetY() == -90) {
            if (!MakeLevel("LevelTwo", "Images/Grass.png", LevelID)) {
                std::cerr << "Couldn't create Level one!" << std::endl;
                return;
            }
            if (!Levels[LevelID]->MakeGameObject(ObjectX, ObjectY, ObjectWidth, ObjectHeight)) {
                std::cerr << "Couldn't create Game Object!" << std::endl;
                return;
            }

            if (!Levels[LevelID]->MakeGameObject(400, ObjectY, ObjectWidth, ObjectHeight)) {
                std::cerr << "Couldn't create Game Object!" << std::endl;
                return;
            }
            PlayerMove(0, 260); // resets spawn point to bottom of map
        }
    return;
}

void Game::PlayerMove(int x, int y) {
    Players[0].get()->Move(x, y);
}

bool Game::MakePlayer(std::string name, int posx, int posy, const char* imagepath, int speed) {
    try {
        auto player = std::make_shared<Player>(name, posx, posy, imagepath, speed);
        Players.push_back(std::move(player)); 
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Cannot make new player: " << e.what() << std::endl;
        return false;
    }
}

bool Game::MakeLevel(std::string levelname, const char* backgroundimagepath, int& LevelID) {
    try {
        auto level = std::make_shared<Level>(levelname, backgroundimagepath);
        Levels.push_back(std::move(level)); 
        LevelID = Levels.size() - 1;       
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
    if (i >= 0 && i < Levels.size()) {
        return Levels[i];
    }
    std::cerr << "Level index out of bounds!" << std::endl;
    return nullptr;
}

size_t Game::GetLevelsCount() const {
    return Levels.size();
}

bool Game::LoadAssets(SDL_Renderer* renderer, int& LevelID) {
    const int PLAYERSTARTX = 190, PLAYERSTARTY = 390, PLAYERSPEED = 2;
    const int ObjectX = -10, ObjectY = -100, ObjectWidth = 100, ObjectHeight = 100;

    if (!MakePlayer("Ethan", PLAYERSTARTX, PLAYERSTARTY, "Images/PlayerDown.png", PLAYERSPEED)) {
        std::cerr << "Ethan not created!" << std::endl;
        return false;
    }

    // Create Level One and update LevelID
    if (!MakeLevel("LevelOne", "Images/RoadBackground.png", LevelID)) {
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