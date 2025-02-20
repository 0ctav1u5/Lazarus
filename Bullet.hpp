#ifndef BULLET
#define BULLET
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>


// this class works


class Bullet {
private:
    int SPEED;
    int DAMAGE;
    SDL_Rect rect = {};
    int X, Y;
    const int WIDTH = 2;
    const int HEIGHT = 2;
    int DIRECTION = 0;
    bool left = false, right = false, up = false, down = false;

public:
    Bullet(int speed, int damage, int startX = 250, int startY = 250)
        : SPEED(speed), DAMAGE(damage), X(startX), Y(startY) {
        rect = { X, Y, WIDTH, HEIGHT };
    }

    void RenderBullet(SDL_Renderer* renderer) {
        if (!renderer) {
            std::cerr << "Renderer is nullptr!" << std::endl;
            return;
        }
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // RGB
        SDL_RenderFillRect(renderer, &rect); // Draws the rectangle
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
    }

    void SetDirection(int num) {
        this->DIRECTION = num;
    }

    int GetDirection() {
        return this->DIRECTION;
    }

    int GetX() {
        return this->X;
    }

    int GetY() {
        return this->Y;
    }


    void MoveBullet(int x, int y) {
        X += (x * SPEED), Y += (y * SPEED);
        rect.x = X;  
        rect.y = Y;  
    }

    SDL_Rect GetRect() const {
        return rect;
    }
};

#endif