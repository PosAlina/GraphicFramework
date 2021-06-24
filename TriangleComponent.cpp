#include "TriangleComponent.h"
#include "inclib.h"
#include "Game.h"

//TriangleComponent::TriangleComponent(Game *inGame, Camera *inCamera):GameComponent(inGame)
//{
//	camera = inCamera;
//	Position = SimpleMath::Vector3::Zero;
//}
TriangleComponent::TriangleComponent(Game* inGame) :GameComponent(inGame)
{
	Position = SimpleMath::Vector3::Zero;
}

void TriangleComponent::Initialize()
{
	HRESULT res;

#pragma region Initialize shaders
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
		return;
	}
	res = game->Device->CreateVertexShader(
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		nullptr, &vertexShader);
	if (FAILED(res))
	{
		std::cout << L"Vertex shader dont compile" << std::endl;
		return;
	}

	ID3DBlob* errorPixelCode;
	res = D3DCompileFromFile(L"Simple.hlsl",
		nullptr /*macros*/,
		nullptr /*include*/,
		"PSMain",
		"vs_5_0",
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
		return;
	}
	res = game->Device->CreatePixelShader(
		pixelShaderByteCode->GetBufferPointer(),
		pixelShaderByteCode->GetBufferSize(),
		nullptr, &pixelShader);
	if (FAILED(res))
	{
		std::cout << L"Pixel shader dont compile" << std::endl;
		return;
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
		return;
	}
#pragma endregion Initialize layout

#pragma region Initialize points value
	points = new SimpleMath::Vector4[6]{
		SimpleMath::Vector4(0.0f, 50.5f, 0.0f, 1.0f), SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 1.0f),
		SimpleMath::Vector4(50.5f, -50.5f, 0.0f, 1.0f), SimpleMath::Vector4(0.0f, 1.0f, 0.0f, 0.0f),
		SimpleMath::Vector4(-50.0f, -50.5f, 0.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 1.0f, 1.0f),
	};
#pragma endregion Initialize points value

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
	constBufDesc.Usage = D3D11_USAGE_DEFAULT;
	constBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
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

	layout->Release();
	pixelShader->Release();
	vertexShader->Release();
	pixelShaderByteCode->Release();
	vertexShaderByteCode->Release();
	vertices->Release();
	rastState->Release();

	constantBuffer->Release();
	annotation->Release();
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

	annotation->BeginEvent(L"Triangle draw event");
	context->Draw(3, 0);
	annotation->EndEvent();
	context->RSSetState(oldState);
	oldState->Release();
}

void TriangleComponent::Update(float deltaTime)
{
	//auto wvp = SimpleMath::Matrix::CreateTranslation(Position) * camera->ViewMatrix * camera->ProjMatrix;
	////game->Context->UpdataSubresource(constantBuffer, 0, nullptr, &wvp, 0, 0);
	//D3D11_MAPPED_SUBRESOURCE res = {};
	//game->Context->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &res);
	//auto dataP = reinterpret_cast<float*>(res.pData);
	//memcpy(dataP, &wvp, sizeof(SimpleMath::Matrix));
	//game->Context->Unmap(constantBuffer, 0);
}
