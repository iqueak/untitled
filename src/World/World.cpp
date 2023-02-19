//
// Created by bohdan.yurov on 19.02.2023.
//

#include "World.h"
#include "map"

namespace World {

    //static ChunkManager::Chunk activeChunks[ActiveChunksResolution * ActiveChunksResolution];
    static std::map<int, ChunkManager::Chunk> activeChunks;


    static Block::BlockType  BlockTypess[int(Block::BlockIDs::_COUNT)];

    void World::Init() {

        activeChunks = std::map<int,ChunkManager::Chunk>();
        activeChunks[0] = Chunk{1,3};

        WorldGenerationMode = WorldGenerationTypes::FLAT_MODE; // TODO first mode for test
    }

}