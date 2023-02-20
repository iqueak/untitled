//
// Created by bohdan.yurov on 19.02.2023.
//
#pragma once
#ifndef UNTITLED_WORLD_H
#define UNTITLED_WORLD_H

#endif //UNTITLED_WORLD_H

#include "ChunkManager.h"

using namespace ChunkManager;

namespace World {

    enum class WorldGenerationTypes {
        FLAT_MODE,
        STANDART_MODE,

        _COUNT
    };

    static WorldGenerationTypes WorldGenerationMode;

    void Init();
}
