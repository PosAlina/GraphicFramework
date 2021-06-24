#include "Camera.h"
#include "Game.h"

#define M_PI 3.14159265358979323846

Camera::Camera(Game* inGame) : game(inGame)
{
	ViewMatrix = SimpleMath::Matrix::Identity;
	UpdateProjectionMatrix();
}

Camera::~Camera()
{
}

SimpleMath::Matrix Camera::GetCameraMatrix() const
{
	return ViewMatrix * ProjMatrix;
}

SimpleMath::Vector3 Camera::GetPosition() const
{
	SimpleMath::Matrix inv;
	ViewMatrix.Invert(inv);
	return inv.Translation();
}

SimpleMath::Vector3 Camera::GetForwardVector() const
{
	SimpleMath::Matrix inv;
	ViewMatrix.Invert(inv);
	return inv.Forward();
}

void Camera::UpdateProjectionMatrix()
{
	ProjMatrix = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		(float)M_PI / 2.0f,
		(float)game->Display->ClientWidth / (float)game->Display->ClientHeight,
		0.1f,
		10000.0f);
}


#undef M_PI