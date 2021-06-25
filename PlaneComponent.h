#pragma once
#include "inclib.h"
#include "GameComponent.h"
#include "Camera.h"

class PlaneComponent :
    public GameComponent
{
	ID3D11InputLayout* layout = nullptr;

	ID3D11PixelShader* pixelShader = nullptr;
	ID3DBlob* pixelShaderByteCode = nullptr;

	ID3D11VertexShader* vertexShader = nullptr;
	ID3DBlob* vertexShaderByteCode = nullptr;

	ID3D11Buffer* vertices = nullptr;
	ID3D11Buffer* constantBuffer = nullptr;

	ID3D11RasterizerState* rastState = nullptr;
	ID3DUserDefinedAnnotation* annotation = nullptr;

	Camera* camera = nullptr;
	SimpleMath::Vector4 *points = nullptr;
public:
	SimpleMath::Vector3 Position;

	PlaneComponent(Game* inGame, Camera* inCamera);
	PlaneComponent(Game* inGame);

	virtual void Initialize() override;
	virtual void DestroyResources() override;
	virtual void Draw(float deltaTime) override;
	virtual void Update(float deltaTime) override;
	virtual void Reload() override {}
};
