#pragma once

#include <windows.h>
#include <cassert>
#include <d3d11.h>
#include <wrl.h>

#pragma comment(lib, "d3d11.lib")

using namespace Microsoft::WRL;

class CDirectX
{
public:
	CDirectX(HINSTANCE hInstance, float WindowWidth, float WindowHeight) :
		m_hInstance{ hInstance }, m_WindowWidth{ WindowWidth }, m_WindowHeight{ WindowHeight } {};
	~CDirectX() {};

	void Create(LPCTSTR Title, WNDPROC WndProc, int nShowCmd);

	void BeginRendering(const float (&ClearColorArray)[4]);
	void EndRendering();

private:
	void CreateWin32Window(LPCTSTR Title, WNDPROC WndProc, int nShowCmd);
	void CreateDirect3DObjects();

private:
	void CreateDevice();
	void CreateAndSetRenderTargetView();

private:
	HWND							m_hWnd{};
	HINSTANCE						m_hInstance{};
	float							m_WindowWidth{};
	float							m_WindowHeight{};

	ComPtr<IDXGISwapChain>			m_SwapChain{};
	ComPtr<ID3D11Device>			m_Device{};
	ComPtr<ID3D11DeviceContext>		m_DeviceContext{};
	ComPtr<ID3D11RenderTargetView>	m_RenderTargetView{};
};