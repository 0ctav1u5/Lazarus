#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "Bullet.hpp"

void Bullet::RenderBullet(SDL_Renderer* renderer) {
    if (!renderer) {
        std::cerr << "Renderer is nullptr!" << std::endl;
        return;
    }
    const char* newImagePath = nullptr;


    if (DIRECTION == 1 && down == false && right == false && left == false) { // up
        MoveBullet(0, -1); // x, y
        newImagePath = "Images/u.png";
        up = true;
    }
    else if (DIRECTION == 2 && up == false && right == false && left == false) { // down
        MoveBullet(0, 1); // x, y
        newImagePath = "Images/d.png";
        down = true;
    }
    else if (DIRECTION == 3 && down == false && right == false && up == false) { // left
        MoveBullet(-1, 0); // x, y
        newImagePath = "Images/l.png";
        left = true;
    }
    else if (DIRECTION == 4 && down == false && up == false && left == false) { // right
        MoveBullet(1, 0); // x, y
        newImagePath = "Images/r.png";
        right = true;
    }
    else {
        return;
    }

    SDL_Surface* newSurface = IMG_Load(newImagePath);
    if (!newSurface) {
        std::cerr << "IMG_Load Error: " << IMG_GetError() << std::endl;
        exit(1);
    }

    if (SURFACE) {
        SDL_FreeSurface(SURFACE);
    }

    SURFACE = newSurface;

    if (texture) {
        SDL_DestroyTexture(texture);
    }

    texture = SDL_CreateTextureFromSurface(renderer, SURFACE);
    if (!texture) {
        std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        return;
    }
    rect = { X, Y, WIDTH, HEIGHT };
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // RGB Opaqueness
    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}

void Bullet::SetDirection(int num) {
    this->DIRECTION = num;
}

int Bullet::GetDirection() {
    return this->DIRECTION;
}

SDL_Rect& Bullet::GetRect() {
    return this->rect;
}

int Bullet::GetX() {
    return this->X;
}

int Bullet::GetY() {
    return this->Y;
}


void Bullet::MoveBullet(int x, int y) {
    X += (x * SPEED), Y += (y * SPEED);
    rect.x = X;
    rect.y = Y;
}

SDL_Rect Bullet::GetRect() const {
    return rect;
}