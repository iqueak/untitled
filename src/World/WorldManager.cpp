//
// Created by bohdan.yurov on 19.02.2023.
//

#include "WorldManager.h"
#include "map"
#include "../Utils/Logger.h"
#include "raylib.h"
#include "string"
#include "ChunkManager.h"
#include "WorldGenerator.h"

using namespace ChunkManager;

namespace WorldManager {

    void WorldManager::Init() {
        _World = new World("MyWorld",123);

         // TODO first mode for test

        Chunk & chunk = CreateChunk(0,0);
        _World->WorldModel = LoadModelFromMesh(chunk.chunkMesh);
        _World->WorldModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = TextureManager::GetAtlas()->texture2D;


    }

    void DrawWorld() {

    }

    Model &getWorldModel() {
        return _World->WorldModel;
    }

    World::World(char *name, int seed) {
        Name = name;
        Seed = seed;
        WorldGenerationMode = WorldGenerationTypes::FLAT_MODE;
    }

    /*World::~World() {
        // Unload models data (GPU VRAM)
        UnloadModel(_World->WorldModel);
    }*/
}