#include "game.h"

GameManager::GameManager()
    : window(nullptr), renderer(nullptr), gameState(GAMESTATE::RUNNING),
      stick1(50, 200, 50, 10), stick2(300, 300, 50, 10), activeStick(&stick1) {}

GameManager::~GameManager() {
    clean();
}

void GameManager::init(const char* title, int xpos, int ypos, int width, int height, Uint32 flags) {
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    }
}

SDL_Texture* GameManager::loadTexture(const char* filepath) {
    SDL_Surface* tempSurface = IMG_Load(filepath);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    return texture;
}

void GameManager::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            gameState = GAMESTATE::QUIT;
        }
        else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_q:
                    activeStick = (activeStick == &stick1) ? &stick2 : &stick1;
                    break;
                case SDLK_w:
                    activeStick->move(0, -5);
                    break;
                case SDLK_s:
                    activeStick->move(0, 5);
                    break;
                case SDLK_a:
                    activeStick->move(-5, 0);
                    break;
                case SDLK_d:
                    activeStick->move(5, 0);
                    break;
            }
        }
    }
}

void GameManager::update() {
    stick1.clampPosition(0, 0, 256 - stick1.getWidth(), 512 - stick1.getHeight());
    stick2.clampPosition(256, 0, 512 - stick2.getWidth(), 512 - stick2.getHeight());
}

void GameManager::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect border = { 0, 0, 512, 512 };
    SDL_RenderDrawRect(renderer, &border);

    stick1.render(renderer);
    stick2.render(renderer);

    SDL_RenderPresent(renderer);
}

void GameManager::clean() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}