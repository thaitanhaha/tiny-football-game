#include "game.h"

GameManager::GameManager()
    : window(nullptr), renderer(nullptr), gameState(GAMESTATE::RUNNING),
      stick1(78, 400, 100, 10), stick2(334, 400, 100, 10), activeStick(&stick1),
      ball(100, 100, 10), goal(206, 0, 100, 20), score(0), 
      stickCom1(88, 100, 80, 10), stickCom2(344, 100, 80, 10) {
        stickCom1.setMovingLeft(true);
        stickCom2.setMovingRight(true);
      }

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
        else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
            bool keyDown = event.type == SDL_KEYDOWN;
            switch (event.key.keysym.sym) {
                case SDLK_q:
                    if (keyDown) activeStick = (activeStick == &stick1) ? &stick2 : &stick1;
                    break;
                case SDLK_w:
                    activeStick->setMovingUp(keyDown);
                    break;
                case SDLK_s:
                    activeStick->setMovingDown(keyDown);
                    break;
                case SDLK_a:
                    activeStick->setMovingLeft(keyDown);
                    break;
                case SDLK_d:
                    activeStick->setMovingRight(keyDown);
                    break;
            }
        }
    }
}

void GameManager::update() {
    stick1.clampPosition(0, 300, 256 - stick1.getWidth(), 512 - stick1.getHeight() - goal.getHeight());
    stick2.clampPosition(256, 300, 512 - stick2.getWidth(), 512 - stick2.getHeight() - goal.getHeight());

    if (activeStick->isMovingUp()) {
        activeStick->move(0, -3);
    }
    if (activeStick->isMovingDown()) {
        activeStick->move(0, 3);
    }
    if (activeStick->isMovingLeft()) {
        activeStick->move(-3, 0);
    }
    if (activeStick->isMovingRight()) {
        activeStick->move(3, 0);
    }

    bool isCom1ReachEdge = stickCom1.clampPosition(0, goal.getHeight(), 256 - stickCom1.getWidth(), 256 - stickCom2.getHeight());
    bool isCom2ReachEdge = stickCom2.clampPosition(256, goal.getHeight(), 512 - stickCom2.getWidth(), 256 - stickCom2.getHeight());
    
    if (isCom1ReachEdge) stickCom1.changeMoving();
    if (isCom2ReachEdge) stickCom2.changeMoving();

    stickCom1.move(3 * (stickCom1.isMovingRight() ? 1 : -1), 0);
    stickCom2.move(-3 * (stickCom2.isMovingLeft() ? 1 : -1), 0);

    ball.update();
    ball.checkCollision(stick1);
    ball.checkCollision(stick2);
    ball.checkCollision(stickCom1);
    ball.checkCollision(stickCom2);
    if (ball.checkGoal(goal)) {
        score++;
    }
}

void GameManager::renderScore(SDL_Renderer* renderer) {
    std::string scoreStr = std::to_string(score);
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

    int tempColor = (activeStick == &stick1) ? 0 : 1;
    stick1.render(renderer, 255, 255 * tempColor, 255);
    stick2.render(renderer, 255, 255 * (1 - tempColor), 255);

    stickCom1.render(renderer, 255, 255, 255);
    stickCom2.render(renderer, 255, 255, 255);

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