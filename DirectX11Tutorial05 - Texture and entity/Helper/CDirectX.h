#pragma once

#include "CommonHeader.h"
#include "CShader.h"
#include "CObject2D.h"

class CDirectX
{
	friend class CEntityPool;

public:
	CDirectX(HINSTANCE hInstance, const XMFLOAT2& WindowSize) : m_hInstance{ hInstance }, m_WindowSize{ WindowSize } {};
	~CDirectX() {};

	void Create(LPCTSTR Title, WNDPROC WndProc, int nShowCmd);

	CShader* AddShader(EShaderType ShaderType, LPCWSTR ShaderFileName, LPCSTR EntryPoint,
		const D3D11_INPUT_ELEMENT_DESC* VSInputElements = nullptr, UINT VSInputElementCount = 0);
	CShader* GetShader(size_t Index);

	CObject2D* AddObject2D(const vector<SVertex2D>& vVertices, const vector<SIndex>& vIndices);
	CObject2D* GetObject2D(size_t Index);

	void BeginRendering(const float (&ClearColorArray)[4]);
	void EndRendering();

private:
	void CreateWin32Window(LPCTSTR Title, WNDPROC WndProc, int nShowCmd);
	void CreateDirect3DObjects();

private:
	void CreateDevice();
	void CreateAndSetRenderTargetView();
	void SetViewport();

private:
	HWND							m_hWnd{};
	HINSTANCE						m_hInstance{};
	XMFLOAT2						m_WindowSize{};

	ComPtr<IDXGISwapChain>			m_SwapChain{};
	ComPtr<ID3D11Device>			m_Device{};
	ComPtr<ID3D11DeviceContext>		m_DeviceContext{};
	ComPtr<ID3D11RenderTargetView>	m_RenderTargetView{};

	vector<unique_ptr<CShader>>		m_vShaders{};
	vector<unique_ptr<CObject2D>>	m_vObject2Ds{};
};