#include "BoxComponent.h"


void BoxComponent::inputPoints(SimpleMath::Vector4* points)
{
	points1 = new SimpleMath::Vector4[8];
	points2 = new SimpleMath::Vector4[8];
	points3 = new SimpleMath::Vector4[8];
	points4 = new SimpleMath::Vector4[8];
	points5 = new SimpleMath::Vector4[8];
	points6 = new SimpleMath::Vector4[8];

	for (auto i = 0; i < 8; ++i)
	{
		points1[i] = points[i];
	}
	for (auto i = 8; i < 16; ++i)
	{
		points2[i - 8] = points[i];
	}
	for (auto i = 0; i < 4; ++i)
	{
		points3[i] = points[i];
	}
	points3[4] = points[10];
	points3[5] = points[11];
	points3[6] = points[8];
	points3[7] = points[9];
	for (auto i = 2; i < 6; ++i)
	{
		points4[i - 2] = points[i];
	}
	points4[4] = points[12];
	points4[5] = points[13];
	points4[6] = points[10];
	points4[7] = points[11];
	for (auto i = 4; i < 8; ++i)
	{
		points5[i - 4] = points[i];
	}
	points5[4] = points[14];
	points5[5] = points[15];
	points5[6] = points[12];
	points5[7] = points[13];

	points6[0] = points[6];
	points6[1] = points[7];
	points6[2] = points[0];
	points6[3] = points[1];
	points6[4] = points[8];
	points6[5] = points[9];
	points6[6] = points[14];
	points6[7] = points[15];
}

BoxComponent::BoxComponent(Game* inGame, Camera* inCamera, SimpleMath::Vector4* points) :GameComponent(inGame)
{
	inputPoints(points);

	edge1 = new EdgeComponent(inGame, inCamera, points1);
	edge2 = new EdgeComponent(inGame, inCamera, points2);
	edge3 = new EdgeComponent(inGame, inCamera, points3);
	edge4 = new EdgeComponent(inGame, inCamera, points4);
	edge5 = new EdgeComponent(inGame, inCamera, points5);
	edge6 = new EdgeComponent(inGame, inCamera, points6);
}

BoxComponent::BoxComponent(Game* inGame, Camera* inCamera) :GameComponent(inGame)
{
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
	inputPoints(points);

	edge1 = new EdgeComponent(inGame, inCamera, points1);
	edge2 = new EdgeComponent(inGame, inCamera, points2);
	edge3 = new EdgeComponent(inGame, inCamera, points3);
	edge4 = new EdgeComponent(inGame, inCamera, points4);
	edge5 = new EdgeComponent(inGame, inCamera, points5);
	edge6 = new EdgeComponent(inGame, inCamera, points6);
}

void BoxComponent::Initialize()
{
	if (edge1 != nullptr)
	{
		edge1->Initialize();
	}
	if (edge2 != nullptr)
	{
		edge2->Initialize();
	}
	if (edge3 != nullptr)
	{
		edge3->Initialize();
	}
	if (edge4 != nullptr)
	{
		edge4->Initialize();
	}
	if (edge5 != nullptr)
	{
		edge5->Initialize();
	}
	if (edge6 != nullptr)
	{
		edge6->Initialize();
	}
}

void BoxComponent::DestroyResources()
{
	if (edge1 != nullptr)
	{
		edge1->DestroyResources();
		delete[] edge1;
	}
	if (edge2 != nullptr)
	{
		edge2->DestroyResources();
		delete[] edge1;
	}
	if (edge3 != nullptr)
	{
		edge3->DestroyResources();
		delete[] edge1;
	}
	if (edge4 != nullptr)
	{
		edge4->DestroyResources();
		delete[] edge1;
	}
	if (edge5 != nullptr)
	{
		edge5->DestroyResources();
		delete[] edge1;
	}
	if (edge6 != nullptr)
	{
		edge6->DestroyResources();
		delete[] edge1;
	}
}

void BoxComponent::Draw(float deltaTime)
{
	if (edge1 != nullptr)
	{
		edge1->Draw(deltaTime);
	}
	if (edge2 != nullptr)
	{
		edge2->Draw(deltaTime);
	}
	if (edge3 != nullptr)
	{
		edge3->Draw(deltaTime);
	}
	if (edge4 != nullptr)
	{
		edge4->Draw(deltaTime);
	}
	if (edge5 != nullptr)
	{
		edge5->Draw(deltaTime);
	}
	if (edge6 != nullptr)
	{
		edge6->Draw(deltaTime);
	}
}

void BoxComponent::Update(float deltaTime)
{
	if (edge1 != nullptr)
	{
		edge1->Update(deltaTime);
	}
	if (edge2 != nullptr)
	{
		edge2->Update(deltaTime);
	}
	if (edge3 != nullptr)
	{
		edge3->Update(deltaTime);
	}
	if (edge4 != nullptr)
	{
		edge4->Update(deltaTime);
	}
	if (edge5 != nullptr)
	{
		edge5->Update(deltaTime);
	}
	if (edge6 != nullptr)
	{
		edge6->Update(deltaTime);
	}
}

void BoxComponent::movePositions(SimpleMath::Vector3 delta)
{
	edge1->movePositions(delta);
	edge2->movePositions(delta);
	edge3->movePositions(delta);
	edge4->movePositions(delta);
	edge5->movePositions(delta);
	edge6->movePositions(delta);
}