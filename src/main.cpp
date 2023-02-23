#include <iostream>
#include "raylib.h"
#include "string"
#include "Window.h"
#include "World/WorldManager.h"


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



    // INIT
    TextureManager::Init();
    Block::Init();

    WorldManager::Init();

    // We generate a checked image for texturing
    //Image checked = GenImageChecked(2, 2, 1, 1, RED, GREEN);
    //Texture2D texture = LoadTextureFromImage(checked);
    //UnloadImage(checked);

    // Set checked texture as default diffuse component for all models materia

    // Define the camera to look into our 3d world
    Camera camera = {{1.0f, 19.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, 45.0f, 0};

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

        DrawModel(WorldManager::getWorld().WorldModel, position, 1.0f, WHITE);

        DrawGrid(10, 1.0);

        EndMode3D();

        //DrawRectangle(30, 400, 310, 30, Fade(SKYBLUE, 0.5f));
        //DrawRectangleLines(30, 400, 310, 30, Fade(DARKBLUE, 0.5f));

        //std::string s = "MESH VERTICES COUNT: " + std::to_string(mesh.vertexCount);
        //char const *pchar = s.c_str();
        //DrawText(pchar, 0, 0, 24, BLACK);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    CloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------


    return 0;
}


