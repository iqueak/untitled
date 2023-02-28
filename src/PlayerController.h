//
// Created by bohdan.yurov on 16.02.2023.
//
#pragma once
#ifndef UNTITLED_PLAYERCONTROLLER_H
#define UNTITLED_PLAYERCONTROLLER_H

#endif //UNTITLED_PLAYERCONTROLLER_H

#include "raylib.h"
#include "rcamera.h"
#include "raymath.h"

namespace PlayerController {

    // Camera
    static const float fovY = 45;
    static const float MouseSensitivity = 600;
    static const float MinimumViewY = -65.0f;
    static const float MaximumViewY = 89.0f;
    static const bool InvertY = false;

    struct Camera {
        Camera3D ViewCamera;
        Vector2 Angle = { 0,0 }; // Camera angle in plane XZ
    };

    //Player
    static const Vector3 MoveSpeed = { 40, 40, 40 };

    typedef enum
    {
        MOVE_FRONT = 0,
        MOVE_BACK,
        MOVE_RIGHT,
        MOVE_LEFT,

        MOVE_UP,
        MOVE_DOWN,

        LAST_KEY
    }PlayerControls;

    static int ControlsKeys[LAST_KEY] = { 'W', 'S', 'D', 'A', KEY_SPACE, KEY_LEFT_SHIFT};

    enum class GameModeTypes{
        FREE_CAMERA_MODE = 0,
        SURVIVAL_MODE,
        CREATIVE_MODE,

        _COUNT
    };

    struct Player {
        Vector3 position;
        GameModeTypes GameMode;
        Camera _Camera;
    };

    static Player _Player;

    void Init();
    void Update();
    void BeginCamera3DMode();



}


