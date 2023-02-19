//
// Created by bohdan.yurov on 18.02.2023.
//
#include "Window.h"
#include "raylib.h"

namespace Window {

    const int screenWidth = 1366;
    const int screenHeight = 768;


    void Window::Init(){
        InitWindow(screenWidth, screenHeight, "raylib [models] example - mesh generation");
        ToggleFullscreen();
    }

}
