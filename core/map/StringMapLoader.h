//
// Created by dar on 11/25/15.
//

#ifndef C003_STRINGMAPLOADER_H
#define C003_STRINGMAPLOADER_H

#include "Map.h"
#include "../map/block/SimpleBlock.h"
#include "../entity/Player.h"

class StringMapLoader : public MapLoader {

public:
    StringMapLoader(char *mapData, int width, int height);
    virtual Map *loadMap() override;

private:
    Map *map = nullptr;
};


#endif //C003_STRINGMAPLOADER_H

StringMapLoader::StringMapLoader(char *mapData, int width, int height) {
    this->map = new Map();
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (mapData[y * width + x] != ' ')
                this->map->addBlock(new SimpleBlock((x + y) % 3 != 1, x, y));
        }
    }
}

Map *StringMapLoader::loadMap() {
    return this->map;
}