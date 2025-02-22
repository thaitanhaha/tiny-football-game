#include "goal.h"
#include <SDL.h>

Goal::Goal(int x, int y, int w, int h) : GameObject(x, y, w, h) {}

void Goal::render(SDL_Renderer* renderer) const {
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_Rect rect = { pos.x, pos.y, width, height };
    SDL_RenderFillRect(renderer, &rect);
}
