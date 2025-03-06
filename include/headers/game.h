#ifndef GAME_H
#define GAME_H 

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "gameobject.h"
#include "ball.h"

enum class GAMESTATE {
    MENU,
    RUNNING,
    QUIT
};

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
    void renderScore(SDL_Renderer* renderer, int y, int score);
    void renderTime(SDL_Renderer* renderer);
    void loadSprites();
    void setObstacle(bool active);

    GAMESTATE gameState;
    SDL_Renderer* renderer;

private:
    SDL_Window* window;
    GameObject stick1, stick2;
    GameObject* activeStick;
    GameObject stickCom1, stickCom2;
    GameObject* activeStickCom;
    GameObject obstacle;
    bool obstacleAppear;
    Ball ball;
    Goal goal, goalCom;
    int score, scoreCom;
    SDL_Texture* numberTextures[10];
    SDL_Texture* option1Texture;
    SDL_Texture* option2Texture;
    SDL_Texture* logoTexture;
    int playerMode;
    Uint32 startTime;
    int remainingTime, gameTime;
};

#endif