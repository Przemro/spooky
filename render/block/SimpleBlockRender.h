//
// Created by dar on 11/25/15.
//

#ifndef C003_SIMPLEBLOCKRENDER_H
#define C003_SIMPLEBLOCKRENDER_H


#include "BlockRender.h"
#include "../Shader.h"
#include "../ShaderProgram.h"

class SimpleBlockRender : public BlockRender {

public:
    SimpleBlockRender();
    ~SimpleBlockRender();
    virtual void render(const Block *const block, glm::mat4 projectionMatrix, glm::mat4 viewMatrix) override;
private:
    float vertices[12];
    GLuint vbo[2];
    GLuint vao;
    Shader vertShader, fragShader;
    ShaderProgram shaderProgram;
    glm::mat4 modelMatrix = glm::mat4(1.0);

    /* -- tmp -- */
    glm::mat4 tmpModelMatrix;

};


#endif //C003_SIMPLEBLOCKRENDER_H
