//
// Created by dar on 11/28/15.
//

#ifndef C003_PLAYER_H
#define C003_PLAYER_H

#include "Entity.h"
#include "EntityMoving.h"

class Player : public EntityMoving {

public:
    Player(Map *map);
};

#endif //C003_PLAYER_H
