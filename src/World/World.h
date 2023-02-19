//
// Created by bohdan.yurov on 19.02.2023.
//

#ifndef UNTITLED_WORLD_H
#define UNTITLED_WORLD_H

#endif //UNTITLED_WORLD_H

#include "ChunkManager.h"

using namespace ChunkManager;

namespace World {

    const int ActiveChunksResolution = 16;

    enum class WorldGenerationTypes {
        FLAT_MODE,
        STANDART_MODE,

        _COUNT
    };

    WorldGenerationTypes WorldGenerationMode;

    void Init();
}
