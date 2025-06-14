#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
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

    if (TTF_Init() == -1) {
        std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
        return false;
    }

    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << '\n';
        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Mix_OpenAudio failed: " << Mix_GetError() << '\n';
        SDL_Quit();
        return 1;
    }

    game = std::make_unique<Game>();
    if (!game->LoadAssets(renderer, LevelID)) {
        Cleanup("Error. Failed to load game assets!");
        return false;
    }

    return true;
}

void GameEngine::GameLoop() {
    bool running = true;
    SDL_Event e;
    const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);
    
    const int FPS = 80; // increase to improve framerate
    const int frameDelay = 1000 / FPS;  // maximum amount of time per frame

    Uint32 frameStart;
    int frameTime;

    music = Mix_LoadMUS("Images/SoundTrack.mp3");
    if (!music) {
        std::cerr << "Failed to load music: " << Mix_GetError() << std::endl;
        return;
    }

    Mix_PlayMusic(music, -1);
    Mix_VolumeMusic(30);


    while (running) {
        frameStart = SDL_GetTicks(); // start of frame
        game->UserInput(running, keyboardState, LevelID);
        game->LoadLevel(e, renderer, LevelID);
        game->HandleEvents(e, running, renderer);
        game->CheckPlayerStatus(LevelID, running, renderer);
        game->CheckAudio(); // CHECK THIS
        
        
        // CheckLevelID(); uncomment when debugging

        SDL_RenderClear(renderer);
        game->GetLevel(LevelID)->RenderLevel(renderer); // renders gameobject as well
        game->GetPlayer(0)->RenderPlayer(renderer); 
        game->GetPlayer(0)->RenderPlayerHP(renderer);
        // if getbulletsize > 0 && Bullets[i] < Bullets.size();
        if (game->GetBulletsSize() > 0 && game->GetBullets()[game->GetBulletsSize() - 1] != nullptr) {
            game->GetBullet(0)->RenderBullet(renderer);
        }
        game->ChangeLevel(LevelID);
        SDL_RenderPresent(renderer);
        game->DisplayMessages(renderer); // TODO: implement this in level class
        frameTime = SDL_GetTicks() - frameStart;  // time taken for this frame to have been rendered
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);  // delay to maintain a consistent fps
        }
    }
    if (game->GetPlayer(0)->GetHP() <= 0) {
        Mix_FreeMusic(music);
        Mix_CloseAudio();
        SDL_Quit();
        std::cout << "You died!" << std::endl;
    }
    else if (game->GetPlayer(0)->GetHP() > 0 && LevelID == 10) {

        int i = 0;

        while (i < 2000) {
            game->EndMessage();
            game->DisplayMessages(renderer);
            i++;
        }
    }
    else {
        Mix_FreeMusic(music);
        Mix_CloseAudio();
        SDL_Quit();
        std::cout << "Game exited." << std::endl;
    }
}

void GameEngine::CheckLevelID() {
    if (LevelID >= 0 && LevelID < game->GetLevelsCount()) {
        std::cout << "Current LevelID (index): " << LevelID
            << ", InstanceID: " << game->GetLevel(LevelID)->GetInstanceID() << std::endl;
    }
    else {
        std::cerr << "Invalid LevelID: " << LevelID << std::endl;
        LevelID = 0; // reset to first level
    }
}

void GameEngine::Cleanup(const std::string& errormsg) {
    std::cerr << errormsg << std::endl;
}