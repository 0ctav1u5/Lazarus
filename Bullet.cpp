#include <iostream>
#include <SDL.h>
#include "Bullet.hpp"

void Bullet::RenderBullet(SDL_Renderer* renderer) {
    if (!renderer) {
        std::cerr << "Renderer is nullptr!" << std::endl;
        return;
    }
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // RGB
    SDL_RenderFillRect(renderer, &rect);
    if (DIRECTION == 1 && down == false && right == false && left == false) { // up
        MoveBullet(0, -1); // x, y
        up = true;
    }
    else if (DIRECTION == 2 && up == false && right == false && left == false) { // down
        MoveBullet(0, 1); // x, y
        down = true;
    }
    else if (DIRECTION == 3 && down == false && right == false && up == false) { // left
        MoveBullet(-1, 0); // x, y
        left = true;
    }
    else if (DIRECTION == 4 && down == false && up == false && left == false) { // right
        MoveBullet(1, 0); // x, y
        right = true;
    }
    else {
        return;
    }
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