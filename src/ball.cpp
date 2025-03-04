#include "ball.h"
#include <SDL.h>
#include <cmath>
#include <random>

std::random_device rd;
std::mt19937 gen(rd());

Ball::Ball(int x, int y, int r)
    : GameObject(x, y, r * 2, r * 2), radius(r), velocityX(2.0f), velocityY(2.0f) {}

void Ball::update() {
    pos.x += (int)velocityX;
    pos.y += (int)velocityY;

    if (pos.x <= 0 || pos.x + radius * 2 >= 512) {
        velocityX = -velocityX;
        velocityX = (velocityX < 0) ? -2.0f : 2.0f;
        pos.x = (pos.x <= 0) ? 0 : 512 - radius * 2;
    }
    if (pos.y <= 0 || pos.y + radius * 2 >= 512) {
        velocityY = -velocityY;
        velocityY = (velocityY < 0) ? -2.0f : 2.0f;
        pos.y = (pos.y <= 0) ? 0 : 512 - radius * 2;
    }
}

void Ball::checkCollision(GameObject& stick) {
    SDL_Rect stickRect = { stick.getPosition().x, stick.getPosition().y, stick.getWidth(), stick.getHeight() };

    int ballCenterX = pos.x + radius;
    int ballCenterY = pos.y + radius;

    if (ballCenterX + radius > stickRect.x &&
        ballCenterX - radius < stickRect.x + stickRect.w &&
        ballCenterY + radius > stickRect.y &&
        ballCenterY - radius < stickRect.y + stickRect.h) {
        
        bool fromTop = ballCenterY < stickRect.y + stickRect.h / 2;
        
        if (fromTop) {
            velocityY = -5.0f;
            pos.y = stickRect.y - radius * 2;
        } else {
            velocityY = 5.0f;
            pos.y = stickRect.y + stickRect.h + radius;
        }
    }
}

bool Ball::checkGoal(Goal& goal) {
    SDL_Rect goalRect = { goal.getPosition().x, goal.getPosition().y, goal.getWidth(), goal.getHeight() };

    int ballCenterX = pos.x + radius;
    int ballCenterY = pos.y + radius;

    if (ballCenterX > goalRect.x && ballCenterX < goalRect.x + goalRect.w &&
        ballCenterY > goalRect.y && ballCenterY < goalRect.y + goalRect.h) {
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> posDist(196, 316);
        std::uniform_int_distribution<int> velDist(0, 1);
        pos.x = posDist(gen);
        pos.y = 100;
        velocityX = velDist(gen) ? 2.0f : -2.0f;
        velocityY = 2.0f;
        return true;
    }

    return false;
}


void Ball::render(SDL_Renderer* renderer) const {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w;
            int dy = radius - h;
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, pos.x + dx, pos.y + dy);
            }
        }
    }
}
