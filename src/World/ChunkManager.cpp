//
// Created by bohdan.yurov on 11.02.2023.
//
#include "raylib.h"
#include <iostream>
#include <rlgl.h>
#include "Block.h"
#include "map"
#include "ChunkManager.h"
#include "WorldGenerator.h"
#include "vector"

#include "config.h"

using namespace std;

using namespace Block;

namespace ChunkManager {

    static std::map<int, Chunk> activeChunks;

    bool checkneighbor(int x, int y, int f) {

        return false;
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
        vector<unsigned short> triangles;// TODO standart unsigned short
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

        chunk.chunkMesh.vertexCount = vertices.size() / 3;
        chunk.chunkMesh.triangleCount = triangles.size() / 3;

        chunk.chunkMesh.vertices = vertices.data();
        chunk.chunkMesh.texcoords = texcoords.data();
        chunk.chunkMesh.normals = normals.data();


        /*** TODO так как indices это short int то количество триугольников ограничено 32 тысячами,
         * из за того что максимальная вершина на которую может указать индекс, 65535, это 2730 блоков
         *   нужно разделить чанк на секции, и хранить в секции меш, придумать какие значения брать для секции  и тд
         * ***/
        chunk.chunkMesh.indices = triangles.data();

        UploadMesh(&chunk.chunkMesh,false);
    }
}
