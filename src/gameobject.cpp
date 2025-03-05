#include "gameobject.h"
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

GameObject::GameObject(int x, int y, int w, int h) : pos(x, y), width(w), height(h) {}

void GameObject::move(int dx, int dy) {
    pos.x += dx;
    pos.y += dy;
}

bool GameObject::clampPosition(int minX, int minY, int maxX, int maxY) {
    bool res = false;
    if (pos.x < minX) {
        pos.x = minX;
        res = true;
    }
    if (pos.y < minY) {
        pos.y = minY;
        res = true;
    }
    if (pos.x > maxX) {
        pos.x = maxX;
        res = true;
    }
    if (pos.y > maxY) {
        pos.y = maxY;
        res = true;
    }
    return res;
}

Position GameObject::getPosition() const {
    return pos;
}

int GameObject::getWidth() const {
    return width;
}

int GameObject::getHeight() const {
    return height;
}

void GameObject::render(SDL_Renderer* renderer, int r, int g, int b) const {
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_Rect rect = { pos.x, pos.y, width, height };
    SDL_RenderFillRect(renderer, &rect);
}

void GameObject::setMovingUp(bool moving) { 
    movingUp = moving; 
    if (moving) movingDown = false;
}
void GameObject::setMovingDown(bool moving) { 
    movingDown = moving; 
    if (moving) movingUp = false;
}
void GameObject::setMovingLeft(bool moving) { 
    movingLeft = moving;
    if (moving) movingRight = false;
}
void GameObject::setMovingRight(bool moving) { 
    movingRight = moving; 
    if (moving) movingLeft = false;
}

bool GameObject::isMovingUp() const { return movingUp; }
bool GameObject::isMovingDown() const { return movingDown; }
bool GameObject::isMovingLeft() const { return movingLeft; }
bool GameObject::isMovingRight() const { return movingRight; }

void GameObject::changeMoving() {
    if (movingLeft) {
        setMovingLeft(false);
        setMovingRight(true);
    }
    else if (movingRight) {
        setMovingRight(false);
        setMovingLeft(true);
    }
}

void GameObject::setWidth(int w) {
    this -> width = w;
}

void GameObject::setPos(int x, int y) {
    this -> pos.x = x;
    this -> pos.y = y;
}

void GameObject::setMoving() {
    setMovingUp(false);
    setMovingDown(false);
    setMovingLeft(false);
    setMovingRight(false);
}
