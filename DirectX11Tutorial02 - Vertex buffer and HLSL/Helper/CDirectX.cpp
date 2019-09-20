#include "CDirectX.h"

void CDirectX::Create(LPCTSTR Title, WNDPROC WndProc, int nShowCmd)
{
	CreateWin32Window(Title, WndProc, nShowCmd);

	CreateDirect3DObjects();
}

void CDirectX::CreateWin32Window(LPCTSTR Title, WNDPROC WndProc, int nShowCmd)
{
	assert(!m_hWnd);

	WNDCLASSEX window_class{};
	window_class.cbClsExtra = 0;
	window_class.cbSize = sizeof(WNDCLASSEX);
	window_class.cbWndExtra = 0;
	window_class.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	window_class.hCursor = LoadCursor(nullptr, IDC_ARROW);
	window_class.hIcon = window_class.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	window_class.hInstance = m_hInstance;
	window_class.lpfnWndProc = WndProc;
	window_class.lpszClassName = TEXT("game_window");
	window_class.lpszMenuName = nullptr;
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	assert(RegisterClassEx(&window_class));

	RECT window_rect{};
	window_rect.left = 0;
	window_rect.top = 0;
	window_rect.right = static_cast<LONG>(m_WindowSize.x);
	window_rect.bottom = static_cast<LONG>(m_WindowSize.y);
	AdjustWindowRect(&window_rect, WS_OVERLAPPEDWINDOW, FALSE);
	
	assert(m_hWnd = CreateWindowEx(0, window_class.lpszClassName, Title, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, window_rect.right - window_rect.left, window_rect.bottom - window_rect.top,
		nullptr, nullptr, m_hInstance, nullptr));

	ShowWindow(m_hWnd, nShowCmd);
	
	UpdateWindow(m_hWnd);
}

void CDirectX::CreateDirect3DObjects()
{
	CreateDevice();

	CreateAndSetRenderTargetView();

	SetViewport();
}

void CDirectX::CreateDevice()
{
	assert(!m_SwapChain);

	DXGI_SWAP_CHAIN_DESC swap_chain_desc{};
	swap_chain_desc.BufferCount = 1;
	swap_chain_desc.BufferDesc.Width = static_cast<UINT>(m_WindowSize.x);
	swap_chain_desc.BufferDesc.Height = static_cast<UINT>(m_WindowSize.y);
	swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
	swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_desc.Flags = 0;
	swap_chain_desc.OutputWindow = m_hWnd;
	swap_chain_desc.SampleDesc.Count = 1;
	swap_chain_desc.SampleDesc.Quality = 0;
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swap_chain_desc.Windowed = TRUE;

	assert(SUCCEEDED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0,
		D3D11_SDK_VERSION, &swap_chain_desc, &m_SwapChain, &m_Device, nullptr, &m_DeviceContext)));
}

void CDirectX::CreateAndSetRenderTargetView()
{
	assert(!m_RenderTargetView);

	ComPtr<ID3D11Texture2D> back_buffer{};
	assert(SUCCEEDED(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&back_buffer)));
	assert(SUCCEEDED(m_Device->CreateRenderTargetView(back_buffer.Get(), nullptr, &m_RenderTargetView)));

	m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), nullptr);
}

void CDirectX::SetViewport()
{
	D3D11_VIEWPORT view_port{};
	view_port.Width = m_WindowSize.x;
	view_port.Height = m_WindowSize.y;
	view_port.TopLeftX = 0.0f;
	view_port.TopLeftY = 0.0f;
	view_port.MinDepth = 0.0f;
	view_port.MaxDepth = 1.0f;

	m_DeviceContext->RSSetViewports(1, &view_port);
}

CShader* CDirectX::AddShader(EShaderType ShaderType, LPCWSTR ShaderFileName, LPCSTR EntryPoint,
	const D3D11_INPUT_ELEMENT_DESC* VSInputElements, UINT VSInputElementCount)
{
	m_vShaders.emplace_back(make_unique<CShader>(m_Device.Get(), m_DeviceContext.Get()));
	m_vShaders.back()->Create(ShaderType, ShaderFileName, EntryPoint, VSInputElements, VSInputElementCount);

	return m_vShaders.back().get();
}

CShader* CDirectX::GetShader(size_t Index)
{
	return m_vShaders[Index].get();
}

CObject2D* CDirectX::AddObject2D(const vector<SVertex2D>& vVertices)
{
	m_vObject2Ds.emplace_back(make_unique<CObject2D>(m_Device.Get(), m_DeviceContext.Get()));
	m_vObject2Ds.back()->Create(vVertices);

	return m_vObject2Ds.back().get();
}

CObject2D* CDirectX::GetObject2D(size_t Index)
{
	return m_vObject2Ds[Index].get();
}

void CDirectX::BeginRendering(const float(&ClearColorArray)[4])
{
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), ClearColorArray);
}

void CDirectX::EndRendering()
{
	m_SwapChain->Present(0, 0);
}