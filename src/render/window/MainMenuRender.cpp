//
// Created by dar on 2/15/16.
//

#include <window/Window.h>
#include <render/RenderContext.h>
#include <window/MainMenu.h>
#include "MainMenuRender.h"

MainMenuRender::MainMenuRender() : WindowRender() {

}

void MainMenuRender::init(RenderContext *const renderContext) {
    viewMatrix = glm::lookAt(glm::vec3(0, 0, 0.0f), glm::vec3(0, 0, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    projectionMatrix = glm::ortho(0.0f, float(renderContext->getWindowWidth()), 0.0f, float(renderContext->getWindowHeight()));
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
}

void MainMenuRender::render(Window *window, RenderContext *const renderContext) {
    MainMenu *game = ((MainMenu *) window);

    glClearColor(0.9, 0.9, 0.9, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    for (GuiElement *guiElement : game->getGuiElements()) {
        renderContext->getGuiElementRender(guiElement)->render(guiElement, projectionMatrix, glm::translate(viewMatrix, glm::vec3(
            0,
            (int) ((signed) renderContext->getWindowHeight()),
            0.0f)), 1.0f);
    }
}

void MainMenuRender::resize(RenderContext *const renderContext) {

}
