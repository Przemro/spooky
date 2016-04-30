//
// Created by dar on 1/23/16.
//

#include "Game.h"
#include "render/RenderManager.h"
#include "InputManager.h"
#include "../logging.h"
#include <gui/GuiText.h>
#include <gui/GuiButton.h>
#include <gui/GuiTextBubble.h>
#include <window/MainMenu.h>
#include <ApplicationContext.h>
#include <core/LevelContext.h>
#include <core/map/entity/EntityBlock.h>
#include <core/map/entity/EntityPlayer.h>
#include <core/map/Map.h>
#include <core/map/entity/EntityFurniture.h>
#include <core/map/entity/EntityGlassDebris.h>

Game::Game(ApplicationContext *applicationContext, const std::string &levelName) : Window(applicationContext) {
    this->levelContext = new LevelContext(levelName);

    auto nullController = [&](const TouchPoint *const touchPoint) {
        return true;
    };

    controller = new GuiButton(GUI_TOP_LEFT, 0, 0, 200, 200, 0);
    controller->setVisible(false);
    controller->setOnClickListener(nullController);
    this->guiElements.push_back(controller);

    GuiButton *backButton = new GuiButton(GUI_TOP_LEFT, 25, 25, 100, 100, new int[2]{8, 16}, 2);
    auto backAction = [=](const TouchPoint *const p) {
        if (p->state == 1) {
            if (backButton->canBeClicked(p)) {
                this->applicationContext->switchWindow(new MainMenu(this->applicationContext));
            }
            return false;
        }
        return true;
    };
    backButton->setOnClickListener(backAction);
    this->guiElements.push_back(backButton);

    GuiElement *character = new GuiElement(GUI_TOP_RIGHT, 0, 50, 225, 225, 17);
    character->setVisible(false);
    this->guiElements.push_back(character);
    GuiTextBubble *window = new GuiTextBubble(GUI_TOP_LEFT, -500, -500, 150 * this->levelContext->getBlockSize() / 64.0, 74 * this->levelContext->getBlockSize() / 64.0);
    this->toyController[1] = window;
    window->setVisible(false);
    this->guiElements.push_back(window);

    GuiButton *possessButton = new GuiButton(GUI_TOP_LEFT, 50, 50, 55 * this->levelContext->getBlockSize() / 64.0, 55 * this->levelContext->getBlockSize() / 64.0, new int[2]{2, 10}, 2);
    this->toyController[2] = possessButton;
    possessButton->setVisible(false);
    auto possessAction = [&](const TouchPoint *const p) {
        if (p->state == 1) {
            if (this->toyPopupAlpha == 1.0 && ((GuiButton *) this->toyController[2])->canBeClicked(p)) {
                if (this->levelContext->getPlayer()->getToy() == nullptr) {
                    this->levelContext->getPlayer()->setToyToMerge(this->markedToy);
                    this->levelContext->getPlayer()->setToy();
                    int missionProgress = this->missionFlags & 0xFF;
                    if (missionProgress == 0) {
                        this->newInfoText = "Head outside of the room.\nExplore the house.";
                        this->infoWindowVisible = false;
                        if (this->infoWindowAlpha == 0.0) this->infoWindowAlpha = 0.001;
                        this->missionFlags = (this->missionFlags >> 8) << 8;
                        this->missionFlags |= missionProgress;
                    }
                } else {
                    this->levelContext->getPlayer()->eject();
                }
                this->toyPopupClickedBy = -1;
            }
            return false;
        }
        return true;
    };
    possessButton->setOnClickListener(possessAction);
    this->guiElements.push_back(possessButton);

    GuiButton *infoButton = new GuiButton(GUI_TOP_RIGHT, 50, 50, 100, 100, new int[2]{33, 32}, 2);
    this->infoControl[0] = infoButton;
    auto infoAction = [=](const TouchPoint *const p) {
        if (p->state == 1) {
            if ((this->infoWindowAlpha == 0.0 || this->infoWindowAlpha == 1.0) && infoButton->canBeClicked(p)) {
                this->infoWindowVisible = !this->infoWindowVisible;
                infoButton->setTexPos(0, 24);
            }
            return false;
        }
        return true;
    };
    infoButton->setOnClickListener(infoAction);
    this->guiElements.push_back(infoButton);

    GuiTextBubble *infoWindow = new GuiTextBubble(GUI_TOP_RIGHT, 40, 190, 400, 500);
    this->infoControl[1] = infoWindow;
    infoWindow->setVisible(false);
    this->guiElements.push_back(infoWindow);

    GuiText *infoText = new GuiText("Try to fly over an\norange car. Then,\npress the possess\nbutton.", 0, 0, GUI_TOP_LEFT, 24, 0x666666FF, 0);
    this->infoControl[2] = infoText;
    infoText->setVisible(false);
    this->guiElements.push_back(infoText);

    GuiText *t = new GuiText(string("Dev Build: ") + __DATE__ + " " + __TIME__, 15, 15, GUI_BOTTOM_LEFT, 32, 0xFFFFFFFF, 0);
    this->guiElements.push_back(t);
#if defined(__DEBUG__)
#else
#endif


}

void Game::reload(unsigned int windowWidth, unsigned int windowHeight) {
    ((GuiTextBubble *) this->infoControl[1])->setupDimensions((GuiText *) this->infoControl[2]);
    for (GuiElement *e : this->guiElements) {
        e->reinit(windowWidth, windowHeight);
    }
    this->infoControl[2]->setX(this->infoControl[1]->getX() + 10);
    this->infoControl[2]->setY(this->infoControl[1]->getY() + 17.5);
    this->resetButtons(nullptr);
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;
}

void Game::tick(double deltaTime) {
    this->levelContext->getMap()->update(deltaTime);
    this->levelContext->getMap()->getWorld()->Step(deltaTime, 8, 3);
    for (int i = 0; i < this->levelContext->getMap()->getEntities().size(); i++) {
        Entity *entity = this->levelContext->getMap()->getEntities().at(i);
        if (entity->isToBeDeleted()) {
            this->levelContext->getMap()->removeEntity(entity);
            i--;
        }
    }

    double scale = this->levelContext->getGeneralScale() * this->levelContext->getBlockSize();
    double player_x = this->windowWidth / 2.0 + (this->levelContext->getPlayer()->getBody()->GetPosition().x + this->levelContext->getCamX() - 0.5) * scale;
    double player_y = this->windowHeight / 2.0 + (this->levelContext->getPlayer()->getBody()->GetPosition().y + this->levelContext->getCamY() - 0.5) * scale;

    double px = this->levelContext->getPlayer()->getX() + this->levelContext->getPlayer()->getWidth() / 2;
    double py = this->levelContext->getPlayer()->getY() + this->levelContext->getPlayer()->getHeight() / 2;
    if (levelContext->getPlayer()->getEjectTime() > 0.0 && levelContext->getPlayer()->getDamagedToy() == levelContext->getPlayer()->getToyToMerge()) {
        py -= 0.2 * levelContext->getPlayer()->getEjectTime();
    }

    double dx = (px - 1 + this->levelContext->getCamX());
    double dy = (py - 1 + this->levelContext->getCamY());
    if (abs(dx) > 0.05) this->levelContext->setCamX(-this->levelContext->getCamX() + (dx) * 0.05);
    if (abs(dy) > 0.05) this->levelContext->setCamY(-this->levelContext->getCamY() + (dy) * 0.05);

    if (controller->isPressed()) {
        double x = (controller->getX() - player_x) / 100.0f;
        double y = (controller->getY() - player_y) / 100.0f;
        this->levelContext->getPlayer()->move(x, y, deltaTime);
    }

    double camX = this->levelContext->getCamX(), camY = this->levelContext->getCamY();
    if (-camX < this->windowWidth / (2.0 * scale) - 1) {
        this->levelContext->setCamX(this->windowWidth / (2.0 * scale) - 1);
    }
    if (-camY < this->windowHeight / (2.0 * scale) - 1) {
        this->levelContext->setCamY(this->windowHeight / (2.0 * scale) - 1);
    }
    if (-camX > -(signed) this->windowWidth / (2.0 * scale) + (this->levelContext->getMap()->getWidth() - 1)) {
        this->levelContext->setCamX(-(signed) this->windowWidth / (2.0 * scale) + (this->levelContext->getMap()->getWidth() - 1));
    }
    if (-camY > -(signed) this->windowHeight / (2.0 * scale) + (this->levelContext->getMap()->getHeight() - 1)) {
        this->levelContext->setCamY(-(signed) this->windowHeight / (2.0 * scale) + (this->levelContext->getMap()->getHeight() - 1));
    }

    for (int i = 1; i < 3; i++) {
        int color = this->toyController[1]->getColor() & 0xFFFFFF00;
        color |= (int) (toyPopupAlpha * 255);
        this->toyController[i]->setColor(color);
    }

    if (this->markedToy != nullptr) {
        double toy_x = this->windowWidth / 2.0 + (this->markedToy->getBody()->GetPosition().x + this->levelContext->getCamX() - 0.5) * scale;
        double toy_y = this->windowHeight / 2.0 + (this->markedToy->getBody()->GetPosition().y + this->levelContext->getCamY() - 0.5) * scale;

        double offset_y = -(std::max(this->markedToy->getWidth(), this->markedToy->getHeight()) / 2 + 0.25) * scale - this->toyController[1]->getHeight();
        if (toy_y < this->windowHeight * 0.25) {
            offset_y = (std::max(this->markedToy->getWidth(), this->markedToy->getHeight()) / 2 + 0.25) * scale;
        }

        this->toyController[1]->setX(toy_x - this->toyController[1]->getWidth() / 2);
        this->toyController[1]->setY(this->toyController[1]->getY() + (toy_y + offset_y - this->toyController[1]->getY()) * 0.3 * deltaTime);
        this->toyController[2]->setX(this->toyController[1]->getX() + 10 * this->levelContext->getBlockSize() / 64.0);
        this->toyController[2]->setY(this->toyController[1]->getY() + 10 * this->levelContext->getBlockSize() / 64.0);

        bool possessButtonEnabled = this->levelContext->getPlayer()->getToy() != nullptr && this->markedToy == this->levelContext->getPlayer()->getToy();
        if (this->levelContext->getPlayer()->getToy() == nullptr) {
            for (b2ContactEdge *edge = this->levelContext->getPlayer()->getBody()->GetContactList(); edge != nullptr; edge = edge->next) {
                if (edge->other == this->markedToy->getBody() && edge->contact->IsTouching()) {
                    possessButtonEnabled = true;
                    break;
                }
            }
        }
        ((GuiButton *) this->toyController[2])->setEnabled(possessButtonEnabled);
    }

    for (int i = 1; i < 3; i++) {
        this->toyController[i]->setVisible(i == 2 && toyPopupAlpha > 0.0f);
    }

    if (toyPopupClickedBy >= 0 && this->markedToy != nullptr) {
        toyPopupAlpha += 0.05f * deltaTime;
        if (toyPopupAlpha > 1.0f) toyPopupAlpha = 1.0f;
    } else if (toyPopupClickedBy < 0) {
        for (int i = 2; i < 3; i++) {
            if (((GuiButton *) this->toyController[i])->isPressed()) {
                return;
            }
        }
        toyPopupAlpha -= 0.05f * deltaTime;
        if (toyPopupAlpha < 0.0f) {
            toyPopupAlpha = 0.0f;
            this->markedToy = nullptr;
            if (this->levelContext->getPlayer()->getToy() == nullptr) {
                this->toyController[2]->setTexPos(0, 2);
                this->toyController[2]->setTexPos(1, 10);
            } else {
                this->toyController[2]->setTexPos(0, 18);
                this->toyController[2]->setTexPos(1, 26);
            }
        }
    }

    if (toyPopupClickedBy < 0 && toyPopupClickedBy != -2 && this->markedToy == nullptr) {
        if (this->levelContext->getPlayer()->getToy() != nullptr && !this->levelContext->getPlayer()->getToy()->getBody()->GetLinearVelocity().Length() > 0) {
            this->markedToy = this->levelContext->getPlayer()->getToy();
            toyPopupClickedBy = 100;
        } else if (this->levelContext->getPlayer()->getToyToMerge() != nullptr) {
            this->markedToy = this->levelContext->getPlayer()->getToyToMerge();
            toyPopupClickedBy = 100;
        }
    }

    if (this->infoWindowVisible) {
        this->infoWindowAlpha += 0.05f * deltaTime;
        if (this->infoWindowAlpha > 1.0f) {
            this->infoWindowAlpha = 1.0f;
        }
    } else if (this->infoWindowAlpha > 0.0) {
        this->infoWindowAlpha -= 0.05f * deltaTime;
        if (this->infoWindowAlpha < 0.0f) {
            this->infoWindowAlpha = 0.0f;
            if (this->newInfoText.length() > 0 && ((GuiText *) this->infoControl[2])->getString().compare(this->newInfoText) != 0) {
                ((GuiText *) this->infoControl[2])->updateString(this->newInfoText);
                if (this->infoControl[1]->isVisible()) {
                    this->infoWindowVisible = true;
                } else {
                    this->infoControl[0]->setTexPos(0, 33);
                }
                ((GuiTextBubble *) this->infoControl[1])->setupDimensions(((GuiText *) this->infoControl[2]));
                ((GuiTextBubble *) this->infoControl[1])->reinit(this->windowWidth, this->windowHeight);
                this->infoControl[2]->setX(this->infoControl[1]->getX() + 10);
                this->infoControl[2]->setY(this->infoControl[1]->getY() + 17.5);
            }
        }
    }

    for (int i = 1; i < 3; i++) {
        int color = this->infoControl[i]->getColor() & 0xFFFFFF00;
        color |= (int) (this->infoWindowAlpha * 255);
        this->infoControl[i]->setColor(color);
        this->infoControl[i]->setVisible(this->infoWindowAlpha > 0.0);
    }

    if (!((this->missionFlags >> 31) & 1)) {
        if (EntityGlass *glass = dynamic_cast<EntityGlass *>(this->levelContext->getMap()->getEntities()[1])) {
            if (this->levelContext->getPlayer()->getToy() != nullptr && (int) this->levelContext->getPlayer()->getX() == 24 && this->levelContext->getPlayer()->getX() < 24.8 && (int) this->levelContext->getPlayer()->getY() == 13 && this->levelContext->getPlayer()->getY() < 13.7) {
                glass->remove();
                this->newInfoText = "A glass has shattered!\nYou have to clean it up\nbefore someone gets hurt!\nThere is a hoover toy\non the other side of the\nhouse. Go take it.";
                this->infoWindowVisible = false;
                if (this->infoWindowAlpha == 0.0) this->infoWindowAlpha = 0.001;
                for (int i = 0; i < 3; i++) {
                    EntityGlassDebris *p = new EntityGlassDebris(this->levelContext->getMap());
                    p->setX(25.0571);
                    p->setY(14.01);
                    p->setAngle(i * M_PI * 0.35 - M_PI * 0.3);
                    p->applyImpulse(sin(-i * M_PI_2 / 3 + M_PI_4 * 3) * 0.0, cos(-i * M_PI_2 / 3 + M_PI_4 * 3) * 0.0);
                    this->levelContext->getMap()->addEntity(p);
                }
                this->missionFlags |= (1 << 31);
            }
        }
    }

    if (!((this->missionFlags >> 30) & 1)) {
        if (this->levelContext->getPlayer()->getToy() != nullptr && (int) this->levelContext->getPlayer()->getX() > 9.25 && this->levelContext->getPlayer()->getX() < 10.5 && this->levelContext->getPlayer()->getY() > 9.65 && this->levelContext->getPlayer()->getY() < 10.25) {
            this->newInfoText = "This toy is too light\nto open the door.\nTry the bulldozer\nfrom the room on\nthe right.";
            this->infoWindowVisible = false;
            if (this->infoWindowAlpha == 0.0) this->infoWindowAlpha = 0.001;
            this->missionFlags |= (1 << 30);
        }
    }
}

void Game::handleClick(const TouchPoint *const p) {
    bool clicked = false;
    for (GuiElement *e : this->guiElements) {
        if (GuiButton *b = dynamic_cast<GuiButton *>(e)) {
            if (b->canBeClicked(p)) {
                if (p->state == 1) this->resetButtons(p, b);
                if ((p->state == 0 && (!b->isPressed()) || b->getTouchedBy() == p->id) ||
                    (b->getTouchedBy() == p->id && p->state == 2)) {
                    if (b->onClick(p) || !b->isEnabled()) {
                        clicked = true;
                        break;
                    }
                }
            }
        } else {
            if (e->isVisible() &&
                p->x > e->getX() && p->x < e->getX() + e->getWidth() &&
                p->y > e->getY() && p->y < e->getY() + e->getHeight()) {
                clicked = true;
            }
        }
    }

    float sx = (float) ((-this->levelContext->getCamX() - ((double) this->windowWidth / (2.0) - p->x) / (this->levelContext->getGeneralScale() * this->levelContext->getBlockSize())) + 0.5);
    float sy = (float) ((-this->levelContext->getCamY() - ((double) this->windowHeight / (2.0) - p->y) / (this->levelContext->getGeneralScale() * this->levelContext->getBlockSize())) + 0.5);

    if (p->state == 0) {
        this->clickedToy = this->levelContext->getMap()->getEntityAt<EntityToy>(sx, sy);
        if (this->markedToy == nullptr) {
            if (this->clickedToy != nullptr) {
                this->toyPopupClickedBy = p->id;
            }
        } else if (this->clickedToy != nullptr) {
            this->toyPopupClickedBy = -2;
        }
    } else if (p->state == 1 && this->toyPopupClickedBy == p->id) {
        if (!clicked && this->markedToy == nullptr) {
            this->markedToy = this->levelContext->getMap()->getEntityAt<EntityToy>(sx, sy);
            if (this->markedToy != this->clickedToy) {
                this->toyPopupClickedBy = -1;
            }
        } else {
            this->toyPopupClickedBy = -1;
        }
    }

    if (this->clickedToy != nullptr && p->state != 1) {
        clicked = true;
    }

    if (!clicked) {
        if (p->state == 0 || (p->state == 2 && controller->isPressed())) {
            double player_x = this->windowWidth / 2.0 + (this->levelContext->getPlayer()->getX() + this->levelContext->getCamX() - 1 + this->levelContext->getPlayer()->getWidth() / 2) * (this->levelContext->getGeneralScale() * this->levelContext->getBlockSize());
            double player_y = this->windowHeight / 2.0 + (this->levelContext->getPlayer()->getY() + this->levelContext->getCamY() - 1 + this->levelContext->getPlayer()->getWidth() / 2) * (this->levelContext->getGeneralScale() * this->levelContext->getBlockSize());

            double x = p->x - player_x;
            double y = p->y - player_y;
            if (std::sqrt(x * x + y * y) > controller->getWidth() / 2) {
                double angle = atan2(y, x);
                x = cos(angle) * controller->getWidth() / 2;
                y = sin(angle) * controller->getHeight() / 2;
            }
            controller->setX(player_x + x), controller->setY(player_y + y);
            controller->setPressed(true);
            if (toyPopupClickedBy >= 0 && (this->levelContext->getPlayer()->getToyToMerge() == nullptr || this->markedToy != this->levelContext->getPlayer()->getToyToMerge())) {
                this->toyPopupClickedBy = -1;
            }
        }
    }

    if (p->state == 1) {
        this->resetButtons(p);
    }
}

void Game::resetButtons(const TouchPoint *const p, const GuiButton *const b) {
    for (GuiElement *e : this->guiElements) {
        if (GuiButton *b1 = dynamic_cast<GuiButton *>(e)) {
            if (b1 != b) {
                if (p == nullptr) {
                    b1->setPressed(false);
                } else if (b1->getTouchedBy() == p->id) {
                    b1->onClick(p);
                }
            }
        }
    }
    controller->setPressed(false);
}

void Game::handleKeyboard(const Keypress *const keypress) {
    //Keyboard input is unsupported on current platform
}

Game::~Game() {
    delete this->levelContext;
}