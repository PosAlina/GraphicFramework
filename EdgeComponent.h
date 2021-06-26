#pragma once
#include "GameComponent.h"
#include "TriangleComponent.h"
#include "FigureComponent.h"

class Game;

class EdgeComponent :
    public GameComponent
{
	std::vector<int> ind;
	void inputPoints(SimpleMath::Vector4* points);
public:
	SimpleMath::Vector4* points1 = nullptr;
	SimpleMath::Vector4* points2 = nullptr;
	FigureComponent* figure = nullptr;
    TriangleComponent *tri1 = nullptr;
    TriangleComponent *tri2 = nullptr;
	int* index = new int[6];

	EdgeComponent(bool light, Game* inGame, Camera* inCamera, SimpleMath::Vector4* points, LPCWSTR inTextureName);//8 points
	EdgeComponent(bool light, Game* inGame, Camera* inCamera, SimpleMath::Vector4* points);//8 points
	EdgeComponent(bool light, Game* inGame, Camera* inCamera);
	~EdgeComponent();

	virtual void Initialize() override;
	virtual void DestroyResources() override;
	virtual void Draw(float deltaTime) override;
	virtual void Update(float deltaTime) override;
	virtual void Reload() override {}

	void movePositions(SimpleMath::Vector3 delta);
};

