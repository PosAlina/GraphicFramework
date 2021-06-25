#include "TransformGame.h"
#include "TriangleComponent.h"
#include "PlaneComponent.h"
#include "InputDevice.h"

PlaneComponent *plane;
TriangleComponent *tri;

void TransformGame::Initialize()
{
	cam = new Camera(this);
	camCont = new CameraController(this, cam);
	plane = new PlaneComponent(this, cam);
	tri = new TriangleComponent(this, cam);
	Components.emplace_back(plane);
	Components.emplace_back(tri);
}

void TransformGame::Update(float deltaTime)
{
	camCont->Update(deltaTime);
	float velocity = 500.0f;
	if (InputDevice->IsKeyDown(Keys::Left))
	{
		tri->Position += velocity * SimpleMath::Vector3::Left * deltaTime;
	}
	if (InputDevice->IsKeyDown(Keys::Right))
	{
		tri->Position += velocity * SimpleMath::Vector3::Right * deltaTime;
	}
	if (InputDevice->IsKeyDown(Keys::Up))
	{
		tri->Position += velocity * SimpleMath::Vector3::Up * deltaTime;
	}
	if (InputDevice->IsKeyDown(Keys::Down))
	{
		tri->Position += velocity * SimpleMath::Vector3::Down * deltaTime;
	}
	if (InputDevice->IsKeyDown(Keys::Escape))
	{
		Exit();
	}
	Game::Update(deltaTime);
}

void TransformGame::OnMouseMove(InputDevice::MouseMoveEventArgs& args)
{
	std::cout << args.Position.x << " " << args.Position.y << std::endl;
}

//void TransformGame::PostDraw(float deltaTime)
//{
//}
//
//void TransformGame::Draw(float deltaTime)
//{
//}