#include "PlaneComponent.h"
#include "inclib.h"
#include "Game.h"

PlaneComponent::PlaneComponent(Game* inGame, Camera* inCamera) :GameComponent(inGame)
{
	camera = inCamera;
	Position = SimpleMath::Vector3::Zero;
}

PlaneComponent::PlaneComponent(Game* inGame) : GameComponent(inGame)
{
	Position = SimpleMath::Vector3::Zero;
}

void PlaneComponent::Initialize()
{
}

void PlaneComponent::DestroyResources()
{
}

void PlaneComponent::Draw(float deltaTime)
{
}

void PlaneComponent::Update(float deltaTime)
{

}
