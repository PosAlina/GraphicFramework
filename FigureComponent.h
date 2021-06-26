#pragma once
#include "GameComponent.h"
#include "inclib.h"
#include "Camera.h"

class Game;

class FigureComponent :
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

	//Textures
	ID3D11Texture2D* texture = nullptr;
	ID3D11ShaderResourceView* texSRV = nullptr;
	ID3D11SamplerState* sampler = nullptr;
	LPCWSTR textureName = L"";
	bool hasTexture = false;
	ID3D11Buffer* indeces = nullptr;
	int* ind = nullptr;
	int count_v = 0;
	int count_i = 0;
	//Light
	//ID3D11Buffer* lightBuffer = nullptr;
	bool onLight = true;

	Camera* camera = nullptr;
	SimpleMath::Vector4* points;

	void InitialColor();
	void InitializeTexture();
	void InitializeColorLight();
	void InitializeTextureLight();
public:
	SimpleMath::Vector3 Position;

	FigureComponent(bool light, int vcount, int icount, Game* inGame, Camera* inCamera, SimpleMath::Vector4* inPoints, LPCWSTR inTextureName, int* indces);
	FigureComponent(bool light, int vcount, int icount, Game* inGame, Camera* inCamera, SimpleMath::Vector4* inPoints, int* indeces);

	virtual void Initialize() override;
	virtual void DestroyResources() override;
	virtual void Draw(float deltaTime) override;
	virtual void Update(float deltaTime) override;
	virtual void Reload() override {}
};

