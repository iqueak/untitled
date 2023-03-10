//
// Created by bohdan.yurov on 11.02.2023.
//
#include "raylib.h"
#include <iostream>
#include <rlgl.h>
#include <thread>
#include "Block.h"
#include "unordered_map"
#include "set"
#include "map"
#include "ChunkManager.h"
#include "WorldGenerator.h"
#include "vector"
#include "../PlayerController.h"
#include "mutex"

#include "config.h"

using namespace std;

using namespace Block;

namespace ChunkManager {
    static std::set<int64_t> activeChunks; // Хранит только айди чанков из буфера

    // TODO попробовать в качестве айди для чанков буфера long int без хешера, используя стандартрный хешер, предлагается обьединять два 32 битных инта в один 64 long int
    static std::unordered_map<int64_t, Chunk> bufferChunks;

    // for information
    static int verticesCount = 0;
    static int trianglesCount = 0;
    static int activeSections = 0;
    static int activeChunksCount = 0;

    uint64_t getID(int32_t x, int32_t z) {
        // TODO на сколько в этом случае статик каст затратен для производительности? почти не влияет тк это указание компилятору
        return static_cast<uint64_t>(x) << 32 | static_cast<uint32_t>(z);
    }

    Chunk &getChunk(int32_t x, int32_t z) {
        uint64_t id = getID(x, z);
        return bufferChunks[id];
    }

    Chunk &newChunk(int32_t x, int32_t z) {
        uint64_t id = getID(x, z);
        bufferChunks[id] = Chunk{x, z};
        return bufferChunks[id];
    }


    int range(int v, int min, int max) {
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
    bool checkNeighborBlocks(Chunk &chunk, int x, int y, int z, int8_t f) {
        // должно распространятся на соседние чанки
        // пока что не выходим за грани чанка и используем для этого range

        x += Block::sideVectorForFace[f].x;
        y += Block::sideVectorForFace[f].y;
        z += Block::sideVectorForFace[f].z;

        if (isOutOfRange(x, 0, chunkSize) || isOutOfRange(y, 0, chunkMaxHeight) || isOutOfRange(z, 0, chunkSize))
            return false;

        uint8_t id = chunk.chunkData[y][x][z];

        if (id == int(BlockIDs::AIR)) return false;

        BlockType &data = getBlockData(id);
        return !data.isTransparent;
    }

    void Init() {
        DefaultChunkMaterial = LoadMaterialDefault();
        DefaultChunkMaterial.maps[MATERIAL_MAP_DIFFUSE].texture = TextureManager::GetAtlas()->texture2D;
    }

    string GetChunksInfo() {
        return "Vertices: " + to_string(verticesCount) + "\nTriangles: " + to_string(trianglesCount) + "\nFaces: " +
               to_string(verticesCount / 4) + "\nChunks: " + to_string(activeChunksCount) + "\nActive Sections: " +
               to_string(activeSections);
    }

    // TODO тест без использования модели только дравмеш с дефолт материалом


    void DrawChunks() {
        verticesCount = 0;
        trianglesCount = 0;
        activeSections = 0;
        activeChunksCount = 0;
        for (const auto &chunkId : activeChunks) { // TODO только для теста буфер н// ужно рисовать только активные
            Chunk &chunk = bufferChunks[chunkId];
            for (uint8_t s = 0; s < maxSectionsCount; s++) {
                // TODO нормально оформить создание этой матрицы
                Matrix modelTransform = MatrixIdentity(); // Создаем единичную матрицу трансформации модели
                modelTransform = MatrixMultiply(modelTransform, MatrixTranslate(chunk.x * chunkSize, 0.0f, chunk.z *
                                                                                                           chunkSize)); // Добавляем трансляцию модели
                modelTransform = MatrixMultiply(modelTransform,
                                                MatrixRotateXYZ(Vector3{0.0f, 0.0f, 0.0f})); // Добавляем поворот модели
                modelTransform = MatrixMultiply(modelTransform,
                                                MatrixScale(1.0f, 1.0f, 1.0f)); // Добавляем масштабирование модели
                if (!chunk.sections[s].isEmpty) {
                    DrawMesh(chunk.chunkMeshes[s], DefaultChunkMaterial, modelTransform);
                    // for info
                    verticesCount += chunk.chunkMeshes[s].vertexCount;
                    trianglesCount += chunk.chunkMeshes[s].triangleCount;
                    activeSections++;
                }
            }
            activeChunksCount++;
        }
    }

    Chunk &CreateChunk(int32_t x_, int32_t z_) {
        uint64_t id = getID(x_, z_);

        Chunk &ch = newChunk(x_, z_);

        // registry this chunk to active
        activeChunks.insert(id);


        WorldManager::generateChunkData(ch);
        buildMeshes(ch);

        return ch;
    }

    void LoadChunk(uint64_t id) {
        Chunk &ch = bufferChunks[id];
        for (int s = 0; s < maxSectionsCount; s++) {
            if (!ch.sections[s].isEmpty) {
                //UploadMesh(&ch.chunkMeshes[s], false);
            }
        }
        activeChunks.insert(id);
    }

    void UpdateChunk() {

    }

    void renderChunks() {
        // TODO возможно стоит использовать AABB или BoundingBox
        Vector3 playerCoords = PlayerController::GetPlayerCoords();

        int chunkX = int(playerCoords.x / chunkSize);
        int chunkZ = int(playerCoords.z / chunkSize);

        int chMinX = chunkX - (ActiveChunksResolution / 2);
        int chMinZ = chunkZ - (ActiveChunksResolution / 2);

        int chMaxX = chunkX + (ActiveChunksResolution / 2);
        int chMaxZ = chunkZ + (ActiveChunksResolution / 2);

        verticesCount = 0;
        trianglesCount = 0;
        activeSections = 0;
        activeChunksCount = 0;

        //Load and Create chunks
        /*** Проходит по области координат видимых чанков, и если такого чанка нет - создает его, а если есть делает
         * обновление чанка (тик и тд) ***/
        for (int32_t x = chMinX; x <= chMaxX; x++) {
            for (int32_t z = chMinZ; z <= chMaxZ; z++) {

                uint64_t id = getID(x, z);

                if (activeChunks.find(id) == activeChunks.end()) {
                    if (bufferChunks.find(id) == bufferChunks.end()) {
                        /*** Has'nt deactivated Chunk in buffer***/
                        CreateChunk(x, z);


                    } else {
                        LoadChunk(id);
                    }
                } else {
                    // Update Chunk
                    UpdateChunk();
                }
            }
        }

        // Unload chunks
        // TODO работает но со скрипом бля надо отдельный поток или норм алгоритм, посмотреть как сделано у других

        /*** Проходит по всем чанкам с помощью итератора, и если чанк не входит в видимую область, удаляет его из активных,
         * а если входит - отрисовывает его + обновляет общую информацию о вершинах, триугольниках и тд ***/
        for (auto chunkId = activeChunks.begin(); chunkId != activeChunks.end();) {
            Chunk &ch = bufferChunks[*chunkId];
            if (!(ch.x >= chMinX && ch.x <= chMaxX &&
                  ch.z >= chMinZ && ch.z <= chMaxZ)) {
                chunkId = activeChunks.erase(chunkId);
                //bufferChunks.erase(*chunkId); // TODO тут нужно удалять чанки из кеша и сохранять их в файл
            } else {
                for (uint8_t s = 0; s < maxSectionsCount; s++) {
                    if (!ch.sections[s].isEmpty) {
                        DrawMesh(ch.chunkMeshes[s], DefaultChunkMaterial, MatrixIdentity());
                        // for info
                        verticesCount += ch.chunkMeshes[s].vertexCount;
                        trianglesCount += ch.chunkMeshes[s].triangleCount;
                        activeSections++;
                    }
                }
                activeChunksCount++;
                ++chunkId; // переходим к следующему элементу
            }
        }
    }

    void Update() {
        renderChunks();
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
            Mesh &mesh = chunk.chunkMeshes[s];

            for (uint8_t y = 0; y < chunkSize; y++) {
                for (uint8_t x = 0; x < chunkSize; x++) {
                    for (uint8_t z = 0; z < chunkSize; z++) {

                        int X = x;
                        int Y = sectionOffset + y; // отступ секции только для Y
                        int Z = z;

                        uint8_t id = chunk.chunkData[Y][X][Z];

                        // TODO : а что если хранить только ячейки с заполненными блоками, воздух не хранить!
                        if (id == int(BlockIDs::AIR)) continue;

                        Block::BlockType &BlockData = getBlockData(id);

                        for (int8_t f = 0; f < 6; f++) {

                            bool neighbor = checkNeighborBlocks(chunk, X, Y, Z, f);
                            if (neighbor) continue;

                            for (int8_t v = 0; v < 4; v++) {

                                // vertices
                                vertices.push_back(
                                        cubeVertices[verticesForFace[f][v]].x + X + chunk.x * chunkSize);
                                vertices.push_back(cubeVertices[verticesForFace[f][v]].y + Y);
                                vertices.push_back(
                                        cubeVertices[verticesForFace[f][v]].z + Z + chunk.z * chunkSize);

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
            // TODO очень кривое определение пустой секции но пока что так, надо определять на моменте генерации, генерить чанк дату по секторно?
            // TODO TEST  в будущем изменить создание секции
            chunk.sections[s] = Section{s, true};

            if (vertices.size() > 0) {
                mesh.vertexCount = vertices.size() / 3;
                mesh.triangleCount = triangles.size() / 3;
                mesh.vertices = vertices.data();
                mesh.texcoords = texcoords.data();
                mesh.normals = normals.data();
                mesh.indices = triangles.data();

                chunk.sections[s].isEmpty = false;
                UploadMesh(&mesh, false);
            }
        }
    }
}
