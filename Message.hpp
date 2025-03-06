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

    void SetColour(Uint8 r, Uint8 g, Uint8 b);
    void SetPosition(int x, int y, int w, int h);
    void RenderMessage(SDL_Renderer* renderer);
};

#endif