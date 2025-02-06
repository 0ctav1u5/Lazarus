#include <iostream>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL.h>
#include <string>
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
            PauseMenu(renderer, running);
        }
    }
}

void Game::PauseMenu(SDL_Renderer* renderer, bool& running) {
    int mouseX, mouseY;
    // load font
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
    static int HP = 100;
    std::string str = std::to_string(HP);
    const char* cstr = str.c_str();



    SDL_Texture* textTexture = nullptr;
    SDL_Texture* textTexture2 = nullptr;
    SDL_Texture* textTexture3 = nullptr;
    SDL_Color textColour = { 255, 0, 0 }; // white color for text
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Paused", textColour); // font, text, colour
    SDL_Surface* textSurface2 = TTF_RenderText_Solid(font, "Save", textColour); // font, text, colour
    SDL_Surface* textSurface3 = TTF_RenderText_Solid(font, "Exit", textColour); // font, text, colour
    

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

    if (textSurface3) {
        textTexture3 = SDL_CreateTextureFromSurface(renderer, textSurface3);
        SDL_FreeSurface(textSurface3);
    }
    else {
        std::cerr << "Text render error: " << TTF_GetError() << std::endl;
        return;
    }
    

    SDL_Rect PauseText = { 140, 220, 200, 50 };  // x, y, width, height
    SDL_Rect SaveText = { 10, 0, 100, 50 };
    SDL_Rect ExitText = { 390, 0, 100, 50 };
 


    while (pauseloop) {
        SDL_GetMouseState(&mouseX, &mouseY);

        while (SDL_PollEvent(&p)) {
            if (p.type == SDL_KEYDOWN && p.key.keysym.sym == SDLK_SPACE) {
                pauseloop = false;  
            } // button left = lmb
            else if (p.type == SDL_MOUSEBUTTONDOWN && p.button.button == SDL_BUTTON_LEFT) {
                if (mouseX >= SaveText.x && mouseX <= SaveText.x + SaveText.w &&
                    mouseY >= SaveText.y && mouseY <= SaveText.y + SaveText.h) {
                    std::cout << "Save button clicked!" << std::endl;
                }

                if (mouseX >= ExitText.x && mouseX <= ExitText.x + ExitText.w &&
                    mouseY >= ExitText.y && mouseY <= ExitText.y + ExitText.h) {
                    std::cout << "Exit button clicked!" << std::endl;
                    running = false;  // Exit pause menu
                    pauseloop = false;
                }
            }
        }

        SDL_RenderCopy(renderer, textTexture, nullptr, &PauseText); // renders texture and rect

        // clears buffer and adds overlay in alpha values
        // sets draw colour for new overlay
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // r, g, b, opaqueness
        SDL_RenderFillRect(renderer, &SaveText);
        SDL_RenderCopy(renderer, textTexture2, nullptr, &SaveText); // draws rectangle + texture
        
        // exit
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &ExitText);
        SDL_RenderCopy(renderer, textTexture3, nullptr, &ExitText);
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
void Game::CheckPlayerStatus(int& LevelID, bool& running) {
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
                ((PlayerY + PlayerHeight - 20 >= ObjectY + 40) && 
                (PlayerY + PlayerHeight - 20 <= ObjectY + 60)) &&
                PlayerX + 10 >= ObjectX &&
                PlayerX <= ObjectX + 50) {
                Players[0]->DamagePlayer(10);
                OldTime = NewTime;  
                break;  
            }
        }
    }

    if (Players[0]->GetHP() <= 0) {
        running = false;
    }
}

void Game::ChangeLevel(int& LevelID) {
        if (LevelID == 0 && Players[0]->GetY() == -90) {
            Level2(LevelID);
        }
        if (LevelID == 1 && Players[0]->GetX() == 470) {
            Level3(LevelID);
        }
    return;
}

void Game::Level2(int& LevelID) { // loads assets for level 2
    ObjectProperties Fire = { 10, 150, 100, 100 }; // x, y, width, height
    ObjectProperties Barrier = { 530, 200, 100, 100 };
    ObjectProperties Barrier2 = { 530, 390, 100, 100 };

    if (!MakeLevel("LevelTwo", "Images/Grass.png", LevelID, -30, 570, -4, 410)) { // left, right, upper, down
        std::cerr << "Couldn't create Level two!" << std::endl;
        return;
    }

    // In the makegameobject method, there are 4 bools at the end, for cancollide, candamage
    // cancollect and visible
    if (!Levels[LevelID]->MakeGameObject(Fire.ObjectX, Fire.ObjectY, Fire.ObjectWidth, 
        Fire.ObjectHeight, false, true, false, false)) {
        std::cerr << "Couldn't create Game Object!" << std::endl;
        return;
    }
    if (!Levels[LevelID]->MakeGameObject(Barrier.ObjectX, Barrier.ObjectY, Barrier.ObjectWidth,
        Barrier.ObjectHeight, true, false, false, true)) {
        std::cerr << "Couldn't create Game Object!" << std::endl;
        return;
    }
    if (!Levels[LevelID]->MakeGameObject(Barrier2.ObjectX, Barrier2.ObjectY, Barrier2.ObjectWidth,
        Barrier2.ObjectHeight, true, false, false, true)) {
        std::cerr << "Couldn't create Game Object!" << std::endl;
        return;
    }
    PlayerMove(0, 280); // resets spawn point to bottom of map
}

void Game::Level3(int& LevelID) {
    if (!MakeLevel("LevelThree", "Images/RoadBackground2.png", LevelID, -30, 570, -4, 410)) { // left, right, upper, down
        std::cerr << "Couldn't create Level three!" << std::endl;
        return;
    }
    PlayerMove(-260, 140);
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

    if (!Levels[0]->MakeGameObject(ObjectX, ObjectY, ObjectWidth, ObjectHeight, true, false, false, true)) {
        std::cerr << "Couldn't create Game Object!" << std::endl;
        return false;
    }

    if (!Levels[0]->MakeGameObject(400, ObjectY, ObjectWidth, ObjectHeight, true, false, false, true)) {
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