#include "game.h"
#include <random>

GameManager::GameManager()
    : window(nullptr), renderer(nullptr), gameState(GAMESTATE::RUNNING),
      stick1(78, 400, 100, 5), stick2(334, 400, 100, 5), 
      activeStick(&stick1), activeStickCom(NULL),
      ball(100, 100, 5), score(0), scoreCom(0), 
      goal(206, 0, 100, 10), goalCom(206, 502, 100, 10),
      stickCom1(88, 100, 80, 5), stickCom2(344, 100, 80, 5), playerMode(0), gameTime(60),
      obstacle(0, 0, 175, 5), obstacleAppear(false) {
        startTime = SDL_GetTicks();
        remainingTime = gameTime;
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

void GameManager::loadSprites() {
    for (int i = 0; i < 10; i++) {
        std::string filename = "sprite/Number" + std::to_string(i) + " 7x10.png";
        numberTextures[i] = loadTexture(filename.c_str());
        if (!numberTextures[i]) {
            std::cerr << "Failed to load " << filename << ": " << SDL_GetError() << std::endl;
        }
    }
    std::string option1 = "sprite/1player.png";
    std::string option2 = "sprite/2players.png";
    option1Texture = loadTexture(option1.c_str());
    option2Texture = loadTexture(option2.c_str());
    std::string logo = "sprite/logo.png";
    logoTexture = loadTexture(logo.c_str());
}

void GameManager::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            gameState = GAMESTATE::QUIT;
        }
        else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
            bool keyDown = event.type == SDL_KEYDOWN;
            if (keyDown && gameState == GAMESTATE::MENU) {
                switch (event.key.keysym.sym) {
                    case SDLK_1:
                        gameState = GAMESTATE::RUNNING;
                        playerMode = 1;
                        startTime = SDL_GetTicks();
                        remainingTime = gameTime;
                        stickCom1.setMovingLeft(true);
                        stickCom2.setMovingRight(true);
                        stickCom1.setWidth(80);
                        stickCom2.setWidth(80);
                        stickCom1.setPos(88, 100);
                        stickCom2.setPos(344, 100);
                        stick1.setMoving();
                        stick2.setMoving();
                        stick1.setPos(78, 400);
                        stick2.setPos(334, 400);
                        activeStick = &stick1;
                        activeStickCom = NULL;
                        score=0; scoreCom=0;
                        break;
                    case SDLK_2:
                        gameState = GAMESTATE::RUNNING;
                        playerMode = 2;
                        startTime = SDL_GetTicks();
                        remainingTime = gameTime;
                        stickCom1.setMoving();
                        stickCom2.setMoving();
                        stickCom1.setPos(78, 100);
                        stickCom2.setPos(334, 100);
                        stickCom1.setWidth(100);
                        stickCom2.setWidth(100);
                        stick1.setMoving();
                        stick2.setMoving();
                        stick1.setPos(78, 400);
                        stick2.setPos(334, 400);
                        activeStickCom = &stickCom1;
                        activeStick = &stick1;
                        score=0; scoreCom=0;
                        break;
                }
            }
            else if (gameState == GAMESTATE::RUNNING) {
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
                    case SDLK_RCTRL:
                        if (keyDown && playerMode == 2) activeStickCom = (activeStickCom == &stickCom1) ? &stickCom2 : &stickCom1;
                        break;
                    case SDLK_UP:
                        if (playerMode == 2) activeStickCom->setMovingUp(keyDown);
                        break;
                    case SDLK_DOWN:
                        if (playerMode == 2) activeStickCom->setMovingDown(keyDown);
                        break;
                    case SDLK_LEFT:
                        if (playerMode == 2) activeStickCom->setMovingLeft(keyDown);
                        break;
                    case SDLK_RIGHT:
                        if (playerMode == 2) activeStickCom->setMovingRight(keyDown);
                        break;
                }
            }
        }
    }
}

void GameManager::update() {
    if (gameState == GAMESTATE::RUNNING) {
        Uint32 currentTime = SDL_GetTicks();
        int elapsedTime = (currentTime - startTime) / 1000;
        if (elapsedTime >= 1) {
            remainingTime = gameTime - elapsedTime;
            if (remainingTime <= 0) {
                gameState = GAMESTATE::MENU;
                playerMode = 0;
            }
            else if (remainingTime % 15 == 0 && obstacleAppear == false) {
                setObstacle(true);
            }
            else if ((remainingTime + 5) % 15 == 0 && obstacleAppear == true) {
                setObstacle(false);
            }
        }
    }

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

    bool isCom1ReachEdge = stickCom1.clampPosition(0, goal.getHeight(), 256 - stickCom1.getWidth(), 212 - stickCom2.getHeight());
    bool isCom2ReachEdge = stickCom2.clampPosition(256, goal.getHeight(), 512 - stickCom2.getWidth(), 212 - stickCom2.getHeight());
    
    if (playerMode == 1) {
        if (isCom1ReachEdge) stickCom1.changeMoving();
        if (isCom2ReachEdge) stickCom2.changeMoving();
    
        stickCom1.move(3 * (stickCom1.isMovingRight() ? 1 : -1), 0);
        stickCom2.move(-3 * (stickCom2.isMovingLeft() ? 1 : -1), 0);
    }
    else if (playerMode == 2) {
        if (activeStickCom->isMovingUp()) {
            activeStickCom->move(0, -3);
        }
        if (activeStickCom->isMovingDown()) {
            activeStickCom->move(0, 3);
        }
        if (activeStickCom->isMovingLeft()) {
            activeStickCom->move(-3, 0);
        }
        if (activeStickCom->isMovingRight()) {
            activeStickCom->move(3, 0);
        }
    }

    ball.update();
    ball.checkCollision(stick1, false);
    ball.checkCollision(stick2, false);
    ball.checkCollision(stickCom1, false);
    ball.checkCollision(stickCom2, false);
    if (obstacleAppear) ball.checkCollision(obstacle, true);
    if (ball.checkGoal(goal)) {
        score++;
    }
    if (playerMode == 2 && ball.checkGoal(goalCom)) {
        scoreCom++;
    }
}

void GameManager::setObstacle(bool active) {
    obstacleAppear = active;
    if (active) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> posYDist(222, 290);
        std::uniform_int_distribution<int> dirDist(0, 1);
        if (dirDist(gen) == 0) {
            obstacle.setPos(0, posYDist(gen));
        }
        else {
            obstacle.setPos(337, posYDist(gen));
        }
    }
}

void GameManager::renderScore(SDL_Renderer* renderer, int y, int score) {
    std::string scoreStr = std::to_string(score);
    int digitWidth = 21, digitHeight = 30;
    int startX = 10;

    for (char digit : scoreStr) {
        int num = digit - '0';

        SDL_Rect srcRect = { 0, 0, 7, 10 };
        SDL_Rect destRect = { startX, y, digitWidth, digitHeight };

        SDL_RenderCopy(renderer, numberTextures[num], &srcRect, &destRect);
        startX += digitWidth + 5;
    }
}

void GameManager::renderTime(SDL_Renderer* renderer) {
    int minutes = remainingTime / 60;
    int seconds = remainingTime % 60;

    std::string timeStr = std::to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + std::to_string(seconds);
    int digitWidth = 21, digitHeight = 30;
    int startX = 415;

    for (char digit : timeStr) {
        if (digit == ':') {
            startX += 10;
            continue;
        }

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

    if (gameState == GAMESTATE::MENU)
    {
        SDL_Rect logoRect = { 59, 100, 393, 32 };
        SDL_Rect menu1Rect = { 148, 250, 216, 24 };
        SDL_Rect menu2Rect = { 136, 350, 240, 24 };

        SDL_RenderCopy(renderer, logoTexture, NULL, &logoRect);
        SDL_RenderCopy(renderer, option1Texture, NULL, &menu1Rect);
        SDL_RenderCopy(renderer, option2Texture, NULL, &menu2Rect);
    }
    else if (gameState == GAMESTATE::RUNNING)
    {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect border = { 0, 0, 512, 512 };
        SDL_RenderDrawRect(renderer, &border);
    
        int tempColor = (activeStick == &stick1) ? 0 : 1;
        stick1.render(renderer, 255, 255 * tempColor, 255);
        stick2.render(renderer, 255, 255 * (1 - tempColor), 255);
    
        if (playerMode == 2) {
            int tempColorCom = (activeStickCom == &stickCom1) ? 0 : 1;
            stickCom1.render(renderer, 255, 255, 255 * tempColorCom);
            stickCom2.render(renderer, 255, 255, 255 * (1 - tempColorCom));
        }
        else {
            stickCom1.render(renderer, 255, 255, 255);
            stickCom2.render(renderer, 255, 255, 255);
        }
    
        ball.render(renderer);
        goal.render(renderer);
        if (obstacleAppear) obstacle.render(renderer, 255, 255, 255);
        if (playerMode == 2) goalCom.render(renderer);
    
        if (playerMode == 1) renderScore(renderer, 10, score);
        else {
            renderScore(renderer, 472, score);
            renderScore(renderer, 10, scoreCom);
        }
        renderTime(renderer);
    }

    SDL_RenderPresent(renderer);
}

void GameManager::clean() {
    for (int i = 0; i < 10; i++) {
        if (numberTextures[i]) {
            SDL_DestroyTexture(numberTextures[i]);
        }
    }
    SDL_DestroyTexture(option1Texture);
    SDL_DestroyTexture(option2Texture);
    SDL_DestroyTexture(logoTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}