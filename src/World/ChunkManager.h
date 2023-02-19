//
// Created by bohdan.yurov on 11.02.2023.
//
#include "raylib.h"
#include "Block.h"
#include "vector"

#ifndef UNTITLED_CHUNKMANAGER_H
#define UNTITLED_CHUNKMANAGER_H

#endif //UNTITLED_CHUNKMANAGER_H


namespace ChunkManager {

    const int chunkSize = 16;
    const int chunkMaxHeight = 256; // must be 512


    struct Chunk {
        int x;
        int z;

        int chunkData[chunkSize * chunkSize * chunkMaxHeight];

        Mesh chunkMesh;

    };

    void buildMesh(Chunk& chunk);


    Mesh getCubeMesh(void);

}
