//
// Created by dar on 11/27/15.
//

#ifndef C003_GAME_H
#define C003_GAME_H
#pragma once

#include <vector>
#include <gui/GuiText.h>
#include "Window.h"

class Core;

class RenderManager;

class Timer;

class GuiElement;

class GuiButton;

class Entity;

class EntityToy;

class Game : public Window {

public:
    Game(ApplicationContext *applicationContext);
    virtual void reload(unsigned int windowWidth, unsigned int windowHeight) override;
    virtual void tick(double deltaTime) override;
    virtual void handleKeyboard(const Keypress *const keypress) override;
    virtual void handleClick(const TouchPoint *const touchPoint) override;

    Core *getCore() const {
        return core;
    }

    const std::vector<GuiElement *> &getGuiElements() const {
        return guiElements;
    }

    virtual ~Game() override;

private:
    Core *core = nullptr;
    std::vector<GuiElement *> guiElements;

#ifndef EDITOR
    GuiButton *controller;

    EntityToy *clickedToy = nullptr;
    float toyPopupAlpha = 0.0f;
    int toyPopupClickedBy = -1;
    EntityToy *markedToy = nullptr;

    GuiElement *toyController[3];
    GuiElement *infoControl[3];
    float infoWindowAlpha = 0.0f;
    bool infoWindowVisible = false;

    void resetButtons(const TouchPoint *const p, const GuiButton *const b);

    void resetButtons(const TouchPoint *const p) {
        this->resetButtons(p, nullptr);
    }
#else
    Entity *heldEntity = nullptr;

    GuiElement *entityRotationRing;
    int mouseLockX, mouseLockY;
#endif //__ANDROID__

    unsigned int windowWidth, windowHeight;

};

#endif //C003_GAME_H
