#ifndef BALL_H
#define BALL_H

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "gameobject.h"
#include "goal.h"

class Ball : public GameObject {
public:
    Ball(int x, int y, int r);
    void update();
    void render(SDL_Renderer* renderer) const;
    void checkCollision(GameObject& stick);
    bool checkGoal(Goal& goal);

private:
    int radius;
    float velocityX, velocityY;
};

#endif
