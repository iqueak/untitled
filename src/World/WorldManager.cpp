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
        _World = new World("MyWorld",123,WorldGenerationTypes::FLAT_MODE);

         // TODO first mode for test

        Chunk & chunk = CreateChunk(0,0);

        //create model
        Model model = { 0 };

        model.transform = { 1.0f, 0.0f, 0.0f, 0.0f,
                            0.0f, 1.0f, 0.0f, 0.0f,
                            0.0f, 0.0f, 1.0f, 0.0f,
                            0.0f, 0.0f, 0.0f, 1.0f };

        model.meshCount = 16;
        model.meshes = (Mesh *)RL_CALLOC(model.meshCount, sizeof(Mesh));

        model.materialCount = 1;
        model.materials = (Material *)RL_CALLOC(model.materialCount, sizeof(Material));
        model.materials[0] = LoadMaterialDefault();
        model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = TextureManager::GetAtlas()->texture2D;

        model.meshMaterial = (int *)RL_CALLOC(model.meshCount, sizeof(int));

        for (int i = 0; i < maxSectionsCount; i++) {
            model.meshes[i] = chunk.chunkMeshes[i];
            model.meshMaterial[i] = 0;
        }

        _World->WorldModel = model;
    }

    void DrawWorld() {

    }

    World & getWorld() {
        return * _World;
    }

    World::World(char *name, int seed, WorldGenerationTypes generationMode) {
        Name = name;
        Seed = seed;
        WorldGenerationMode = generationMode;
    }

    /*World::~World() {
        // Unload models data (GPU VRAM)
        UnloadModel(_World->WorldModel);
    }*/
}