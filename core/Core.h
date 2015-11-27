//
// Created by dar on 11/25/15.
//

#ifndef C003_CORE_H
#define C003_CORE_H


#include "map/Map.h"

class Core {

public:
    Core() { }

    void setMap(Map *map) {
        Core::map = *map;
    }

    const Map *getMap() const {
        return &map;
    }

private:
    Map map;
};


#endif //C003_CORE_H
