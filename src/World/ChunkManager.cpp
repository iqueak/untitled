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

    int range(int v,int min,int max){
        if (v > max) return max;
        if (v < min) return min;
        return v;
    }

    /*** included min but not max value !!!!!!!!! ***/
    bool isOutOfRange(int16_t v, int16_t min, int16_t max) {
        return !(v < max && v >= min);
    }

    /*** True сосед есть, рисовать ненужно! ***/
    bool checkNeighborBlocks(Chunk & chunk, int x, int y, int z, int8_t f) {
        // должно распространятся на соседние чанки
        // пока что не выходим за грани чанка и используем для этого range

        x += Block::sideVectorForFace[f].x;
        y += Block::sideVectorForFace[f].y;
        z += Block::sideVectorForFace[f].z;

        // TODO убрать это было для дебага
        bool xOut = isOutOfRange(x,0,chunkSize);
        bool yOut = isOutOfRange(y,0,chunkMaxHeight);
        bool zOut = isOutOfRange(z,0,chunkSize);

        if( xOut|| yOut || zOut) return false;

        uint8_t id = chunk.chunkData[y][x][z];

        if(id == int(BlockIDs::AIR)) return false;

        BlockType & data = getBlockData(id);
        return !data.isTransparent;
    }

    void Init() {
        activeChunks = std::map<int,ChunkManager::Chunk>();
    }

    Chunk & CreateChunk(int x, int z) {
        int id = z + ActiveChunksCount * x;
        activeChunks[id] = Chunk{0,0};
        WorldManager::generateChunkData(activeChunks[id]);
        buildMesh(activeChunks[id]);
        return activeChunks[id];
    }

    void UnloadChunk(int x, int z) {

    }

    void buildMesh(Chunk &chunk) {

        // TODO DEBUG ограничил одной секцией
        for (uint8_t s = 0; s < maxSectionsCount; s++) {

            vector<float> vertices;
            vector<unsigned short> triangles;
            vector<float> texcoords;
            vector<float> normals;

            uint16_t vCount = 0;
            uint16_t sectionOffset = s * chunkSize;

            chunk.chunkMeshes[s] = Mesh{};
            Mesh & mesh = chunk.chunkMeshes[s];

            for (uint8_t y = 0; y < chunkSize; y++) {
                for (uint8_t x = 0; x < chunkSize; x++) {
                    for (uint8_t z = 0; z < chunkSize; z++) {

                        int X = x;
                        int Y = sectionOffset + y; // отступ секции только для Y
                        int Z = z;

                        uint8_t id = chunk.chunkData[Y][X][Z];


                        // TODO : а что если хранить только ячейки с заполненными блоками, воздух не хранить!
                        if (id == int(BlockIDs::AIR)) continue;

                        Block::BlockType& BlockData = getBlockData(id);

                        for (int8_t f = 0; f < 6; f++) {

                            bool neighbor = checkNeighborBlocks(chunk,X,Y,Z,f);
                            if(neighbor) continue;

                            for (int8_t v = 0; v < 4; v++) {

                                // vertices
                                vertices.push_back(cubeVertices[verticesForFace[f][v]].x + X);
                                vertices.push_back(cubeVertices[verticesForFace[f][v]].y + Y);
                                vertices.push_back(cubeVertices[verticesForFace[f][v]].z + Z);

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
            mesh.vertexCount = vertices.size() / 3;
            mesh.triangleCount = triangles.size() / 3;

            mesh.vertices = vertices.data();
            mesh.texcoords = texcoords.data();
            mesh.normals = normals.data();


            /*** из за того что максимальная вершина на которую может указать индекс, 65535, это 2730 блоков
            *    нужно разделить чанк на секции, и хранить в секции меш, придумать какие значения брать для секции  и тд
            ***/
            mesh.indices = triangles.data();

            UploadMesh(&mesh,false);

        }
    }
}
