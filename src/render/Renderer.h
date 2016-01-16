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
#include "../core/Core.h"
#include "opengl.h"
#include <sstream>
#include "Render.h"
#include "../core/Timer.h"


class Renderer {

public:
    Renderer(Core *core);
    ~Renderer();
    bool init();
    void run();
    void resize(unsigned int width, unsigned int height);

private:
#ifndef __ANDROID__
    SDL_Window *gWindow = NULL;
    SDL_GLContext gContext;
#endif // __ANDROID__
    unsigned int windowWidth = 1366;
    unsigned int windowHeight = 750;
    Core *core;
    Texture textureAtlas;
    Fbo fbo;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    bool initWindow();
    bool initGL();
    bool initTextures();
    void tick();
};

#endif //C003_RENDERER_H
