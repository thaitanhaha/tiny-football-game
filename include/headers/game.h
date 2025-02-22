#ifndef GAME_H
#define GAME_H 

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "gameobject.h"
#include "ball.h"

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
    void renderScore(SDL_Renderer* renderer);
    void loadNumberSprites();

    GAMESTATE gameState;
    SDL_Renderer* renderer;

private:
    SDL_Window* window;
    GameObject stick1;
    GameObject stick2;
    GameObject* activeStick;
    Ball ball;
    Goal goal;
    int score;
    SDL_Texture* numberTextures[10];
};

#endif