#ifndef GOAL_H
#define GOAL_H

#include <SDL.h>
#include <SDL_image.h>
#include "gameobject.h"

class Goal : public GameObject {
public:
    Goal(int x, int y, int w, int h);
    void render(SDL_Renderer* renderer) const;
};

#endif
