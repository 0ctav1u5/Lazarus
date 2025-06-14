#include <iostream>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <string>
#include "Game.hpp"
#include "Player.hpp"
#include "Level.hpp"
#include "Message.hpp"

std::vector<std::string> inventory = {};
int collected = 0;

bool riflecollected = false; // TODO: Implement functionality to the handlevents function
bool keycardcollected = false;

int Level::LevelIDCounter = 0;
bool LoaderEnabled = false;

bool endscreen = false;

// buttons for levels
bool l2 = false;
bool l3 = false;
bool l4 = false;
bool l5 = false;
bool l6 = false;
bool l7 = false;
bool l8 = false;
bool l9 = false;
bool l10 = false;

void Game::HandleEvents(SDL_Event& e, bool& running, SDL_Renderer* renderer) {
    while (SDL_PollEvent(&e)) {
        int cooldown = 800;
        static int oldtime = 0;
        int newtime = SDL_GetTicks();

        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
            PauseMenu(renderer, running);
        }
        // HANDLES SHOOTING BULLETS
        if ((e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) && Bullets.size() == 0 && collected > 0) {
            int RightX = Players[0]->GetX() + 70;
            int RightY = Players[0]->GetY() + 40; // bullet spawn points determined by player graphic

            int LeftX = Players[0]->GetX() + 30;
            int LeftY = Players[0]->GetY() + 40; 

            int UpX = Players[0]->GetX() + 40;
            int UpY = Players[0]->GetY(); 

            int DownX = Players[0]->GetX() + 40;
            int DownY = Players[0]->GetY() + 65; 

            /*std::cout << Players[0]->GetDirection() << std::endl;*/ // prints direction


            // makes the bullets when conditions are met, certain direction and cooldown
            if (Players[0]->GetDirection() == 2 && (newtime - oldtime > cooldown)) { // right
                if (!MakeBullet(6, 10, RightX, RightY)) { // speed, damage
                    std::cerr << "Bullet not created!" << std::endl;
                }
                Bullets[0]->SetDirection(4);
                oldtime = newtime;
            }
            if (Players[0]->GetDirection() == 1 && (newtime - oldtime > cooldown)) { // left
                if (!MakeBullet(6, 10, LeftX, LeftY)) { // speed, damage
                    std::cerr << "Bullet not created!" << std::endl;
                }
                Bullets[0]->SetDirection(3);
                oldtime = newtime;
            }
            if (Players[0]->GetDirection() == 3 && (newtime - oldtime > cooldown)) { // up
                if (!MakeBullet(6, 10, UpX, UpY)) { // speed, damage
                    std::cerr << "Bullet not created!" << std::endl;
                }
                Bullets[0]->SetDirection(1);
                oldtime = newtime;
            }
            if (Players[0]->GetDirection() == 4 && (newtime - oldtime > cooldown)) { // down
                if (!MakeBullet(6, 10, DownX, DownY)) { // speed, damage
                    std::cerr << "Bullet not created!" << std::endl;
                }
                Bullets[0]->SetDirection(2);
                oldtime = newtime;
            }
        }
    }

    if (endscreen) {
        running = false;
    }
}


// CHECK THIS
void Game::CheckAudio() {
    static bool bulletPlayed = false;

    if (!bulletPlayed && !Bullets.empty()) {
        Mix_PlayChannel(-1, BulletSound, 0);
        bulletPlayed = true;
    }

    if (Bullets.empty()) {
        bulletPlayed = false;
    }

    // DO NOT free BulletSound or close audio here.
}

void Game::PauseMenu(SDL_Renderer* renderer, bool& running) {
    int mouseX = 0, mouseY = 0;
    
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
    // has to be const char* for the ttf text library, can't be string 
    std::string str = std::to_string(HP); // converts HP member to a string
    const char* cstr = str.c_str(); // converts that string to a const char*

    SDL_Texture* textTexture = nullptr;
    SDL_Texture* textTexture2 = nullptr;
    SDL_Texture* textTexture3 = nullptr;
    SDL_Color textColour = { 255, 0, 0 }; // red colour for text
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Paused", textColour); // font, text, colour
    SDL_Surface* textSurface2 = TTF_RenderText_Solid(font, "Load Level", textColour); // font, text, colour
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
    SDL_Rect LevelLoaderText = { 0, 0, 180, 50 };
    SDL_Rect ExitText = { 400, 0, 100, 50 };

    while (pauseloop) {
        SDL_GetMouseState(&mouseX, &mouseY);


        while (SDL_PollEvent(&p)) {
            if (p.type == SDL_KEYDOWN && p.key.keysym.sym == SDLK_ESCAPE) {
                pauseloop = false;  
            } // button left = lmb
            else if (p.type == SDL_MOUSEBUTTONDOWN && p.button.button == SDL_BUTTON_LEFT) {
                if (mouseX >= LevelLoaderText.x && mouseX <= LevelLoaderText.x + LevelLoaderText.w &&
                    mouseY >= LevelLoaderText.y && mouseY <= LevelLoaderText.y + LevelLoaderText.h) {
                    std::cout << "Level Loader Enabled" << std::endl;
                    this->LevelLoader = true;
                    pauseloop = false;
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

        // sets draw colour for new overlay
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // r, g, b, opaqueness
        SDL_RenderFillRect(renderer, &LevelLoaderText);
        SDL_RenderCopy(renderer, textTexture2, nullptr, &LevelLoaderText); // draws rectangle + texture
        
        // exit
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &ExitText);
        SDL_RenderCopy(renderer, textTexture3, nullptr, &ExitText);
        SDL_RenderPresent(renderer);

    } // end of while loop

    SDL_DestroyTexture(textTexture);  // destroy the texture after it's been used
    TTF_CloseFont(font);  // for closing font when function ends
}



void Game::LoadLevel(SDL_Event& e, SDL_Renderer* renderer, int& LevelID) {
    int mouseX = 0, mouseY = 0;

    // general
    if (LevelLoader) { // condition has to be met
        TTF_Font* font = TTF_OpenFont("Fonts/Vipnagorgialla Rg.otf", 14);
        if (!font) {
            std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
            return;
        }
        else {
            std::cout << "Level Loader Loaded!" << std::endl;
        }

        SDL_Texture* L10textTexture = nullptr;
        SDL_Texture* L9textTexture = nullptr;
        SDL_Texture* L8textTexture = nullptr; 
        SDL_Texture* L7textTexture = nullptr; 
        SDL_Texture* L6textTexture = nullptr; 
        SDL_Texture* L5textTexture = nullptr; 
        SDL_Texture* L4textTexture = nullptr; 
        SDL_Texture* L3textTexture = nullptr; 
        SDL_Texture* L2textTexture = nullptr; 


        SDL_Color textColour = { 255, 0, 0 }; // red - general

        SDL_Surface* L10textSurface = TTF_RenderText_Solid(font, "L10", textColour);
        SDL_Surface* L9textSurface = TTF_RenderText_Solid(font, "L9", textColour);
        SDL_Surface* L8textSurface = TTF_RenderText_Solid(font, "L8", textColour);
        SDL_Surface* L7textSurface = TTF_RenderText_Solid(font, "L7", textColour);
        SDL_Surface* L6textSurface = TTF_RenderText_Solid(font, "L6", textColour);
        SDL_Surface* L5textSurface = TTF_RenderText_Solid(font, "L5", textColour);
        SDL_Surface* L4textSurface = TTF_RenderText_Solid(font, "L4", textColour);
        SDL_Surface* L3textSurface = TTF_RenderText_Solid(font, "L3", textColour);
        SDL_Surface* L2textSurface = TTF_RenderText_Solid(font, "L2", textColour);



        // #L10
        if (L10textSurface) {
            L10textTexture = SDL_CreateTextureFromSurface(renderer, L10textSurface);
            SDL_FreeSurface(L10textSurface);
        }
        else {
            std::cerr << "Text render error: " << TTF_GetError() << std::endl;
            return;
        }

        // #L9
        if (L9textSurface) {
            L9textTexture = SDL_CreateTextureFromSurface(renderer, L9textSurface);
            SDL_FreeSurface(L9textSurface);
        }
        else {
            std::cerr << "Text render error: " << TTF_GetError() << std::endl;
            return;
        }

        // #L8
        if (L8textSurface) {
            L8textTexture = SDL_CreateTextureFromSurface(renderer, L8textSurface);
            SDL_FreeSurface(L8textSurface);
        }
        else {
            std::cerr << "Text render error: " << TTF_GetError() << std::endl;
            return;
        }

        // #L7
        if (L7textSurface) {
            L7textTexture = SDL_CreateTextureFromSurface(renderer, L7textSurface);
            SDL_FreeSurface(L7textSurface);
        }
        else {
            std::cerr << "Text render error: " << TTF_GetError() << std::endl;
            return;
        }

        // #L6
        if (L6textSurface) {
            L6textTexture = SDL_CreateTextureFromSurface(renderer, L6textSurface);
            SDL_FreeSurface(L6textSurface);
        }
        else {
            std::cerr << "Text render error: " << TTF_GetError() << std::endl;
            return;
        }

        // #L5
        if (L5textSurface) {
            L5textTexture = SDL_CreateTextureFromSurface(renderer, L5textSurface);
            SDL_FreeSurface(L5textSurface);
        }
        else {
            std::cerr << "Text render error: " << TTF_GetError() << std::endl;
            return;
        }

        // #L4
        if (L4textSurface) {
            L4textTexture = SDL_CreateTextureFromSurface(renderer, L4textSurface);
            SDL_FreeSurface(L4textSurface);
        }
        else {
            std::cerr << "Text render error: " << TTF_GetError() << std::endl;
            return;
        }

        // #L3
        if (L3textSurface) {
            L3textTexture = SDL_CreateTextureFromSurface(renderer, L3textSurface);
            SDL_FreeSurface(L3textSurface);
        }
        else {
            std::cerr << "Text render error: " << TTF_GetError() << std::endl;
            return;
        }

        // #L2
        if (L2textSurface) {
            L2textTexture = SDL_CreateTextureFromSurface(renderer, L2textSurface);
            SDL_FreeSurface(L2textSurface);
        }
        else {
            std::cerr << "Text render error: " << TTF_GetError() << std::endl;
            return;
        }

        SDL_Rect L10Text = { 320, 0, 30, 30 };  // x, y, width, height
        SDL_Rect L9Text = { 280, 0, 30, 30 };  // x, y, width, height
        SDL_Rect L8Text = { 240, 0, 30, 30 };  // x, y, width, height
        SDL_Rect L7Text = { 200, 0, 30, 30 };  // x, y, width, height
        SDL_Rect L6Text = { 160, 0, 30, 30 };  // x, y, width, height
        SDL_Rect L5Text = { 120, 0, 30, 30 };  // x, y, width, height
        SDL_Rect L4Text = { 80, 0, 30, 30 };  // x, y, width, height
        SDL_Rect L3Text = { 40, 0, 30, 30 };  // x, y, width, height
        SDL_Rect L2Text = { 0, 0, 30, 30 };  // x, y, width, height
        


        while (LevelLoader) {
            SDL_GetMouseState(&mouseX, &mouseY);
            while (SDL_PollEvent(&e)) {

                if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {

                    if (mouseX >= L10Text.x && mouseX <= L10Text.x + L10Text.w &&
                        mouseY >= L10Text.y && mouseY <= L10Text.y + L10Text.h) {
                        SDL_DestroyTexture(L10textTexture);  // destroy the texture after it's been used
                        TTF_CloseFont(font);  // for closing font when function ends
                        LoaderEnabled = true;
                        l10 = true;
                        LevelLoader = false;
                    }

                    if (mouseX >= L9Text.x && mouseX <= L9Text.x + L9Text.w &&
                        mouseY >= L9Text.y && mouseY <= L9Text.y + L9Text.h) {
                        SDL_DestroyTexture(L9textTexture);  // destroy the texture after it's been used
                        TTF_CloseFont(font);  // for closing font when function ends
                        LoaderEnabled = true;
                        l9 = true;
                        LevelLoader = false;
                    }

                    if (mouseX >= L8Text.x && mouseX <= L8Text.x + L8Text.w &&
                        mouseY >= L8Text.y && mouseY <= L8Text.y + L8Text.h) {
                        SDL_DestroyTexture(L8textTexture);  // destroy the texture after it's been used
                        TTF_CloseFont(font);  // for closing font when function ends
                        LoaderEnabled = true;
                        l8 = true;
                        LevelLoader = false;
                    }

                    if (mouseX >= L7Text.x && mouseX <= L7Text.x + L7Text.w &&
                        mouseY >= L7Text.y && mouseY <= L7Text.y + L7Text.h) {
                        SDL_DestroyTexture(L7textTexture);  // destroy the texture after it's been used
                        TTF_CloseFont(font);  // for closing font when function ends
                        LoaderEnabled = true;
                        l7 = true;
                        LevelLoader = false;
                    }

                    if (mouseX >= L6Text.x && mouseX <= L6Text.x + L6Text.w &&
                        mouseY >= L6Text.y && mouseY <= L6Text.y + L6Text.h) {
                        SDL_DestroyTexture(L6textTexture);  // destroy the texture after it's been used
                        TTF_CloseFont(font);  // for closing font when function ends
                        LoaderEnabled = true;
                        l6 = true;
                        LevelLoader = false;
                    }

                    if (mouseX >= L5Text.x && mouseX <= L5Text.x + L5Text.w &&
                        mouseY >= L5Text.y && mouseY <= L5Text.y + L5Text.h) {
                        SDL_DestroyTexture(L5textTexture);  // destroy the texture after it's been used
                        TTF_CloseFont(font);  // for closing font when function ends
                        LoaderEnabled = true;
                        l5 = true;
                        LevelLoader = false;
                    }

                    if (mouseX >= L4Text.x && mouseX <= L4Text.x + L4Text.w &&
                        mouseY >= L4Text.y && mouseY <= L4Text.y + L4Text.h) {
                        SDL_DestroyTexture(L4textTexture);  // destroy the texture after it's been used
                        TTF_CloseFont(font);  // for closing font when function ends
                        LoaderEnabled = true;
                        l4 = true;
                        LevelLoader = false;
                    }

                    if (mouseX >= L3Text.x && mouseX <= L3Text.x + L3Text.w &&
                        mouseY >= L3Text.y && mouseY <= L3Text.y + L3Text.h) {
                        SDL_DestroyTexture(L3textTexture);  // destroy the texture after it's been used
                        TTF_CloseFont(font);  // for closing font when function ends
                        LoaderEnabled = true;
                        l3 = true;
                        LevelLoader = false;
                    }

                    if (mouseX >= L2Text.x && mouseX <= L2Text.x + L2Text.w &&
                        mouseY >= L2Text.y && mouseY <= L2Text.y + L2Text.h) {
                        SDL_DestroyTexture(L2textTexture);  // destroy the texture after it's been used
                        TTF_CloseFont(font);  // for closing font when function ends
                        LoaderEnabled = true;
                        l2 = true;
                        LevelLoader = false;
                    }
                }


                // we'll keep this as the exit option from the load level menu for now
                if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_g) {
                    SDL_DestroyTexture(L8textTexture);  // destroy the texture after it's been used
                    TTF_CloseFont(font);  // for closing font when function ends
                    this->LevelLoader = false;
                }


            }
            
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // r, g, b, opaqueness

            SDL_RenderFillRect(renderer, &L10Text);
            SDL_RenderCopy(renderer, L10textTexture, nullptr, &L10Text); // renders texture and rect

            SDL_RenderFillRect(renderer, &L9Text);
            SDL_RenderCopy(renderer, L9textTexture, nullptr, &L9Text); // renders texture and rect

            SDL_RenderFillRect(renderer, &L8Text);
            SDL_RenderCopy(renderer, L8textTexture, nullptr, &L8Text); // renders texture and rect

            SDL_RenderFillRect(renderer, &L7Text); // fills the rectangle shape
            SDL_RenderCopy(renderer, L7textTexture, nullptr, &L7Text); // copies the text

            SDL_RenderFillRect(renderer, &L6Text); // fills the rectangle shape
            SDL_RenderCopy(renderer, L6textTexture, nullptr, &L6Text); // copies the text

            SDL_RenderFillRect(renderer, &L5Text); // fills the rectangle shape
            SDL_RenderCopy(renderer, L5textTexture, nullptr, &L5Text); // copies the text

            SDL_RenderFillRect(renderer, &L4Text); // fills the rectangle shape
            SDL_RenderCopy(renderer, L4textTexture, nullptr, &L4Text); // copies the text

            SDL_RenderFillRect(renderer, &L3Text); // fills the rectangle shape
            SDL_RenderCopy(renderer, L3textTexture, nullptr, &L3Text); // copies the text

            SDL_RenderFillRect(renderer, &L2Text); // fills the rectangle shape
            SDL_RenderCopy(renderer, L2textTexture, nullptr, &L2Text); // copies the text

            SDL_RenderPresent(renderer);
        }

        
    }
}


void Game::UserInput(bool& running, const Uint8* keyboardState, int& LevelID) {
    int playerX = Players[0]->GetX(), playerY = Players[0]->GetY();
    int playerWidth = Players[0]->GetPlayerWidth(), playerHeight = Players[0]->GetPlayerHeight();
    static bool opendoorcondition = true;

    bool blockBottom = false, blockTop = false, blockLeft = false, blockRight = false;

    // LevelNum used for collisionchecker of objects
    int LevelNum = GetLevelsCount() - 1; // tells us the position of each level in the array

    // checks the collisions with game objects to see if the player can move in a specific direction
    GameObjectCollisionChecker(LevelNum, playerY, playerX, playerWidth, playerHeight, blockBottom, blockTop, blockRight, blockLeft);

    // for instance, it would be level[i]->LEFT_BOUNDARY
    if (keyboardState[SDL_SCANCODE_LEFT] || keyboardState[SDL_SCANCODE_A]) {
        if (!blockLeft && playerX > Levels[LevelID]->GetLeftBoundary()) {
            PlayerMove(-1, 0); // Move left
            Players[0]->SetDirectionGraphic(1);
        }
    }

    if (keyboardState[SDL_SCANCODE_RIGHT] || keyboardState[SDL_SCANCODE_D]) {
        if (!blockRight && (playerX + playerWidth < Levels[LevelID]->GetRightBoundary())) {
            PlayerMove(1, 0); // Move right
            Players[0]->SetDirectionGraphic(2);
        }
    }

    if (keyboardState[SDL_SCANCODE_UP] || keyboardState[SDL_SCANCODE_W]) {
        if (!blockTop && (playerY >= Levels[LevelID]->GetUpperBoundary())) {
            PlayerMove(0, -1); // Move up
            Players[0]->SetDirectionGraphic(3);
            // std::cout << "X: " << Players[0]->GetX() << "| Y: " << Players[0]->GetY() << std::endl;
        }
    }

    if (keyboardState[SDL_SCANCODE_DOWN] || keyboardState[SDL_SCANCODE_S]) {
        if (!blockBottom && playerY < Levels[LevelID]->GetLowerBoundary()) {
            PlayerMove(0, 1); // Move down
            Players[0]->SetDirectionGraphic(4);
        }
    }

    if (keyboardState[SDL_SCANCODE_E] && LevelID == 9 && opendoorcondition && Players[0]->GetY() < 200) {
        Level10p2(LevelID);
        opendoorcondition = false;
    }
}


void Game::CheckPlayerStatus(int& LevelID, bool& running, SDL_Renderer* renderer) {
    static int OldTime = 0;
    int cooldown = 500;
    int NewTime = SDL_GetTicks();
    int PlayerX = Players[0]->GetX(), PlayerY = Players[0]->GetY();
    int PlayerWidth = Players[0]->GetPlayerWidth(), PlayerHeight = Players[0]->GetPlayerHeight();


    // if set time has passed and objectcandamage is true, then the gameobject can damage player
    if (NewTime - OldTime > cooldown) { // cooldown period
        for (int i = 0; i < Levels[LevelID]->GetGameObjectsCount(); ++i) {
            bool objectcandamage = Levels[LevelID]->GetGameObject(i)->GetCanDamage();
            int ObjectX = Levels[LevelID]->GetGameObject(i)->GetX();
            int ObjectY = Levels[LevelID]->GetGameObject(i)->GetY();
            int ObjectWidth = Levels[LevelID]->GetGameObject(i)->GetGameObjectWidth();

            if (objectcandamage &&
                ((PlayerY + PlayerHeight - 20 >= ObjectY + 30) &&
                    (PlayerY + PlayerHeight - 20 <= ObjectY + 60)) &&
                PlayerX + 10 >= ObjectX &&
                PlayerX <= ObjectX + 40) {
                Players[0]->DamagePlayer(10);
                OldTime = NewTime;
                break;
            }
        }
    }

    std::shared_ptr<Level> level = Levels[LevelID];
    auto& objects = level->GetGameObjectVector(); // Game Objects for 1 level

    // removes gameobjects which are collectible if 
    objects.erase(std::remove_if(objects.begin(), objects.end(),
        [&](const std::shared_ptr<GameObject>& obj) {
            bool objectcollectible = obj->GetCanCollect(); // true if cancollect = true



            int PlayerX = Players[0]->GetX();
            int PlayerY = Players[0]->GetY();
            int OXU = obj->GetXU();
            int OXL = obj->GetXL();
            int OYU = obj->GetYU();
            int OYL = obj->GetYL();

          
               /* XUB = 206
                XLB = 198
                YUB = 8
                YLB = 4*/

            if (objectcollectible && (PlayerX >= OXL && PlayerX <= OXU) && (PlayerY >= OYL && PlayerY <= OYU)) {

                for (auto& game : objects) {
                    inventory.push_back(game->GetName());
                }

                std::string message = "You picked up a ";
                std::string message2 = inventory[collected];
                message.append(message2);
                if (!MakeMessage(message, 140, 220, 200, 50)) { // left, right, upper, down
                    std::cerr << "Message failed to load!" << std::endl;
                    return false;
                }
                collected++; // increments by 1 each time an object is collected
                return true;
            }
            // if false, then object is not removed
            return false;
        }), objects.end());


    if (!Bullets.empty()) { // this method removes the bullets when out of bounds or when enemies[0] shares an intersection with a bullet

        if (Bullets[0]->GetX() < 0 || Bullets[0]->GetX() > 500 || // if bullet goes out of window
            Bullets[0]->GetY() < 0 || Bullets[0]->GetY() > 500)
        {
            Bullets.erase(Bullets.begin()); 
        }
        else if (GetLevel(LevelID)->GetEnemiesSize() > 0) { 


            for (auto& enemy : GetLevel(LevelID)->GetEnemiesVector()) {

                SDL_Rect enemyRect = enemy->GetRect();
                SDL_Rect bulletRect = Bullets[0]->GetRect();
                if (SDL_HasIntersection(&enemyRect, &bulletRect)) { // removes bullet if intersection with enemy rect
                    enemy->DamageEnemy(10); // DAMAGE ENEMY IS HERE
                    Bullets.erase(Bullets.begin());
                    std::cout << enemy->GetEnemyName() << " HP: " << enemy->GetEnemyHP() << std::endl;
                    break;
                }
            }
        }
    }

    int PlayerRectX = Players[0]->GetX();
    int PlayerRectY = Players[0]->GetY();
    int OffsetX = 0; // needed to slightly adjust the enemy objects so they aren't on top of eachother
    int OffsetY = 0;

    for (auto& enemy : GetLevel(LevelID)->GetEnemiesVector()) {
        if (GetLevel(LevelID)->GetEnemiesSize() > 0 && enemy->GetStationary() == false) {
            enemy->MoveEnemy(PlayerRectX + OffsetX, PlayerRectY + OffsetY);
            OffsetX += 10;
            OffsetY += 10;
        }
    }

    if (GetLevel(LevelID)->GetEnemiesSize() > 0) {
        static int oldtime = 0;
        int newtime = SDL_GetTicks();
        int cooldown = 100;
        for (auto& enemy : GetLevel(LevelID)->GetEnemiesVector()) {

            SDL_Rect playerRect = Players[0]->GetRect();
            SDL_Rect enemyRect = enemy->GetRect();
            enemyRect.h -= 70;
            enemyRect.w -= 40;
            playerRect.h -= 50;
            playerRect.w -= 50;
            if (SDL_HasIntersection(&playerRect, &enemyRect) && newtime - oldtime > cooldown) {
                Players[0]->DamagePlayer(1);
                oldtime = newtime;
            }
        }
    }

    static int level3barrier = true;
    static int level5barrier = true;
    static int level6barrier = true;
    static int level9barrier = true; // COME BACK HERE

    if (collected > 0 && LevelID == 3 && level3barrier) { // this will be modified to handle more than one barrier
        Levels[LevelID]->GetBarrierVector().erase(Levels[LevelID]->GetBarrierVector().begin());
        level3barrier = false;
    }
    else if (LevelID == 4 && Levels[LevelID]->GetEnemiesSize() == 0 && level5barrier) {
        Levels[LevelID]->GetBarrierVector().erase(Levels[LevelID]->GetBarrierVector().begin() + 1);
        level5barrier = false;
    }
    else if (LevelID == 5 && Levels[LevelID]->GetEnemiesSize() == 0 && level6barrier) {
        Levels[LevelID]->GetBarrierVector().erase(Levels[LevelID]->GetBarrierVector().begin() + 1);
        level6barrier = false;
    }
    else if (LevelID == 8 && Levels[LevelID]->GetEnemiesSize() == 0 && level9barrier) {
        Levels[LevelID]->GetBarrierVector().erase(Levels[LevelID]->GetBarrierVector().begin());
        level9barrier = false;
    }

    if (Players[0]->GetHP() <= 0) {
        running = false;
    }
}


void Game::DisplayMessages(SDL_Renderer* renderer) {
    static bool rifle = true, keycard = true;


    if (collected > 0) { // collected > 0 && collected < 2


        if (Messages.size() == 1 && rifle) {
            Players[0]->GunCollected();
            Messages[collected - 1]->RenderMessage(renderer);
            SDL_RenderPresent(renderer);
            SDL_Delay(1000); // this will be changed to a time based delay later
            rifle = false;
        }
        else if (Messages.size() == 2 && keycard) {
            Messages[collected - 1]->RenderMessage(renderer);
            SDL_RenderPresent(renderer);
            SDL_Delay(1000);
            keycard = false;
        }
        else if (Messages.size() == 3 && endscreen) {
            Messages[collected - 1]->RenderMessage(renderer);
            SDL_RenderPresent(renderer);
            SDL_Delay(1000);
            endscreen = false;
        }
        else {
            return;
        }
    }
}

void Game::ChangeLevel(int& LevelID) {

    if (!LoaderEnabled && !l2 && !l3 && !l4 && !l5 && !l6 && !l7 && !l8) {
        if (LevelID == 0 && Players[0]->GetY() == -90) {
            Level2(LevelID);
        }
        if (LevelID == 1 && Players[0]->GetX() == 470) {
            Level3(LevelID);
        }
        if (LevelID == 2 && Players[0]->GetY() == -90) {
            Level4(LevelID);
        }
        if (LevelID == 3 && Players[0]->GetX() == 470) {
            Level5(LevelID);
        }
        if (LevelID == 4 && Players[0]->GetX() == 470) {
            Level6(LevelID);
        }
        if (LevelID == 5 && Players[0]->GetX() == 470) {
            Level7(LevelID);
        }
        if (LevelID == 6 && Players[0]->GetY() == -90) {
            Level8(LevelID);
        }
        if (LevelID == 7 && Players[0]->GetY() > 490) {
            Level9(LevelID);
        }
        if (LevelID == 8 && Players[0]->GetX() < -60) {
            Level10p1(LevelID);
        }
    }
    else if (LoaderEnabled && l10) {
        Level10p1(LevelID);
        l10 = false;
    }
    else if (LoaderEnabled && l9) {
        Level9(LevelID);
        l9 = false;
    }
    else if (LoaderEnabled && l8) {
        Level8(LevelID);
        l8 = false;
    }
    else if (LoaderEnabled && l7) {
        Level7(LevelID);
        l7 = false;
    }
    else if (LoaderEnabled && l6) {
        Level6(LevelID);
        l6 = false;
    }
    else if (LoaderEnabled && l5) {
        Level5(LevelID);
        l5 = false;
    }
    else if (LoaderEnabled && l4) {
        Level4(LevelID);
        l4 = false;
    }
    else if (LoaderEnabled && l3) {
        Level3(LevelID);
        l3 = false;
    }
    else if (LoaderEnabled && l2) {
        Level2(LevelID);
        l2 = false;
    }
    return;   
}

void Game::Level2(int& LevelID) { 

    ObjectProperties Fire = { 10, 150, 100, 100 }; // x, y, width, height
    ObjectProperties Barrier = { 530, 220, 100, 300 };


    if (!MakeLevel("LevelTwo", "Images/Level2.png", LevelID, -30, 570, -4, 410)) { // left, right, upper, down
        std::cerr << "Couldn't create Level two!" << std::endl;
        return;
    }

    if (!Levels[LevelID]->MakeGameObject("Fire", Fire.ObjectX, Fire.ObjectY, Fire.ObjectWidth,
        Fire.ObjectHeight, false, true, false, false, 0, 0, 0, 0)) {
        std::cerr << "Couldn't create Game Object!" << std::endl;
        return;
    }
    if (!Levels[LevelID]->MakeBarrier(Barrier.ObjectX, Barrier.ObjectY, Barrier.ObjectWidth,
        Barrier.ObjectHeight)) {
        std::cerr << "Couldn't create barrier!" << std::endl;
        return;
    }

    if (!LoaderEnabled) {
        PlayerMove(0, 280); // resets spawn point to bottom of map
    }
    else {
        Players[0]->SetX(210);
        Players[0]->SetY(400);
    }
}

void Game::Level3(int& LevelID) {
    ObjectProperties Barrier = { 0, -50, 250, 50 };

    if (!MakeLevel("LevelThree", "Images/Level3.png", LevelID, -30, 530, -100, 410)) { // left, right, upper, down
        std::cerr << "Couldn't create Level three!" << std::endl;
        return;
    }

    if (!Levels[LevelID]->MakeBarrier(Barrier.ObjectX, Barrier.ObjectY, Barrier.ObjectWidth,
        Barrier.ObjectHeight)) {
        std::cerr << "Couldn't create barrier!" << std::endl;
        return;
    }
    if (!LoaderEnabled) {
        PlayerMove(-260, 140);
    }
    else {
        Players[0]->SetX(210);
        Players[0]->SetY(400);
    }
}

void Game::Level4(int& LevelID) {
    if (!MakeLevel("LevelFour", "Images/Level4.png", LevelID, -30, 570, -4, 410)) { // left, right, upper, down
        std::cerr << "Couldn't create Level four!" << std::endl;
        return;
    }

    if (!Levels[LevelID]->MakeGameObject("Rifle", 170, 10, 150,
        150, false, false, true, false, 208, 186, 2, -6)) { // cancollide, candamage, cancollect, visible
        std::cerr << "Couldn't create Game Object!" << std::endl;
        return;
    }

    if (!Levels[LevelID]->MakeBarrier(530, 20, 10, 400)) { // x, y, width, height
        std::cerr << "Couldn't create Barrier!" << std::endl;
        return;
    }

    if (!Levels[LevelID]->MakeBarrier(540, 0, 100, 400)) { // x, y, width, height
        std::cerr << "Couldn't create Barrier!" << std::endl;
        return;
    }
    

    Levels[LevelID]->GetGameObject(0)->SetTexture("Images/Rifle.png"); // sets texture 

    if (!LoaderEnabled) {
        PlayerMove(-150, 280); // resets spawn point to bottom of map
    }
    else {
        Players[0]->SetX(210);
        Players[0]->SetY(400);
    }
}

void Game::Level5(int& LevelID) {
    if (!MakeLevel("LevelFive", "Images/Level5.png", LevelID, -30, 570, -4, 410)) { // left, right, upper, down
        std::cerr << "Couldn't create Level five!" << std::endl;
        return;
    }
    if (!Levels[LevelID]->MakeEnemy("Morgus", 400, 400, 50, 
        80, "Images/Zombie2.png")) {
        std::cerr << "Couldn't create Enemy!" << std::endl;
        return;
    }

    if (!Levels[LevelID]->MakeBarrier(0, 10, 500, 30)) { // x, y, width, height
        std::cerr << "Couldn't create Barrier!" << std::endl;
        return;
    } 

    if (!Levels[LevelID]->MakeBarrier(540, 260, 10, 130)) { 
        std::cerr << "Couldn't create Barrier!" << std::endl;
        return;
    }
    if (!Levels[LevelID]->MakeBarrier(530, 410, 10, 100)) { 
        std::cerr << "Couldn't create Barrier!" << std::endl;
        return;
    }
    if (!Levels[LevelID]->MakeBarrier(530, 50, 10, 200)) { 
        std::cerr << "Couldn't create Barrier!" << std::endl;
        return;
    }
    Levels[LevelID]->GetBarrier(0)->SetInvisible();

    if (!LoaderEnabled) {
        PlayerMove(-250, 0); // x y
    }
    else {
        Players[0]->SetX(210);
        Players[0]->SetY(400);
    }
}

void Game::Level6(int& LevelID) {
    if (!MakeLevel("LevelSix", "Images/Level6.png", LevelID, -30, 570, -4, 410)) { // left, right, upper, down
        std::cerr << "Couldn't create Level six!" << std::endl;
        return;
    }
    if (!Levels[LevelID]->MakeEnemy("Arceus", 350, 70, 50, // x, y, width, height
        80, "Images/Zombie.png")) {
        std::cerr << "Couldn't create Enemy!" << std::endl;
        return;
    }
    if (!Levels[LevelID]->MakeEnemy("Korfu", 350, 150, 50, // x, y, width, height
        80, "Images/Zombie2.png")) {
        std::cerr << "Couldn't create Enemy!" << std::endl;
        return;
    }
    
    if (!Levels[LevelID]->MakeBarrier(0, 10, 500, 30)) { // x, y, width, height
        std::cerr << "Couldn't create Barrier!" << std::endl;
        return;
    }

    if (!Levels[LevelID]->MakeBarrier(540, 260, 10, 130)) {
        std::cerr << "Couldn't create Barrier!" << std::endl;
        return;
    }
    if (!Levels[LevelID]->MakeBarrier(530, 410, 10, 100)) {
        std::cerr << "Couldn't create Barrier!" << std::endl;
        return;
    }
    if (!Levels[LevelID]->MakeBarrier(530, 50, 10, 200)) {
        std::cerr << "Couldn't create Barrier!" << std::endl;
        return;
    }
    
    Levels[LevelID]->GetBarrier(0)->SetInvisible();

    if (!LoaderEnabled) {
        PlayerMove(-260, 0); // x y
    }
    else {
        Players[0]->SetX(210);
        Players[0]->SetY(400);
    }
}

void Game::Level7(int& LevelID) {
    if (!MakeLevel("LevelSeven", "Images/Level7.png", LevelID, -30, 530, -100, 410)) { // left, right, upper, down
        std::cerr << "Couldn't create Level seven!" << std::endl;
        return;
    }
    if (!Levels[LevelID]->MakeBarrier(0, -100, 170, 100)) { // x, y, width, height
        std::cerr << "Couldn't create Barrier!" << std::endl;
        return;
    }
    if (!Levels[LevelID]->MakeBarrier(390, -100, 100, 100)) {
        std::cerr << "Couldn't create Barrier!" << std::endl;
        return;
    }
    if (!LoaderEnabled) {
        PlayerMove(-260, 0); // x y
    }
    else {
        Players[0]->SetX(210);
        Players[0]->SetY(400);
    }
}

void Game::Level8(int& LevelID) {
    if (!MakeLevel("LevelEight", "Images/Level8.png", LevelID, -30, 530, 0, 530)) { // left, right, upper, down
        std::cerr << "Couldn't create Level eight!" << std::endl;
        return;
    }

    if (!Levels[LevelID]->MakeBarrier(-50, 510, 170, 100)) { // x, y, width, height
        std::cerr << "Couldn't create Barrier!" << std::endl;
        return;
    }
    if (!Levels[LevelID]->MakeBarrier(390, 510, 100, 100)) {
        std::cerr << "Couldn't create Barrier!" << std::endl;
        return;
    }

    if (!Levels[LevelID]->MakeGameObject("Key card", 230, 66, 40, // x, y, width, height
        40, false, false, true, false, 206, 198, 8, 4)) { // cancollide, candamage, cancollect, visible, xu, xl, yu, yl
        std::cerr << "Couldn't create Game Object!" << std::endl;
        return;
    }

    Levels[LevelID]->GetGameObject(0)->SetTexture("Images/Key card.png"); // sets texture 
    
    if (!LoaderEnabled) {
        PlayerMove(-20, 260); // x y
    }
    else {
        Players[0]->SetX(210);
        Players[0]->SetY(400);
    }
}

void Game::Level9(int& LevelID) {
    if (!MakeLevel("LevelNine", "Images/Level7.png", LevelID, -100, 530, 0, 410)) { // left, right, upper, down
        std::cerr << "Couldn't create Level nine!" << std::endl;
        return;
    }

    if (!Levels[LevelID]->MakeEnemy("Juniper", 50, 450, 50, // x, y, width, height
        80, "Images/Zombie3.png")) {
        std::cerr << "Couldn't create Enemy!" << std::endl;
        return;
    }

    if (!Levels[LevelID]->MakeEnemy("Jordan", 300, 450, 50, // x, y, width, height
        80, "Images/Zombie2.png")) {
        std::cerr << "Couldn't create Enemy!" << std::endl;
        return;
    }

    if (!Levels[LevelID]->MakeEnemy("Xenith", 50, 350, 50, // x, y, width, height
        80, "Images/Zombie.png")) {
        std::cerr << "Couldn't create Enemy!" << std::endl;
        return;
    }

    if (!Levels[LevelID]->MakeBarrier(-130, 0, 100, 600)) { // x, y, width, height
        std::cerr << "Couldn't create Barrier!" << std::endl;
        return;
    }

    if (!Levels[LevelID]->MakeBarrier(-40, 0, 10, 250)) { // x, y, width, height
        std::cerr << "Couldn't create Barrier!" << std::endl;
        return;
    }

    if (!Levels[LevelID]->MakeBarrier(-40, 410, 10, 150)) { // x, y, width, height
        std::cerr << "Couldn't create Barrier!" << std::endl;
        return;
    }

    if (!LoaderEnabled) {
        PlayerMove(20, -270); // x y
    }
    else {
        Players[0]->SetX(210);
        Players[0]->SetY(400);
    }
}

void Game::Level10p1(int& LevelID) {
    if (!MakeLevel("LevelTenp1", "Images/Level10p1.png", LevelID, -30, 530, 0, 410)) { // left, right, upper, down
        std::cerr << "Couldn't create Level ten!" << std::endl;
        return;
    }
    if (!Levels[LevelID]->MakeBarrier(0, 10, 500, 30)) { // x, y, width, height
        std::cerr << "Couldn't create Barrier!" << std::endl;
        return;
    }

    Levels[LevelID]->GetBarrier(0)->SetInvisible();

    if (!LoaderEnabled) {
        PlayerMove(280, 0); // x y
    }
    else {
        Players[0]->SetX(210);
        Players[0]->SetY(400);
    }
}

void Game::Level10p2(int& LevelID) {
    if (!MakeLevel("LevelTenp2", "Images/Level10p2.png", LevelID, -30, 530, 0, 410)) { // left, right, upper, down
        std::cerr << "Couldn't create Level ten!" << std::endl;
        return;
    }
    if (!LoaderEnabled) {
        PlayerMove(0, 0); // x y
    }
    else {
        Players[0]->SetX(210);
        Players[0]->SetY(400);
    }
    collected++;
    endscreen = true;
}

void Game::EndMessage() {
    std::string message = "Preview Over";

    if (!MakeMessage(message, 140, 220, 200, 50)) { // left, right, upper, down
        std::cerr << "Message failed to load!" << std::endl;
        return;
    }
}

void Game::PlayerMove(int x, int y) {
    Players[0].get()->Move(x, y);
}

size_t Game::GetBulletsSize() {
    return Bullets.size();
}

std::vector<std::shared_ptr<Bullet>> Game::GetBullets() {
    return this->Bullets;
}

bool Game::MakeMessage(std::string msg, int x, int y, int w, int h) {
    try {
        auto message = std::make_shared<Message>(msg, x, y, w, h);
        Messages.push_back(std::move(message));
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Cannot make new message: " << e.what() << std::endl;
        return false;
    }
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

bool Game::MakeBullet(int speed, int damage, int startx, int starty) {
    try {
        auto bullet = std::make_shared<Bullet>(speed, damage, startx, starty);
        Bullets.push_back(std::move(bullet));
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Cannot make new bullet: " << e.what() << std::endl;
        return false;
    }
}

bool Game::MakeLevel(std::string levelname, const char* backgroundimagepath, int& LevelID,
    int lb, int rb, int ub, int lwb) {
    try {
        auto level = std::make_shared<Level>(levelname, backgroundimagepath, lb, rb, ub, lwb);
        Levels.push_back(std::move(level)); 
        LevelID = Levels.size() - 1; // needs to be changed for level loader
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

std::shared_ptr<Bullet> Game::GetBullet(int i) {

    if (Bullets[i] != nullptr && i < Bullets.size()) {
        return Bullets[i];
    }
    std::cerr << "Bullet out of bounds!" << std::endl;
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
    if (!MakeLevel("LevelOne", "Images/Level1.png", LevelID, -30, 530, -100, 410)) { // l, r, u, b
        std::cerr << "Couldn't create Level one!" << std::endl;
        return false;
    }

    if (!Levels[LevelID]->MakeGameObject("Block1", ObjectX, ObjectY, ObjectWidth, ObjectHeight, true, false, false, true, 0, 0, 0, 0)) {
        std::cerr << "Couldn't create Game Object!" << std::endl;
        return false;
    }

    if (!Levels[LevelID]->MakeGameObject("Block2", 400, ObjectY, ObjectWidth, ObjectHeight, true, false, false, true, 0, 0, 0, 0)) {
        std::cerr << "Couldn't create Game Object!" << std::endl;
        return false;
    }

    this->BulletSound = Mix_LoadWAV("Images/BulletSound.mp3");
    if (!BulletSound) {
        std::cerr << "Failed to load sound: " << Mix_GetError() << '\n';
        Mix_CloseAudio();
        SDL_Quit();
        return false;
    }
    return true;
}

void Game::GameObjectCollisionChecker(int levelnum, int playerY, int playerX, int playerWidth, int playerHeight,
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

    for (size_t i = 0; i < Levels[levelnum]->GetBarriersCount(); ++i) {
        auto barrier = Levels[levelnum]->GetBarrier(i); // get all barriers in level[i]
        if (!barrier) {
            continue;
        }

        std::string boundaryCheck = barrier->CheckBoundary(playerY, playerX, playerWidth, playerHeight);

        // checkcollidable returns true if the object is collidable
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

