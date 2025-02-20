#include "gameobject.h"
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

GameObject::GameObject(int x, int y, int w, int h) : pos(x, y), width(w), height(h) {}

void GameObject::move(int dx, int dy) {
    pos.x += dx;
    pos.y += dy;
}

void GameObject::clampPosition(int minX, int minY, int maxX, int maxY) {
    if (pos.x < minX) pos.x = minX;
    if (pos.y < minY) pos.y = minY;
    if (pos.x > maxX) pos.x = maxX;
    if (pos.y > maxY) pos.y = maxY;
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

void GameObject::render(SDL_Renderer* renderer) const {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect rect = { pos.x, pos.y, width, height };
    SDL_RenderFillRect(renderer, &rect);
}