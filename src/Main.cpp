#include <SDL.h>
#include <iostream>
#include <random>
#include "game.h"
using namespace std;

const int screenWidth = 512;
const int screenHeight = 512;

int main(int argc, char *argv[])
{
    GameManager game;

    game.init("TinyFootball", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    // SDL_Texture* background = game.loadTexture("sprite/background.png");

    int frame = 0;
    game.loadNumberSprites();

    while (game.gameState != GAMESTATE::QUIT)
    {
        game.handleEvents();
        game.update();
        game.render();
        SDL_Delay(16);
    }

    return 0;
}
