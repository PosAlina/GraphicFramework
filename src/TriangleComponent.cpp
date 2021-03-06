#include "../include/TriangleComponent.h"
#include "../include/inclib.h"
#include "../include/Game.h"
#include "../include/TextureLoader.h"

struct ConstantData_Simple
{
	SimpleMath::Matrix WorldViewProj;
};

//struct ConstantData_Light
//{
//	SimpleMath::Matrix WorldViewProj;
//	SimpleMath::Matrix World;
//	SimpleMath::Vector4 ViewerPos;
//};
//
//struct LightData
//{
//	SimpleMath::Vector4 Direction;
//	SimpleMath::Vector4 Color;
//	SimpleMath::Vector4 KaSpecPowKsX;
//};

struct ConstantLightData
{
	SimpleMath::Matrix WorldViewProj;
	SimpleMath::Matrix World;
	SimpleMath::Vector4 ViewerPos;

	SimpleMath::Vector4 Direction;
	SimpleMath::Vector4 Color;
	SimpleMath::Vector4 KaSpecPowKsX;
};

TriangleComponent::TriangleComponent(bool light, Game* inGame, Camera* inCamera, SimpleMath::Vector4* inPoints, LPCWSTR inTextureName) :GameComponent(inGame)
{
	camera = inCamera;
	Position = SimpleMath::Vector3::Zero;
	points = new SimpleMath::Vector4[6];
	for (auto i = 0; i < 6; ++i)
	{
		points[i] = inPoints[i];
	}
	textureName = inTextureName;
	hasTexture = true;
	onLight = light;
	ind = new int[3]{ 0, 1, 2 };
}

TriangleComponent::TriangleComponent(bool light, Game* inGame, Camera* inCamera, SimpleMath::Vector4* inPoints) :GameComponent(inGame)
{
	camera = inCamera;
	Position = SimpleMath::Vector3::Zero;
	points = new SimpleMath::Vector4[6];
	for (auto i = 0; i < 6; ++i)
	{
		points[i] = inPoints[i];
	}
	onLight = light;
	ind = new int[3]{ 0, 1, 2 };
}

TriangleComponent::TriangleComponent(bool light, Game* inGame, Camera* inCamera) :GameComponent(inGame)
{
	camera = inCamera;
	Position = SimpleMath::Vector3::Zero;
	points = new SimpleMath::Vector4[6]{
		SimpleMath::Vector4(0.0f, 1.0f, -1.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 1.0f),
		SimpleMath::Vector4(1.0f, 1.0f, 0.0f, 1.0f), SimpleMath::Vector4(0.0f, 1.0f, 0.0f, 0.0f),
		SimpleMath::Vector4(0.0f, 1.0f, -2.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 1.0f, 1.0f),
	};
	onLight = light;
	ind = new int[3]{ 0, 1, 2 };
}

TriangleComponent::~TriangleComponent()
{
	DestroyResources();
}

void TriangleComponent::Initialize()
{
	if (hasTexture)
	{
		onLight ? InitializeTextureLight() : InitializeTexture();
		return;
	}

	onLight ? InitializeColorLight() : InitialColor();
}

void TriangleComponent::DestroyResources()
{
	if (points != nullptr) delete[] points;

	if (layout != nullptr) layout->Release();
	if (pixelShader != nullptr) pixelShader->Release();
	if (vertexShader != nullptr) vertexShader->Release();
	if (pixelShaderByteCode != nullptr) pixelShaderByteCode->Release();
	if (vertexShaderByteCode != nullptr) vertexShaderByteCode->Release();
	
	
	if (vertices != nullptr) vertices->Release();
	if (indeces != nullptr) indeces->Release();
	if (rastState != nullptr) rastState->Release();

	//if (lightBuffer != nullptr) lightBuffer->Release();
	if (constantBuffer != nullptr) constantBuffer->Release();
	if (annotation != nullptr) annotation->Release();
	if (texture != nullptr) texture->Release();
	if (sampler != nullptr) sampler->Release();
	if (texSRV != nullptr) texSRV->Release();
}

void TriangleComponent::Draw(float deltaTime)
{
	auto context = game->Context;
	ID3D11RasterizerState* oldState;
	const UINT stride = 32;
	const UINT offset = 0;
	context->RSGetState(&oldState);
	context->RSSetState(rastState);
	context->IASetInputLayout(layout);
	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetVertexBuffers(0, 1, &vertices, &stride, &offset);
	context->IASetIndexBuffer(indeces, DXGI_FORMAT_R32_UINT, 0);
	context->VSSetShader(vertexShader, nullptr, 0);
	context->PSSetShader(pixelShader, nullptr, 0);
	context->VSSetConstantBuffers(0, 1, &constantBuffer);
	if (hasTexture)
	{
		context->PSSetShaderResources(0, 1, &texSRV);
		context->PSSetSamplers(0, 1, &sampler);
	}
	if (onLight)
	{
		context->PSSetConstantBuffers(0, 1, &constantBuffer);
	}

	annotation->BeginEvent(L"Triangle draw event");
	context->DrawIndexed(3, 0, 0);
	//context->Draw(3, 0);
	annotation->EndEvent();
	context->RSSetState(oldState);
	if (oldState != nullptr) oldState->Release();
}

void TriangleComponent::Update(float deltaTime)
{
	HRESULT resalt;

	if (onLight)
	{
		//if (constantBuffer != nullptr)
		//{
			auto constantData = ConstantLightData{};
	
			constantData.WorldViewProj = SimpleMath::Matrix::CreateTranslation(Position) * camera->ViewMatrix * camera->ProjMatrix;
			constantData.World = SimpleMath::Matrix::CreateTranslation(Position);
			auto cam_pos = camera->GetPosition();
			constantData.ViewerPos = SimpleMath::Vector4(cam_pos.x, cam_pos.y, cam_pos.z, 1.0f);

			constantData.Direction = SimpleMath::Vector4(1.0f, 30.0f, 0.0f, 1.0f);
			constantData.Color = SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
			constantData.KaSpecPowKsX = SimpleMath::Vector4(0.2f, 0.4f, 1.0f, 1.0f);

			//game->Context->UpdateSubresource(constantBuffer, 0, nullptr, &constantData, 0, 0);

			D3D11_MAPPED_SUBRESOURCE res1 = {};
			resalt = game->Context->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &res1); ZCHECK(resalt);
		//	if (res1.pData != nullptr)
		//	{
				auto dataP1 = reinterpret_cast<float*>(res1.pData);
				memcpy(dataP1, &constantData, sizeof(constantData));
				game->Context->Unmap(constantBuffer, 0);
		//	}
		/*}

		if (lightBuffer != nullptr)
		{
			auto lightData = LightData{};
			lightData.Direction = SimpleMath::Vector4(1.0f, 30.0f, 0.0f, 1.0f);
			lightData.Color = SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
			lightData.KaSpecPowKsX = SimpleMath::Vector4(0.5f, 1.0f, 1.0f, 1.0f);

			game->Context->UpdateSubresource(lightBuffer, 0, nullptr, &lightData, 0, 0);

			D3D11_MAPPED_SUBRESOURCE res2 = {};
			resalt = game->Context->Map(lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &res2);
			if (FAILED(resalt))
			{
				std::cout << L"Const buffer dont update" << std::endl;
			}
			if (res2.pData != nullptr)
			{
				auto dataP2 = reinterpret_cast<float*>(res2.pData);
				memcpy(dataP2, &lightData, sizeof(lightData));
				game->Context->Unmap(lightBuffer, 0);
			}
		}*/
	}
	else
	{
		//if (constantBuffer != nullptr)
		//{
			auto constantData = ConstantData_Simple{};
			constantData.WorldViewProj = SimpleMath::Matrix::CreateTranslation(Position) * camera->ViewMatrix * camera->ProjMatrix;

			//game->Context->UpdateSubresource(constantBuffer, 0, nullptr, &constantData, 0, 0);

			D3D11_MAPPED_SUBRESOURCE res = {};
			resalt = game->Context->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &res); ZCHECK(resalt);
			//if (res.pData != nullptr)
			//{
				auto dataP = reinterpret_cast<float*>(res.pData);
				memcpy(dataP, &constantData, sizeof(constantData));
				game->Context->Unmap(constantBuffer, 0);
			//}

		//}
	}
}

void TriangleComponent::InitialColor() {
	HRESULT res;

#pragma region Initialize shaders wwith color
	ID3DBlob* errorVertexCode;
	res = D3DCompileFromFile(L"shader/Simple.hlsl",
		nullptr /*macros*/,
		nullptr /*include*/,
		"VSMain",
		"vs_5_0",
		D3DCOMPILE_PACK_MATRIX_ROW_MAJOR,
		0,
		&vertexShaderByteCode,
		&errorVertexCode);

	if (FAILED(res))
	{
		if (errorVertexCode)
		{
			char* compileErrors = (char*)(errorVertexCode->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
		else
		{
			MessageBox(game->Display->hWnd, L"shader/Simple.hlsl", L"Missing Shader File", MB_OK);
		}
	}
	res = game->Device->CreateVertexShader(
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		nullptr, &vertexShader); ZCHECK(res);

	ID3DBlob* errorPixelCode;
	res = D3DCompileFromFile(L"shader/Simple.hlsl",
		nullptr /*macros*/,
		nullptr /*include*/,
		"PSMain",
		"ps_5_0",
		D3DCOMPILE_PACK_MATRIX_ROW_MAJOR,
		0,
		&pixelShaderByteCode,
		&errorPixelCode);

	if (FAILED(res))
	{
		if (errorPixelCode)
		{
			char* compileErrors = (char*)(errorPixelCode->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
		else
		{
			MessageBox(game->Display->hWnd, L"shader/Simple.hlsl", L"Missing Shader File", MB_OK);
		}
	}
	res = game->Device->CreatePixelShader(
		pixelShaderByteCode->GetBufferPointer(),
		pixelShaderByteCode->GetBufferSize(),
		nullptr, &pixelShader); ZCHECK(res);
#pragma endregion Initialize shaders

#pragma region Initialize layout
	D3D11_INPUT_ELEMENT_DESC inputElements[] = {
		D3D11_INPUT_ELEMENT_DESC {
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0},
		D3D11_INPUT_ELEMENT_DESC {
			"COLOR",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0}
	};
	res = game->Device->CreateInputLayout(
		inputElements,
		2,
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		&layout); ZCHECK(res);
#pragma endregion Initialize layout

#pragma region Initialize bufferss
	D3D11_BUFFER_DESC bufDesc = {};
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	bufDesc.CPUAccessFlags = 0;
	bufDesc.MiscFlags = 0;
	bufDesc.StructureByteStride = 32;
	bufDesc.ByteWidth = sizeof(SimpleMath::Vector4) * 6;

	D3D11_SUBRESOURCE_DATA positionsData = {};
	positionsData.pSysMem = points;
	positionsData.SysMemPitch = 0;
	positionsData.SysMemSlicePitch = 0;

	res = game->Device->CreateBuffer(&bufDesc, &positionsData, &vertices); ZCHECK(res);

	D3D11_BUFFER_DESC indDesc = {};
	indDesc.Usage = D3D11_USAGE_DEFAULT;
	indDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indDesc.CPUAccessFlags = 0;
	indDesc.MiscFlags = 0;
	indDesc.StructureByteStride = 32;
	indDesc.ByteWidth = sizeof(sizeof(int)) * 3;

	D3D11_SUBRESOURCE_DATA indData = {};
	indData.pSysMem = ind;
	indData.SysMemPitch = 0;
	indData.SysMemSlicePitch = 0;

	res = game->Device->CreateBuffer(&indDesc, &indData, &indeces); ZCHECK(res);

	D3D11_BUFFER_DESC constBufDesc = {};
	constBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	constBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufDesc.MiscFlags = 0;
	constBufDesc.StructureByteStride = 0;
	constBufDesc.ByteWidth = sizeof(ConstantData_Simple);

	res = game->Device->CreateBuffer(&constBufDesc, nullptr, &constantBuffer); ZCHECK(res);
#pragma endregion Initialize buffers

#pragma region Initialize rasterization state
	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	res = game->Device->CreateRasterizerState(&rastDesc, &rastState); ZCHECK(res);
	res = game->Context->QueryInterface(IID_ID3DUserDefinedAnnotation, (void**)&annotation); ZCHECK(res);
#pragma endregion Initialize rasterization state
}

void TriangleComponent::InitializeTexture()
{
	HRESULT res;

#pragma region Initialize shaders
	ID3DBlob* errorVertexCode;
	res = D3DCompileFromFile(L"shader/Texture.hlsl",
		nullptr,
		nullptr,
		"VSMain",
		"vs_5_0",
		D3DCOMPILE_PACK_MATRIX_ROW_MAJOR,
		0,
		&vertexShaderByteCode,
		&errorVertexCode);

	if (FAILED(res))
	{
		if (errorVertexCode)
		{
			char* compileErrors = (char*)(errorVertexCode->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
		else
		{
			MessageBox(game->Display->hWnd, L"Texturer.hlsl", L"Missing Shader File", MB_OK);
		}
	}
	res = game->Device->CreateVertexShader(
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		nullptr, &vertexShader); ZCHECK(res);

	ID3DBlob* errorPixelCode;
	res = D3DCompileFromFile(L"shader/Texture.hlsl",
		nullptr,
		nullptr,
		"PSMain",
		"ps_5_0",
		D3DCOMPILE_PACK_MATRIX_ROW_MAJOR,
		0,
		&pixelShaderByteCode,
		&errorPixelCode);

	if (FAILED(res))
	{
		if (errorPixelCode)
		{
			char* compileErrors = (char*)(errorPixelCode->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
		else
		{
			MessageBox(game->Display->hWnd, L"shader/Texture.hlsl", L"Missing Shader File", MB_OK);
		}
	}
	res = game->Device->CreatePixelShader(
		pixelShaderByteCode->GetBufferPointer(),
		pixelShaderByteCode->GetBufferSize(),
		nullptr, &pixelShader); ZCHECK(res);
#pragma endregion Initialize shaders

#pragma region Initialize layout
	D3D11_INPUT_ELEMENT_DESC inputElements[] = {
		D3D11_INPUT_ELEMENT_DESC {
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0},
		D3D11_INPUT_ELEMENT_DESC {
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0}
	};
	res = game->Device->CreateInputLayout(
		inputElements,
		2,
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		&layout); ZCHECK(res);
#pragma endregion Initialize layout

#pragma region Initialize bufferss
	D3D11_BUFFER_DESC bufDesc = {};
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	bufDesc.CPUAccessFlags = 0;
	bufDesc.MiscFlags = 0;
	bufDesc.StructureByteStride = 32;
	bufDesc.ByteWidth = sizeof(SimpleMath::Vector4) * 6;

	D3D11_SUBRESOURCE_DATA positionsData = {};
	positionsData.pSysMem = points;
	positionsData.SysMemPitch = 0;
	positionsData.SysMemSlicePitch = 0;

	res = game->Device->CreateBuffer(&bufDesc, &positionsData, &vertices); ZCHECK(res);


	D3D11_BUFFER_DESC indDesc = {};
	indDesc.Usage = D3D11_USAGE_DEFAULT;
	indDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indDesc.CPUAccessFlags = 0;
	indDesc.MiscFlags = 0;
	indDesc.StructureByteStride = 32;
	indDesc.ByteWidth = sizeof(sizeof(int)) * 3;

	D3D11_SUBRESOURCE_DATA indData = {};
	indData.pSysMem = ind;
	indData.SysMemPitch = 0;
	indData.SysMemSlicePitch = 0;

	res = game->Device->CreateBuffer(&indDesc, &indData, &indeces); ZCHECK(res);

	D3D11_BUFFER_DESC constBufDesc = {};
	constBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	constBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufDesc.MiscFlags = 0;
	constBufDesc.StructureByteStride = 0;
	constBufDesc.ByteWidth = sizeof(ConstantData_Simple);

	res = game->Device->CreateBuffer(&constBufDesc, nullptr, &constantBuffer); ZCHECK(res);
#pragma endregion Initialize buffers

#pragma region Initialize rasterization state
	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	res = game->Device->CreateRasterizerState(&rastDesc, &rastState); ZCHECK(res);
	res = game->Context->QueryInterface(IID_ID3DUserDefinedAnnotation, (void**)&annotation); ZCHECK(res);
#pragma endregion Initialize rasterization state

#pragma region Load texture
	res = game->TextureLoader->LoadTextureFromFile(textureName, texture, texSRV, true, false); ZCHECK(res);

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 1.0f;
	samplerDesc.BorderColor[1] = 0.0f;
	samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 1.0f;
	samplerDesc.MaxLOD = INT_MAX;

	res = game->Device->CreateSamplerState(&samplerDesc, &sampler); ZCHECK(res);

#pragma endregion Load texture
}

void TriangleComponent::InitializeTextureLight()
{
	HRESULT res;

#pragma region Initialize shaders
	ID3DBlob* errorVertexCode;
	res = D3DCompileFromFile(L"shader/LightShaderTexture.hlsl",
		nullptr,
		nullptr,
		"VSMain",
		"vs_5_0",
		D3DCOMPILE_PACK_MATRIX_ROW_MAJOR,
		0,
		&vertexShaderByteCode,
		&errorVertexCode);

	if (FAILED(res))
	{
		if (errorVertexCode)
		{
			char* compileErrors = (char*)(errorVertexCode->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
		else
		{
			MessageBox(game->Display->hWnd, L"shader/LightShaderTexture.hlsl", L"Missing Shader File", MB_OK);
		}
	}
	res = game->Device->CreateVertexShader(
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		nullptr, &vertexShader); ZCHECK(res);

	ID3DBlob* errorPixelCode;
	res = D3DCompileFromFile(L"shader/LightShaderTexture.hlsl",
		nullptr,
		nullptr,
		"PSMain",
		"ps_5_0",
		D3DCOMPILE_PACK_MATRIX_ROW_MAJOR,
		0,
		&pixelShaderByteCode,
		&errorPixelCode);

	if (FAILED(res))
	{
		if (errorPixelCode)
		{
			char* compileErrors = (char*)(errorPixelCode->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
		else
		{
			MessageBox(game->Display->hWnd, L"shader/LightShaderTexture.hlsl", L"Missing Shader File", MB_OK);
		}
	}
	res = game->Device->CreatePixelShader(
		pixelShaderByteCode->GetBufferPointer(),
		pixelShaderByteCode->GetBufferSize(),
		nullptr, &pixelShader); ZCHECK(res);
#pragma endregion Initialize shaders

#pragma region Initialize layout
	D3D11_INPUT_ELEMENT_DESC inputElements[] = {
		D3D11_INPUT_ELEMENT_DESC {
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0},

		D3D11_INPUT_ELEMENT_DESC {
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0},
		D3D11_INPUT_ELEMENT_DESC {
			"NORMAL",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0}
	};
	res = game->Device->CreateInputLayout(
		inputElements,
		3,
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		&layout); ZCHECK(res);
#pragma endregion Initialize layout

#pragma region Initialize bufferss
	D3D11_BUFFER_DESC bufDesc = {};
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	bufDesc.CPUAccessFlags = 0;
	bufDesc.MiscFlags = 0;
	bufDesc.StructureByteStride = 32;
	bufDesc.ByteWidth = sizeof(SimpleMath::Vector4) * 6;

	D3D11_SUBRESOURCE_DATA positionsData = {};
	positionsData.pSysMem = points;
	positionsData.SysMemPitch = 0;
	positionsData.SysMemSlicePitch = 0;

	res = game->Device->CreateBuffer(&bufDesc, &positionsData, &vertices); ZCHECK(res);


	D3D11_BUFFER_DESC indDesc = {};
	indDesc.Usage = D3D11_USAGE_DEFAULT;
	indDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indDesc.CPUAccessFlags = 0;
	indDesc.MiscFlags = 0;
	indDesc.StructureByteStride = 32;
	indDesc.ByteWidth = sizeof(sizeof(int)) * 3;

	D3D11_SUBRESOURCE_DATA indData = {};
	indData.pSysMem = ind;
	indData.SysMemPitch = 0;
	indData.SysMemSlicePitch = 0;

	res = game->Device->CreateBuffer(&indDesc, &indData, &indeces); ZCHECK(res);
	//D3D11_BUFFER_DESC lightBufDesc = {};
	//lightBufDesc.Usage = D3D11_USAGE_DEFAULT;
	//lightBufDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	//lightBufDesc.CPUAccessFlags = 0;// D3D11_CPU_ACCESS_WRITE;
	//lightBufDesc.MiscFlags = 0;
	//lightBufDesc.StructureByteStride = 0;
	//lightBufDesc.ByteWidth = sizeof(LightData);

	//res = game->Device->CreateBuffer(&lightBufDesc, nullptr, &lightBuffer);
	//if (FAILED(res))
	//{
	//	std::cout << L"Light buffer dont initialize" << std::endl;
	//}

	D3D11_BUFFER_DESC constBufDesc = {};
	constBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	constBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufDesc.MiscFlags = 0;
	constBufDesc.StructureByteStride = 0;
	constBufDesc.ByteWidth = sizeof(ConstantLightData);

	res = game->Device->CreateBuffer(&constBufDesc, nullptr, &constantBuffer); ZCHECK(res);
#pragma endregion Initialize buffers

#pragma region Initialize rasterization state
	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	res = game->Device->CreateRasterizerState(&rastDesc, &rastState); ZCHECK(res);
	res = game->Context->QueryInterface(IID_ID3DUserDefinedAnnotation, (void**)&annotation); ZCHECK(res);
#pragma endregion Initialize rasterization state

#pragma region Load texture
	res = game->TextureLoader->LoadTextureFromFile(textureName, texture, texSRV, true, false); ZCHECK(res);

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 1.0f;
	samplerDesc.BorderColor[1] = 0.0f;
	samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 1.0f;
	samplerDesc.MaxLOD = INT_MAX;

	res = game->Device->CreateSamplerState(&samplerDesc, &sampler); ZCHECK(res);
#pragma endregion Load texture
}


void TriangleComponent::InitializeColorLight() {
	HRESULT res;

#pragma region Initialize shaders wwith color
	ID3DBlob* errorVertexCode;
	res = D3DCompileFromFile(L"shader/LightShaderSimple.hlsl",
		nullptr /*macros*/,
		nullptr /*include*/,
		"VSMain",
		"vs_5_0",
		D3DCOMPILE_PACK_MATRIX_ROW_MAJOR,
		0,
		&vertexShaderByteCode,
		&errorVertexCode);

	if (FAILED(res))
	{
		if (errorVertexCode)
		{
			char* compileErrors = (char*)(errorVertexCode->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
		else
		{
			MessageBox(game->Display->hWnd, L"shader/LightShaderSimple.hlsl", L"Missing Shader File", MB_OK);
		}
	}
	res = game->Device->CreateVertexShader(
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		nullptr, &vertexShader); ZCHECK(res);

	ID3DBlob* errorPixelCode;
	res = D3DCompileFromFile(L"shader/LightShaderSimple.hlsl",
		nullptr /*macros*/,
		nullptr /*include*/,
		"PSMain",
		"ps_5_0",
		D3DCOMPILE_PACK_MATRIX_ROW_MAJOR,
		0,
		&pixelShaderByteCode,
		&errorPixelCode);

	if (FAILED(res))
	{
		if (errorPixelCode)
		{
			char* compileErrors = (char*)(errorPixelCode->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
		else
		{
			MessageBox(game->Display->hWnd, L"shader/LightShaderSimple.hlsl", L"Missing Shader File", MB_OK);
		}
	}
	res = game->Device->CreatePixelShader(
		pixelShaderByteCode->GetBufferPointer(),
		pixelShaderByteCode->GetBufferSize(),
		nullptr, &pixelShader); ZCHECK(res);
#pragma endregion Initialize shaders

#pragma region Initialize layout
	D3D11_INPUT_ELEMENT_DESC inputElements[] = {
		D3D11_INPUT_ELEMENT_DESC {
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0},
		D3D11_INPUT_ELEMENT_DESC {
			"NORMAL",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0},
		D3D11_INPUT_ELEMENT_DESC {
			"COLOR",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0}
	};
	res = game->Device->CreateInputLayout(
		inputElements,
		3,
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		&layout); ZCHECK(res);
#pragma endregion Initialize layout

#pragma region Initialize bufferss
	D3D11_BUFFER_DESC bufDesc = {};
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	bufDesc.CPUAccessFlags = 0;
	bufDesc.MiscFlags = 0;
	bufDesc.StructureByteStride = 32;
	bufDesc.ByteWidth = sizeof(SimpleMath::Vector4) * 6;

	D3D11_SUBRESOURCE_DATA positionsData = {};
	positionsData.pSysMem = points;
	positionsData.SysMemPitch = 0;
	positionsData.SysMemSlicePitch = 0;

	res = game->Device->CreateBuffer(&bufDesc, &positionsData, &vertices); ZCHECK(res);


	D3D11_BUFFER_DESC indDesc = {};
	indDesc.Usage = D3D11_USAGE_DEFAULT;
	indDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indDesc.CPUAccessFlags = 0;
	indDesc.MiscFlags = 0;
	indDesc.StructureByteStride = 32;
	indDesc.ByteWidth = sizeof(sizeof(int)) * 3;

	D3D11_SUBRESOURCE_DATA indData = {};
	indData.pSysMem = ind;
	indData.SysMemPitch = 0;
	indData.SysMemSlicePitch = 0;

	res = game->Device->CreateBuffer(&indDesc, &indData, &indeces); ZCHECK(res);

	D3D11_BUFFER_DESC constBufDesc = {};
	constBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	constBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufDesc.MiscFlags = 0;
	constBufDesc.StructureByteStride = 0;
	constBufDesc.ByteWidth = sizeof(ConstantLightData);

	res = game->Device->CreateBuffer(&constBufDesc, nullptr, &constantBuffer); ZCHECK(res);
#pragma endregion Initialize buffers

#pragma region Initialize rasterization state
	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	res = game->Device->CreateRasterizerState(&rastDesc, &rastState); ZCHECK(res);
	res = game->Context->QueryInterface(IID_ID3DUserDefinedAnnotation, (void**)&annotation); ZCHECK(res);
#pragma endregion Initialize rasterization state
}
