//
// Created by dar on 11/25/15.
//

#ifndef C003_RENDER_H
#define C003_RENDER_H

#include <typeinfo>
#include <map>
#include <string>
#include <memory>
#include "../core/block/SimpleBlock.h"
#include "block/BlockRender.h"
#include "block/SimpleBlockRender.h"

static std::map<const char*, BlockRender*> blockRenders;

BlockRender *getBlockRender(Block *b) {
    return blockRenders[typeid(*b).name()];
}

void init() {
    blockRenders.insert(std::make_pair(typeid(SimpleBlock).name(), new SimpleBlockRender()));
}

#endif //C003_RENDER_H
