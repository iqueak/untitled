//
// Created by bohdan.yurov on 19.02.2023.
//
#pragma once
#ifndef UNTITLED_WORLD_H
#define UNTITLED_WORLD_H
#endif //UNTITLED_WORLD_H

#include <map>
#include "ChunkManager.h"

using namespace ChunkManager;

namespace WorldManager {

    enum class WorldGenerationTypes {
        FLAT_MODE,
        STANDART_MODE,

        _COUNT
    };

    class World {
    public:
        char * Name;
        Model WorldModel;
        int Seed;
        WorldGenerationTypes WorldGenerationMode;
        Chunk * ch;
        World(char * name, int seed, WorldGenerationTypes generationType);
        //~World();
    };

    static World * _World;

    void Init();

    World & getWorld();
}
