#include <iostream>
#include "raylib.h"
#include "string"
#include "Window.h"
#include "World/WorldManager.h"
#include "PlayerController.h"



// TODO подключение ANL
#define ANL_IMPLEMENTATION
 #include "anl.h"

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

    Window::Init();

    TextureManager::Init();
    Block::Init();

    WorldManager::Init();
    PlayerController::Init();





    // Model drawing position
    Vector3 position = {0.0f, 0.0f, 0.0f};


    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------

        //CameraController::Update();
        PlayerController::Update();


        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);
        PlayerController::BeginCamera3DMode();


        ChunkManager::DrawChunks();

        DrawGrid(10, 1.0);

        EndMode3D();

        //DrawRectangle(30, 400, 310, 30, Fade(SKYBLUE, 0.5f));
        //DrawRectangleLines(30, 400, 310, 30, Fade(DARKBLUE, 0.5f));

        //std::string s = "MESH VERTICES COUNT: " + std::to_string(mesh.vertexCount);
        //char const *pchar = s.c_str();
        DrawText(ChunkManager::GetChunksInfo().c_str(), 0, 0, 24, BLACK);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    CloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------


    return 0;
}
/*
#include "raylib.h"
#include "raymath.h"
#include "rlFPCamera.h"
#include "rlgl.h"

#pragma comment(lib, "raylib.lib")

int main(int argc, char* argv[])
{
    // Initialization
    //--------------------------------------------------------------------------------------
    int screenWidth = 1900;
    int screenHeight = 900;

    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "raylibExtras [camera] example - First person camera");
    SetTargetFPS(144);

    Image img = GenImageChecked(256, 256, 32, 32, DARKGRAY, WHITE);
    Texture tx = LoadTextureFromImage(img);
    UnloadImage(img);
    SetTextureFilter(tx, TEXTURE_FILTER_ANISOTROPIC_16X);
    SetTextureWrap(tx, TEXTURE_WRAP_CLAMP);

    rlFPCamera cam;
    cam.Setup(45,Vector3{ 1, 0, 0 });
    cam.MoveSpeed.z = 10;
    cam.MoveSpeed.x = 5;

    cam.FarPlane = 5000;

    Mesh cube = GenMeshCube(1, 1, 1);
    Material greenMaterial = LoadMaterialDefault();
    greenMaterial.maps[MATERIAL_MAP_ALBEDO].color = GREEN;
    greenMaterial.maps[MATERIAL_MAP_ALBEDO].texture = tx;

    Material brownMaterial = LoadMaterialDefault();
    brownMaterial.maps[MATERIAL_MAP_ALBEDO].color = BROWN;
    brownMaterial.maps[MATERIAL_MAP_ALBEDO].texture = tx;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        cam.Update();

        BeginDrawing();
        ClearBackground(WHITE);

        cam.BeginMode3D();

        // grid of cube trees on a plane to make a "world"
        DrawPlane(Vector3{ 0, 0, 0 }, Vector2{ 50, 50 }, BEIGE); // simple world plane
        float spacing = 4;
        int count = 5;

        int total = 0;
        int vis = 0;

        for (float x = -count * spacing; x <= count * spacing; x += spacing)
        {
            for (float z = -count * spacing; z <= count * spacing; z += spacing)
            {
                Matrix transform = MatrixTranslate(x, 1.5f, z);
                DrawMesh(cube, greenMaterial, transform);

                transform = MatrixTranslate(x, 0.5f, z);
                transform = MatrixMultiply(MatrixScale(0.25, 1, 0.25), transform);
                DrawMesh(cube, brownMaterial, transform);
            }
        }

        cam.EndMode3D();

        DrawText(TextFormat("%d visible of %d total", vis, total), 200, 0, 20, GREEN);

        DrawFPS(0, 0);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }
    UnloadMesh(cube);
    UnloadTexture(tx);
    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
*/
