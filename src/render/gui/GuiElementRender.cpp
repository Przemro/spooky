//
// Created by dar on 1/27/16.
//

#include "GuiElementRender.h"

GuiElementRender::
GuiElementRender(const string &textureFile, const string &shader) {
    texture.loadTexture2D(textureFile + string(".png"), false);
    texture.setFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR);

    int a = this->vertShader.load(shader + string(".vert"), GL_VERTEX_SHADER);
    int b = this->fragShader.load(shader + string(".frag"), GL_FRAGMENT_SHADER);

    this->shaderProgram.createProgram();
    this->shaderProgram.addShaderToProgram(&this->vertShader);
    this->shaderProgram.addShaderToProgram(&this->fragShader);

    /* initializing square's vertices */
    this->vertices[0] = 0.0f;
    this->vertices[1] = 1.0f;

    this->vertices[3] = 0.0f;
    this->vertices[4] = 0.0f;

    this->vertices[6] = 1.0f;
    this->vertices[7] = 1.0f;

    this->vertices[9] = 1.0f;
    this->vertices[10] = 0.0f;

    /* 3rd dimension vertices (unused) */
    this->vertices[2] = this->vertices[5] = this->vertices[8] = this->vertices[11] = 0.0f;

    glGenVertexArrays(1, &this->vao);
    glGenBuffers(2, this->vbo);

    glBindVertexArray(this->vao);

    glBindBuffer(GL_ARRAY_BUFFER, this->vbo[0]); /* vertices vbo */
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), this->vertices, GL_STATIC_DRAW);
    glBindAttribLocation(this->shaderProgram.getProgramID(), 0, "inPosition");
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    float size = (float) (1.0 - (atlasSize + 1) / texture.getWidth()) / atlasSize;
    float tCoords[] = {
        size, 0.0f,
        size, size,
        0.0f, 0.0f,
        0.0f, size,
    };

    glBindBuffer(GL_ARRAY_BUFFER, this->vbo[1]); /* texture coords vbo */
    glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), tCoords, GL_STATIC_DRAW);
    glBindAttribLocation(this->shaderProgram.getProgramID(), 1, "inCoord");
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);

    this->shaderProgram.linkProgram();
    this->shaderProgram.useProgram();
}

void GuiElementRender::render(const GuiElement *const element, glm::mat4 projectionMatrix, glm::mat4 viewMatrix, double scale) {
    if (element->isVisible()) {
        this->texture.bindTexture(0);
        this->shaderProgram.useProgram();
        this->shaderProgram.setUniform("projectionMatrix", projectionMatrix);
        this->shaderProgram.setUniform("gSampler", texture.getBoundId());

        double px = element->getX() * scale;
        double py = element->getY() * scale;
        double pwidth = element->getWidth() * scale;
        double pheight =  element->getHeight() * scale;


        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        int screenWidth = viewport[2];
        int screenHeight = viewport[3];

        switch(element->getPositionFlag()) {
            default:
            case GUI_TOP_LEFT:
                px += pwidth;
                py += pheight;
                break;
            case GUI_TOP_CENTER:
                px += pwidth / 2 + screenWidth / 2;
                py += pheight;
                break;
            case GUI_TOP_RIGHT:
                px = screenWidth - px;
                py += pheight;
                break;
            case GUI_MIDDLE_LEFT:
                px += pwidth;
                py += pheight / 2 + screenHeight / 2;
                break;
            case GUI_MIDDLE_CENTER:
                px += pwidth / 2 + screenWidth / 2;
                py += pheight / 2 + screenHeight / 2;
                break;
            case GUI_MIDDLE_RIGHT:
                px = screenWidth - px;
                py += pheight / 2 + screenHeight / 2;
                break;
            case GUI_BOTTOM_LEFT:
                px += pwidth;
                py = screenHeight - py;
                break;
            case GUI_BOTTOM_CENTER:
                px += pwidth / 2 + screenWidth / 2;
                py = screenHeight - py;
                break;
            case GUI_BOTTOM_RIGHT:
                px = screenWidth - px;
                py = screenHeight - py;
                break;
        }

        this->tmpModelMatrix = glm::translate(this->modelMatrix, glm::vec3(-px, -py, 0.0f));
        this->tmpModelMatrix = glm::scale(this->tmpModelMatrix, glm::vec3(element->getWidth() * scale, element->getHeight() * scale, 1.0f));
        this->tmpModelMatrix = glm::rotate(this->tmpModelMatrix, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f)); // Just a variation of first rotating

        shaderProgram.setUniform("modelViewMatrix", viewMatrix * this->tmpModelMatrix);
        float x = (float) (this->getTexPos(element) % atlasSize);
        float y = (float) (this->getTexPos(element) / atlasSize);
        shaderProgram.setUniform("texPosX", (x + 1.5f) / this->texture.getWidth() + x / atlasSize);
        shaderProgram.setUniform("texPosY", (y + 1.5f) / this->texture.getWidth() + y / atlasSize);

        glBindVertexArray(this->vao);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
}

int GuiElementRender::getTexPos(const GuiElement *const element) {
    return element->getTexPos();
}

GuiElementRender::~GuiElementRender() {
    glDeleteBuffers(2, this->vbo);
    glDeleteVertexArrays(1, &this->vao);
}
