#include <iostream>
#include "raylib.h"
#include "string"
#include "World/ChunkManager.h"
#include "World/Block.h"

#define ANL_IMPLEMENTATION
 #include "anl.h"


// TODO покдлючить АНЛ





using namespace std;

/*** Сделать генерацию меша куба, каждую сторону отдельно, в заивисимости от проверки, на создание (есть ли сосед или пустота)
 *  и протестить сколько вершин будет, сколько чего и кого, а так же ковырнуть сразу освещение блока,
 *  а потом уже перетянуть это в генерацию меша чанка
 *
 *  использовать для примера Generate Mesh Plane, смотерть как он внутри устроен и ковырять
 */


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void) {

    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1280;
    const int screenHeight = 768;

    InitWindow(screenWidth, screenHeight, "raylib [models] example - mesh generation");


    anl::CKernel kernel;
    anl::CNoiseExecutor vm(kernel);

    auto b=kernel.gradientBasis(kernel.constant(3), kernel.seed(1546));

    auto i=vm.evaluateScalar(0.1,0.1,b);
    std::cout << i << std::endl;


    // INIT
    Logger::Init();
    TextureManager::Init();
    Block::Init();






    // We generate a checked image for texturing
    //Image checked = GenImageChecked(2, 2, 1, 1, RED, GREEN);
    //Texture2D texture = LoadTextureFromImage(checked);
    //UnloadImage(checked);

    Model model;
    Mesh mesh = Chunk::getCubeMesh();
    model = LoadModelFromMesh(mesh);

    Texture2D tex = TextureManager::GetAtlas()->texture2D;
    // Set checked texture as default diffuse component for all models material
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = tex;

    // Define the camera to look into our 3d world
    Camera camera = {{5.0f, 5.0f, 5.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, 45.0f, 0};

    // Model drawing position
    Vector3 position = {0.0f, 0.0f, 0.0f};

    SetCameraMode(camera, CAMERA_FIRST_PERSON);  // Set a orbital camera mode

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        UpdateCamera(&camera);

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        DrawModel(model, position, 1.0f, WHITE);
        DrawGrid(10, 1.0);

        EndMode3D();

        //DrawRectangle(30, 400, 310, 30, Fade(SKYBLUE, 0.5f));
        //DrawRectangleLines(30, 400, 310, 30, Fade(DARKBLUE, 0.5f));

        std::string s = "MESH VERTICES COUNT: " + std::to_string(mesh.vertexCount);
        char const *pchar = s.c_str();
        DrawText(pchar, 0, 0, 24, BLACK);

        DrawText(to_string(tex.width).c_str(), 0, 30, 24, BLACK);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(tex); // Unload texture

    // Unload models data (GPU VRAM)
    UnloadModel(model);



    CloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------


    return 0;
}


