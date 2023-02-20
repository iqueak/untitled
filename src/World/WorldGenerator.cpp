//
// Created by bohdan.yurov on 19.02.2023.
//

#include "WorldGenerator.h"
#include "World.h"

using namespace ChunkManager;
using namespace Block;

namespace World {

    void World::generateChunkData(Chunk &chunk) {
        if (WorldGenerationMode == WorldGenerationTypes::FLAT_MODE) {

            BlockIDs setType = static_cast<BlockIDs>(0); // Type Air

            for (int y = 0; y < chunkMaxHeight; y++) {
                for (int x = 0; x < chunkSize; x++) {
                    for (int z = 0; z < chunkSize; z++) {
                        if (y == 0) setType = BlockIDs::BEDROCK;
                        if (y > 0) setType = BlockIDs::STONE;
                        if (y > 10) setType = BlockIDs::DIRT;
                        if (y > 16) setType = BlockIDs::AIR;
                        chunk.chunkData[y][x][z] = setType;
                    }
                }
            }
        }
    }
}
