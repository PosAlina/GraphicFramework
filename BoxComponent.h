#pragma once
#include "GameComponent.h"
#include "EdgeComponent.h"

class Game;

class BoxComponent :
    public GameComponent
{
	void inputPoints(SimpleMath::Vector4* points);
public:
	SimpleMath::Vector4* points1;
	SimpleMath::Vector4* points2;
	SimpleMath::Vector4* points3;
	SimpleMath::Vector4* points4;
	SimpleMath::Vector4* points5;
	SimpleMath::Vector4* points6;
	EdgeComponent* edge1 = nullptr;
	EdgeComponent* edge2 = nullptr;
	EdgeComponent* edge3 = nullptr;
	EdgeComponent* edge4 = nullptr;
	EdgeComponent* edge5 = nullptr;
	EdgeComponent* edge6 = nullptr;

	BoxComponent(bool light, Game* inGame, Camera* inCamera, SimpleMath::Vector4* points, LPCWSTR inTextureName);//8 points
	BoxComponent(bool light, Game* inGame, Camera* inCamera, SimpleMath::Vector4* points);//16 points
	BoxComponent(bool light, Game* inGame, Camera* inCamera);
	~BoxComponent();

	virtual void Initialize() override;
	virtual void DestroyResources() override;
	virtual void Draw(float deltaTime) override;
	virtual void Update(float deltaTime) override;
	virtual void Reload() override {}

	void movePositions(SimpleMath::Vector3 delta);
};


