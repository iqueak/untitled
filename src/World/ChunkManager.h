//
// Created by bohdan.yurov on 11.02.2023.
//
#pragma once
#include "raylib.h"
#include "Block.h"

#ifndef UNTITLED_CHUNKMANAGER_H
#define UNTITLED_CHUNKMANAGER_H

#endif //UNTITLED_CHUNKMANAGER_H


namespace ChunkManager {

    const int chunkSize = 16;
    const int chunkMaxHeight = 256; // must be 512

    const int ActiveChunksResolution = 16;
    const int ActiveChunksCount = ActiveChunksResolution * ActiveChunksResolution;

    struct Chunk {
        int x;
        int z;

        //int chunkData[chunkSize * chunkSize * chunkMaxHeight]; TODO: найти лучший способ хранения чанк даты
        /***              Y               X           Z     ***/
        Block::BlockIDs chunkData[chunkMaxHeight][chunkSize][chunkSize];

        Mesh chunkMesh;

        int getChunkId(){
            return z + ActiveChunksCount * x;
        }

    };

    void Init();

    void buildMesh(Chunk& chunk);

    Chunk & CreateChunk(int x, int z);

    void UnloadChunk(int x, int z);

    Mesh getCubeMesh(void);

}
