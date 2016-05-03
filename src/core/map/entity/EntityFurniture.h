//
// Created by dar on 3/1/16.
//

#ifndef C003_ENTITYFURNITURE_H
#define C003_ENTITYFURNITURE_H

#include "EntityMoving.h"
#include <core/map/block/SimpleBlock.h>

class EntityFurniture : public EntityMoving {
public:
    EntityFurniture(LevelContext &levelContext, b2Shape *shape, double width, double height, int texId);

    int getTexId() const {
        return texId;
    }

    virtual ~EntityFurniture() { };

protected:
    int texId;
};

class EntityFridge : public EntityFurniture {
public:
    EntityFridge(LevelContext &levelContext) : EntityFurniture(levelContext, new b2PolygonShape, 1.0, 0.5, 1) {
        this->body->GetFixtureList()[0].SetDensity(15.0f);
    }

    virtual ~EntityFridge() override { }

};

class EntityWardrobe : public EntityFurniture {
public:
    EntityWardrobe(LevelContext &levelContext) : EntityFurniture(levelContext, new b2PolygonShape, 1.0, 0.5, 2) {
        this->body->GetFixtureList()[0].SetDensity(15.0f);
    }

    virtual ~EntityWardrobe() override { }

};

class EntityToaster : public EntityFurniture {
public:
    EntityToaster(LevelContext &levelContext) : EntityFurniture(levelContext, new b2PolygonShape, 0.5, 0.375, 23) { }

    virtual bool doesCollide(IPositionable *obj) override {
        if (SimpleBlock *b = dynamic_cast<SimpleBlock *>(obj)) {
            if (b->getTexPos() >= 20 && b->getTexPos() <= 22) {
                return false;
            }
        }
        return Entity::doesCollide(obj);
    }

    virtual ~EntityToaster() override { }
};

class EntityRadio : public EntityFurniture {
public:
    EntityRadio(LevelContext &levelContext) : EntityFurniture(levelContext, new b2PolygonShape, 0.8, 0.4, 22) { }

    virtual ~EntityRadio() override { }
};

class EntitySink : public EntityFurniture {
public:
    EntitySink(LevelContext &levelContext) : EntityFurniture(levelContext, new b2PolygonShape, 0.66, 0.8, 21) {
        this->setBodyType(b2_staticBody);
    }

    virtual ~EntitySink() override { }
};

class EntityCuttingBoard : public EntityFurniture {
public:
    EntityCuttingBoard(LevelContext &levelContext) : EntityFurniture(levelContext, new b2PolygonShape, 0.46, 0.66, 20) { }

    virtual bool doesCollide(IPositionable *obj) override {
        if (SimpleBlock *b = dynamic_cast<SimpleBlock *>(obj)) {
            if (b->getTexPos() >= 20 && b->getTexPos() <= 22) {
                return false;
            }
        }
        return Entity::doesCollide(obj);
    }

    virtual ~EntityCuttingBoard() override { }
};

class EntityFlowerPot : public EntityFurniture {
public:
    EntityFlowerPot(LevelContext &levelContext) : EntityFurniture(levelContext, new b2CircleShape, 0.85, 0.85, 16) { }
};

class EntityTallLight : public EntityFurniture {
public:
    EntityTallLight(LevelContext &levelContext) : EntityFurniture(levelContext, new b2CircleShape, 0.815, 0.815, 24) { }
};

class EntityWallLight : public EntityFurniture {
public:
    EntityWallLight(LevelContext &levelContext) : EntityFurniture(levelContext, new b2PolygonShape, 0.5, 0.5, 17) {
        this->setBodyType(b2_staticBody);
    }

    virtual ~EntityWallLight() override { }
};

class EntityChestHandle : public EntityFurniture {
public:
    EntityChestHandle(LevelContext &levelContext) : EntityFurniture(levelContext, new b2CircleShape, 0.18, 0.18, 18) {
        this->setBodyType(b2_staticBody);
    }

    virtual ~EntityChestHandle() override { }
};

class EntityNotebook : public EntityFurniture {
public:
    EntityNotebook(LevelContext &levelContext) : EntityFurniture(levelContext, new b2PolygonShape, 0.46, 0.66, 19) { }

    virtual bool doesCollide(IPositionable *obj) override {
        if (SimpleBlock *b = dynamic_cast<SimpleBlock *>(obj)) {
            if (b->getTexPos() == 37) {
                return false;
            }
        }
        return Entity::doesCollide(obj);
    }

    virtual ~EntityNotebook() override { }
};

class EntityCupboardTop : public EntityFurniture {
public:
    EntityCupboardTop(LevelContext &levelContext) : EntityFurniture(levelContext, new b2PolygonShape, 0.85, 1.0, 32) { }

    virtual ~EntityCupboardTop() override { }
};

class EntityCupboardBottom : public EntityFurniture {
public:
    EntityCupboardBottom(LevelContext &levelContext) : EntityFurniture(levelContext, new b2PolygonShape, 0.85, 1.0, 40) { }

    virtual ~EntityCupboardBottom() override { }
};

class EntityToiletPaper : public EntityFurniture {
public:
    EntityToiletPaper(LevelContext &levelContext) : EntityFurniture(levelContext, new b2PolygonShape, 0.3125, 0.625, 7) {
        this->setBodyType(b2_staticBody);
    }

    virtual ~EntityToiletPaper() override { }
};

class EntityChair : public EntityFurniture {
public:
    EntityChair(LevelContext &levelContext) : EntityFurniture(levelContext, new b2PolygonShape, 0.6, 0.6, 3) {
        this->body->GetFixtureList()[0].SetDensity(6.0f);
    }
};

class EntityArmchair : public EntityFurniture {
public:
    EntityArmchair(LevelContext &levelContext) : EntityFurniture(levelContext, new b2PolygonShape, 0.875, 0.76, 11) {
        this->body->GetFixtureList()[0].SetDensity(4.5f);
    }
};

class EntityPouf : public EntityFurniture {
public:
    EntityPouf(LevelContext &levelContext) : EntityFurniture(levelContext, new b2PolygonShape, 0.75, 0.625, 12) {
        this->body->GetFixtureList()[0].SetDensity(3.5f);
    }
};

class EntityGlass : public EntityFurniture {
public:
    EntityGlass(LevelContext &levelContext) : EntityFurniture(levelContext, new b2CircleShape, 0.3, 0.3, 61) { }

    virtual bool doesCollide(IPositionable *obj) override {
        if (EntityWardrobe *b = dynamic_cast<EntityWardrobe *>(obj)) {
            return false;
        }
        return Entity::doesCollide(obj);
    }
};

#endif //C003_ENTITYFURNITURE_H
