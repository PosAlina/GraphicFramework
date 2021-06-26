#include "TransformGame.h"
#include "TriangleComponent.h"
#include "EdgeComponent.h"
#include "BoxComponent.h"
#include "PlaneComponent.h"
#include "InputDevice.h"

PlaneComponent *plane;
//TriangleComponent *tri;
//EdgeComponent* edge;
BoxComponent* box1;
BoxComponent* box2;
BoxComponent* box3;
BoxComponent* box4;
BoxComponent* box5;
BoxComponent* box6;
BoxComponent* box7;
BoxComponent* box8;
BoxComponent* box9;
BoxComponent* box10;

bool light = true;

std::vector<BoxComponent*> boxes;

void TransformGame::Initialize()
{
	cam = new Camera(this);
	camCont = new CameraController(this, cam);

	plane = new PlaneComponent(this, cam);

	//SimpleMath::Vector4* tripoints = new SimpleMath::Vector4[6]{
	//	SimpleMath::Vector4(-4.0f, 8.0f, -2.0f, 1.0f), SimpleMath::Vector4(0.0f, 1.0f, 0.0f, 1.0f),
	//	SimpleMath::Vector4(-4.0f, 15.0f, -4.0f, 1.0f), SimpleMath::Vector4(0.5f, 0.0f, 0.0f, 1.0f),
	//	SimpleMath::Vector4(-2.0f, 8.0f, -4.0f, 1.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	//};
	//tri = new TriangleComponent(this, cam, tripoints, L"ground_Tex.png");
	//Components.emplace_back(tri);
	//edge = new EdgeComponent(this, cam);
	box1 = new BoxComponent(light, this, cam);
	boxes.emplace_back(box1);

	//SimpleMath::Vector4* points = new SimpleMath::Vector4[16]{
	//	SimpleMath::Vector4(-4.0f, 3.0f, -2.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 1.0f),
	//	SimpleMath::Vector4(-4.0f, 3.0f, -4.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 1.0f),
	//	SimpleMath::Vector4(-2.0f, 3.0f, -4.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 1.0f),
	//	SimpleMath::Vector4(-2.0f, 3.0f, -2.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 1.0f),

	//	SimpleMath::Vector4(-4.0f, 1.0f, -2.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 1.0f),
	//	SimpleMath::Vector4(-4.0f, 1.0f, -4.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 1.0f),
	//	SimpleMath::Vector4(-2.0f, 1.0f, -4.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 1.0f),
	//	SimpleMath::Vector4(-2.0f, 1.0f, -2.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 1.0f),
	//};
	SimpleMath::Vector4* points = new SimpleMath::Vector4[8]{
		SimpleMath::Vector4(-4.0f, 3.0f, -2.0f, 1.0f),
		SimpleMath::Vector4(-4.0f, 3.0f, -4.0f, 1.0f),
		SimpleMath::Vector4(-2.0f, 3.0f, -4.0f, 1.0f),
		SimpleMath::Vector4(-2.0f, 3.0f, -2.0f, 1.0f),
		SimpleMath::Vector4(-4.0f, 1.0f, -2.0f, 1.0f),
		SimpleMath::Vector4(-4.0f, 1.0f, -4.0f, 1.0f),
		SimpleMath::Vector4(-2.0f, 1.0f, -4.0f, 1.0f),
		SimpleMath::Vector4(-2.0f, 1.0f, -2.0f, 1.0f),
	};
	box2 = new BoxComponent(light, this, cam, points, L"pillow_Tex.jpg");
	boxes.emplace_back(box2);

	points = new SimpleMath::Vector4[16]{
		SimpleMath::Vector4(-4.0f, 4.0f, -6.0f, 1.0f), SimpleMath::Vector4(0.0f, 1.0f, 0.0f, 1.0f),
		SimpleMath::Vector4(-4.0f, 4.0f, -9.0f, 1.0f), SimpleMath::Vector4(0.0f, 1.0f, 0.0f, 1.0f),
		SimpleMath::Vector4(-2.0f, 4.0f, -9.0f, 1.0f), SimpleMath::Vector4(0.0f, 1.0f, 0.0f, 1.0f),
		SimpleMath::Vector4(-2.0f, 4.0f, -6.0f, 1.0f), SimpleMath::Vector4(0.0f, 1.0f, 0.0f, 1.0f),

		SimpleMath::Vector4(-4.5f, 2.0f, -6.0f, 1.0f), SimpleMath::Vector4(0.0f, 1.0f, 0.0f, 1.0f),
		SimpleMath::Vector4(-4.5f, 2.0f, -9.0f, 1.0f), SimpleMath::Vector4(0.0f, 1.0f, 0.0f, 1.0f),
		SimpleMath::Vector4(-2.5f, 2.0f, -9.0f, 1.0f), SimpleMath::Vector4(0.0f, 1.0f, 0.0f, 1.0f),
		SimpleMath::Vector4(-2.5f, 2.0f, -6.0f, 1.0f), SimpleMath::Vector4(0.0f, 1.0f, 0.0f, 1.0f),
	};
	box3 = new BoxComponent(light, this, cam, points);
	boxes.emplace_back(box3);

	points = new SimpleMath::Vector4[16]{
		SimpleMath::Vector4(-7.0f, 3.5f, -2.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 1.0f, 1.0f),
		SimpleMath::Vector4(-7.0f, 3.5f, -4.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 1.0f, 1.0f),
		SimpleMath::Vector4(-5.0f, 3.0f, -4.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 1.0f, 1.0f),
		SimpleMath::Vector4(-5.0f, 3.0f, -2.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 1.0f, 1.0f),

		SimpleMath::Vector4(-7.0f, 1.5f, -2.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 1.0f, 1.0f),
		SimpleMath::Vector4(-7.0f, 1.5f, -4.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 1.0f, 1.0f),
		SimpleMath::Vector4(-5.0f, 1.0f, -4.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 1.0f, 1.0f),
		SimpleMath::Vector4(-5.0f, 1.0f, -2.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 1.0f, 1.0f),
	};
	box4 = new BoxComponent(light, this, cam, points);// , L"ground_Tex.png");
	boxes.emplace_back(box4);

	//points = new SimpleMath::Vector4[16]{
	//	SimpleMath::Vector4(-7.0f, 5.0f, -5.0f, 1.0f), SimpleMath::Vector4(1.0f, 1.0f, 0.0f, 1.0f),
	//	SimpleMath::Vector4(-7.0f, 5.0f, -7.0f, 1.0f), SimpleMath::Vector4(1.0f, 1.0f, 0.0f, 1.0f),
	//	SimpleMath::Vector4(-5.0f, 5.0f, -7.0f, 1.0f), SimpleMath::Vector4(1.0f, 1.0f, 0.0f, 1.0f),
	//	SimpleMath::Vector4(-5.0f, 5.0f, -5.0f, 1.0f), SimpleMath::Vector4(1.0f, 1.0f, 0.0f, 1.0f),

	//	SimpleMath::Vector4(-7.0f, 2.0f, -5.0f, 1.0f), SimpleMath::Vector4(1.0f, 1.0f, 0.0f, 1.0f),
	//	SimpleMath::Vector4(-7.0f, 2.0f, -7.0f, 1.0f), SimpleMath::Vector4(1.0f, 1.0f, 0.0f, 1.0f),
	//	SimpleMath::Vector4(-5.0f, 2.0f, -7.0f, 1.0f), SimpleMath::Vector4(1.0f, 1.0f, 0.0f, 1.0f),
	//	SimpleMath::Vector4(-5.0f, 2.0f, -5.0f, 1.0f), SimpleMath::Vector4(1.0f, 1.0f, 0.0f, 1.0f),
	//};
	points = new SimpleMath::Vector4[16]{
		SimpleMath::Vector4(-7.0f, 5.0f, -5.0f, 1.0f),
		SimpleMath::Vector4(-7.0f, 5.0f, -7.0f, 1.0f),
		SimpleMath::Vector4(-5.0f, 5.0f, -7.0f, 1.0f),
		SimpleMath::Vector4(-5.0f, 5.0f, -5.0f, 1.0f),
		SimpleMath::Vector4(-7.0f, 3.0f, -5.0f, 1.0f),
		SimpleMath::Vector4(-7.0f, 3.0f, -7.0f, 1.0f),
		SimpleMath::Vector4(-5.0f, 3.0f, -7.0f, 1.0f),
		SimpleMath::Vector4(-5.0f, 3.0f, -5.0f, 1.0f),
	};
	box5 = new BoxComponent(light, this, cam, points, L"camouflage_Tex.jpg");
	boxes.emplace_back(box5);

	points = new SimpleMath::Vector4[16]{
		SimpleMath::Vector4(1.0f, 3.0f, -6.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.0f, 1.0f, 1.0f),
		SimpleMath::Vector4(0.0f, 3.0f, -7.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.0f, 1.0f, 1.0f),
		SimpleMath::Vector4(-1.0f, 3.0f, -6.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.0f, 1.0f, 1.0f),
		SimpleMath::Vector4(0.0f, 3.0f, -5.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.0f, 1.0f, 1.0f),

		SimpleMath::Vector4(1.0f, 1.0f, -6.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.0f, 1.0f, 1.0f),
		SimpleMath::Vector4(0.0f, 1.0f, -7.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.0f, 1.0f, 1.0f),
		SimpleMath::Vector4(-1.0f, 1.0f, -6.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.0f, 1.0f, 1.0f),
		SimpleMath::Vector4(0.0f, 1.0f, -5.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.0f, 1.0f, 1.0f),
	};
	box6 = new BoxComponent(light, this, cam, points);
	boxes.emplace_back(box6);

	//points = new SimpleMath::Vector4[16]{
	//	SimpleMath::Vector4(-2.0f, 3.0f, 2.0f, 1.0f), SimpleMath::Vector4(0.5f, 0.1f, 0.5f, 1.0f),
	//	SimpleMath::Vector4(-2.0f, 3.0f, 4.0f, 1.0f), SimpleMath::Vector4(0.5f, 0.1f, 0.5f, 1.0f),
	//	SimpleMath::Vector4(-4.0f, 3.0f, 4.0f, 1.0f), SimpleMath::Vector4(0.5f, 0.1f, 0.5f, 1.0f),
	//	SimpleMath::Vector4(-4.0f, 3.0f, 2.0f, 1.0f), SimpleMath::Vector4(0.5f, 0.1f, 0.5f, 1.0f),

	//	SimpleMath::Vector4(-2.0f, 1.0f, 2.0f, 1.0f), SimpleMath::Vector4(0.5f, 0.1f, 0.5f, 1.0f),
	//	SimpleMath::Vector4(-2.0f, 1.0f, 4.0f, 1.0f), SimpleMath::Vector4(0.5f, 0.1f, 0.5f, 1.0f),
	//	SimpleMath::Vector4(-4.0f, 1.0f, 4.0f, 1.0f), SimpleMath::Vector4(0.5f, 0.1f, 0.5f, 1.0f),
	//	SimpleMath::Vector4(-4.0f, 1.0f, 2.0f, 1.0f), SimpleMath::Vector4(0.5f, 0.1f, 0.5f, 1.0f),
	//};
	points = new SimpleMath::Vector4[8]{
		SimpleMath::Vector4(-2.0f, 3.0f, 2.0f, 1.0f),
		SimpleMath::Vector4(-2.0f, 3.0f, 4.0f, 1.0f),
		SimpleMath::Vector4(-4.0f, 3.0f, 4.0f, 1.0f),
		SimpleMath::Vector4(-4.0f, 3.0f, 2.0f, 1.0f),
		SimpleMath::Vector4(-2.0f, 1.0f, 2.0f, 1.0f),
		SimpleMath::Vector4(-2.0f, 1.0f, 4.0f, 1.0f),
		SimpleMath::Vector4(-4.0f, 1.0f, 4.0f, 1.0f),
		SimpleMath::Vector4(-4.0f, 1.0f, 2.0f, 1.0f),
	};
	box7 = new BoxComponent(light, this, cam, points, L"box_Tex.jpg");
	boxes.emplace_back(box7);

	//points = new SimpleMath::Vector4[16]{
	//	SimpleMath::Vector4(1.0f, 3.0f, 4.0f, 1.0f), SimpleMath::Vector4(0.1f, 0.5f, 0.5f, 1.0f),
	//	SimpleMath::Vector4(1.0f, 3.0f, 6.0f, 1.0f), SimpleMath::Vector4(0.1f, 0.5f, 0.5f, 1.0f),
	//	SimpleMath::Vector4(-1.0f, 3.0f, 6.0f, 1.0f), SimpleMath::Vector4(0.1f, 0.5f, 0.5f, 1.0f),
	//	SimpleMath::Vector4(-1.0f, 3.0f, 4.0f, 1.0f), SimpleMath::Vector4(0.1f, 0.5f, 0.5f, 1.0f),

	//	SimpleMath::Vector4(1.0f, 1.0f, 4.0f, 1.0f), SimpleMath::Vector4(0.1f, 0.5f, 0.5f, 1.0f),
	//	SimpleMath::Vector4(1.0f, 1.0f, 6.0f, 1.0f), SimpleMath::Vector4(0.1f, 0.5f, 0.5f, 1.0f),
	//	SimpleMath::Vector4(-1.0f, 1.0f, 6.0f, 1.0f), SimpleMath::Vector4(0.1f, 0.5f, 0.5f, 1.0f),
	//	SimpleMath::Vector4(-1.0f, 1.0f, 4.0f, 1.0f), SimpleMath::Vector4(0.1f, 0.5f, 0.5f, 1.0f),
	//};
	points = new SimpleMath::Vector4[8]{
		SimpleMath::Vector4(1.0f, 3.0f, 4.0f, 1.0f),
		SimpleMath::Vector4(1.0f, 3.0f, 6.0f, 1.0f),
		SimpleMath::Vector4(-1.0f, 3.0f, 6.0f, 1.0f),
		SimpleMath::Vector4(-1.0f, 3.0f, 4.0f, 1.0f),

		SimpleMath::Vector4(1.0f, 1.0f, 4.0f, 1.0f),
		SimpleMath::Vector4(1.0f, 1.0f, 6.0f, 1.0f),
		SimpleMath::Vector4(-1.0f, 1.0f, 6.0f, 1.0f),
		SimpleMath::Vector4(-1.0f, 1.0f, 4.0f, 1.0f),
	};
	box8 = new BoxComponent(light, this, cam, points, L"ground_Tex.png");
	boxes.emplace_back(box8);

	points = new SimpleMath::Vector4[16]{
		SimpleMath::Vector4(4.0f, 5.0f, 1.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.5f, 0.3f, 1.0f),
		SimpleMath::Vector4(4.0f, 5.0f, -1.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.5f, 0.3f, 1.0f),
		SimpleMath::Vector4(5.0f, 5.0f, -1.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.5f, 0.3f, 1.0f),
		SimpleMath::Vector4(5.0f, 5.0f, 1.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.5f, 0.3f, 1.0f),

		SimpleMath::Vector4(4.0f, 2.0f, 1.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.5f, 0.3f, 1.0f),
		SimpleMath::Vector4(4.0f, 2.0f, -1.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.5f, 0.3f, 1.0f),
		SimpleMath::Vector4(5.0f, 2.0f, -1.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.5f, 0.3f, 1.0f),
		SimpleMath::Vector4(5.0f, 2.0f, 1.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.5f, 0.3f, 1.0f),
	};
	box9 = new BoxComponent(light, this, cam, points);
	boxes.emplace_back(box9);

	//points = new SimpleMath::Vector4[16]{
	//	SimpleMath::Vector4(4.0f, 3.0f, 4.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.5f, 0.3f, 1.0f),
	//	SimpleMath::Vector4(4.0f, 3.0f, 2.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.5f, 0.3f, 1.0f),
	//	SimpleMath::Vector4(2.0f, 3.0f, 2.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.5f, 0.3f, 1.0f),
	//	SimpleMath::Vector4(2.0f, 3.0f, 4.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.5f, 0.3f, 1.0f),

	//	SimpleMath::Vector4(4.5f, 1.0f, 4.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.5f, 0.3f, 1.0f),
	//	SimpleMath::Vector4(4.5f, 1.0f, 2.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.5f, 0.3f, 1.0f),
	//	SimpleMath::Vector4(2.5f, 1.0f, 2.f, 1.0f), SimpleMath::Vector4(1.0f, 0.5f, 0.3f, 1.0f),
	//	SimpleMath::Vector4(2.5f, 1.0f, 4.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.5f, 0.3f, 1.0f),
	//};
	points = new SimpleMath::Vector4[8]{
		SimpleMath::Vector4(4.0f, 3.0f, 4.0f, 1.0f),
		SimpleMath::Vector4(4.0f, 3.0f, 2.0f, 1.0f),
		SimpleMath::Vector4(2.0f, 3.0f, 2.0f, 1.0f),
		SimpleMath::Vector4(2.0f, 3.0f, 4.0f, 1.0f),

		SimpleMath::Vector4(4.5f, 1.0f, 4.0f, 1.0f),
		SimpleMath::Vector4(4.5f, 1.0f, 2.0f, 1.0f),
		SimpleMath::Vector4(2.5f, 1.0f, 2.f, 1.0f),
		SimpleMath::Vector4(2.5f, 1.0f, 4.0f, 1.0f),
	};
	box10 = new BoxComponent(light, this, cam, points, L"wood_Tex.jpg");
	boxes.emplace_back(box10);

	Components.emplace_back(plane);
	//Components.emplace_back(tri);
	//Components.emplace_back(edge);
	//Components.emplace_back(box);
	for (auto box : boxes)
	{
		Components.emplace_back(box);
	}
}

void TransformGame::Update(float deltaTime)
{
	camCont->Update(deltaTime);
	float velocity = 500.0f;
	if (InputDevice->IsKeyDown(Keys::Left))
	{
		for (auto box : boxes)
		{
			box->movePositions(-velocity * SimpleMath::Vector3::Left * deltaTime);
		}
		//tri->Position += velocity * SimpleMath::Vector3::Left * deltaTime;
	}
	if (InputDevice->IsKeyDown(Keys::Right))
	{
		for (auto box : boxes)
		{
			box->movePositions(velocity * SimpleMath::Vector3::Right * deltaTime);
		}
		//tri->Position += velocity * SimpleMath::Vector3::Right * deltaTime;
	}
	if (InputDevice->IsKeyDown(Keys::Up))
	{
		for (auto box : boxes)
		{
			box->movePositions(-velocity * SimpleMath::Vector3::Up * deltaTime);
		}
		//tri->Position += velocity * SimpleMath::Vector3::Up * deltaTime;
	}
	if (InputDevice->IsKeyDown(Keys::Down))
	{
		for (auto box : boxes)
		{
			box->movePositions(velocity * SimpleMath::Vector3::Down * deltaTime);
		}
		//tri->Position += velocity * SimpleMath::Vector3::Down * deltaTime;
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