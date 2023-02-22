//
// Created by bohdan.yurov on 19.02.2023.
//

#include "WorldGenerator.h"
#include "WorldManager.h"

using namespace ChunkManager;
using namespace Block;

namespace WorldManager {

    void WorldManager::generateChunkData(Chunk &chunk) {
        //TODO: почему то не получает ворлдгенератионмод???
        //if (_World->WorldGenerationMode == WorldGenerationTypes::FLAT_MODE) {

            BlockIDs setType = BlockIDs::AIR; // Type Air

            for (int y = 0; y < chunkMaxHeight; y++) {
                for (int x = 0; x < chunkSize; x++) {
                    for (int z = 0; z < chunkSize; z++) {
                        if (y == 0) setType = BlockIDs::BEDROCK;
                        if (y == 1) setType = BlockIDs::STONE;
                        if (y == 2) setType = BlockIDs::BEDROCK;
                        if (y == 3) setType = BlockIDs::STONE;
                        if (y == 4) setType = BlockIDs::BEDROCK;
                        if (y == 5) setType = BlockIDs::STONE;
                        if (y == 6) setType = BlockIDs::BEDROCK;
                        if (y == 7) setType = BlockIDs::STONE;
                        if (y == 8) setType = BlockIDs::BEDROCK;
                        if (y == 9) setType = BlockIDs::STONE;
                        if (y == 10) setType = BlockIDs::BEDROCK;
                        if (y == 11) setType = BlockIDs::STONE;
                        if (y > 11) setType = BlockIDs::AIR;
                        chunk.chunkData[y][x][z] = setType;
                    }
                }
            }
        //}
    }
}

