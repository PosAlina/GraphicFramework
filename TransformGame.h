#pragma once
#include "Game.h"

class TransformGame :
    public Game
{
public:
    TransformGame(std::wstring WindowName):Game(WindowName) {}
    void Initialize() override;
    void Update(float deltaTime) override;
    void Draw(float deltaTime) override;
    void PostDraw(float deltaTime) override;
};

