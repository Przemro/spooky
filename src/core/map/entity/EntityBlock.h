//
// Created by dar on 12/25/15.
//

#ifndef C003_SIMPLESHAPE_H
#define C003_SIMPLESHAPE_H

#include "EntityMoving.h"

class EntityBlock : public EntityMoving {
public:
    EntityBlock(Map *map, unsigned int shapeId, unsigned int variation);
    EntityBlock(Map *map, unsigned int shapeId);
    EntityBlock(Map *map, unsigned int shapeId, double width, double height);
    unsigned int getTexPos() const;
    virtual ~EntityBlock();

private:
    unsigned int texPos;
    unsigned int variation;

    struct ShapeDef {
        ShapeDef(double width, double height, b2FixtureDef *fixtureDef) : width(width), height(height), fixtureDef(fixtureDef) { };
        double width, height;
        const b2FixtureDef *fixtureDef;

        static ShapeDef *createShapeDef(double width, double height);
    };

    struct ShapeDefContainer {
        ShapeDefContainer();
        std::vector<ShapeDef *> shapeDefs;
    };

    static ShapeDefContainer shapeDefContainer;
};

#endif //C003_SIMPLESHAPE_H
