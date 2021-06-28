#pragma once
#include "../include/inclib.h"

class Game;

class Camera
{
	Game* game;
public:
	SimpleMath::Matrix ViewMatrix;
	SimpleMath::Matrix ProjMatrix;

	Camera(Game* game);
	~Camera();

	SimpleMath::Matrix GetCameraMatrix() const;
	SimpleMath::Vector3 GetPosition() const;
	SimpleMath::Vector3 GetForwardVector() const;

	void UpdateProjectionMatrix();
};

