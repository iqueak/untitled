//
// Created by bohdan.yurov on 13.02.2023.
//
#pragma once

#ifndef UNTITLED_BLOCK_H
#define UNTITLED_BLOCK_H

#endif //UNTITLED_BLOCK_H

#include "raylib.h"

#include "TextureManager.h"



namespace Block {

    using namespace TextureManager;

    /*** Vertices of cube ***/
    static const Vector3 cubeVertices[8] = {
            {0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {1.0f, 1.0f, 0.0f},
            {1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f},
            {0.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, 1.0f},
            {1.0f, 0.0f, 1.0f}
    };

    /*** Vectors sides for each face of cube or vectors for normals ***/
    static const Vector3 sideVectorForFace[6] = {
            {0.0f,  0.0f,  -1.0f},
            {1.0f,  0.0f,  0.0f},
            {0.0f,  0.0f,  1.0f},
            {-1.0f, 0.0f,  0.0f},
            {0.0f,  -1.0f, 0.0f},
            {0.0f,  1.0f,  0.0f}
    };

    /*** Vertices list for Face ***/
    static const int verticesForFace[6][4] = {
            {0, 1, 2, 3}, // front
            {3, 2, 6, 7}, // right
            {7, 6, 5, 4}, // back
            {4, 5, 1, 0}, // left
            {4, 0, 3, 7}, // bottom
            {1, 5, 6, 2} // top
    };

    enum class BlockIDs {
        AIR = 0,
        BEDROCK,
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