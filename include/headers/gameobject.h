#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

struct Position {
    int x, y;
    Position(int x = 0, int y = 0) : x(x), y(y) {}
};

class GameObject {
public:
    GameObject(int x, int y, int w, int h);
    void move(int dx, int dy);
    void clampPosition(int minX, int minY, int maxX, int maxY);
    Position getPosition() const;
    int getWidth() const;
    int getHeight() const;
    void render(SDL_Renderer* renderer) const;

protected:
    Position pos;
    int width, height;
};

#endif