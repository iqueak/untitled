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
#include "../PlayerController.h"

#include "config.h"

using namespace std;

using namespace Block;

namespace ChunkManager {

    // TODO почемуто не отрисовует!!!
    static std::map<int, Chunk> activeChunks;
    static std::map<int, Chunk> bufferChunks;

    int range(int v,int min,int max){
        if (v > max) return max;
        if (v < min) return min;
        return v;
    }

    /*** included min but not max value !!!!!!!!! ***/
    bool isOutOfRange(int16_t v, int16_t min, int16_t max) {
        return !(v < max && v >= min);
    }

    // must be a global coords translated to chunk coords and block coords in this chunk, if coord is out of blocks coords in buffer, must returned false
    /*** True сосед есть, рисовать ненужно! ***/
    bool checkNeighborBlocks(Chunk & chunk, int x, int y, int z, int8_t f) {
        // должно распространятся на соседние чанки
        // пока что не выходим за грани чанка и используем для этого range

        x += Block::sideVectorForFace[f].x;
        y += Block::sideVectorForFace[f].y;
        z += Block::sideVectorForFace[f].z;

        // TODO для теста отключил отрисовку по краям чанка иза выхода за границы
        if( isOutOfRange(x,0,chunkSize) || isOutOfRange(y,0,chunkMaxHeight) || isOutOfRange(z,0,chunkSize)) return true;

        uint8_t id = chunk.chunkData[y][x][z];

        if(id == int(BlockIDs::AIR)) return false;

        BlockType & data = getBlockData(id);
        return !data.isTransparent;
    }

    void Init() {

    }

    string GetChunksInfo(){
        int vertices = 0;
        int triangles = 0;
        int chunksCount = 0;
        int activeSections = 0;

        for (const auto& chunks : activeChunks) {
            for (int i = 0; i < maxSectionsCount; i++) {
                if(chunks.second.chunkMeshes[i].vertexCount > 0) {
                    vertices += chunks.second.chunkMeshes[i].vertexCount;
                    triangles += chunks.second.chunkMeshes[i].triangleCount;
                    activeSections++;
                }
            }
            chunksCount++;
        }

        return "Vertices: " + to_string(vertices) + "\nTriangles: " + to_string(triangles) + "\nFaces: " + to_string(vertices/4) + "\nChunks: " + to_string(chunksCount) + "\nActive Sections: " + to_string(activeSections);
    }

    void DrawChunks() {
        for (const auto& value : activeChunks) {
            DrawModel(value.second.chunkModel, Vector3{float(value.second.x * chunkSize),0,float(value.second.z * chunkSize)}, 1.0f, WHITE);
        }
    }

    Chunk & CreateChunk(int x_, int z_) {
        Logger::LogToFile("CREATE CHUNK:","(" + to_string(x_) + "," + to_string(z_));
        int id = z_ + ActiveChunksCount * x_;
        activeChunks[id] = Chunk{x_,z_};
        Chunk & ch = activeChunks[id];

        WorldManager::generateChunkData(ch);
        buildMeshes(ch);

        ch.generateModel();
        return ch;
    }

    void UnloadChunk(int x, int z) {
        int id = z + ActiveChunksCount * x;

        /*for (int i = 0; i < maxSectionsCount; ++i) {
            UnloadMesh(activeChunks[id].chunkMeshes[i]);
        }*/
        // Unload Model should also meshes unload
        UnloadModel(activeChunks[id].chunkModel);

        bufferChunks[id] = activeChunks[id];

        activeChunks.erase(id);
    }

    void LoadChunk(int x, int z) {
        int id = z + ActiveChunksCount * x;
        activeChunks[id] = bufferChunks[id];

        for (int i = 0; i < maxSectionsCount; i++) {
            UploadMesh(&activeChunks[id].chunkMeshes[i],false);
        }
        activeChunks[id].generateModel();
        bufferChunks.erase(id);
    }

    void UpdateChunk() {

    }

    void Update() {
        // TODO возможно стоит использовать AABB или BoundingBox
        Vector3 playerCoords = PlayerController::GetPlayerCoords();

        int chunkX = int(playerCoords.x / chunkSize);
        int chunkZ = int(playerCoords.z / chunkSize);

        int chMinX = chunkX - (ActiveChunksResolution/2);
        int chMinZ = chunkZ - (ActiveChunksResolution/2);

        int chMaxX = chunkX + (ActiveChunksResolution/2);
        int chMaxZ = chunkZ + (ActiveChunksResolution/2);


        //Load and Create chunks
        for (int x = chMinX; x <= chMaxX; x++) {
            for (int z = chMinZ; z <= chMaxZ; z++) {
                int id = z + ActiveChunksCount * x;
                if(activeChunks.find(id) == activeChunks.end()) {
                    //Logger::LogToFile("CHUNK DONT FOUND:","(" + to_string(x) + "," + to_string(z));
                    CreateChunk(x,z);
                    //if(bufferChunks.find(id) == bufferChunks.end()) {
                        /*** Has'nt deactivated Chunk in buffer***/

                    //} else {
                        //LoadChunk(x,z);
                    //}
                }
                else {
                    //Logger::LogToFile("CHUNK BEEN FOUNDED:","(" + to_string(x) + "," + to_string(z));
                    // Update Chunk
                    UpdateChunk();
                }
            }
        }

        // Unload chunks
        for (const auto& value : activeChunks) {
            if (!(value.second.x >= chMinX && value.second.x <= chMaxX && value.second.z >= chMinZ && value.second.z <= chMaxZ)){
                //UnloadChunk(value.second.x,value.second.z);
            }
        }
    }

    void buildMeshes(Chunk &chunk) {
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
                                vertices.push_back(cubeVertices[verticesForFace[f][v]].x + X + chunk.position.x * chunkSize);
                                vertices.push_back(cubeVertices[verticesForFace[f][v]].y + Y);
                                vertices.push_back(cubeVertices[verticesForFace[f][v]].z + Z + chunk.position.z * chunkSize);

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

    void Chunk::generateModel() {
        //TODO возможно это не лучший вариант, хранить в чанке модель, и делать одну модель на каждый чанк,
        //TODO но покачто это будет так, а потом в итоге, рендер клиента будет отделен от сервера

        // Crate Model
        chunkModel = Model{0};
        chunkModel.transform = { 1.0f, 0.0f, 0.0f, 0.0f,
                                    0.0f, 1.0f, 0.0f, 0.0f,
                                    0.0f, 0.0f, 1.0f, 0.0f,
                                    0.0f, 0.0f, 0.0f, 1.0f };
        chunkModel.meshCount = maxSectionsCount;
        chunkModel.meshes = (Mesh *)RL_CALLOC(chunkModel.meshCount, sizeof(Mesh));
        chunkModel.materialCount = 1;
        chunkModel.materials = (Material *)RL_CALLOC(chunkModel.materialCount, sizeof(Material));
        chunkModel.materials[0] = LoadMaterialDefault();
        chunkModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = TextureManager::GetAtlas()->texture2D;
        chunkModel.meshMaterial = (int *)RL_CALLOC(chunkModel.meshCount, sizeof(int));

        for (int i = 0; i < ChunkManager::maxSectionsCount; i++) {
            // TODO дублируются ли тут меши или нет?????
            chunkModel.meshes[i] = chunkMeshes[i];
            chunkModel.meshMaterial[i] = 0;
        }
    }

    int Chunk::getChunkId() {
        return z + ActiveChunksCount * x;
    }
}
