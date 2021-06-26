#include "EdgeComponent.h"

bool fig_state = true;

void EdgeComponent::inputPoints(SimpleMath::Vector4* points)
{
	points1 = new SimpleMath::Vector4[6];
	points2 = new SimpleMath::Vector4[6];
	//for (auto i = 0; i < 6; ++i)
	//{
	//	points1[i] = points[i];
	//}
	points1[0] = points[0];
	points1[1] = points[1];
	points1[2] = points[4];
	points1[3] = points[5];
	points1[4] = points[2];
	points1[5] = points[3];

	points2[0] = points[0];
	points2[1] = points[1];
	points2[2] = points[4];
	points2[3] = points[5];
	points2[4] = points[6];
	points2[5] = points[7];

	//ind.push_back(2);
	//ind.push_back(3);
	//ind.push_back(0);
	//ind.push_back(1);
	//ind.push_back(4);
	//ind.push_back(5);
}

EdgeComponent::EdgeComponent(bool light, Game* inGame, Camera* inCamera, SimpleMath::Vector4* points, LPCWSTR inTextureName) :GameComponent(inGame)
{
	if (fig_state)
	{
		index = new int[]
		{
			0, 2, 1,
			3, 2, 0
		};
		figure = new FigureComponent(light, 4, 2, inGame, inCamera, points, inTextureName, index);
		return;
	}
	inputPoints(points);
	tri1 = new TriangleComponent(light, inGame, inCamera, points1, inTextureName);
	tri2 = new TriangleComponent(light, inGame, inCamera, points2, inTextureName);
}

EdgeComponent::EdgeComponent(bool light, Game* inGame, Camera* inCamera, SimpleMath::Vector4* points) :GameComponent(inGame)
{
	if (fig_state)
	{
		index = new int[]
		{
			0, 2, 1,
			3, 2, 0
		};
		figure = new FigureComponent(light, 4, 2, inGame, inCamera, points, index);
		return;
	}
	inputPoints(points);

	tri1 = new TriangleComponent(light, inGame, inCamera, points1);
	tri2 = new TriangleComponent(light, inGame, inCamera, points2);
}

EdgeComponent::EdgeComponent(bool light, Game* inGame, Camera* inCamera) :GameComponent(inGame)
{
	if (fig_state)
	{
		points1 = new SimpleMath::Vector4[8]{
			SimpleMath::Vector4(-1.0f, 1.0f, -1.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 1.0f),
			SimpleMath::Vector4(-1.0f, 1.0f, 1.0f, 1.0f), SimpleMath::Vector4(0.0f, 1.0f, 0.0f, 0.0f),
			SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 1.0f, 1.0f),
			SimpleMath::Vector4(1.0f, 1.0f, -1.0f, 1.0f), SimpleMath::Vector4(0.5f, 0.5f, 0.0f, 0.0f),
		};
		int* index = new int[]
		{
			0, 2, 1,
				3, 2, 0
		};
		figure = new FigureComponent(light, 4, 2, inGame, inCamera, points1, index);
		return;
	}
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
	if (fig_state) {
		figure->Initialize();
	}
	else {
		if (tri1 != nullptr)
		{
			tri1->Initialize();
		}
		if (tri2 != nullptr)
		{
			tri2->Initialize();
		}
	}

}

void EdgeComponent::DestroyResources()
{
	if (fig_state) {
		figure->DestroyResources();
		delete figure;
		delete[] index;
	}
	else {
		if (tri1 != nullptr)
		{
			tri1->DestroyResources();
			delete tri1;
		}
		if (tri2 != nullptr)
		{
			tri2->DestroyResources();
			delete tri2;
		}
	}
	if (points1 != nullptr)
	{
		delete[] points1;
	}
	if (points2 != nullptr)
	{
		delete[] points2;
	}
}

void EdgeComponent::Draw(float deltaTime)
{
	if (fig_state) {
		figure->Draw(deltaTime);
	}
	else {
		if (tri1 != nullptr)
		{
			tri1->Draw(deltaTime);
		}
		if (tri2 != nullptr)
		{
			tri2->Draw(deltaTime);
		}
	}

}

void EdgeComponent::Update(float deltaTime)
{
	if (fig_state) {
		figure->Update(deltaTime);
	}
	else {
		if (tri1 != nullptr)
		{
			tri1->Update(deltaTime);
		}
		if (tri2 != nullptr)
		{
			tri2->Update(deltaTime);
		}
	}
}

void EdgeComponent::movePositions(SimpleMath::Vector3 delta)
{
	if (fig_state) {
		figure->Position += delta;
	}
	else {
		tri1->Position += delta;
		tri2->Position += delta;
	}
}
