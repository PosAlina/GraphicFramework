#include "CameraController.h"
#include "Game.h"

CameraController::CameraController(Game* inGame, ::Camera* inCamera)
{
	game = inGame;
	Camera = inCamera;

	Yaw = 0;
	Pitch = 0;
	CameraController& cont = *this;
	game->InputDevice->MouseMove.AddRaw(this, &CameraController::OnMouseMove);
	CameraPosition = SimpleMath::Vector3(0.0f, 2.0f, 1.0f);
}

void CameraController::Update(float deltaTime)
{
	auto input = game->InputDevice;
	auto rotMat = SimpleMath::Matrix::CreateFromYawPitchRoll(Yaw, Pitch, 0);
	auto velDirection = SimpleMath::Vector3::Zero;
	if (input->IsKeyDown(Keys::W)) velDirection += SimpleMath::Vector3(1.0f, 0.0f, 0.0f);
	if (input->IsKeyDown(Keys::S)) velDirection += SimpleMath::Vector3(-1.0f, 0.0f, 0.0f);
	if (input->IsKeyDown(Keys::A)) velDirection += SimpleMath::Vector3(0.0f, 0.0f, -1.0f);
	if (input->IsKeyDown(Keys::D)) velDirection += SimpleMath::Vector3(0.0f, 0.0f, 1.0f);

	if (input->IsKeyDown(Keys::Space)) velDirection += SimpleMath::Vector3(0.0f, 1.0f, 0.0f);
	if (input->IsKeyDown(Keys::C)) velDirection += SimpleMath::Vector3(0.0f, -1.0f, 0.0f);

	velDirection.Normalize();

	auto velDir = rotMat.Forward() * velDirection.x + SimpleMath::Vector3::Up * velDirection.y + rotMat.Right() * velDirection.z;
	if (velDir.Length() != 0)
	{
		velDir.Normalize();
	}
	CameraPosition = CameraPosition + velDir * VelocityMagnitude * deltaTime;

	Camera->ViewMatrix = SimpleMath::Matrix::CreateLookAt(CameraPosition, CameraPosition * rotMat.Forward(), rotMat.Up());
	Camera->UpdateProjectionMatrix();
}

void CameraController::OnMouseMove(const InputDevice::MouseMoveEventArgs& args)
{
	if (game->InputDevice->IsKeyDown(Keys::LeftShift)) return;
	Yaw -= args.Offset.x * 0.003f * MouseSensitivity;
	Pitch -= args.Offset.y * 0.003f * MouseSensitivity;

	if (args.WheelDelta == 0) return;

	if (args.WheelDelta > 0) VelocityMagnitude += 1;
	if (args.WheelDelta < 0) VelocityMagnitude -= 1;
}
