#ifndef GAME_H
#define GAME_H 

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "gameobject.h"

enum class GAMESTATE { RUNNING, QUIT };

class GameManager {
public:
    GameManager();
    ~GameManager();

    void init(const char* title, int xpos, int ypos, int width, int height, Uint32 flags);
    void handleEvents();
    void update();
    void render();
    void clean();
    SDL_Texture* loadTexture(const char* filepath);

    GAMESTATE gameState;
    SDL_Renderer* renderer;

private:
    SDL_Window* window;
    GameObject stick1;
    GameObject stick2;
    GameObject* activeStick;
};

#endif