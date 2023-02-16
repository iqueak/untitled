//
// Created by bohdan.yurov on 13.02.2023.
//

#ifndef UNTITLED_BLOCK_H
#define UNTITLED_BLOCK_H

#endif //UNTITLED_BLOCK_H

#include "raylib.h"

#include "TextureManager.h"



namespace Block {

    using namespace TextureManager;

    enum class BlockIDs {
        AIR = 0,
        DIRT,
        GRASS,
        STONE,
        SAND,
        ICE,

        _COUNT
    };

    enum class Faces {
        FRONT = 0,
        RIGHT,
        BACK,
        LEFT,
        BOTTOM,
        TOP,

        _COUNT
    };

    struct BlockType {

        char* name;

        bool isTransparent{};

        Vector2* getTextureCoords(Faces face){
            return coordForFace[int(face)];
        }

        Vector2 coordForFace[6][4]{};

    };

    void blockRegistry(BlockIDs id, BlockType & type, int index);
    void blockRegistry(BlockIDs id, BlockType & type, int indexes[6]);

    BlockType & getBlockData(BlockIDs id);

    void Init();
}