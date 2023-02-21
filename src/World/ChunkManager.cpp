//
// Created by bohdan.yurov on 11.02.2023.
//
#include "raylib.h"
#include <iostream>
#include "Block.h"
#include "map"
#include "ChunkManager.h"
#include "WorldGenerator.h"
#include "vector"

using namespace std;

using namespace Block;

namespace ChunkManager {

    static std::map<int, Chunk> activeChunks;

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



    void Init() {
        activeChunks = std::map<int,ChunkManager::Chunk>();
    }

    Chunk & CreateChunk(int x, int z) {
        int id = z + ActiveChunksCount * x;
        activeChunks[id] = Chunk{x,z};
        WorldManager::generateChunkData(activeChunks[id]);
        buildMesh(activeChunks[id]);
        return activeChunks[id];
    }

    void UnloadChunk(int x, int z) {

    }

    void buildMesh(Chunk &chunk) {
        vector<float> vertices;
        vector<unsigned short> triangles;
        vector<float> texcoords;
        vector<float> normals;

        int vCount = 0;
        for (int y = 0; y < chunkMaxHeight; y++) {
            for (int x = 0; x < chunkSize; x++) {
                for (int z = 0; z < chunkSize; z++) {

                    BlockIDs id = chunk.chunkData[y][x][z];

                    // TODO : а что если хранить только ячейки с заполненными блоками, воздух не хранить!
                    if (id == BlockIDs::AIR) continue;

                    Block::BlockType& BlockData = getBlockData(id);

                    for (int f = 0; f < 6; f++) {
                        // TODO: проверка фейса на то нужео его рисовать или нет

                        for (int v = 0; v < 4; v++) {

                            // vertices
                            vertices.push_back(cubeVertices[verticesForFace[f][v]].x + x + chunk.x);
                            vertices.push_back(cubeVertices[verticesForFace[f][v]].y + y);
                            vertices.push_back(cubeVertices[verticesForFace[f][v]].z + z + chunk.z);

                            // normals
                            normals.push_back(sideVectorForFace[f].x);
                            normals.push_back(sideVectorForFace[f].y);
                            normals.push_back(sideVectorForFace[f].z);

                            texcoords.push_back(BlockData.getTextureCoords(static_cast<Block::Faces>(f))[v].x);
                            texcoords.push_back(BlockData.getTextureCoords(static_cast<Block::Faces>(f))[v].y);
                        }
                        vCount += 4;

                        // triangles
                        triangles.push_back(vCount - 4);
                        triangles.push_back(vCount - 3);
                        triangles.push_back(vCount - 2);
                        triangles.push_back(vCount - 2);
                        triangles.push_back(vCount - 1);
                        triangles.push_back(vCount - 4);
                    }
                }
            }
        }

        //chunk.chunkMesh = Mesh{0};

        chunk.chunkMesh.vertexCount = vertices.size() / 3;
        chunk.chunkMesh.triangleCount = triangles.size() / 3;

        chunk.chunkMesh.vertices = vertices.data();//(float *) MemAlloc(vertices.size() * sizeof(float));
        chunk.chunkMesh.texcoords = texcoords.data();//(float *) MemAlloc(texcoords.size() * sizeof(float));
        chunk.chunkMesh.normals = normals.data();//(float *) MemAlloc(normals.size() * sizeof(float));

        /*** так как это short int то количество триугольников ограничено 32 тысячами
         *   возможным решением будет не использование триугольников, или создание меша с помощью rlgl
         *      https://github.com/raysan5/raylib/issues/807
         *
         *      попробовать класть триугольники в
         *        mesh->   unsigned int vaoId;     // OpenGL Vertex Array Object id
                  mesh->  unsigned int *vboId;
         *      игнорируя short int для indices
         *
         * ***/
        chunk.chunkMesh.indices = triangles.data();//(unsigned short *) RL_MALLOC(triangles.size() * sizeof(unsigned short));

        UploadMesh(&chunk.chunkMesh, false);
    }
}