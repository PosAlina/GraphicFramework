#include "EdgeComponent.h"

void EdgeComponent::inputPoints(SimpleMath::Vector4* points)
{
	points1 = new SimpleMath::Vector4[6];
	points2 = new SimpleMath::Vector4[6];
	for (auto i = 0; i < 6; ++i)
	{
		points1[i] = points[i];
	}
	points2[0] = points[0];
	points2[1] = points[1];
	points2[2] = points[4];
	points2[3] = points[5];
	points2[4] = points[6];
	points2[5] = points[7];
}

EdgeComponent::EdgeComponent(bool light, Game* inGame, Camera* inCamera, SimpleMath::Vector4* points, LPCWSTR inTextureName) :GameComponent(inGame)
{
	inputPoints(points);

	tri1 = new TriangleComponent(light, inGame, inCamera, points1, inTextureName);
	tri2 = new TriangleComponent(light, inGame, inCamera, points2, inTextureName);
}

EdgeComponent::EdgeComponent(bool light, Game* inGame, Camera* inCamera, SimpleMath::Vector4* points) :GameComponent(inGame)
{
	inputPoints(points);

	tri1 = new TriangleComponent(light, inGame, inCamera, points1);
	tri2 = new TriangleComponent(light, inGame, inCamera, points2);
}

EdgeComponent::EdgeComponent(bool light, Game* inGame, Camera* inCamera) :GameComponent(inGame)
{
	SimpleMath::Vector4 *points = new SimpleMath::Vector4[8]{
		SimpleMath::Vector4(-1.0f, 1.0f, -1.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 1.0f),
		SimpleMath::Vector4(-1.0f, 1.0f, 1.0f, 1.0f), SimpleMath::Vector4(0.0f, 1.0f, 0.0f, 0.0f),
		SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 1.0f, 1.0f),
		SimpleMath::Vector4(1.0f, 1.0f, -1.0f, 1.0f), SimpleMath::Vector4(0.5f, 0.5f, 0.0f, 0.0f),
	};
	inputPoints(points);

	tri1 = new TriangleComponent(light, inGame, inCamera, points1);
	tri2 = new TriangleComponent(light, inGame, inCamera, points2);
}

void EdgeComponent::Initialize()
{
	if (tri1 != nullptr)
	{
		tri1->Initialize();
	}
	if (tri2 != nullptr)
	{
		tri2->Initialize();
	}
}

void EdgeComponent::DestroyResources()
{
	if (tri1 != nullptr)
	{
		tri1->DestroyResources();
		delete[] tri1;
	}
	if (tri2 != nullptr)
	{
		tri2->DestroyResources();
		delete[] tri2;
	}
}

void EdgeComponent::Draw(float deltaTime)
{
	if (tri1 != nullptr)
	{
		tri1->Draw(deltaTime);
	}
	if (tri2 != nullptr)
	{
		tri2->Draw(deltaTime);
	}
}

void EdgeComponent::Update(float deltaTime)
{
	if (tri1 != nullptr)
	{
		tri1->Update(deltaTime);
	}
	if (tri2 != nullptr)
	{
		tri2->Update(deltaTime);
	}
}

void EdgeComponent::movePositions(SimpleMath::Vector3 delta)
{
	tri1->Position += delta;
	tri2->Position += delta;
}
