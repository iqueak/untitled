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

    const uint8_t chunkSize = 16;
    const uint16_t chunkMaxHeight = 256; // must be 512

    const uint8_t ActiveChunksResolution = 4;
    const uint16_t ActiveChunksCount = ActiveChunksResolution * ActiveChunksResolution;
    const uint8_t maxSectionsCount = chunkMaxHeight / chunkSize;

    /*struct Section {
        //uint8_t id;
    };*/

    struct Chunk {
    public:
        int x;
        int z;
        //Section sections[maxSectionsCount];
        Mesh chunkMeshes[maxSectionsCount];

        Model chunkModel;

        /***        Y               X           Z     ***/
        uint8_t chunkData[chunkMaxHeight][chunkSize][chunkSize];

        Vector3 position;

        //int chunkData[chunkSize * chunkSize * chunkMaxHeight]; TODO: найти лучший способ хранения чанк даты
        // TODO: хранить тип блока в виде unsigned short или еще меньше,т.к нужно не так много типов блоков, найти подходящий тип данных по обьему
        // TODO: предположительно на это можно затратить около 1 байта и максимальное количество типов блока будет 255 для такой версии,
        // урезанной этого должно хватить

        // TODO: так же при генерации данных чанка, стоит хранить маску видимых сторон блока, что бы не перепроверять весь чанк всегда, но только
        // надо изучить сколько это сможет сэкономить нагрузки на процессор, и сколько потребует памяти!

        // TODO: орагнизовать для всех таких моментов, в репозитории список проблем

        // TODO использовать подходящие типы данных экономящие память в итоге

        // TODO применять одни и те же правила для наименований переменных и тд CamelCase?

        void generateModel();

        int getChunkId();
    };

    void Init();

    void DrawChunks();

    void buildMeshes(Chunk& chunk);

    Chunk & CreateChunk(int x, int z);

    void UnloadChunk(int x, int z);

    string GetChunksInfo();

    void Update();

}
