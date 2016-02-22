//
// Created by dar on 2/22/16.
//

#include "GuiElement.h"

GuiElement::GuiElement(char positionFlag, double x, double y, double width, double height, int *texturePos, int texturesNum) {
    this->positionFlag = positionFlag;
    this->rawX = x;
    this->rawY = y;
    this->width = width;
    this->height = height;
    this->texturesNum = texturesNum;
    this->texturePos = texturePos;
    this->angle = 0;
}