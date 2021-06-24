#pragma once
#include "DisplayWin32.h"

struct GameSpecification {
	DisplayWin32* Display = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Device> Device;
	ID3D11DeviceContext* Context = nullptr;

	void Initialize(DisplayWin32* Display, Microsoft::WRL::ComPtr<ID3D11Device> Device, ID3D11DeviceContext* Context)
	{
		this->Display = Display;
		this->Device = Device;
		this->Context = Context;
	}
};

class GameComponent
{
public:
	GameSpecification *game = nullptr;

	GameComponent() {}
	virtual int Initialize() = 0;	
	virtual void DestroyResources() = 0;
	virtual void Draw(float deltaTime) = 0;
	virtual void Reload() = 0;
	virtual void Update(float deltaTime) = 0;
};