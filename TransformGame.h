#pragma once
#include "Game.h"
#include "InputDevice.h"
#include "CameraController.h"

class TransformGame :
    public Game
{
    void InitViaEdges();
public:
    Camera *cam = nullptr;
    CameraController *camCont = nullptr;

    TransformGame(std::wstring WindowName):Game(WindowName) {}
    virtual void Initialize() override;
    virtual void Update(float deltaTime) override;
    //virtual void Draw(float deltaTime) override;
    //virtual void PostDraw(float deltaTime) override;

    void OnMouseMove(InputDevice::MouseMoveEventArgs& args);
};
