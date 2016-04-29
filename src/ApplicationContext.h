//
// Created by dar on 4/8/16.
//

#ifndef C003_APPLICATIONCONTEXT_H
#define C003_APPLICATIONCONTEXT_H

#include <functional>
#include <core/LevelData.h>
#include "SettingsData.h"

class Window;

class ApplicationContext {
public:
    ApplicationContext(const std::function<bool(Window *)> &switchWindow);

    const bool switchWindow(Window *window) {
        return switchWindowFunc(window);
    }

    SettingsData &getSettingsData() {
        return settingsData;
    }

    const LevelData &getLevelData() const {
        return levelData;
    }

private:
    const std::function<bool(Window *window)> switchWindowFunc;
    SettingsData settingsData;
    LevelData levelData;
};

#endif //C003_APPLICATIONCONTEXT_H
