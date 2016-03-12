//
// Created by dar on 11/27/15.
//

#include <stdio.h>
#include <stdlib.h>
#include <gui/GuiText.h>
#include <gui/GuiButton.h>
#include "Game.h"
#include "../core/Core.h"
#include "../core/map/TiledTxtMapLoader.h"
#include "render/RenderManager.h"
#include "InputManager.h"
#include <core/map/entity/EntityBullet.h>
#include <gui/GuiTextBubble.h>
#include <core/map/entity/EntityDoor.h>
#include <core/map/entity/EntityFurniture.h>
#include "../logging.h"

Game::Game(const std::function<bool(Window *window)> &switchWindow) : Window(switchWindow) {
    initShapeDefinitions();
    MapLoader *mapLoader = new TiledTxtMapLoader("test_map");
    Map *bmap = mapLoader->loadMap();
    this->core = new Core(bmap);
    delete mapLoader;

    double ringScale = 2 * this->core->getBlockSize();
    this->entityRotationRing = new GuiElement(GUI_TOP_LEFT, 0, 0, ringScale, ringScale, 6);
    this->entityRotationRing->setVisible(false);
    this->entityRotationRing->setAngle(2);
    this->guiElements.push_back(this->entityRotationRing);

    GuiElement *character = new GuiElement(GUI_TOP_RIGHT, 0, 50, 225, 225, 17);
    character->setVisible(false);
    this->guiElements.push_back(character);
    GuiElement *window = new GuiTextBubble(GUI_TOP_RIGHT, -500, 0, 280, 51);
    window->setVisible(false);
    this->guiElements.push_back(window);
    GuiText *text = new GuiText(string("Oh... Em... Hello!"), -500, 76, GUI_TOP_LEFT, 24, 0x666666FF, 0);
    text->setVisible(false);
    this->guiElements.push_back(text);
    this->popup[0] = character;
    this->popup[1] = window;
    this->popup[2] = text;

    GuiText *t = new GuiText(string("Dev Build: ") + __DATE__ + " " + __TIME__, 15, 15, GUI_BOTTOM_LEFT, 32, 0xFFFFFFFF, 0);
    this->guiElements.push_back(t);
}

void Game::reload(unsigned int windowWidth, unsigned int windowHeight) {
    for (GuiElement *e : this->guiElements) {
        e->reinit(windowWidth, windowHeight);
    }

    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;
    SDL_StartTextInput();
}

void Game::tick(double deltaTime) {
    this->core->getMap()->update(deltaTime);
    this->core->getMap()->getWorld()->Step(deltaTime, 8, 3);
    for (int i = 0; i < this->core->getMap()->getEntities().size(); i++) {
        Entity *entity = this->core->getMap()->getEntities().at(i);
        if (entity->isToBeDeleted()) {
            this->core->getMap()->removeEntity(entity);
            i--;
        }
    }

    static float ghostMovement = -0.2f;
    static float dialogueAlpha = -0.2f;
    static int dialogueNum = 1;
    static float dialogueDuration = 1.5f;
    ghostMovement += deltaTime * 0.95;

    if (ghostMovement >= 0) {
        if (ghostMovement > 1) ghostMovement = 1;

        double gx;
        double gy;
        double ghostAlpha;
        double ghostAngle;
        double ghostSize;

        if (dialogueNum >= 5) {
            gx = (this->windowWidth * 0.7 - this->popup[0]->getWidth() / 2) + this->windowWidth * 0.3 * ghostMovement;
            gy = (this->windowHeight / 4) + this->popup[0]->getHeight() / 2 - ghostMovement * ghostMovement * (this->windowHeight / 4 - 50);
            ghostAlpha = 1.0f;
            ghostAngle = 0.3f * M_PI_2 * (1 - (2 * ghostMovement - 1) * (2 * ghostMovement - 1)) * (1.5 - ghostMovement);
            ghostSize = 225 - 75 * ghostMovement;
        } else {
            gx = (this->windowWidth * 0.7 - this->popup[0]->getWidth() / 2) * (ghostMovement);
            gy = (this->windowHeight / 2) * (1.5 - ghostMovement * ghostMovement) + (this->popup[0]->getHeight() / 2);
            ghostAlpha = std::min(ghostMovement * 2.0f, 1.0f);
            ghostAngle = M_PI_2 * (1 - ghostMovement * ghostMovement);
            ghostSize = 225;
        }

        this->popup[0]->setX(gx - this->popup[0]->getWidth() / 2);
        this->popup[0]->setY(gy - this->popup[0]->getHeight() / 2);
        int color = this->popup[0]->getColor() & 0xFFFFFF00;
        color |= (int) (ghostAlpha * 255);
        this->popup[0]->setAngle(ghostAngle);
        this->popup[0]->setWidth(ghostSize);
        this->popup[0]->setHeight(ghostSize);
        this->popup[1]->setX(this->popup[0]->getX() - this->popup[1]->getWidth() - 15);
        this->popup[1]->setY(this->popup[0]->getY() + this->popup[0]->getHeight() * 0.3);
        this->popup[0]->setColor(color);
        this->popup[2]->setX(this->popup[1]->getX() + 10);
        this->popup[2]->setY(this->popup[1]->getY() + 17);

        this->popup[0]->setVisible(true);
        for (int i = 1; i < 3; i++) {
            this->popup[i]->setVisible(dialogueAlpha > 0);
        }

        if (ghostMovement == 1) {
            dialogueAlpha += deltaTime * 0.9;
            if (dialogueAlpha > 0) {
                for (int i = 1; i < 3; i++) {
                    int color = this->popup[i]->getColor() & 0xFFFFFF00;
                    float alpha = (dialogueAlpha < dialogueDuration) ? dialogueAlpha : std::max(dialogueDuration + 1.0f - dialogueAlpha, 0.0f);
                    color |= (int) (std::min(1.0f, alpha) * 255);
                    this->popup[i]->setColor(color);
                }
            }

            if (dialogueAlpha > dialogueDuration + 1.0f) {
                dialogueNum++;
                switch (dialogueNum) {
                    case 2: {
                        this->popup[1]->setWidth(415);
                        this->popup[1]->setHeight(80);
                        this->popup[1]->reinit(this->windowWidth, this->windowHeight);
                        ((GuiText *) this->popup[2])->updateString("My name is Willy. I'm the\nchildren guardian ghost.");
                        dialogueAlpha = -0.2f;
                        dialogueDuration = 2.8f;
                        break;
                    }
                    case 3: {
                        this->popup[1]->setWidth(278);
                        this->popup[1]->setHeight(80);
                        this->popup[1]->reinit(this->windowWidth, this->windowHeight);
                        ((GuiText *) this->popup[2])->updateString("And you must be\n<Player>");
                        dialogueAlpha = -0.2f;
                        dialogueDuration = 2.0f;
                        break;
                    }
                    case 4: {
                        this->popup[1]->setWidth(290);
                        this->popup[1]->setHeight(51);
                        this->popup[1]->reinit(this->windowWidth, this->windowHeight);
                        ((GuiText *) this->popup[2])->updateString("Nice to meet you!");
                        dialogueAlpha = -0.2f;
                        dialogueDuration = 2.0f;
                        break;
                    }
                    case 5: {
                        this->popup[1]->setWidth(450);
                        this->popup[1]->setHeight(175);
                        this->popup[1]->reinit(this->windowWidth, this->windowHeight);
                        ((GuiText *) this->popup[2])->updateString("See? That's me over there!\nYou can move my body by\npushing the screen and\nheading in the direction\nyou want me to move.");
                        dialogueAlpha = -1.6f;
                        dialogueDuration = 6.5f;
                        ghostMovement = -0.2f;
                        break;
                    }
                    default:
                        break;
                }
            }
        }
    }

    double dx = (this->core->getPlayer()->getX() + this->core->getPlayer()->getWidth() / 2 - 1) * this->core->getBlockSize() * this->core->getGeneralScale() + this->core->getCamX();
    double dy = (this->core->getPlayer()->getY() + this->core->getPlayer()->getHeight() / 2 - 1) * this->core->getBlockSize() * this->core->getGeneralScale() + this->core->getCamY();
    if (abs(dx) > 2) this->core->setCamX(-this->core->getCamX() + (dx) * 0.05);
    if (abs(dy) > 2) this->core->setCamY(-this->core->getCamY() + (dy) * 0.05);

    double camX = this->core->getCamX(), camY = this->core->getCamY();
    if (-camX < this->windowWidth / 2 - this->core->getBlockSize() * this->core->getGeneralScale()) {
        this->core->setCamX(this->windowWidth / 2 - this->core->getBlockSize() * this->core->getGeneralScale());
    }
    if (-camY < this->windowHeight / 2 - this->core->getBlockSize() * this->core->getGeneralScale()) {
        this->core->setCamY(this->windowHeight / 2 - this->core->getBlockSize() * this->core->getGeneralScale());
    }
    if (-camX > -(signed) this->windowWidth / 2 + (this->core->getMap()->getWidth() - 1) * this->core->getBlockSize() * this->core->getGeneralScale()) {
        this->core->setCamX(-(signed) this->windowWidth / 2 + (this->core->getMap()->getWidth() - 1) * this->core->getBlockSize() * this->core->getGeneralScale());
    }
    if (-camY > -(signed) this->windowHeight / 2 + (this->core->getMap()->getHeight() - 1) * this->core->getBlockSize() * this->core->getGeneralScale()) {
        this->core->setCamY(-(signed) this->windowHeight / 2 + (this->core->getMap()->getHeight() - 1) * this->core->getBlockSize() * this->core->getGeneralScale());
    }
}

void Game::handleKeyboard(const Keypress *const keypress) {
    if (keypress[SDLK_c].isPressed()) {
        double angle = atan2(0, 0) + M_PI; //TODO
        EntityBullet *p = new EntityBullet(this->core->getMap(), angle, 1);
        p->setX(this->core->getPlayer()->getX() - (p->getWidth()) + 0.5 * cos(angle));
        p->setY(this->core->getPlayer()->getY() - (p->getHeight()) + 0.5 * sin(angle));
        this->core->getMap()->addEntity(p);
    }
    if (keypress[SDLK_n].isPressed()) {
        Entity *p;
        switch (rand() % 6) {
            case 0:
                p = new EntityFridge(this->core->getMap());
                break;
            case 1:
                p = new EntityTruck(this->core->getMap());
                break;
            case 2:
                p = new EntityBulldozer(this->core->getMap());
                break;
            case 3:
                p = new EntityWardrobe(this->core->getMap());
                break;
            case 4:
                p = new EntityTable(this->core->getMap());
                break;
            case 5:
                p = new EntityChair(this->core->getMap());
                break;
            default:
                p = new EntityDoor(this->core->getMap(), 0);
                break;
        }
        p->setX(this->core->getPlayer()->getX() - this->core->getPlayer()->getWidth() / 2);
        p->setY(this->core->getPlayer()->getY() - this->core->getPlayer()->getHeight() / 2);
        this->core->getMap()->addEntity(p);
    }
    if (keypress[SDLK_m].isPressed()) {
        SimpleShape *p = new SimpleShape(this->core->getMap(), (unsigned int) (rand() % 3));
        p->setX(this->core->getPlayer()->getX() - this->core->getPlayer()->getWidth() / 2);
        p->setY(this->core->getPlayer()->getY() - this->core->getPlayer()->getHeight() / 2);
        this->core->getMap()->addEntity(p);
    }
    if (keypress[SDLK_k].isPressed()) {
        if (this->core->getPlayer()->getToy() == nullptr) {
            this->core->getPlayer()->setToy();
        } else {
            this->core->getPlayer()->eject();
        }
    }
    double playerSpeed = this->core->getPlayer()->getSpeed();
    if (keypress[SDLK_w].isDown()) {
        this->core->getPlayer()->applyImpulse(0, -playerSpeed);
    }
    if (keypress[SDLK_s].isDown()) {
        this->core->getPlayer()->applyImpulse(0, playerSpeed);
    }
    if (keypress[SDLK_a].isDown()) {
        this->core->getPlayer()->applyImpulse(-playerSpeed, 0);
    }
    if (keypress[SDLK_d].isDown()) {
        this->core->getPlayer()->applyImpulse(playerSpeed, 0);
    }
    if (keypress[SDLK_q].isPressed()) {
        this->core->stop();
    }
    if (keypress[SDLK_p].isPressed()) {
        this->core->getMap()->saveEntities();
    }
    if (keypress[SDLK_MINUS].isDown()) {
        this->core->setBlockSize(this->core->getBlockSize() - 0.15);
    }
    if (keypress[SDLK_EQUALS].isDown()) {
        this->core->setBlockSize(this->core->getBlockSize() + 0.15);
    }
}

void Game::handleClick(const TouchPoint *const p) {
    float x = (float) ((-this->core->getCamX() - (double) this->windowWidth / 2 + p->x) / this->core->getGeneralScale() / this->core->getBlockSize() + 0.5);
    float y = (float) ((-this->core->getCamY() - (double) this->windowHeight / 2 + p->y) / this->core->getGeneralScale() / this->core->getBlockSize() + 0.5);
    if (p->id == SDL_BUTTON_LEFT) {
        if (p->state == 0) {
            this->heldEntity = this->core->getMap()->getEntityAt<Entity>(x, y);
            int i = 0;
        } else if (p->state == 2) {
            if (this->heldEntity != nullptr && !this->entityRotationRing->isVisible()) {
                Entity *e = this->heldEntity;
                e->setX(x + e->getWidth() / 2);
                e->setY(y + e->getHeight() / 2);
            }
        } else if (p->state == 1) {
            if (this->heldEntity == nullptr) {
                SimpleShape *s = new SimpleShape(this->core->getMap(), (unsigned int) (rand() % 3));
                s->setX(x + s->getWidth() / 2);
                s->setY(y + s->getWidth() / 2);
                this->core->getMap()->addEntity(s);
            } else {
                this->heldEntity = nullptr;
                if (this->entityRotationRing->isVisible()) {
                    this->entityRotationRing->setVisible(false);
                    SDL_ShowCursor(true);
                    SDL_WarpMouseGlobal(this->mouseLockX, this->mouseLockY);
                }
            }
        }
    } else if (p->id == SDL_BUTTON_MIDDLE) {
        if (p->state == 0) {
            this->heldEntity = this->core->getMap()->getEntityAt<Entity>(x, y);
        } else if (p->state == 1) {
            if (this->heldEntity != nullptr && this->core->getMap()->getEntityAt<Entity>(x, y) == this->heldEntity) {
                this->heldEntity->remove();
            }
        }
    } else if (p->id == SDL_BUTTON_RIGHT) {
        if (p->state == 0) {
            if (this->heldEntity != nullptr) {
                this->entityRotationRing->setX(p->x - this->entityRotationRing->getWidth() / 2);
                this->entityRotationRing->setY(p->y - this->entityRotationRing->getHeight() / 2);
                this->entityRotationRing->setAngle(this->heldEntity->getAngle());
                this->entityRotationRing->setVisible(true);
                SDL_GetGlobalMouseState(&this->mouseLockX, &this->mouseLockY);
                SDL_ShowCursor(false);
            }
        } else if (p->state == 2) {
            if (this->heldEntity != nullptr) {
                int x, y;
                SDL_GetGlobalMouseState(&x, &y);
                double angle = 10.0 * (x - this->mouseLockX) / this->windowWidth;
                this->entityRotationRing->setAngle(angle);
                this->heldEntity->setAngle(angle);
            }
        } else if (p->state == 1) {
            if (this->entityRotationRing->isVisible()) {
                this->entityRotationRing->setVisible(false);
                SDL_ShowCursor(true);
                SDL_WarpMouseGlobal(this->mouseLockX, this->mouseLockY);
            }
        }
    }
}

Game::~Game() {
    SDL_StopTextInput();
    delete this->core;
}
