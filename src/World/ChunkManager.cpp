//
// Created by bohdan.yurov on 11.02.2023.
//
#include "raylib.h"
#include <iostream>
#include "Block.h"

using namespace std;

namespace Chunk {

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
// Векторы сторон для каждой грани куба, или нормали
    static const Vector3 sideVectorForFace[6] = {
            {0.0f,  0.0f,  -1.0f},
            {1.0f,  0.0f,  0.0f},
            {0.0f,  0.0f,  1.0f},
            {-1.0f, 0.0f,  0.0f},
            {0.0f,  -1.0f, 0.0f},
            {0.0f,  1.0f,  0.0f}
    };
// список вершин для грани
    static const int verticesForFace[6][4] = {
            {0, 1, 2, 3}, // front
            {3, 2, 6, 7}, // right
            {7, 6, 5, 4}, // back
            {4, 5, 1, 0}, // left
            {4, 0, 3, 7}, // bottom
            {1, 5, 6, 2} // top
    };
// текстуры
    static const Vector2 texCoord[4][2] = {
            {0.0f, 0.0f},
            {0.0f, 1.0f},
            {1.0f, 1.0f},
            {1.0f, 0.0f}
    };

    bool checkneighbor(int x, int y, int f) {

        return false;
    }

    Mesh getCubeMesh() {

        Mesh mesh = Mesh{0};

        mesh.vertexCount = 24;
        mesh.triangleCount = 12;

        mesh.vertices = (float *) MemAlloc(24 * 3 * sizeof(float));
        mesh.texcoords = (float *) MemAlloc(24 * 2 * sizeof(float));
        mesh.normals = (float *) MemAlloc(24 * 3 * sizeof(float));
        mesh.indices = (unsigned short *) RL_MALLOC(mesh.triangleCount * 3 * sizeof(unsigned short));

        // iteration for Faces

        int vCount = 0;
        int tx = 0;
        int n = 0;
        int t = 0;
        int w = 0;

        for (int f = 0; f < 6; f++) {
            //if (!checkneighbor(1, 1, f)) continue;

            Block::BlockType& BlockData = Block::getBlockData(Block::BlockIDs::DIRT);


            for (int v = 0; v < 4; v++) {
                // vertices
                mesh.vertices[w++] = cubeVertices[verticesForFace[f][v]].x;
                mesh.vertices[w++] = cubeVertices[verticesForFace[f][v]].y;
                mesh.vertices[w++] = cubeVertices[verticesForFace[f][v]].z;

                // normals
                mesh.normals[n++] = sideVectorForFace[f].x;
                mesh.normals[n++] = sideVectorForFace[f].y;
                mesh.normals[n++] = sideVectorForFace[f].z;

                //texcoords
                //mesh.texcoords[tx++] = texCoord[v]->x;
                //mesh.texcoords[tx++] = texCoord[v]->y;

                float x = BlockData.getTextureCoords(static_cast<Block::Faces>(f))[v].x;
                float y = BlockData.getTextureCoords(static_cast<Block::Faces>(f))[v].y;

                mesh.texcoords[tx++] = x;
                mesh.texcoords[tx++] = y;
            }

            vCount += 4;

            // triangles
            mesh.indices[t++] = vCount - 4;
            mesh.indices[t++] = vCount - 3;
            mesh.indices[t++] = vCount - 2;
            mesh.indices[t++] = vCount - 2;
            mesh.indices[t++] = vCount - 1;
            mesh.indices[t++] = vCount - 4;
        }

        UploadMesh(&mesh, false);

        return mesh;
    }
}