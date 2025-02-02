#include <iostream>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL.h>
#include "Game.hpp"
#include "Player.hpp"
#include "Level.hpp"

int Level::LevelIDCounter = 0;

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
    TTF_Font* font = TTF_OpenFont("Fonts/Vipnagorgialla Rg.otf", 24);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return;  
    }
    else {
        std::cout << "Game Paused!" << std::endl;
    }

    bool pauseloop = true;
    SDL_Event p;

    SDL_Texture* textTexture = nullptr;
    SDL_Texture* textTexture2 = nullptr;
    SDL_Color textColour = { 255, 0, 0 }; // white color for text
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Paused", textColour); // font, text, colour
    SDL_Surface* textSurface2 = TTF_RenderText_Solid(font, "Save", textColour); // font, text, colour
    if (textSurface) {
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FreeSurface(textSurface);  
    }
    else {
        std::cerr << "Text render error: " << TTF_GetError() << std::endl;
        return; 
    }

    if (textSurface2) {
        textTexture2 = SDL_CreateTextureFromSurface(renderer, textSurface2);
        SDL_FreeSurface(textSurface2);
    }
    else {
        std::cerr << "Text render error: " << TTF_GetError() << std::endl;
        return;
    }

    while (pauseloop) {
        // Handle events
        while (SDL_PollEvent(&p)) {
            if (p.type == SDL_KEYDOWN && p.key.keysym.sym == SDLK_SPACE) {
                pauseloop = false;  
            }
        }

        // clears buffer and adds overlay in alpha values
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); 
        // sets draw colour for new overlay
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);  // r, g, b, opaqueness


        SDL_Rect textRect = { 140, 220, 200, 50 };  // drawing the transparent background rect
        SDL_Rect textRect2 = { 10, 0, 100, 50 };  
        SDL_RenderCopy(renderer, textTexture, nullptr, &textRect); // renders texture and rect


        // clears buffer and adds overlay in alpha values
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        // sets draw colour for new overlay
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);  // r, g, b, opaqueness


        SDL_RenderCopy(renderer, textTexture2, nullptr, &textRect2); // renders texture and rect
        // together

        SDL_RenderPresent(renderer);
    } // end of while loop


    SDL_DestroyTexture(textTexture);  // destroy the texture after it's been used
    TTF_CloseFont(font);  // for closing font when function ends
}


// we'll use LevelID here to determine which level, for setting boundaries
// boundaries will be a struct, and we'll have an array
// we'll have boundaries as a member of the level class
void Game::UserInput(bool& running, const Uint8* keyboardState, int& LevelID) {
    int playerX = Players[0]->GetX(), playerY = Players[0]->GetY();
    int playerWidth = Players[0]->GetPlayerWidth(), playerHeight = Players[0]->GetPlayerHeight();

    bool blockBottom = false, blockTop = false, blockLeft = false, blockRight = false;

    // LevelNum used for collisionchecker of objects
    int LevelNum = GetLevelsCount() - 1; // tells us the position of each level in the array


    CollisionChecker(LevelNum, playerY, playerX, playerWidth, playerHeight, blockBottom, blockTop, blockRight, blockLeft);
    
    // for instance, it would be level[i]->LEFT_BOUNDARY
    if (keyboardState[SDL_SCANCODE_LEFT]) {
        if (!blockLeft && playerX > Levels[LevelID]->GetLeftBoundary()) {
            PlayerMove(-1, 0); // Move left
            Players[0]->SetDirectionGraphic(1); 
        }
    }

    if (keyboardState[SDL_SCANCODE_RIGHT]) {
        if (!blockRight && (playerX + playerWidth < Levels[LevelID]->GetRightBoundary())) {
            PlayerMove(1, 0); // Move right
            Players[0]->SetDirectionGraphic(2); 
        }
    }

    // || (playerX > 96 && playerX < 306) for below

    if (keyboardState[SDL_SCANCODE_UP]) {
        if (!blockTop && (playerY >= Levels[LevelID]->GetUpperBoundary())) {
            PlayerMove(0, -1); // Move up
            Players[0]->SetDirectionGraphic(3); 
        }
    }

    if (keyboardState[SDL_SCANCODE_DOWN]) {
        if (!blockBottom && playerY < Levels[LevelID]->GetLowerBoundary()) {
            PlayerMove(0, 1); // Move down
            Players[0]->SetDirectionGraphic(4); 
        }
    }
}

// this function will check if the player is being damaged, as well as other effects
void Game::CheckPlayerStatus(int& LevelID) {
    static int OldTime = 0;  
    int cooldown = 500;
    int NewTime = SDL_GetTicks();

    int PlayerX = Players[0]->GetX(), PlayerY = Players[0]->GetY();
    int PlayerWidth = Players[0]->GetPlayerWidth(), PlayerHeight = Players[0]->GetPlayerHeight();

    if (NewTime - OldTime > cooldown) {
        for (int i = 0; i < Levels[LevelID]->GetGameObjectsCount(); ++i) {
            bool objectcandamage = Levels[LevelID]->GetGameObject(i)->GetCanDamage();

            int ObjectX = Levels[LevelID]->GetGameObject(i)->GetX();
            int ObjectY = Levels[LevelID]->GetGameObject(i)->GetY();
            int ObjectWidth = Levels[LevelID]->GetGameObject(i)->GetGameObjectWidth();

            if (objectcandamage &&
                PlayerY + PlayerHeight - 20 == ObjectY + 50 &&
                PlayerX + 10 >= ObjectX &&
                PlayerX <= ObjectX + 50) {
                Players[0]->DamagePlayer(10);
                OldTime = NewTime;  
                break;  
            }
        }
    }

    if (Players[0]->GetHP() <= 0) {
        std::cout << "GAME OVER!" << std::endl;
        exit(1);
    }
}

void Game::ChangeLevel(int& LevelID) {
        if (LevelID == 0 && Players[0]->GetY() == -90) {
            Level2(LevelID);
        }
    return;
}

//const signed int LEFT_BOUNDARY = -30;
//const int RIGHT_BOUNDARY = 530;
//const signed int UPPER_BOUNDARY = -4;
//const int LOWER_BOUNDARY = 410; 


void Game::Level2(int& LevelID) { // loads assets for level 2
    ObjectProperties Fire = { 10, 150, 100, 100 };

    if (!MakeLevel("LevelTwo", "Images/Grass.png", LevelID, -30, 530, -4, 410)) { // left, right, upper, down
        std::cerr << "Couldn't create Level two!" << std::endl;
        return;
    }
    if (!Levels[LevelID]->MakeGameObject(Fire.ObjectX, Fire.ObjectY, Fire.ObjectWidth, 
        Fire.ObjectHeight, false, true, false)) {
        std::cerr << "Couldn't create Game Object!" << std::endl;
        return;
    }
    PlayerMove(0, 280); // resets spawn point to bottom of map
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

//const signed int LEFT_BOUNDARY = -30;
//const int RIGHT_BOUNDARY = 530;
//const signed int UPPER_BOUNDARY = -4;
//const int LOWER_BOUNDARY = 410; 

bool Game::MakeLevel(std::string levelname, const char* backgroundimagepath, int& LevelID,
    int lb, int rb, int ub, int lwb) {
    try {
        auto level = std::make_shared<Level>(levelname, backgroundimagepath, lb, rb, ub, lwb);
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
    if (!MakeLevel("LevelOne", "Images/RoadBackground.png", LevelID, -30, 530, -100, 410)) { // l, r, u, b
        std::cerr << "Couldn't create Level one!" << std::endl;
        return false;
    }

    if (!Levels[0]->MakeGameObject(ObjectX, ObjectY, ObjectWidth, ObjectHeight, true, false, false)) {
        std::cerr << "Couldn't create Game Object!" << std::endl;
        return false;
    }

    if (!Levels[0]->MakeGameObject(400, ObjectY, ObjectWidth, ObjectHeight, true, false, false)) {
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

        // checkcollidable returns true if the object is collidable
        if (boundaryCheck == "top" && gameObject->CheckCollidable() == true) {
            blockBottom = true;
        }
        if (boundaryCheck == "bottom" && gameObject->CheckCollidable() == true) {
            blockTop = true;
        }
        if (boundaryCheck == "left" && gameObject->CheckCollidable() == true) {
            blockRight = true;
        }
        if (boundaryCheck == "right" && gameObject->CheckCollidable() == true) {
            blockLeft = true;
        }
    }
}