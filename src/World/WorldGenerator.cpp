//
// Created by bohdan.yurov on 19.02.2023.
//

#include "WorldGenerator.h"
#include "WorldManager.h"

using namespace ChunkManager;
using namespace Block;

namespace WorldManager {

    void WorldManager::generateChunkData(Chunk &chunk) {
        if (WorldManager::getWorld().WorldGenerationMode == WorldGenerationTypes::FLAT_MODE) {

            int setType = int(BlockIDs::AIR); // Type Air

            for (int y = 0; y < chunkMaxHeight; y++) {
                for (int x = 0; x < chunkSize; x++) {
                    for (int z = 0; z < chunkSize; z++) {
                        if (y == 0)  setType = int(BlockIDs::BEDROCK);
                        if (y >= 1) setType = int(BlockIDs::STONE);
                        if (y >= 16) setType = int(BlockIDs::DIRT);
                        if (y >= 18) setType = int(BlockIDs::GRASS);
                        if (y >= 19) setType = int(BlockIDs::AIR);
                        chunk.chunkData[y][x][z] = setType;
                    }
                }
            }
        }
    }
}

