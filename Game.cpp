#include "Game.h"
#include "InputDevice.h"
#include "Camera.h"
#include "TextureLoader.h"

Game* Game::Instance = nullptr;

struct QueryBuf {
	ID3D11Query* queryDisjoint;
	ID3D11Query* queryAtStart;
	ID3D11Query* queryAtEnd;
};
QueryBuf qBuf;
QueryBuf qBufSecond;
QueryBuf* qBufCur;

void CollectTimestamps(ID3D11DeviceContext* pContext, QueryBuf* buf)
{
	while (pContext->GetData(buf->queryDisjoint, NULL, 0, 0) == S_FALSE)
	{
		Sleep(1);
	}

	D3D10_QUERY_DATA_TIMESTAMP_DISJOINT tsDisjoint;
	pContext->GetData(buf->queryDisjoint, &tsDisjoint, sizeof(tsDisjoint), 0);
}

Game::Game(std::wstring WindowName)
{
	Name = WindowName;
	TextureLoader = new ::TextureLoader(this);

	Instance = this;
}

Game::~Game()
{
}

void Game::Run(int WindowWidth, int WindowHeight)
{
	Display = new DisplayWin32(Name, WindowWidth, WindowHeight, WndProc);
	if (!Display->hWnd)
	{
		DestroyResources();
		return;
	}
	PrepareResources();
	Initialize();
	for (auto component : Components) component->Initialize();

	StartTime = new std::chrono::time_point<std::chrono::steady_clock>();
	PrevTime = new std::chrono::time_point<std::chrono::steady_clock>();
	*StartTime = std::chrono::steady_clock::now();
	*PrevTime = *StartTime;
	TotalTime = new std::chrono::duration<long long>();

	MSG msg = {};

	while (!isExitRequested)
	{
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		UpdateInternal();
	}
	delete StartTime;
	delete PrevTime;
	DestroyResources();
}

void Game::Exit()
{
	isExitRequested = true;
}

// After Display initialization
void Game::CreateBackBuffer()
{
	if (backBuffer != nullptr) backBuffer->Release();
	if (RenderView != nullptr) RenderView->Release();
	if (depthBuffer != nullptr) depthBuffer->Release();
	if (DepthView != nullptr) DepthView->Release();
	if (RenderSRV != nullptr) RenderSRV->Release();
	HRESULT res;
	res = SwapChain->GetBuffer(0, IID_ID3D11Texture2D, (void**)&backBuffer); ZCHECK(res);
	Device->CreateRenderTargetView(backBuffer, nullptr, &RenderView);

	D3D11_TEXTURE2D_DESC depthTexDesc = {};
	depthTexDesc.ArraySize = 1;
	depthTexDesc.MipLevels = 1;
	depthTexDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	depthTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	depthTexDesc.CPUAccessFlags = 0;
	depthTexDesc.MiscFlags = 0;
	depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
	depthTexDesc.Width = Display->ClientWidth;
	depthTexDesc.Height = Display->ClientHeight;
	depthTexDesc.SampleDesc = { 1, 0 };
	res = Device->CreateTexture2D(&depthTexDesc, nullptr, &depthBuffer); ZCHECK(res);
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStenDesc = {};
	depthStenDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStenDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStenDesc.Flags = 0;
	Device->CreateDepthStencilView(depthBuffer, &depthStenDesc, &DepthView);
}

void Game::PrepareResources()
{
	HRESULT res;
	DXGI_SWAP_CHAIN_DESC swapDesc = {};
	swapDesc.BufferCount = 2;
	swapDesc.BufferDesc.Width = Display->ClientWidth;
	swapDesc.BufferDesc.Height = Display->ClientHeight;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = Display->hWnd;
	swapDesc.Windowed = true;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;

	D3D_FEATURE_LEVEL featureLevel[] = { D3D_FEATURE_LEVEL_11_1 };
	res = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
#ifdef _DEBUG
		D3D11_CREATE_DEVICE_DEBUG |
#endif
		D3D11_CREATE_DEVICE_BGRA_SUPPORT,
		featureLevel,
		1,

		D3D11_SDK_VERSION,
		&swapDesc,
		&SwapChain,
		&Device,
		nullptr,
		&Context); //ZCHECK(res)

	SwapChain->QueryInterface<IDXGISwapChain1>(&SwapChain1);
	Context->QueryInterface(IID_ID3DUserDefinedAnnotation, (void**)&DebugAnnotation);

	CreateBackBuffer();

	D3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_WIREFRAME;

	this->InputDevice = new ::InputDevice(this);
	GameCamera = new Camera(this);

	D3D11_QUERY_DESC qDesc = {};
	qDesc.Query = D3D11_QUERY_TIMESTAMP_DISJOINT;
	qDesc.MiscFlags = 0;
	Device->CreateQuery(&qDesc, &qBuf.queryDisjoint);
}

// After Game Component run
void Game::RestoreTargets()
{
	Context->OMSetRenderTargets(1, &RenderView, DepthView);
	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<float>(Display->ClientWidth);
	viewport.Height = static_cast<float>(Display->ClientHeight);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;

	Context->RSSetViewports(1, &viewport);
}

void Game::Update(float deltaTime)
{
	for (auto component : Components) component->Update(deltaTime);
}

void Game::Draw(float deltaTime)
{
	for (auto component : Components) component->Draw(deltaTime);
}

void Game::PostDraw(float deltaTime) // UI above
{
}

void Game::EndFrame()
{
	// SwapChain->Present(1, 0);
	SwapChain1->Present(1, 0);
	//SwapChain1->Present(1, OXGI_PRESENT_DO_NOT_WAIT);
}

void Game::UpdateInternal()
{
#pragma region DrawSomeStaff
	auto currentTime = std::chrono::steady_clock::now();
	float deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - *PrevTime).count() / 1000000.0f;
	*PrevTime = currentTime;
	*TotalTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - *StartTime);

	PrepareFrame();
	Update(deltaTime);
	Draw(deltaTime);
	PostDraw(deltaTime);
	EndFrame();
#pragma endregion DrawSomeStaff
}

void Game::DestroyResources()
{
	/*for (auto i = 0; i < Components.size(); ++i) {
		Components[i]->DestroyResources();
	}*/
	if (backBuffer != nullptr) backBuffer->Release();
	
	if (GameCamera != nullptr) delete GameCamera;
	if (InputDevice != nullptr) delete InputDevice;
	if (TextureLoader != nullptr) delete TextureLoader;
	if (Device != nullptr) Device->Release();
	if (SwapChain != nullptr) SwapChain->Release();
	if (SwapChain1 != nullptr) SwapChain1->Release();
	if (Context != nullptr) Context->Release();
	if (depthBuffer != nullptr) depthBuffer->Release();
	if (RenderView != nullptr) RenderView->Release();
	if (DepthView != nullptr) DepthView->Release();
	if (RenderSRV != nullptr) RenderSRV->Release();
	if (RastState != nullptr) RastState->Release();
	if (DepthState != nullptr) DepthState->Release();
	if (DebugAnnotation != nullptr) DebugAnnotation->Release();
}

//
// HANDLER
//
LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	return Game::Instance->MessageHandler(hwnd, umessage, wparam, lparam);
}

LRESULT Game::MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	case WM_DESTROY:
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		isExitRequested = true;

		return 0;
	}
	case WM_ENTERSIZEMOVE:
	{
		return 0;
	}
	case WM_EXITSIZEMOVE:
	{
		return 0;
	}
	case WM_INPUT:
	{
		UINT dwSize = 0;
		GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT, nullptr, &dwSize, sizeof(RAWINPUTHEADER));
		LPBYTE lpb = new BYTE[dwSize];
		if (lpb == nullptr) {
			return 0;
		}

		if (GetRawInputData((HRAWINPUT)lparam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
			OutputDebugString(TEXT("GetRawInputData does not return correct size !\n"));

		RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(lpb);

		if (raw->header.dwType == RIM_TYPEKEYBOARD)
		{
			Instance->InputDevice->OnKeyDown({
				raw->data.keyboard.MakeCode,
				raw->data.keyboard.Flags,
				raw->data.keyboard.VKey,
				raw->data.keyboard.Message
				});
		}
		else if (raw->header.dwType == RIM_TYPEMOUSE)
		{
			Instance->InputDevice->OnMouseMove({
				raw->data.mouse.usFlags,
				raw->data.mouse.usButtonFlags,
				static_cast<int>(raw->data.mouse.ulExtraInformation),
				static_cast<int>(raw->data.mouse.ulRawButtons),
				static_cast<short>(raw->data.mouse.usButtonData),
				raw->data.mouse.lLastX,
				raw->data.mouse.lLastY
				});
		}

		delete[] lpb;
		return DefWindowProc(hwnd, umessage, wparam, lparam);
	}
	default:
		return DefWindowProc(hwnd, umessage, wparam, lparam);
	}
}

void Game::Initialize()
{
}

void Game::PrepareFrame()
{
	Context->ClearState();

	Context->OMSetRenderTargets(1, &RenderView, DepthView);

	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<float>(Display->ClientWidth);
	viewport.Height = static_cast<float>(Display->ClientHeight);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;

	Context->RSSetViewports(1, &viewport);
	Context->RSSetState(RastState);

	Context->ClearRenderTargetView(RenderView, SimpleMath::Color(0.0f, 0, 0, 1.0f));
	Context->ClearDepthStencilView(DepthView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}
