#ifndef MESSAGE
#define MESSAGE
#include <iostream>
#include <SDL_ttf.h>
#include <SDL.h>

class Message {
private:
	std::string MSGBOX = "";
	Uint8 R = 255, G = 0, B = 0;
	int X, Y, W, H;
	SDL_Texture* textTexture = nullptr;
	SDL_Surface* surface = {};
	SDL_Color textColour = {R, G, B};
	SDL_Rect rectangle = {};
	TTF_Font* font = nullptr;
public:

	Message(std::string message, int x, int y, int w, int h) : MSGBOX(message),
	X(x), Y(y), W(w), H(h) {
		font = TTF_OpenFont("Fonts/Vipnagorgialla Rg.otf", 24);
		if (!font) {
			std::cerr << "Font cannot be initialised!" << std::endl;
			return;
		}
	} 

	~Message() {
		if (textTexture) {
			SDL_DestroyTexture(textTexture);
		}
		if (font) {
			TTF_CloseFont(font);
		}
		if (surface) {
			SDL_FreeSurface(surface);
		}
	}

	void SetColour(Uint8 r, Uint8 g, Uint8 b) {
		this->R = r;
		this->G = g;
		this->B = b;
		textColour = { R, G, B };
	}

	void SetPosition(int x, int y, int w, int h) {
		this->X = x;
		this->Y = y;
		this->W = w;
		this->H = h;
	}

	void RenderMessage(SDL_Renderer* renderer) {
		if (!font) {
			std::cerr << "Unable to initialise font!" << std::endl;
			return;
		}
		surface = TTF_RenderText_Solid(font, MSGBOX.c_str(), textColour);

		if (surface) {
			textTexture = SDL_CreateTextureFromSurface(renderer, surface);
			SDL_FreeSurface(surface);
		}
		else {
			std::cerr << "Text render error: " << TTF_GetError() << std::endl;
			return;
		}

		rectangle = { X, Y, W, H }; // rectangle for text 


		SDL_SetRenderDrawColor(renderer, R, G, B, 255);  // r, g, b, opaqueness
		SDL_RenderCopy(renderer, textTexture, nullptr, &rectangle);
	}
};

#endif