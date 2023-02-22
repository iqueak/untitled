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
        // TODO: хранить тип блока в виде unsigned short или еще меньше,т.к нужно не так много типов блоков, найти подходящий тип данных по обьему
        // TODO: предположительно на это можно затратить около 1 байта и максимальное количество типов блока будет 255 для такой версии,
        // урезанной этого должно хватить

        // TODO: так же при генерации данных чанка, стоит хранить маску видимых сторон блока, что бы не перепроверять весь чанк всегда, но только
        // надо изучить сколько это сможет сэкономить нагрузки на процессор, и сколько потребует памяти!

        // TODO: орагнизовать для всех таких моментов, в репозитории список проблем
        
        /***        Y               X           Z     ***/
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

}
