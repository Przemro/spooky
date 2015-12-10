//
// Created by dar on 11/28/15.
//

#ifndef C003_ENTITY_H
#define C003_ENTITY_H
#pragma once

#include <Box2D/Box2D.h>
#include "../IPositionable.h"

class Map;

class Entity : public IPositionable {
public:
    Entity(Map *map, double width, double height);

    virtual double getX() const override {
        return x;
    }

    void setX(double x);

    virtual double getY() const override {
        return y;
    }

    void setY(double y);

    virtual void update();

    virtual double getWidth() const override {
        return this->width;
    }

    virtual double getHeight() const override {
        return this->height;
    }

    /*
     * Called when collision occurs
     * State values:
     *  0 - collision begin
     *  1 - collision end
     */
    virtual void onCollision(IPositionable *object, char state) { };

    void remove() {
        this->toBeDeleted = true;
    }

    bool isToBeDeleted() const {
        return toBeDeleted;
    }

    virtual ~Entity();

protected:
    Map *map;
    double x = 0, y = 0;
    double width, height;
    b2Body *body;
    b2BodyDef bodyDef;
    b2FixtureDef fixDef;
    bool toBeDeleted = false;
};

#endif //C003_ENTITY_H
