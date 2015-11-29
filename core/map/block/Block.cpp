//
// Created by dar on 11/29/15.
//

#include "Block.h"
#include "../Map.h"

Block::Block(Map *map, int x, int y) {
    this->x = x;
    this->y = y;
    bodyDef.position.Set(this->x, this->y);
    body = map->getWorld()->CreateBody(&bodyDef);
    shape.SetAsBox(0.5, 0.5);
    body->CreateFixture(&shape, 1.0f);
}

void Block::setY(int y) {
    this->y = y;
    body->SetTransform(b2Vec2(this->getX(), this->getY()), body->GetAngle());
}

void Block::setX(int x) {
    this->x = x;
    body->SetTransform(b2Vec2(this->getX(), this->getY()), body->GetAngle());
}