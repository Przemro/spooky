//
// Created by dar on 1/27/16.
//

#ifndef C003_GUIELEMENT_H
#define C003_GUIELEMENT_H

class GuiElement {
public:
    double getX() const {
        return x;
    }

    void setX(double x) {
        GuiElement::x = x;
    }

    double getY() const {
        return y;
    }

    void setY(double y) {
        GuiElement::y = y;
    }

    double getWidth() const {
        return width;
    }

    void setWidth(double width) {
        GuiElement::width = width;
    }

    double getHeight() const {
        return height;
    }

    void setHeight(double height) {
        GuiElement::height = height;
    }

protected:
    double x, y;
    double width, height;
};

#endif //C003_GUIELEMENT_H
