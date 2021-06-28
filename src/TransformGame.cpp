#include "../include/TransformGame.h"
#include "../include/TriangleComponent.h"
#include "../include/EdgeComponent.h"
#include "../include/BoxComponent.h"
#include "../include/PlaneComponent.h"
#include "../include/InputDevice.h"
#include "../include/FigureComponent.h"

PlaneComponent* plane;
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
FigureComponent* fig1;
FigureComponent* fig2;
FigureComponent* fig3;
FigureComponent* fig4;
FigureComponent* fig5;
FigureComponent* fig6;
FigureComponent* fig7;
FigureComponent* fig8;
FigureComponent* fig9;
FigureComponent* fig10;

bool light = true;

std::vector<BoxComponent*> boxes;
std::vector<FigureComponent*> figures;

void TransformGame::Initialize()
{
	cam = new Camera(this);
	camCont = new CameraController(this, cam);

	plane = new PlaneComponent(this, cam);

	//InitViaEdges();
	InitViaFigures();

	Components.emplace_back(plane);
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
		for (auto fig : figures)
		{
			fig->Position += (-velocity * SimpleMath::Vector3::Left * deltaTime);
		}
		//tri->Position += velocity * SimpleMath::Vector3::Left * deltaTime;
	}
	if (InputDevice->IsKeyDown(Keys::Right))
	{
		for (auto box : boxes)
		{
			box->movePositions(velocity * SimpleMath::Vector3::Right * deltaTime);
		}
		for (auto fig : figures)
		{
			fig->Position += (-velocity * SimpleMath::Vector3::Left * deltaTime);
		}
		//tri->Position += velocity * SimpleMath::Vector3::Right * deltaTime;
	}
	if (InputDevice->IsKeyDown(Keys::Up))
	{
		for (auto box : boxes)
		{
			box->movePositions(-velocity * SimpleMath::Vector3::Up * deltaTime);
		}
		for (auto fig : figures)
		{
			fig->Position += (-velocity * SimpleMath::Vector3::Left * deltaTime);
		}
		//tri->Position += velocity * SimpleMath::Vector3::Up * deltaTime;
	}
	if (InputDevice->IsKeyDown(Keys::Down))
	{
		for (auto box : boxes)
		{
			box->movePositions(velocity * SimpleMath::Vector3::Down * deltaTime);
		}
		for (auto fig : figures)
		{
			fig->Position += (-velocity * SimpleMath::Vector3::Left * deltaTime);
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

void TransformGame::InitViaEdges()
{
	//SimpleMath::Vector4* tripoints = new SimpleMath::Vector4[6]{
//	SimpleMath::Vector4(-4.0f, 8.0f, -2.0f, 1.0f), SimpleMath::Vector4(0.0f, 1.0f, 0.0f, 1.0f),
//	SimpleMath::Vector4(-4.0f, 15.0f, -4.0f, 1.0f), SimpleMath::Vector4(0.5f, 0.0f, 0.0f, 1.0f),
//	SimpleMath::Vector4(-2.0f, 8.0f, -4.0f, 1.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f),
//};
//tri = new TriangleComponent(this, cam, tripoints, L"texture/ground_Tex.png");
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
	box2 = new BoxComponent(light, this, cam, points, L"texture/pillow_Tex.jpg");
	boxes.emplace_back(box2);

	//points = new SimpleMath::Vector4[16]{
	//	SimpleMath::Vector4(-4.0f, 4.0f, -6.0f, 1.0f), SimpleMath::Vector4(0.0f, 1.0f, 0.0f, 1.0f),
	//	SimpleMath::Vector4(-4.0f, 4.0f, -9.0f, 1.0f), SimpleMath::Vector4(0.0f, 1.0f, 0.0f, 1.0f),
	//	SimpleMath::Vector4(-2.0f, 4.0f, -9.0f, 1.0f), SimpleMath::Vector4(0.0f, 1.0f, 0.0f, 1.0f),
	//	SimpleMath::Vector4(-2.0f, 4.0f, -6.0f, 1.0f), SimpleMath::Vector4(0.0f, 1.0f, 0.0f, 1.0f),

	//	SimpleMath::Vector4(-4.5f, 2.0f, -6.0f, 1.0f), SimpleMath::Vector4(0.0f, 1.0f, 0.0f, 1.0f),
	//	SimpleMath::Vector4(-4.5f, 2.0f, -9.0f, 1.0f), SimpleMath::Vector4(0.0f, 1.0f, 0.0f, 1.0f),
	//	SimpleMath::Vector4(-2.5f, 2.0f, -9.0f, 1.0f), SimpleMath::Vector4(0.0f, 1.0f, 0.0f, 1.0f),
	//	SimpleMath::Vector4(-2.5f, 2.0f, -6.0f, 1.0f), SimpleMath::Vector4(0.0f, 1.0f, 0.0f, 1.0f),
	//};

	points = new SimpleMath::Vector4[16]{
	SimpleMath::Vector4(-4.0f, 4.0f, -6.0f, 1.0f),
	SimpleMath::Vector4(-4.0f, 4.0f, -9.0f, 1.0f),
	SimpleMath::Vector4(-2.0f, 4.0f, -9.0f, 1.0f),
	SimpleMath::Vector4(-2.0f, 4.0f, -6.0f, 1.0f),
	SimpleMath::Vector4(-4.5f, 2.0f, -6.0f, 1.0f),
	SimpleMath::Vector4(-4.5f, 2.0f, -9.0f, 1.0f),
	SimpleMath::Vector4(-2.5f, 2.0f, -9.0f, 1.0f),
	SimpleMath::Vector4(-2.5f, 2.0f, -6.0f, 1.0f),
	};
	box3 = new BoxComponent(light, this, cam, points, L"texture/brick_Tex.png");
	boxes.emplace_back(box3);

	//points = new SimpleMath::Vector4[16]{
	//	SimpleMath::Vector4(-7.0f, 3.5f, -2.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 1.0f, 1.0f),
	//	SimpleMath::Vector4(-7.0f, 3.5f, -4.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 1.0f, 1.0f),
	//	SimpleMath::Vector4(-5.0f, 3.0f, -4.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 1.0f, 1.0f),
	//	SimpleMath::Vector4(-5.0f, 3.0f, -2.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 1.0f, 1.0f),

	//	SimpleMath::Vector4(-7.0f, 1.5f, -2.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 1.0f, 1.0f),
	//	SimpleMath::Vector4(-7.0f, 1.5f, -4.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 1.0f, 1.0f),
	//	SimpleMath::Vector4(-5.0f, 1.0f, -4.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 1.0f, 1.0f),
	//	SimpleMath::Vector4(-5.0f, 1.0f, -2.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 1.0f, 1.0f),
	//};

	points = new SimpleMath::Vector4[16]{
		SimpleMath::Vector4(-7.0f, 3.5f, -2.0f, 1.0f),
		SimpleMath::Vector4(-7.0f, 3.5f, -4.0f, 1.0f),
		SimpleMath::Vector4(-5.0f, 3.0f, -4.0f, 1.0f),
		SimpleMath::Vector4(-5.0f, 3.0f, -2.0f, 1.0f),
		SimpleMath::Vector4(-7.0f, 1.5f, -2.0f, 1.0f),
		SimpleMath::Vector4(-7.0f, 1.5f, -4.0f, 1.0f),
		SimpleMath::Vector4(-5.0f, 1.0f, -4.0f, 1.0f),
		SimpleMath::Vector4(-5.0f, 1.0f, -2.0f, 1.0f),
	};
	box4 = new BoxComponent(light, this, cam, points, L"texture/ground_Tex.png");
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
	box5 = new BoxComponent(light, this, cam, points, L"texture/camouflage_Tex.jpg");
	boxes.emplace_back(box5);

	//points = new SimpleMath::Vector4[16]{
	//	SimpleMath::Vector4(1.0f, 3.0f, -6.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.0f, 1.0f, 1.0f),
	//	SimpleMath::Vector4(0.0f, 3.0f, -7.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.0f, 1.0f, 1.0f),
	//	SimpleMath::Vector4(-1.0f, 3.0f, -6.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.0f, 1.0f, 1.0f),
	//	SimpleMath::Vector4(0.0f, 3.0f, -5.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.0f, 1.0f, 1.0f),

	//	SimpleMath::Vector4(1.0f, 1.0f, -6.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.0f, 1.0f, 1.0f),
	//	SimpleMath::Vector4(0.0f, 1.0f, -7.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.0f, 1.0f, 1.0f),
	//	SimpleMath::Vector4(-1.0f, 1.0f, -6.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.0f, 1.0f, 1.0f),
	//	SimpleMath::Vector4(0.0f, 1.0f, -5.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.0f, 1.0f, 1.0f),
	//};
	points = new SimpleMath::Vector4[16]{
		SimpleMath::Vector4(1.0f, 3.0f, -6.0f, 1.0f),
		SimpleMath::Vector4(0.0f, 3.0f, -7.0f, 1.0f),
		SimpleMath::Vector4(-1.0f, 3.0f, -6.0f, 1.0f),
		SimpleMath::Vector4(0.0f, 3.0f, -5.0f, 1.0f),
		SimpleMath::Vector4(1.0f, 1.0f, -6.0f, 1.0f),
		SimpleMath::Vector4(0.0f, 1.0f, -7.0f, 1.0f),
		SimpleMath::Vector4(-1.0f, 1.0f, -6.0f, 1.0f),
		SimpleMath::Vector4(0.0f, 1.0f, -5.0f, 1.0f),
	};
	box6 = new BoxComponent(light, this, cam, points, L"texture/camouflage_Tex.jpg");
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
	box7 = new BoxComponent(light, this, cam, points, L"texture/box_Tex.jpg");
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
	box8 = new BoxComponent(light, this, cam, points, L"texture/ground_Tex.png");
	boxes.emplace_back(box8);

	//points = new SimpleMath::Vector4[16]{
	//	SimpleMath::Vector4(4.0f, 5.0f, 1.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.5f, 0.3f, 1.0f),
	//	SimpleMath::Vector4(4.0f, 5.0f, -1.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.5f, 0.3f, 1.0f),
	//	SimpleMath::Vector4(5.0f, 5.0f, -1.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.5f, 0.3f, 1.0f),
	//	SimpleMath::Vector4(5.0f, 5.0f, 1.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.5f, 0.3f, 1.0f),

	//	SimpleMath::Vector4(4.0f, 2.0f, 1.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.5f, 0.3f, 1.0f),
	//	SimpleMath::Vector4(4.0f, 2.0f, -1.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.5f, 0.3f, 1.0f),
	//	SimpleMath::Vector4(5.0f, 2.0f, -1.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.5f, 0.3f, 1.0f),
	//	SimpleMath::Vector4(5.0f, 2.0f, 1.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.5f, 0.3f, 1.0f),
	//};
	points = new SimpleMath::Vector4[16]{
		SimpleMath::Vector4(4.0f, 5.0f, 1.0f, 1.0f),
		SimpleMath::Vector4(4.0f, 5.0f, -1.0f, 1.0f),
		SimpleMath::Vector4(5.0f, 5.0f, -1.0f, 1.0f),
		SimpleMath::Vector4(5.0f, 5.0f, 1.0f, 1.0f),
		SimpleMath::Vector4(4.0f, 2.0f, 1.0f, 1.0f),
		SimpleMath::Vector4(4.0f, 2.0f, -1.0f, 1.0f),
		SimpleMath::Vector4(5.0f, 2.0f, -1.0f, 1.0f),
		SimpleMath::Vector4(5.0f, 2.0f, 1.0f, 1.0f),
	};
	box9 = new BoxComponent(light, this, cam, points, L"texture/wood_Tex.jpg");
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
	box10 = new BoxComponent(light, this, cam, points, L"texture/wood_Tex.jpg");
	boxes.emplace_back(box10);

	for (auto box : boxes)
	{
		Components.emplace_back(box);
	}
}

void TransformGame::InitViaFigures()
{

	//box1 = new BoxComponent(light, this, cam);
	//boxes.emplace_back(box1);

	SimpleMath::Vector4* points = new SimpleMath::Vector4[16]{
		SimpleMath::Vector4(-1.0f, 3.0f, -2.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 1.0f),
		SimpleMath::Vector4(-1.0f, 3.0f, -4.0f, 1.0f), SimpleMath::Vector4(0.0f, 1.0f, 0.0f, 1.0f),
		SimpleMath::Vector4(1.0f, 3.0f, -4.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 1.0f, 1.0f),
		SimpleMath::Vector4(1.0f, 3.0f, -2.0f, 1.0f), SimpleMath::Vector4(0.5f, 0.5f, 0.0f, 1.0f),

		SimpleMath::Vector4(-1.0f, 1.0f, -2.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 1.0f),
		SimpleMath::Vector4(-1.0f, 1.0f, -4.0f, 1.0f), SimpleMath::Vector4(0.0f, 1.0f, 0.0f, 1.0f),
		SimpleMath::Vector4(1.0f, 1.0f, -4.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 1.0f, 1.0f),
		SimpleMath::Vector4(1.0f, 1.0f, -2.0f, 1.0f), SimpleMath::Vector4(0.5f, 0.5f, 0.0f, 1.0f),
	};
	int *index = new int[]
	{
			0, 2, 1, 3, 2, 0,
			4, 6, 5, 7, 6, 4,
			8, 10, 9, 11, 10, 8,
			12, 14, 13, 15, 14, 12,
			16, 18, 17, 19, 18, 16,
			20, 22, 21, 23, 22, 20,
			24, 26, 25, 27, 26, 24,
			28, 30, 29, 31, 30, 28,
			32, 34, 33, 35, 34, 32,
			36, 38, 37, 39, 38, 36,
			40, 42, 41, 43, 42, 40,
			44, 46, 45, 47, 46, 44,
	};
	inputPoints(points);
	fig1 = new FigureComponent(light, 48, 12, this, cam, fpoints, index);
	figures.emplace_back(fig1);
	/**/
	points = new SimpleMath::Vector4[8]{
		SimpleMath::Vector4(-4.0f, 3.0f, -2.0f, 1.0f),
		SimpleMath::Vector4(-4.0f, 3.0f, -4.0f, 1.0f),
		SimpleMath::Vector4(-2.0f, 3.0f, -4.0f, 1.0f),
		SimpleMath::Vector4(-2.0f, 3.0f, -2.0f, 1.0f),
		SimpleMath::Vector4(-4.0f, 1.0f, -2.0f, 1.0f),
		SimpleMath::Vector4(-4.0f, 1.0f, -4.0f, 1.0f),
		SimpleMath::Vector4(-2.0f, 1.0f, -4.0f, 1.0f),
		SimpleMath::Vector4(-2.0f, 1.0f, -2.0f, 1.0f),
	};

	inputTexPoints(points);
	fig2 = new FigureComponent(light, 48, 12, this, cam, fpoints, L"texture/pillow_Tex.jpg", index);
	figures.emplace_back(fig2);

	points = new SimpleMath::Vector4[16]{
	SimpleMath::Vector4(-4.0f, 4.0f, -6.0f, 1.0f),
	SimpleMath::Vector4(-4.0f, 4.0f, -9.0f, 1.0f),
	SimpleMath::Vector4(-2.0f, 4.0f, -9.0f, 1.0f),
	SimpleMath::Vector4(-2.0f, 4.0f, -6.0f, 1.0f),
	SimpleMath::Vector4(-4.5f, 2.0f, -6.0f, 1.0f),
	SimpleMath::Vector4(-4.5f, 2.0f, -9.0f, 1.0f),
	SimpleMath::Vector4(-2.5f, 2.0f, -9.0f, 1.0f),
	SimpleMath::Vector4(-2.5f, 2.0f, -6.0f, 1.0f),
	};
	inputTexPoints(points);
	fig3 = new FigureComponent(light, 48, 12, this, cam, fpoints, L"texture/brick_Tex.png", index);
	figures.emplace_back(fig3);

	points = new SimpleMath::Vector4[16]{
		SimpleMath::Vector4(-7.0f, 3.5f, -2.0f, 1.0f),
		SimpleMath::Vector4(-7.0f, 3.5f, -4.0f, 1.0f),
		SimpleMath::Vector4(-5.0f, 3.0f, -4.0f, 1.0f),
		SimpleMath::Vector4(-5.0f, 3.0f, -2.0f, 1.0f),
		SimpleMath::Vector4(-7.0f, 1.5f, -2.0f, 1.0f),
		SimpleMath::Vector4(-7.0f, 1.5f, -4.0f, 1.0f),
		SimpleMath::Vector4(-5.0f, 1.0f, -4.0f, 1.0f),
		SimpleMath::Vector4(-5.0f, 1.0f, -2.0f, 1.0f),
	};
	boxes.emplace_back(box4);
	inputTexPoints(points);
	fig4 = new FigureComponent(light, 48, 12, this, cam, fpoints, L"texture/ground_Tex.png", index);
	figures.emplace_back(fig4);

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
	inputTexPoints(points);
	fig5 = new FigureComponent(light, 48, 12, this, cam, fpoints, L"texture/camouflage_Tex.jpg", index);
	figures.emplace_back(fig5);

	points = new SimpleMath::Vector4[16]{
		SimpleMath::Vector4(1.0f, 3.0f, -6.0f, 1.0f),
		SimpleMath::Vector4(0.0f, 3.0f, -7.0f, 1.0f),
		SimpleMath::Vector4(-1.0f, 3.0f, -6.0f, 1.0f),
		SimpleMath::Vector4(0.0f, 3.0f, -5.0f, 1.0f),
		SimpleMath::Vector4(1.0f, 1.0f, -6.0f, 1.0f),
		SimpleMath::Vector4(0.0f, 1.0f, -7.0f, 1.0f),
		SimpleMath::Vector4(-1.0f, 1.0f, -6.0f, 1.0f),
		SimpleMath::Vector4(0.0f, 1.0f, -5.0f, 1.0f),
	};
	inputTexPoints(points);
	fig6 = new FigureComponent(light, 48, 12, this, cam, fpoints, L"texture/camouflage_Tex.jpg", index);
	figures.emplace_back(fig6);

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
	inputTexPoints(points);
	fig7 = new FigureComponent(light, 48, 12, this, cam, fpoints, L"texture/box_Tex.jpg", index);
	figures.emplace_back(fig7);

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
	inputTexPoints(points);
	fig8 = new FigureComponent(light, 48, 12, this, cam, fpoints, L"texture/ground_Tex.png", index);
	figures.emplace_back(fig8);

	points = new SimpleMath::Vector4[16]{
		SimpleMath::Vector4(4.0f, 5.0f, 1.0f, 1.0f),
		SimpleMath::Vector4(4.0f, 5.0f, -1.0f, 1.0f),
		SimpleMath::Vector4(5.0f, 5.0f, -1.0f, 1.0f),
		SimpleMath::Vector4(5.0f, 5.0f, 1.0f, 1.0f),
		SimpleMath::Vector4(4.0f, 2.0f, 1.0f, 1.0f),
		SimpleMath::Vector4(4.0f, 2.0f, -1.0f, 1.0f),
		SimpleMath::Vector4(5.0f, 2.0f, -1.0f, 1.0f),
		SimpleMath::Vector4(5.0f, 2.0f, 1.0f, 1.0f),
	};
	inputTexPoints(points);
	fig9 = new FigureComponent(light, 48, 12, this, cam, fpoints, L"texture/wood_Tex.jpg", index);
	figures.emplace_back(fig9);

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
	inputTexPoints(points);
	fig10 = new FigureComponent(light, 48, 12, this, cam, fpoints, L"texture/wood_Tex.jpg", index);
	figures.emplace_back(fig10);


	for (auto fig : figures)
	{
		Components.emplace_back(fig);
	}
}

void TransformGame::inputPoints(SimpleMath::Vector4* points)
{
	fpoints = new SimpleMath::Vector4[48];

	for (auto i = 0; i < 8; ++i)
	{
		fpoints[i] = points[i];
	}
	for (auto i = 8; i < 16; ++i)
	{
		fpoints[i] = points[i];
	}
	for (auto i = 0; i < 4; ++i)
	{
		fpoints[i+16] = points[i];
	}
	fpoints[4 + 16] = points[10];
	fpoints[5 + 16] = points[11];
	fpoints[6 + 16] = points[8];
	fpoints[7 + 16] = points[9];
	for (auto i = 2; i < 6; ++i)
	{
		fpoints[i - 2 + 24] = points[i];
	}
	fpoints[4 + 24] = points[12];
	fpoints[5 + 24] = points[13];
	fpoints[6 + 24] = points[10];
	fpoints[7 + 24] = points[11];
	for (auto i = 4; i < 8; ++i)
	{
		fpoints[i - 4 + 32] = points[i];
	}
	fpoints[4 + 32] = points[14];
	fpoints[5 + 32] = points[15];
	fpoints[6 + 32] = points[12];
	fpoints[7 + 32] = points[13];

	fpoints[0 + 40] = points[6];
	fpoints[1 + 40] = points[7];
	fpoints[2 + 40] = points[0];
	fpoints[3 + 40] = points[1];
	fpoints[4 + 40] = points[8];
	fpoints[5 + 40] = points[9];
	fpoints[6 + 40] = points[14];
	fpoints[7 + 40] = points[15];
}

void TransformGame::inputTexPoints(SimpleMath::Vector4* points)
{
	fpoints = new SimpleMath::Vector4[48];

	fpoints[0] = points[0];
	fpoints[1] = SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	fpoints[2] = points[1];
	fpoints[3] = SimpleMath::Vector4(0.0f, 1.0f, 0.0f, 0.0f);
	fpoints[4] = points[2];
	fpoints[5] = SimpleMath::Vector4(1.0f, 1.0f, 0.0f, 0.0f);
	fpoints[6] = points[3];
	fpoints[7] = SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 0.0f);

	fpoints[8] = points[4];
	fpoints[9] = SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	fpoints[10] = points[5];
	fpoints[11] = SimpleMath::Vector4(0.0f, 1.0f, 0.0f, 0.0f);
	fpoints[12] = points[6];
	fpoints[13] = SimpleMath::Vector4(1.0f, 1.0f, 0.0f, 0.0f);
	fpoints[14] = points[7];
	fpoints[15] = SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 0.0f);

	fpoints[16] = points[0];
	fpoints[17] = SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	fpoints[18] = points[1];
	fpoints[19] = SimpleMath::Vector4(0.0f, 1.0f, 0.0f, 0.0f);
	fpoints[20] = points[5];
	fpoints[21] = SimpleMath::Vector4(1.0f, 1.0f, 0.0f, 0.0f);
	fpoints[22] = points[4];
	fpoints[23] = SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 0.0f);
	fpoints[24] = points[1];
	fpoints[25] = SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	fpoints[26] = points[2];
	fpoints[27] = SimpleMath::Vector4(0.0f, 1.0f, 0.0f, 0.0f);
	fpoints[28] = points[6];
	fpoints[29] = SimpleMath::Vector4(1.0f, 1.0f, 0.0f, 0.0f);
	fpoints[30] = points[5];
	fpoints[31] = SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 0.0f);

	fpoints[32] = points[2];
	fpoints[33] = SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	fpoints[34] = points[3];
	fpoints[35] = SimpleMath::Vector4(0.0f, 1.0f, 0.0f, 0.0f);
	fpoints[36] = points[7];
	fpoints[37] = SimpleMath::Vector4(1.0f, 1.0f, 0.0f, 0.0f);
	fpoints[38] = points[6];
	fpoints[39] = SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 0.0f);

	fpoints[40] = points[3];
	fpoints[41] = SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	fpoints[42] = points[0];
	fpoints[43] = SimpleMath::Vector4(0.0f, 1.0f, 0.0f, 0.0f);
	fpoints[44] = points[4];
	fpoints[45] = SimpleMath::Vector4(1.0f, 1.0f, 0.0f, 0.0f);
	fpoints[46] = points[7];
	fpoints[47] = SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 0.0f);
}
