//
// Created by dar on 11/20/15.
//

#ifndef C003_CSHADER_H
#define C003_CSHADER_H
#pragma once

#include "opengl.h"
#include <string>
#include <glm/glm.hpp>


class Shader {
public:
    ~Shader();
    bool load(const std::string &file, int type);
    bool isLoaded();
    GLuint getID();

private:
    GLuint id;
    int type; // GL_VERTEX_SHADER, GL_FRAGMENT_SHADER
    bool loaded = false;
};


#endif //C003_CSHADER_H
