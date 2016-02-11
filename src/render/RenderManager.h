//
// Created by dar on 11/25/15.
//

#ifndef C003_RENDERER_H
#define C003_RENDERER_H
#pragma once

#define GLM_FORCE_RADIANS
#define _USE_MATH_DEFINES

#ifndef __ANDROID__
#include <SDL2/SDL.h>
#endif // __ANDROID__
#include <glm/gtc/matrix_transform.hpp>
#include "Texture.h"
#include "Fbo.h"
#include "opengl.h"
#include <sstream>
#include <render/window/WindowRender.h>
#include "Render.h"
#include "TextManager.h"

class RenderManager {

public:
    RenderManager();
    ~RenderManager();
    bool init();
    void render(Window *window);
    void resize(Window *window, unsigned int width, unsigned int height);
    void initWindow(Window *window);

    unsigned int getWindowWidth() const {
        return windowWidth;
    }

    unsigned int getWindowHeight() const {
        return windowHeight;
    }

private:
#ifndef __ANDROID__
    SDL_Window *gWindow = NULL;
    SDL_GLContext gContext;
#endif // __ANDROID__
    unsigned int windowWidth = 1366;
    unsigned int windowHeight = 750;

    bool initWindow();
    bool initGL();
    bool initRenders();

    std::map<const char *, WindowRender *> windowRenders;

    WindowRender * getWindowRender(const Window *const window);
};

#endif //C003_RENDERER_H
