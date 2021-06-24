#pragma once
#include "inclib.h"

#include "GameComponent.h"
#include "DisplayWin32.h"
#include "InputDevice.h"

class Game
{
	bool isExitRequested = false;
public:
	std::wstring Name;
	virtual LRESULT MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

#pragma region Associations
	DisplayWin32 *Display = nullptr;
	InputDevice *InputDevice = nullptr;
	std::vector<GameComponent*> Components;
#pragma endregion Associations

#pragma region DirectX resources
	Microsoft::WRL::ComPtr<ID3D11Device> Device;
	ID3D11Texture2D *backBuffer = nullptr;
	ID3D11DeviceContext *Context = nullptr;
	IDXGISwapChain *SwapChain = nullptr;
	ID3D11RenderTargetView *RenderView = nullptr;
	ID3DUserDefinedAnnotation *DebugAnnotation = nullptr;
	D3D11_VIEWPORT RenderSRV = {};

	IDXGISwapChain1 *SwapChain1 = nullptr;
	ID3D11Debug *Debug = nullptr;
#pragma endregion DirectX resources
	int ScreenResized;

	std::chrono::time_point<std::chrono::steady_clock> *StartTime;
	std::chrono::time_point<std::chrono::steady_clock> *PrevTime;
	float TotalTime = 0;
	unsigned int frameCount = 0;

	Game(std::wstring WindowName);

	void Exit();
	void RestoreTargets();
	void Run(int WindowWidth, int WindowHeight);
protected:
	virtual void Initialize();
	virtual void Update(float deltaTime);
	virtual void Draw(float deltaTime);
	virtual void PostDraw(float deltaTime);

	void DestroyResources();
	int PrepareResources();
	void UpdateInternal();
	void PrepareFrame();
	void EndFrame();
private:
	int CreateBackBuffer();
};
