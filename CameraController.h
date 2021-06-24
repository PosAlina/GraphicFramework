#pragma once
#include "inclib.h"
#include "InputDevice.h"
#include "Camera.h"

class Game;

class CameraController
{
	Game* game;
public:
	float Yaw;
	float Pitch;
	float VelocityMagnitude = 100.0f;
	float MouseSensitivity = 1.0f;
	Camera* Camera;
	SimpleMath::Vector3 CameraPosition;
	CameraController(Game* inGame, ::Camera* inCamera);
	void Update(float deltaTime);
protected:
	void OnMouseMove(const InputDevice::MouseMoveEventArgs& args);
};

