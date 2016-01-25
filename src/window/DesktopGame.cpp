//
// Created by dar on 11/27/15.
//

#include <stdio.h>
#include <stdlib.h>
#include "Game.h"
#include "../core/Core.h"
#include "../core/map/TiledTxtMapLoader.h"
#include "render/RenderManager.h"
#include "../logging.h"

Game::Game() {
    this->reload();
}

void Game::reload() {
    if (this->core == nullptr) {
        MapLoader *mapLoader = new TiledTxtMapLoader("test_map.txt");
        Map *bmap = mapLoader->loadMap();
        this->core = new Core(bmap);
        delete mapLoader;
    }
    SDL_StartTextInput();
}

void Game::tick(double deltaTime) {
    for (int i = 0; i < 256; i++) {
        //if (this->pressDelays[i] > 0) this->pressDelays[i]--;
    }

    this->core->getMap()->update();
    this->core->getMap()->getWorld()->Step(deltaTime, 8, 3);
    for (int i = 0; i < this->core->getMap()->getEntities().size(); i++) {
        Entity *entity = this->core->getMap()->getEntities().at(i);
        if (entity->isToBeDeleted()) {
            this->core->getMap()->removeEntity(entity);
            i--;

        }
    }
    double dx = (this->core->getPlayer()->getX() + this->core->getPlayer()->getWidth() / 2 - 1) * this->core->getBlockSize() * this->core->getGeneralScale() + this->core->getCamX();
    double dy = (this->core->getPlayer()->getY() + this->core->getPlayer()->getHeight() / 2 - 1) * this->core->getBlockSize() * this->core->getGeneralScale() + this->core->getCamY();
    if (abs(dx) > 2) this->core->setCamX(-this->core->getCamX() + (dx) * 0.05);
    if (abs(dy) > 2) this->core->setCamY(-this->core->getCamY() + (dy) * 0.05);
}

void Game::handleKeyboard() {
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);
    double playerSpeed = this->core->getPlayer()->getSpeed();
    if (keystate[SDL_SCANCODE_W]) {
        this->core->getPlayer()->applyImpulse(0, -playerSpeed);
    }
    if (keystate[SDL_SCANCODE_S]) {
        this->core->getPlayer()->applyImpulse(0, playerSpeed);
    }
    if (keystate[SDL_SCANCODE_A]) {
        this->core->getPlayer()->applyImpulse(-playerSpeed, 0);
    }
    if (keystate[SDL_SCANCODE_D]) {
        this->core->getPlayer()->applyImpulse(playerSpeed, 0);
    }
    if (keystate[SDL_SCANCODE_Q]) {
        this->core->stop();
    }
    if (keystate[SDL_SCANCODE_MINUS]) {
        this->core->setBlockSize(this->core->getBlockSize() - 0.15);
    }
    if (keystate[SDL_SCANCODE_EQUALS]) {
        this->core->setBlockSize(this->core->getBlockSize() + 0.15);
    }
}

void Game::handleKeypress(void *event1) {
    SDL_Event *event = (SDL_Event *) event1;
    switch (event->type) {
        case SDL_KEYDOWN: {
            SDL_Keycode key = event->key.keysym.sym;
            unsigned char delay_tmp = 255;
            static int TELEPORT_DELAY = 255 - 25;
            switch (key) { //TODO move to KeyboardManager (?)
                case SDLK_a:
                    /*if (this->pressDelays[event->key.keysym.sym] > TELEPORT_DELAY)*/ {
                    //this->core->getPlayer()->teleport(-1.95, 0);
                    delay_tmp = 0, resetMovementPressDelays();
                }
                    break;
                case SDLK_d:
                    /*if (this->pressDelays[event->key.keysym.sym] > TELEPORT_DELAY)*/ {
                    //this->core->getPlayer()->teleport(1.95, 0);
                    delay_tmp = 0, resetMovementPressDelays();
                }
                    break;
                case SDLK_w:
                    /*if (this->pressDelays[event->key.keysym.sym] > TELEPORT_DELAY)*/ {
                    //this->core->getPlayer()->teleport(0, -1.95);
                    delay_tmp = 0, resetMovementPressDelays();
                }
                    break;
                case SDLK_s:
                    /*if (this->pressDelays[event->key.keysym.sym] > TELEPORT_DELAY)*/ {
                    //this->core->getPlayer()->teleport(0, 1.95);
                    delay_tmp = 0, resetMovementPressDelays();
                }
                    break;
                case SDLK_c: {
                    double angle = atan2(0, 0) + M_PI; //TODO
                    EntityBullet *p = new EntityBullet(this->core, angle, 1);
                    p->setX(this->core->getPlayer()->getX() - (p->getWidth()) + 0.5 * cos(angle));
                    p->setY(this->core->getPlayer()->getY() - (p->getHeight()) + 0.5 * sin(angle));
                    this->core->getMap()->addEntity(p);
                    break;
                }
                case SDLK_n: {
                    EntityToy *p = new EntityToy(this->core);
                    p->setX(this->core->getPlayer()->getX() - this->core->getPlayer()->getWidth() / 2);
                    p->setY(this->core->getPlayer()->getY() - this->core->getPlayer()->getHeight() / 2);
                    this->core->getMap()->addEntity(p);
                    break;
                }
                case SDLK_m: {
                    SimpleShape *p = new SimpleShape(this->core, (unsigned int) (rand() % 3));
                    p->setX(this->core->getPlayer()->getX() - this->core->getPlayer()->getWidth() / 2);
                    p->setY(this->core->getPlayer()->getY() - this->core->getPlayer()->getHeight() / 2);
                    this->core->getMap()->addEntity(p);
                    break;
                }
                case SDLK_k: {
                    if (this->core->getPlayer()->getToy() == nullptr) {
                        this->core->getPlayer()->setToy();
                    } else {
                        this->core->getPlayer()->eject();
                    }
                }
                default:
                    break;
            }
            if (event->key.keysym.sym >= 0 && event->key.keysym.sym < 256)
                //this->pressDelays[event->key.keysym.sym] = delay_tmp;
                break;
        }
        case SDL_KEYUP:
            break;

        default:
            break;
    }
}

void Game::resetMovementPressDelays() {
    //this->pressDelays[SDLK_a] = this->pressDelays[SDLK_d] = this->pressDelays[SDLK_w] = this->pressDelays[SDLK_s] = 0;
}

void Game::handleClick(int unused, int action, float x, float y) {
    double playerSpeed = this->core->getPlayer()->getSpeed();
    this->core->getPlayer()->applyImpulse(playerSpeed, 0);
}

Game::~Game() {
    SDL_StopTextInput();
    delete this->core;
}
