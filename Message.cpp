#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include "Message.hpp"


void Message::SetColour(Uint8 r, Uint8 g, Uint8 b) { // uint8 is the var type for colours
    R = r;
    G = g;
    B = b;
    textColour = { R, G, B };
}

void Message::SetPosition(int x, int y, int w, int h) {
    X = x;
    Y = y;
    W = w;
    H = h;
}

void Message::RenderMessage(SDL_Renderer* renderer) {
    if (!font) {
        std::cerr << "Unable to initialize font!" << std::endl;
        return;
    }

    // frees last texture
    if (textTexture) {
        SDL_DestroyTexture(textTexture);
        textTexture = nullptr;
    }

    surface = TTF_RenderText_Solid(font, MSGBOX.c_str(), textColour);
    if (!surface) {
        std::cerr << "Text render error: " << TTF_GetError() << std::endl;
        return;
    }

    textTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface); // frees surface after texture is made
    // surface is a pointer
    surface = nullptr; // needs to be set to nullptr or there may be a dangling pointer

    if (!textTexture) {
        std::cerr << "Texture creation failed: " << SDL_GetError() << std::endl;
        return;
    }

    rectangle = { X, Y, W, H };

    SDL_SetRenderDrawColor(renderer, R, G, B, 255);
    SDL_RenderCopy(renderer, textTexture, nullptr, &rectangle);
}


