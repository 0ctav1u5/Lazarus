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
    const char* IMAGEPATH = nullptr;
    SDL_Surface* SURFACE = nullptr;
    SDL_Texture* texture = nullptr;



    int X, Y;
    const int WIDTH = 12;
    const int HEIGHT = 12;
    int DIRECTION = 0;
    bool left = false, right = false, up = false, down = false;

public:
    Bullet(int speed, int damage, int startX = 250, int startY = 250)
        : SPEED(speed), DAMAGE(damage), X(startX), Y(startY) {
        rect = { X, Y, WIDTH, HEIGHT };
    }
    void RenderBullet(SDL_Renderer* renderer);
    void SetDirection(int num);
    int GetDirection();
    SDL_Rect& GetRect();
    int GetX();
    int GetY();
    void MoveBullet(int x, int y);
    SDL_Rect GetRect() const;
};

#endif