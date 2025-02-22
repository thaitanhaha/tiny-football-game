#include "game.h"

GameManager::GameManager()
    : window(nullptr), renderer(nullptr), gameState(GAMESTATE::RUNNING),
      stick1(50, 200, 100, 10), stick2(300, 300, 100, 10), activeStick(&stick1),
      ball(100, 100, 10), goal(216, 0, 80, 20), score(0) {}

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

void GameManager::loadNumberSprites() {
    for (int i = 0; i < 10; i++) {
        std::string filename = "sprite/Number" + std::to_string(i) + " 7x10.png";
        numberTextures[i] = loadTexture(filename.c_str());
        if (!numberTextures[i]) {
            std::cerr << "Failed to load " << filename << ": " << SDL_GetError() << std::endl;
        }
    }
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
                    activeStick->move(0, -15);
                    break;
                case SDLK_s:
                    activeStick->move(0, 15);
                    break;
                case SDLK_a:
                    activeStick->move(-15, 0);
                    break;
                case SDLK_d:
                    activeStick->move(15, 0);
                    break;
            }
        }
    }
}

void GameManager::update() {
    stick1.clampPosition(0, 0, 256 - stick1.getWidth(), 512 - stick1.getHeight());
    stick2.clampPosition(256, 0, 512 - stick2.getWidth(), 512 - stick2.getHeight());

    ball.update();
    ball.checkCollision(stick1);
    ball.checkCollision(stick2);
    if (ball.checkGoal(goal)) {
        score++;
    }
}

void GameManager::renderScore(SDL_Renderer* renderer) {
    std::string scoreStr = std::to_string(score);
    std::cout<<score;
    int digitWidth = 21, digitHeight = 30;
    int startX = 10;

    for (char digit : scoreStr) {
        int num = digit - '0';

        SDL_Rect srcRect = { 0, 0, 7, 10 };
        SDL_Rect destRect = { startX, 10, digitWidth, digitHeight };

        SDL_RenderCopy(renderer, numberTextures[num], &srcRect, &destRect);
        startX += digitWidth + 5;
    }
}

void GameManager::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect border = { 0, 0, 512, 512 };
    SDL_RenderDrawRect(renderer, &border);

    stick1.render(renderer);
    stick2.render(renderer);
    ball.render(renderer);
    goal.render(renderer);

    renderScore(renderer);

    SDL_RenderPresent(renderer);
}

void GameManager::clean() {
    for (int i = 0; i < 10; i++) {
        if (numberTextures[i]) {
            SDL_DestroyTexture(numberTextures[i]);
        }
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}