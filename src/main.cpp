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
    //SetConfigFlags(FLAG_VSYNC_HINT);
    //SetConfigFlags(FLAG_MSAA_4X_HINT);

    Window::Init();

    TextureManager::Init();
    ChunkManager::Init();
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

        ChunkManager::Update();
        //ChunkManager::DrawChunks();

        DrawGrid(10, 1.0);

        EndMode3D();

        //DrawRectangle(30, 400, 310, 30, Fade(SKYBLUE, 0.5f));
        //DrawRectangleLines(30, 400, 310, 30, Fade(DARKBLUE, 0.5f));

        //std::string s = "MESH VERTICES COUNT: " + std::to_string(mesh.vertexCount);
        //char const *pchar = s.c_str();

        string message = "FPS:" + to_string(GetFPS()) + "\n" + ChunkManager::GetChunksInfo() + "\nX: " + to_string(PlayerController::GetPlayerCoords().x) +
                "; Y: " + to_string(PlayerController::GetPlayerCoords().y) +
                "; Z: " + to_string(PlayerController::GetPlayerCoords().z);

        message += "\nChunk_X: " + to_string(int(PlayerController::GetPlayerCoords().x / chunkSize)) +
                               "; Chunk_Z: " + to_string(int(PlayerController::GetPlayerCoords().z / chunkSize));

        DrawText(message.c_str() , 0, 0, 14, BLACK);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    CloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------


    return 0;
}
