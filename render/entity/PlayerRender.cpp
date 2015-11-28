//
// Created by dar on 11/28/15.
//

#include "PlayerRender.h"
#include "../../core/map/block/Block.h"

PlayerRender::PlayerRender() {
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
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    texture.loadTexture2D("player.png", true);
    texture.setFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR_MIPMAP);
    texture.bindTexture(1);

    float tCoords[] = {
            1.0f / atlasSize, 0.0f,
            1.0f / atlasSize, 1.0f / atlasSize,
            0.0f, 0.0f,
            0.0f, 1.0f / atlasSize,
    };

    glBindBuffer(GL_ARRAY_BUFFER, this->vbo[1]); /* texture coords vbo */
    glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), tCoords, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);

    this->vertShader.load("shader.vert", GL_VERTEX_SHADER);
    this->fragShader.load("shader.frag", GL_FRAGMENT_SHADER);

    this->shaderProgram.createProgram();
    this->shaderProgram.addShaderToProgram(&this->vertShader);
    this->shaderProgram.addShaderToProgram(&this->fragShader);

    this->shaderProgram.linkProgram();
    this->shaderProgram.useProgram();
}

void PlayerRender::render(const Entity *const entity, glm::mat4 projectionMatrix, glm::mat4 viewMatrix) {
    this->texture.bindTexture(1);
    this->shaderProgram.useProgram();
    this->shaderProgram.setUniform("projectionMatrix", projectionMatrix);
    this->shaderProgram.setUniform("gSampler", texture.getBoundId());

    this->tmpModelMatrix = glm::translate(this->modelMatrix, glm::vec3(0.0f - entity->getX() * blockSize * generalScale, 0.0f - entity->getY() * blockSize * generalScale, 0.0f));
    this->tmpModelMatrix = glm::scale(this->tmpModelMatrix, glm::vec3(blockSize * generalScale, blockSize * generalScale, 1.0f));
    this->tmpModelMatrix = glm::rotate(this->tmpModelMatrix, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f)); // Just a variation of first rotating

    shaderProgram.setUniform("modelViewMatrix", viewMatrix * this->tmpModelMatrix);
    shaderProgram.setUniform("texPosX", (float) (this->getTexPos() % atlasSize) / atlasSize);
    shaderProgram.setUniform("texPosY", (float) (this->getTexPos() / atlasSize) / atlasSize);

    glBindVertexArray(this->vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

int PlayerRender::getTexPos() {
    return 0;
}

PlayerRender::~PlayerRender() {
    glDeleteBuffers(2, this->vbo);
    glDeleteVertexArrays(1, &this->vao);
}
