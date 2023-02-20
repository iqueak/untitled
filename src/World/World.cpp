//
// Created by bohdan.yurov on 19.02.2023.
//

#include "World.h"
#include "map"
#include "../Utils/Logger.h"
#include "string"
#include "ChunkManager.h"
#include "WorldGenerator.h"

using namespace ChunkManager;

namespace World {

    void World::Init() {

        WorldGenerationMode = WorldGenerationTypes::FLAT_MODE; // TODO first mode for test

        Chunk & chunk = CreateChunk(0,0);
        generateChunkData(chunk);

    }


}