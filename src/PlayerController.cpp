//
// Created by bohdan.yurov on 16.02.2023.
//
#include <cmath>
#include "PlayerController.h"
#include "raylib.h"
#include "Utils/Logger.h"

namespace PlayerController {

    void Init(){

        //PlayerInit
        _Player = Player{};
        _Player.position = Vector3 { 0.0f, 10.0f, -10.0f };
        _Player.GameMode = GameModeTypes::FREE_CAMERA_MODE;

        //CameraInit
        _Player._Camera = Camera{};
        _Player._Camera.ViewCamera.position = _Player.position;
        _Player._Camera.ViewCamera.target = Vector3Add(_Player.position, Vector3{10, 10.0f, 10 });
        _Player._Camera.ViewCamera.up = {0.0f, 1.0f, 0.0f };
        _Player._Camera.ViewCamera.fovy = fovY;
        _Player._Camera.ViewCamera.projection = CAMERA_PERSPECTIVE;

        SetCameraMode(_Player._Camera.ViewCamera, CAMERA_CUSTOM);

    };


    float GetSpeedForAxis(PlayerControls axis, float speed)
    {
        int key = ControlsKeys[axis];
        if (key == -1)
            return 0;

        float factor = 1.0f;
        /*if (IsKeyDown(ControlsKeys[SPRINT]))
            factor = 2;*/

        if (IsKeyDown(ControlsKeys[axis]))
            return speed * GetFrameTime() * factor;

        return 0.0f;
    }

    void Movement() {
        float direction[MOVE_DOWN + 1] = { GetSpeedForAxis(MOVE_FRONT,MoveSpeed.z),
                                           GetSpeedForAxis(MOVE_BACK,MoveSpeed.z),
                                           GetSpeedForAxis(MOVE_RIGHT,MoveSpeed.x),
                                           GetSpeedForAxis(MOVE_LEFT,MoveSpeed.x),
                                           GetSpeedForAxis(MOVE_UP,MoveSpeed.y),
                                           GetSpeedForAxis(MOVE_DOWN,MoveSpeed.y) };

        Vector3 forward = Vector3Subtract(_Player._Camera.ViewCamera.target, _Player._Camera.ViewCamera.position);
        /*** Don't ignore y for vector forward for perfectly fly ***/
        //forward.y = 0;
        forward = Vector3Normalize(forward);

        Vector3 right{ forward.z * -1.0f, 0, forward.x };

        _Player.position = Vector3Add(_Player.position, Vector3Scale(forward, direction[MOVE_FRONT] - direction[MOVE_BACK]));
        _Player.position = Vector3Add(_Player.position, Vector3Scale(right, direction[MOVE_RIGHT] - direction[MOVE_LEFT]));
        _Player.position.y += direction[MOVE_UP] - direction[MOVE_DOWN];
    }

    void BeginCamera3DMode(){
        BeginMode3D(_Player._Camera.ViewCamera);
    }

    void CameraAngle() {
        DisableCursor();

        // Mouse movement detection
        Vector2 mousePositionDelta = GetMouseDelta();
        float yFactor = InvertY ? -1.0f : 1.0f;

        _Player._Camera.Angle.x += (mousePositionDelta.x / MouseSensitivity);
        _Player._Camera.Angle.y += (yFactor * mousePositionDelta.y / MouseSensitivity);

        // Angle clamp
        if (_Player._Camera.Angle.y < MinimumViewY * DEG2RAD)
            _Player._Camera.Angle.y = MinimumViewY * DEG2RAD;
        else if (_Player._Camera.Angle.y > MaximumViewY * DEG2RAD)
            _Player._Camera.Angle.y = MaximumViewY * DEG2RAD;

        // Recalculate camera target considering translation and rotation
        Vector3 target = Vector3Transform(Vector3{ 0, 0, 1 }, MatrixRotateZYX(Vector3{_Player._Camera.Angle.y, -_Player._Camera.Angle.x, 0 }));

        _Player._Camera.ViewCamera.position = _Player.position;

        _Player._Camera.ViewCamera.target.x = _Player._Camera.ViewCamera.position.x + target.x;
        _Player._Camera.ViewCamera.target.y = _Player._Camera.ViewCamera.position.y + target.y;
        _Player._Camera.ViewCamera.target.z = _Player._Camera.ViewCamera.position.z + target.z;
    }

    void Update() {
        Movement();
        CameraAngle();
    }

    Vector3 GetPlayerCoords() {
        return _Player.position;
    }
}