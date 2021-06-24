#include "Game.h"

Game *Instance;

Game::Game(std::wstring WindowName)
{
	Name = WindowName;
	Instance = this;
	Initialize();
}

void Game::Exit()
{
	isExitRequested = true;
}

// After Display initialization
int Game::CreateBackBuffer()
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
		D3D11_CREATE_DEVICE_DEBUG,
		featureLevel,
		1,
		D3D11_SDK_VERSION,
		&swapDesc,
		&SwapChain,
		&Device,
		nullptr,
		&Context);
	ZCHECK(res);

	res = SwapChain->GetBuffer(0, IID_ID3D11Texture2D, (void**)&backBuffer);	ZCHECK(res);
	return 0;
}

int Game::PrepareResources()
{
	HRESULT res;
	CreateBackBuffer();
	res = Device->CreateRenderTargetView(backBuffer, nullptr, &RenderView);	ZCHECK(res);
	SwapChain->QueryInterface<IDXGISwapChain1>(&SwapChain1);
	Context->QueryInterface(IID_ID3DUserDefinedAnnotation, (void**)&DebugAnnotation);
	Device->QueryInterface(IID_ID3D11Debug, (void**)&Debug);

	RenderSRV.Width = static_cast<float>(Display->ClientWidth);
	RenderSRV.Height = static_cast<float>(Display->ClientHeight);
	RenderSRV.TopLeftX = 0;
	RenderSRV.TopLeftY = 0;
	RenderSRV.MinDepth = 0;
	RenderSRV.MaxDepth = 1.0f;

	Context->RSSetViewports(1, &RenderSRV);
	Context->OMSetRenderTargets(1, &RenderView, nullptr);
	return 0;
}

// After Game Component run
void Game::RestoreTargets()
{
	RenderSRV.Width = static_cast<float>(Display->ClientWidth);
	RenderSRV.Height = static_cast<float>(Display->ClientHeight);
	RenderSRV.TopLeftX = 0;
	RenderSRV.TopLeftY = 0;
	RenderSRV.MinDepth = 0;
	RenderSRV.MaxDepth = 1.0f;

	Context->RSSetViewports(1, &RenderSRV);
	Context->OMSetRenderTargets(1, &RenderView, nullptr);
}

//
// Functions for Run()
//
LRESULT CALLBACK Game::WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	return Instance->MessageHandler(hwnd, umessage, wparam, lparam);
}

LRESULT Game::MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	//UIRendererClass::WndProcHandler(hwnd, umessage, wparam, lparam); // From class work
	switch (umessage)
	{
	case WM_DESTROY:
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		//isExitRequested = true;
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
	//case WM_INPUT:
	//{
	//	UINT dwSize = 0;
	//	GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT, nullptr, &dwSize, sizeof(RAWINPUTHEADER));
	//	LPBYTE lpb = new BYTE[dwSize];
	//	if (lpb == nullptr) {
	//		return 0;
	//	}
	//	if (GetRawInputData((HRAWINPUT)lparam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
	//		OutputDebugString(TEXT("GetRawInputData does not return correct size"));
	//	RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(lpb);
	//	if (raw->header.dwType == RIM_TYPEKEYBOARD)
	//	{
	//		InputDevice->OnKeyDown({
	//			raw->data.keyboard.MakeCode,
	//			raw->data.keyboard.Flags,
	//			raw->data.keyboard.VKey,
	//			raw->data.keyboard.Message
	//			});
	//	}
	//	else if (raw->header.dwType == RIM_TYPEMOUSE)
	//	{
	//		InputDevice->OnMouseMove({
	//			raw->data.mouse.usFlags,
	//			static_cast<int>(raw->data.mouse.ulExtraInformation),
	//			static_cast<int>(raw->data.mouse.ulRawButtons),
	//			static_cast<int>(raw->data.mouse.usButtonData),
	//			raw->data.mouse.lLastX,
	//			raw->data.mouse.lLastY
	//			});
	//	}
	//	delete[] lpb;
	//	return DefWindowProc(hwnd, umessage, wparam, lparam);
	//}
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
	case WM_SIZE:
	{
		std::cout << "Width " << LOWORD(lparam) << "Height " << HIWORD(lparam) << std::endl;
		//if (Instance->Device)
		//{
			//int newWidth = LOWORD(lparam);
			//int newHeight = HIWORD(lparam);
			//if (newWidth == 0 || newHeight == 0 || ((Instance->Display->ClientWidth == newWidth) && (Instance->Display->ClientHeight == newHeight)))
			//{
			//	return 0;
			//}
			//Instance->Display->ClientWidth = newWidth;
			//Instance->Display->ClientHeight = newHeight;
			//if (Instance->backBuffer != nullptr)
			//{
			//	Instance->backBuffer->Release();
			//	Instance->backBuffer = nullptr;
			//}
			//if (Instance->RenderView != nullptr)
			//{
			//	Instance->RenderView->Release();
			//	Instance->RenderView = nullptr;
			//}
			//if (depthBuffer != nullptr)
			//{
			//	depthBuffer->Release();
			//	depthBuffer = nullptr;
			//}
			//Render2D->UnloadResources();
			//SwapChain1->ResizeBuffer(2, Display->ClientWidth, Display->ClientHeight, DXGI_FORMAT_R8G8S8A8_UNORM, 0);
			//CreateBackBuffer();
			//Renderer2D->SetTarget(RenderView);
			//const auto screenSize = Vector2{ static_cast<float>(Display->ClientWidth), static_cast<float>(Display->ClientHeight) };
			//ScreenResized.Broadcast(screenSize);
		//}
		return 0;
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
	//TotalTime += deltaTime;
//frameCount++;

//if (TotalTime > 1.0f) {
//	float fps = frameCount / TotalTime;

//	TotalTime = 0.0f;

//	WCHAR text[256];
//	swprintf_s(text, TEXT("FPS: %f"), fps);
//	SetWindowText(Display->hWnd, text);

//	frameCount = 0;
//}
//// Background
//float color[] = { TotalTime, 0.1f, 0.1f, 1.0f };
//Context->OMSetRenderTargets(1, &RenderView, nullptr);
//Context->ClearRenderTargetView(RenderView, color);
	Context->ClearState();
}

void Game::Update(float deltaTime)
{
	for (auto component : Components) component->Update(deltaTime);
}

void Game::Draw(float deltaTime)
{
	DebugAnnotation->BeginEvent(L"BeginDraw");
	for (auto component : Components) component->Draw(deltaTime);
	DebugAnnotation->EndEvent();
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
	TotalTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - *StartTime).count() / 1000000.0f;

	PrepareFrame();
	Update(deltaTime);
	Draw(deltaTime);
	PostDraw(deltaTime);
	EndFrame();
#pragma endregion DrawSomeStaff
}

void Game::DestroyResources()
{
	Device->Release();
	Debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	backBuffer->Release();

	Context->Release();
	SwapChain->Release();
	SwapChain1->Release();
	RenderView->Release();
	DebugAnnotation->Release();

	delete InputDevice;
	delete Display;

	for (auto component : Components) component->DestroyResources();
}

void Game::Run(int WindowWidth, int WindowHeight)
{
	Display = new DisplayWin32(Name, WindowWidth, WindowHeight, WndProc);
	if (!Display->hWnd)
	{
		DestroyResources();
		return;
	}
	if (!PrepareResources())
	{
		DestroyResources();
		return;
	}
	Initialize();
	for (auto component : Components) component->Initialize();

	StartTime = new std::chrono::time_point<std::chrono::steady_clock>();
	PrevTime = new std::chrono::time_point<std::chrono::steady_clock>();
	*StartTime = std::chrono::steady_clock::now();
	*PrevTime = *StartTime;

	MSG msg = {};

	while (!isExitRequested)
	{
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT)
		{
			isExitRequested = true;
		}
		UpdateInternal();
	}
	delete StartTime;
	delete PrevTime;
	DestroyResources();
}
