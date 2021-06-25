#include "TransformGame.h"
#include "TriangleComponent.h"
#include "EdgeComponent.h"
#include "BoxComponent.h"
#include "PlaneComponent.h"
#include "InputDevice.h"

PlaneComponent *plane;
//TriangleComponent *tri;
//EdgeComponent* edge;
BoxComponent* box;

void TransformGame::Initialize()
{
	cam = new Camera(this);
	camCont = new CameraController(this, cam);
	plane = new PlaneComponent(this, cam);
	//tri = new TriangleComponent(this, cam);
	//edge = new EdgeComponent(this, cam);
	box = new BoxComponent(this, cam);

	Components.emplace_back(plane);
	//Components.emplace_back(tri);
	//Components.emplace_back(edge);
	Components.emplace_back(box);
}

void TransformGame::Update(float deltaTime)
{
	camCont->Update(deltaTime);
	float velocity = 500.0f;
	if (InputDevice->IsKeyDown(Keys::Left))
	{
		box->movePositions(velocity * SimpleMath::Vector3::Left * deltaTime);
	}
	if (InputDevice->IsKeyDown(Keys::Right))
	{
		box->movePositions(velocity * SimpleMath::Vector3::Right * deltaTime);
	}
	if (InputDevice->IsKeyDown(Keys::Up))
	{
		box->movePositions(velocity * SimpleMath::Vector3::Up * deltaTime);
	}
	if (InputDevice->IsKeyDown(Keys::Down))
	{
		box->movePositions(velocity * SimpleMath::Vector3::Down * deltaTime);
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