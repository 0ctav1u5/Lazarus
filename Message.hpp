#ifndef MESSAGE
#define MESSAGE
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>

class Message {
private:
    std::string MSGBOX;
    Uint8 R = 255, G = 0, B = 0;
    int X, Y, W, H;
    SDL_Texture* textTexture = nullptr;
    SDL_Surface* surface = nullptr;
    SDL_Color textColour = {};
    SDL_Rect rectangle = {};
    TTF_Font* font = nullptr;

public:
    Message(std::string message, int x, int y, int w, int h)
        : MSGBOX(std::move(message)), X(x), Y(y), W(w), H(h), textColour{ R, G, B } {

        font = TTF_OpenFont("Fonts/Vipnagorgialla Rg.otf", 24);
        if (!font) {
            std::cerr << "Font cannot be initialized: " << TTF_GetError() << std::endl;
        }
    }

    ~Message() {
        if (textTexture != nullptr) {
            SDL_DestroyTexture(textTexture);
            textTexture = nullptr;
        }
    }


    Message(const Message&) = delete;
    Message& operator=(const Message&) = delete;

    void SetColour(Uint8 r, Uint8 g, Uint8 b) {
        R = r;
        G = g;
        B = b;
        textColour = { R, G, B };
    }

    void SetPosition(int x, int y, int w, int h) {
        X = x;
        Y = y;
        W = w;
        H = h;
    }

    void RenderMessage(SDL_Renderer* renderer) {
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

};

#endif