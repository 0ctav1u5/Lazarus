#include <iostream>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL.h>
#include <string>
#include "Game.hpp"
#include "Player.hpp"
#include "Level.hpp"
#include "Message.hpp"

// TODO: Add AI properties to the enemy class
// Handleevents method creates the bullets
// Checkplayerstats method destroys the bullets
// TODO: bring them both into one method

std::vector<std::string> inventory = {};
int collected = 0;
int Level::LevelIDCounter = 0;

// if gun is collected
// && if gun is up, left, right or down, we are going to have
// the bullet spawn in a different area
// it must be deleted after it its a certain point

void Game::HandleEvents(SDL_Event& e, bool& running, SDL_Renderer* renderer) {
    while (SDL_PollEvent(&e)) {
        int cooldown = 500;
        static int oldtime = 0;
        int newtime = SDL_GetTicks();
        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
            PauseMenu(renderer, running);
        }
        if ((e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) && Bullets.size() == 0 && collected > 0) {
            int RightX = Players[0]->GetX() + 70;
            int RightY = Players[0]->GetY() + 45; // bullet spawn points determined by player graphic

            int LeftX = Players[0]->GetX() + 30;
            int LeftY = Players[0]->GetY() + 45; 

            int UpX = Players[0]->GetX() + 50;
            int UpY = Players[0]->GetY(); 

            int DownX = Players[0]->GetX() + 50;
            int DownY = Players[0]->GetY() + 65; 

            /*std::cout << Players[0]->GetDirection() << std::endl;*/ // prints direction


            // makes the bullets when conditions are met, certain direction and cooldown
            if (Players[0]->GetDirection() == 2 && (newtime - oldtime > cooldown)) { // right
                if (!MakeBullet(9, 10, RightX, RightY)) { // speed, damage
                    std::cerr << "Bullet not created!" << std::endl;
                }
                Bullets[0]->SetDirection(4);
                oldtime = newtime;
            }
            if (Players[0]->GetDirection() == 1 && (newtime - oldtime > cooldown)) { // left
                if (!MakeBullet(9, 10, LeftX, LeftY)) { // speed, damage
                    std::cerr << "Bullet not created!" << std::endl;
                }
                Bullets[0]->SetDirection(3);
                oldtime = newtime;
            }
            if (Players[0]->GetDirection() == 3 && (newtime - oldtime > cooldown)) { // up
                if (!MakeBullet(9, 10, UpX, UpY)) { // speed, damage
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
}

// TODO: convert all ttf messages to message class objects directly
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
    // has to be const char* for the ttf text library, can't be string 
    std::string str = std::to_string(HP); // converts HP member to a string
    const char* cstr = str.c_str(); // converts that string to a const char*

    SDL_Texture* textTexture = nullptr;
    SDL_Texture* textTexture2 = nullptr;
    SDL_Texture* textTexture3 = nullptr;
    SDL_Color textColour = { 255, 0, 0 }; // red colour for text
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
            if (p.type == SDL_KEYDOWN && p.key.keysym.sym == SDLK_ESCAPE) {
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

void Game::DisplayMessages(SDL_Renderer* renderer) {
    static int displaytime = 0;
    if (collected > 0 && collected < 2) {
        Players[0]->GunCollected();
        // collected is 1 but messages is the 0th index of the array
        while (SDL_GetTicks() < displaytime + 1000) {
            Messages[collected - 1]->RenderMessage(renderer);
            SDL_RenderPresent(renderer);
        }
    }
    else { // we will have a lot of elseif statements for each item
        displaytime = SDL_GetTicks();
    }
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


    GameObjectCollisionChecker(LevelNum, playerY, playerX, playerWidth, playerHeight, blockBottom, blockTop, blockRight, blockLeft);

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

// this function will check if the player is being damaged, as well as other effects on the player
// TODO: Make a new function for some of the functionality found here
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
    auto& objects = level->GetGameObjectVector();

    // removes gameobjects which are collectible if 
    objects.erase(std::remove_if(objects.begin(), objects.end(),
        [&](const std::shared_ptr<GameObject>& obj) {
            bool objectcollectible = obj->GetCanCollect(); // true if cancollect = true
            int ObjectX = obj->GetX();
            int ObjectY = obj->GetY();

            // if object can be collected and is within range of the texture for gun object
            // needs to be adjusted for new textures (new guns or other objects being added)
            // problem here is that I am using magic numbers to define ranges
            if (objectcollectible &&
                ((PlayerY + PlayerHeight - 20 >= ObjectY + 30) &&
                    (PlayerY + PlayerHeight - 20 <= ObjectY + 70)) &&
                PlayerX + 10 >= ObjectX &&
                PlayerX <= ObjectX + 40) {

                for (auto& game : objects) {
                    inventory.push_back(game->GetName());
                }

                for (int i = 0; i < inventory.size(); i++) {
                    std::cout << "You currently have in your inventory: " << std::endl;
                    std::cout << inventory[i] << std::endl;
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
            // TODO: Get all enemies in Enemies Vector (RESOLVED)
            // use range loop and return vector from level class
            for (auto& enemy : GetLevel(LevelID)->GetEnemiesVector()) {

                SDL_Rect enemyRect = enemy->GetRect();
                SDL_Rect bulletRect = Bullets[0]->GetRect();
                if (SDL_HasIntersection(&enemyRect, &bulletRect)) { // removes bullet if intersection with enemy rect
                    enemy->DamageEnemy(10);
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

    // enemy follows player 
    // TODO: Have all enemies follow player
    for (auto& enemy : GetLevel(LevelID)->GetEnemiesVector()) {
        if (GetLevel(LevelID)->GetEnemiesSize() > 0 && enemy->GetStationary() == false) {
            enemy->MoveEnemy(PlayerRectX + OffsetX, PlayerRectY + OffsetY);
            OffsetX += 10;
            OffsetY += 10;
        }
    }

    // Damage player if enemy and player share an intersection
    // TODO: Have GetEnemy() include all possible enemies
    if (GetLevel(LevelID)->GetEnemiesSize() > 0) {
        static int oldtime = 0;
        int newtime = SDL_GetTicks();
        int cooldown = 100;
        for (auto& enemy : GetLevel(LevelID)->GetEnemiesVector()) {

            SDL_Rect playerRect = Players[0]->GetRect();
            SDL_Rect enemyRect = enemy->GetRect();
            // required to make the intersection box smaller, so that the graphic fits within the boundaries
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

    // moving barriers to unlock doors
    static int barrierints = 0;
    static int level5barrier = true;

    if (collected > 0 && barrierints < 150 && LevelID == 3) { // this will be modified to handle more than one barrier
        Levels[LevelID]->GetBarrier(0)->MoveBarrierY(-1);
        barrierints += 10;
    }
    else if (LevelID == 4 && Levels[LevelID]->GetEnemiesSize() == 0 && level5barrier) {
        Levels[LevelID]->GetBarrierVector().erase(Levels[LevelID]->GetBarrierVector().begin());
        // TODO: Add functionality for moving a barrier in level 5
        // Bullets.erase(Bullets.begin()); 
        level5barrier = false;
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
        if (LevelID == 2 && Players[0]->GetY() == -90) {
            Level4(LevelID);
        }
        if (LevelID == 3 && Players[0]->GetX() == 470) {
            Level5(LevelID);
        }
        if (LevelID == 4 && Players[0]->GetX() == 470) {
            Level6(LevelID);
        }
    return;
}

void Game::Level2(int& LevelID) { // loads assets for level 2

    // TODO: Make four structs for MakeLevel functions, such that we can have
    // standardized borders for left, right, up and down

    ObjectProperties Fire = { 10, 150, 100, 100 }; // x, y, width, height
    ObjectProperties Barrier = { 530, 220, 100, 300 };


    if (!MakeLevel("LevelTwo", "Images/Grass.png", LevelID, -30, 570, -4, 410)) { // left, right, upper, down
        std::cerr << "Couldn't create Level two!" << std::endl;
        return;
    }

    // In the makegameobject method, there are 4 bools at the end, for cancollide, candamage
    // cancollect and visible
    if (!Levels[LevelID]->MakeGameObject("Fire", Fire.ObjectX, Fire.ObjectY, Fire.ObjectWidth,
        Fire.ObjectHeight, false, true, false, false)) {
        std::cerr << "Couldn't create Game Object!" << std::endl;
        return;
    }
    if (!Levels[LevelID]->MakeBarrier(Barrier.ObjectX, Barrier.ObjectY, Barrier.ObjectWidth,
        Barrier.ObjectHeight)) {
        std::cerr << "Couldn't create barrier!" << std::endl;
        return;
    }
    PlayerMove(0, 280); // resets spawn point to bottom of map
}


void Game::Level3(int& LevelID) {
    ObjectProperties Barrier = { 0, -50, 250, 50 };

    if (!MakeLevel("LevelThree", "Images/RoadBackground2.png", LevelID, -30, 530, -100, 410)) { // left, right, upper, down
        std::cerr << "Couldn't create Level three!" << std::endl;
        return;
    }

    if (!Levels[LevelID]->MakeBarrier(Barrier.ObjectX, Barrier.ObjectY, Barrier.ObjectWidth,
        Barrier.ObjectHeight)) {
        std::cerr << "Couldn't create barrier!" << std::endl;
        return;
    }
    PlayerMove(-260, 140);
}

void Game::Level4(int& LevelID) {
    if (!MakeLevel("LevelFour", "Images/Map4.png", LevelID, -30, 570, -4, 410)) { // left, right, upper, down
        std::cerr << "Couldn't create Level three!" << std::endl;
        return;
    }
    if (!Levels[LevelID]->MakeGameObject("Gun", 170, 10, 150,
        150, false, false, true, false)) { // cancollide, candamage, cancollect, visible
        std::cerr << "Couldn't create Game Object!" << std::endl;
        return;
    }

    // 530
    if (!Levels[LevelID]->MakeBarrier(530, 20, 10, 400)) { // x, y, width, height
        std::cerr << "Couldn't create Barrier!" << std::endl;
        return;
    }

    if (!Levels[LevelID]->MakeBarrier(540, 0, 100, 400)) { // x, y, width, height
        std::cerr << "Couldn't create Barrier!" << std::endl;
        return;
    }
    

    Levels[LevelID]->GetGameObject(0)->SetTexture("Images/Rifle.png"); // sets texture 
    PlayerMove(-150, 280); // resets spawn point to bottom of map
}

void Game::Level5(int& LevelID) {
    if (!MakeLevel("LevelFive", "Images/Level5.png", LevelID, -30, 570, -4, 410)) { // left, right, upper, down
        std::cerr << "Couldn't create Level four!" << std::endl;
        return;
    }
    if (!Levels[LevelID]->MakeEnemy("Morgus", 350, 90, 50, // x, y, width, height
        80, "Images/Zombie.png")) {
        std::cerr << "Couldn't create Enemy!" << std::endl;
        return;
    }
    if (!Levels[LevelID]->MakeBarrier(540, 260, 10, 130)) { // x, y, width, height
        std::cerr << "Couldn't create Barrier!" << std::endl;
        return;
    }
    if (!Levels[LevelID]->MakeBarrier(530, 410, 10, 100)) { // x, y, width, height
        std::cerr << "Couldn't create Barrier!" << std::endl;
        return;
    }
    if (!Levels[LevelID]->MakeBarrier(530, 50, 10, 200)) { // x, y, width, height
        std::cerr << "Couldn't create Barrier!" << std::endl;
        return;
    }
    if (!Levels[LevelID]->MakeBarrier(0, 10, 500, 30)) { // x, y, width, height
        std::cerr << "Couldn't create Barrier!" << std::endl;
        return;
    }
    Levels[LevelID]->GetBarrier(3)->SetInvisible();
    PlayerMove(-250, 0); // x y
}


void Game::Level6(int& LevelID) {
    if (!MakeLevel("LevelSix", "Images/Level6.png", LevelID, -30, 570, -4, 410)) { // left, right, upper, down
        std::cerr << "Couldn't create Level four!" << std::endl;
        return;
    }
    if (!Levels[LevelID]->MakeEnemy("Arceus", 350, 70, 50, // x, y, width, height
        80, "Images/Zombie.png")) {
        std::cerr << "Couldn't create Enemy!" << std::endl;
        return;
    }
    if (!Levels[LevelID]->MakeEnemy("Korfu", 350, 150, 50, // x, y, width, height
        80, "Images/Zombie.png")) {
        std::cerr << "Couldn't create Enemy!" << std::endl;
        return;
    }
    PlayerMove(-260, 0); // x y
}

void Game::PlayerMove(int x, int y) {
    Players[0].get()->Move(x, y);
}

int Game::GetBulletsSize() {
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
    if (!MakeLevel("LevelOne", "Images/RoadBackground.png", LevelID, -30, 530, -100, 410)) { // l, r, u, b
        std::cerr << "Couldn't create Level one!" << std::endl;
        return false;
    }

    if (!Levels[LevelID]->MakeGameObject("Block1", ObjectX, ObjectY, ObjectWidth, ObjectHeight, true, false, false, true)) {
        std::cerr << "Couldn't create Game Object!" << std::endl;
        return false;
    }

    if (!Levels[LevelID]->MakeGameObject("Block2", 400, ObjectY, ObjectWidth, ObjectHeight, true, false, false, true)) {
        std::cerr << "Couldn't create Game Object!" << std::endl;
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

