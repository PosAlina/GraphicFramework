#include "TriangleComponent.h"
#include "inclib.h"
#include "Game.h"
#include "TextureLoader.h"

TriangleComponent::TriangleComponent(Game* inGame, Camera* inCamera, SimpleMath::Vector4* inPoints, LPCWSTR inTextureName) :GameComponent(inGame)
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
}

TriangleComponent::TriangleComponent(Game* inGame, Camera* inCamera, SimpleMath::Vector4 *inPoints) :GameComponent(inGame)
{
	camera = inCamera;
	Position = SimpleMath::Vector3::Zero;
	points = new SimpleMath::Vector4[6];
	for (auto i = 0; i < 6; ++i)
	{
		points[i] = inPoints[i];
	}
}

TriangleComponent::TriangleComponent(Game *inGame, Camera *inCamera):GameComponent(inGame)
{
	camera = inCamera;
	Position = SimpleMath::Vector3::Zero;
	points = new SimpleMath::Vector4[6]{
		SimpleMath::Vector4(0.0f, 1.0f, -1.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 1.0f),
		SimpleMath::Vector4(1.0f, 1.0f, 0.0f, 1.0f), SimpleMath::Vector4(0.0f, 1.0f, 0.0f, 0.0f),
		SimpleMath::Vector4(0.0f, 1.0f, -2.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 1.0f, 1.0f),
	};
}

void TriangleComponent::Initialize()
{
	if (hasTexture)
	{
		InitializeTexture();
		return;
	}

	HRESULT res;

#pragma region Initialize shaders wwith color
	ID3DBlob* errorVertexCode;
	res = D3DCompileFromFile(L"Simple.hlsl",
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
			MessageBox(game->Display->hWnd, L"Simple.hlsl", L"Missing Shader File", MB_OK);
		}
	}
	res = game->Device->CreateVertexShader(
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		nullptr, &vertexShader);
	if (FAILED(res))
	{
		std::cout << L"Vertex shader dont compile" << std::endl;
	}

	ID3DBlob* errorPixelCode;
	res = D3DCompileFromFile(L"Simple.hlsl",
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
			MessageBox(game->Display->hWnd, L"Simple.hlsl", L"Missing Shader File", MB_OK);
		}
	}
	res = game->Device->CreatePixelShader(
		pixelShaderByteCode->GetBufferPointer(),
		pixelShaderByteCode->GetBufferSize(),
		nullptr, &pixelShader);
	if (FAILED(res))
	{
		std::cout << L"Pixel shader dont compile" << std::endl;
	}
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
		&layout);
	if (FAILED(res))
	{
		std::cout << L"Layout dont compile" << std::endl;
	}
#pragma endregion Initialize layout

#pragma region Initialize bufferss
	D3D11_BUFFER_DESC bufDesc = {};
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufDesc.CPUAccessFlags = 0;
	bufDesc.MiscFlags = 0;
	bufDesc.StructureByteStride = 32;
	bufDesc.ByteWidth = sizeof(SimpleMath::Vector4) * 6;

	D3D11_SUBRESOURCE_DATA positionsData = {};
	positionsData.pSysMem = points;
	positionsData.SysMemPitch = 0;
	positionsData.SysMemSlicePitch = 0;

	res = game->Device->CreateBuffer(&bufDesc, &positionsData, &vertices);
	if (FAILED(res))
	{
		std::cout << L"Points buffer dont initialize" << std::endl;
	}

	D3D11_BUFFER_DESC constBufDesc = {};
	constBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufDesc.MiscFlags = 0;
	constBufDesc.StructureByteStride = 0;
	constBufDesc.ByteWidth = sizeof(SimpleMath::Matrix);

	res = game->Device->CreateBuffer(&constBufDesc, nullptr, &constantBuffer);
	if (FAILED(res))
	{
		std::cout << L"Const buffer dont initialize" << std::endl;
	}
#pragma endregion Initialize buffers

#pragma region Initialize rasterization state
	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	res = game->Device->CreateRasterizerState(&rastDesc, &rastState);
	if (FAILED(res))
	{
		std::cout << L"Rast state dont initialize" << std::endl;
	}
	res = game->Context->QueryInterface(IID_ID3DUserDefinedAnnotation, (void**)&annotation);
	if (FAILED(res))
	{
		std::cout << L"Annotation dont initialize" << std::endl;
	}
#pragma endregion Initialize rasterization state
}

void TriangleComponent::DestroyResources()
{
	delete[] points;

	if (layout != nullptr) layout->Release();
	if (pixelShader != nullptr) pixelShader->Release();
	if (vertexShader != nullptr) vertexShader->Release();
	if (pixelShaderByteCode != nullptr) pixelShaderByteCode->Release();
	if (vertexShaderByteCode != nullptr) vertexShaderByteCode->Release();
	if (vertices != nullptr) vertices->Release();
	if (rastState != nullptr) rastState->Release();

	if (constantBuffer != nullptr) constantBuffer->Release();
	if (annotation != nullptr) annotation->Release();

	if (texture != nullptr) texture->Release();
	if (sampler != nullptr) sampler->Release();
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
	context->VSSetShader(vertexShader, nullptr, 0);
	context->PSSetShader(pixelShader, nullptr, 0);
	context->VSSetConstantBuffers(0, 1, &constantBuffer);
	if (hasTexture)
	{
		context->PSSetShaderResources(0, 1, &texSRV);
		context->PSSetSamplers(0, 1, &sampler);
	}

	annotation->BeginEvent(L"Triangle draw event");
	context->Draw(3, 0);
	annotation->EndEvent();
	context->RSSetState(oldState);
	if (oldState != nullptr) oldState->Release();
}

void TriangleComponent::Update(float deltaTime)
{
	auto wvp = SimpleMath::Matrix::CreateTranslation(Position) * camera->ViewMatrix * camera->ProjMatrix;
	//game->Context->UpdataSubresource(constantBuffer, 0, nullptr, &wvp, 0, 0);
	D3D11_MAPPED_SUBRESOURCE res = {};
	game->Context->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &res);
	auto dataP = reinterpret_cast<float*>(res.pData);
	memcpy(dataP, &wvp, sizeof(SimpleMath::Matrix));
	game->Context->Unmap(constantBuffer, 0);
}

void TriangleComponent::InitializeTexture()
{
	HRESULT res;

#pragma region Initialize shaders
	ID3DBlob* errorVertexCode;
	res = D3DCompileFromFile(L"ObjModelShader.hlsl",
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
			MessageBox(game->Display->hWnd, L"ObjModelShader.hlsl", L"Missing Shader File", MB_OK);
		}
	}
	res = game->Device->CreateVertexShader(
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		nullptr, &vertexShader);
	if (FAILED(res))
	{
		std::cout << L"Vertex shader dont compile" << std::endl;
	}

	ID3DBlob* errorPixelCode;
	res = D3DCompileFromFile(L"ObjModelShader.hlsl",
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
			MessageBox(game->Display->hWnd, L"ObjModelShade.hlsl", L"Missing Shader File", MB_OK);
		}
	}
	res = game->Device->CreatePixelShader(
		pixelShaderByteCode->GetBufferPointer(),
		pixelShaderByteCode->GetBufferSize(),
		nullptr, &pixelShader);
	if (FAILED(res))
	{
		std::cout << L"Pixel shader dont compile" << std::endl;
	}
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
		&layout);
	if (FAILED(res))
	{
		std::cout << L"Layout dont compile" << std::endl;
	}
#pragma endregion Initialize layout

#pragma region Initialize bufferss
	D3D11_BUFFER_DESC bufDesc = {};
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufDesc.CPUAccessFlags = 0;
	bufDesc.MiscFlags = 0;
	bufDesc.StructureByteStride = 32;
	bufDesc.ByteWidth = sizeof(SimpleMath::Vector4) * 6;

	D3D11_SUBRESOURCE_DATA positionsData = {};
	positionsData.pSysMem = points;
	positionsData.SysMemPitch = 0;
	positionsData.SysMemSlicePitch = 0;

	res = game->Device->CreateBuffer(&bufDesc, &positionsData, &vertices);
	if (FAILED(res))
	{
		std::cout << L"Points buffer dont initialize" << std::endl;
	}

	D3D11_BUFFER_DESC constBufDesc = {};
	constBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufDesc.MiscFlags = 0;
	constBufDesc.StructureByteStride = 0;
	constBufDesc.ByteWidth = sizeof(SimpleMath::Matrix);

	res = game->Device->CreateBuffer(&constBufDesc, nullptr, &constantBuffer);
	if (FAILED(res))
	{
		std::cout << L"Const buffer dont initialize" << std::endl;
	}
#pragma endregion Initialize buffers

#pragma region Initialize rasterization state
	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	res = game->Device->CreateRasterizerState(&rastDesc, &rastState);
	if (FAILED(res))
	{
		std::cout << L"Rast state dont initialize" << std::endl;
	}
	res = game->Context->QueryInterface(IID_ID3DUserDefinedAnnotation, (void**)&annotation);
	if (FAILED(res))
	{
		std::cout << L"Annotation dont initialize" << std::endl;
	}
#pragma endregion Initialize rasterization state

#pragma region Load texture
		res = game->TextureLoader->LoadTextureFromFile(textureName, texture, texSRV, true, false);
		if (FAILED(res))
		{
			std::cout << L"Texture dont load" << std::endl;
		}

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

		res = game->Device->CreateSamplerState(&samplerDesc, &sampler);
		if (FAILED(res))
		{
			std::cout << L"Sampler state dont initialize" << std::endl;
		}
#pragma endregion Load texture
}
