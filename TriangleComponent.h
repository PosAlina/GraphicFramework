#pragma once
#include "inclib.h"
#include "GameComponent.h"

struct Points
{
	std::vector<SimpleMath::Vector4> points;
	std::vector<SimpleMath::Vector4> position;
	std::vector<SimpleMath::Vector4> color;
	ID3D11Buffer* buffer = nullptr; // vertex buffer
	void Initialize(std::vector<SimpleMath::Vector4> points)
	{
		for (auto i = 0; i < points.size(); ++i)
		{
			this->points.emplace_back(points[i]);
			(i % 2) ? color.emplace_back(points[i]) : position.emplace_back(points[i]);
		}
	}
	void Release()
	{
		buffer->Release();
	}
};

struct Indeces
{
	std::vector<int> indeces;
	ID3D11Buffer* buffer = nullptr;
	void Initialize(std::vector<int> indeces)
	{
		for (auto index : indeces)
		{
			this->indeces.emplace_back(index);
		}
	}
	void Release()
	{
		buffer->Release();
	}
};

class TriangleComponent : public GameComponent
{
	ID3D11InputLayout* layout = nullptr;

	ID3D11PixelShader *pixelShader = nullptr;
	ID3DBlob *pixelShaderByteCode = nullptr;

	ID3D11VertexShader *vertexShader = nullptr;
	ID3DBlob *vertexShaderByteCode = nullptr;

	Points points;
	Indeces indeces;

	ID3D11RasterizerState *rastState = nullptr;
public:
	TriangleComponent(DisplayWin32* Display, Microsoft::WRL::ComPtr<ID3D11Device> Device, ID3D11DeviceContext* Context);

	int Initialize() override;
	void DestroyResources() override;
	void Draw(float deltaTime) override;
	void Update(float deltaTime) override;
};
