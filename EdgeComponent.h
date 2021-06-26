#pragma once
#include "GameComponent.h"
#include "TriangleComponent.h"

class Game;

class EdgeComponent :
    public GameComponent
{
	void inputPoints(SimpleMath::Vector4* points);
public:
	SimpleMath::Vector4* points1;
	SimpleMath::Vector4* points2;
    TriangleComponent *tri1 = nullptr;
    TriangleComponent *tri2 = nullptr;

	EdgeComponent(bool light, Game* inGame, Camera* inCamera, SimpleMath::Vector4* points, LPCWSTR inTextureName);//8 points
	EdgeComponent(bool light, Game* inGame, Camera* inCamera, SimpleMath::Vector4* points);//8 points
	EdgeComponent(bool light, Game* inGame, Camera* inCamera);

	virtual void Initialize() override;
	virtual void DestroyResources() override;
	virtual void Draw(float deltaTime) override;
	virtual void Update(float deltaTime) override;
	virtual void Reload() override {}

	void movePositions(SimpleMath::Vector3 delta);
};

