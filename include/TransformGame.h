#pragma once
#include "Game.h"
#include "InputDevice.h"
#include "CameraController.h"

class TransformGame :
    public Game
{
    void InitViaEdges();
    void InitViaFigures();
    void inputPoints(SimpleMath::Vector4* points);
    void inputTexPoints(SimpleMath::Vector4* points);
public:
    Camera *cam = nullptr;
    CameraController *camCont = nullptr;
    SimpleMath::Vector4* fpoints;

    TransformGame(std::wstring WindowName):Game(WindowName) {}
    virtual void Initialize() override;
    virtual void Update(float deltaTime) override;
    //virtual void Draw(float deltaTime) override;
    //virtual void PostDraw(float deltaTime) override;

    void OnMouseMove(InputDevice::MouseMoveEventArgs& args);
};
