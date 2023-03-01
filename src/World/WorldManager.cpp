//
// Created by bohdan.yurov on 19.02.2023.
//
#include "WorldManager.h"
#include "map"
#include "../Utils/Logger.h"


using namespace ChunkManager;

namespace WorldManager {

    void WorldManager::Init() {
        _World = new World("MyWorld",123,WorldGenerationTypes::FLAT_MODE);

        CreateChunk(0,0);
        CreateChunk(1,0);
        CreateChunk(0,1);
    }

    World & getWorld() {
        return * _World;
    }

    World::World(char *name, int seed, WorldGenerationTypes generationMode) {
        Name = name;
        Seed = seed;
        WorldGenerationMode = generationMode;
    }

}