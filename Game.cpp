//
// Created by dar on 11/27/15.
//

#include "Game.h"

Game::Game(Core *core) : core(core), renderer(core) {
}

void Game::run() {
    this->renderer.init();
    SDL_StartTextInput();
    double deltaTime = timer.GetDelta();
    double accumulator = 0.0;
    const double TIME_STEP = 0.03;
    while (this->core->isRunning()) {
        deltaTime = timer.GetDelta();
        accumulator += deltaTime;
        while (accumulator > TIME_STEP) {
            this->update();
            accumulator -= TIME_STEP;
        }
        this->renderer.run();
    }
    SDL_StopTextInput();
}

void Game::update() {
    handleKeyboard();
    this->core->getMap()->update();
    this->core->getMap()->getWorld()->Step(0.03, 8, 3);
    this->core->setCamX(-this->core->getPlayer()->getX() * blockSize * generalScale);
    this->core->setCamY(-this->core->getPlayer()->getY() * blockSize * generalScale);
}

void Game::handleKeyboard() {
    float SPEED = 1.0f;
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);
    if (keystate[SDL_SCANCODE_W]) {
        this->core->getPlayer()->setVelY(-SPEED);
    }
    if (keystate[SDL_SCANCODE_S]) {
        this->core->getPlayer()->setVelY(SPEED);
    }
    if (keystate[SDL_SCANCODE_A]) {
        this->core->getPlayer()->setVelX(-SPEED);
    }
    if (keystate[SDL_SCANCODE_D]) {
        this->core->getPlayer()->setVelX(SPEED);
    }
    if (keystate[SDL_SCANCODE_Q]) {
        this->core->stop();
    }
}

Game::~Game() {
    delete this->core;
}
