#pragma once
#include "inclib.h"

#include "GameComponent.h"
#include "DisplayWin32.h"
#include "Delegates.h"
//#include "Camera.h"

class InputDevice;

class Game
{
public:
	static Game* Instance;
	std::wstring* Name;

#pragma region Associations
	std::vector<GameComponent*> Components;
	MulticastDelegate<const SimpleMath::Vector2&> ScreenResized;
	DisplayWin32 *Display = nullptr;
	InputDevice *InputDevice;
#pragma endregion Associations

	virtual LRESULT MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

#pragma region DirectX resources
	ID3D11Device *Device = nullptr;
	IDXGISwapChain* SwapChain = nullptr;
	IDXGISwapChain1* SwapChain1 = nullptr;
	ID3D11DeviceContext* Context = nullptr;

	ID3D11Texture2D *backBuffer = nullptr;
	ID3D11Texture2D* depthBuffer = nullptr;

	ID3D11RenderTargetView *RenderView = nullptr;
	ID3D11DepthStencilView *DepthView = nullptr;

	ID3D11ShaderResourceView* RenderSRV = nullptr;

	ID3D11RasterizerState* RastState;
	ID3D11DepthStencilState* DepthState;

	ID3DUserDefinedAnnotation *DebugAnnotation = nullptr;
private:
	DXGI_SWAP_CHAIN_DESC swapDesc = {};
#pragma endregion DirectX resources

public:
	std::chrono::time_point<std::chrono::steady_clock> *StartTime = nullptr;
	std::chrono::time_point<std::chrono::steady_clock> *PrevTime = nullptr;
	std::chrono::seconds *TotalTime = nullptr;

	//Camera* GameCamera = nullptr;

	bool isExitRequested = false;
	bool isActive = false;

	Game(std::wstring& WindowName);
	virtual ~Game();

	void Exit();
	void RestoreTargets();
	void Run(int WindowWidth, int WindowHeight);
protected:
	virtual void Initialize();
	virtual void Update(float deltaTime);
	virtual void Draw(float deltaTime);
	virtual void PostDraw(float deltaTime);

	void DestroyResources();
	void PrepareResources();
	void UpdateInternal();
	void PrepareFrame();
	void EndFrame();
private:
	void CreateBackBuffer();
};

static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

