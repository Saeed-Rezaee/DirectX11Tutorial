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
	window_rect.right = static_cast<LONG>(m_WindowWidth);
	window_rect.bottom = static_cast<LONG>(m_WindowHeight);
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
}

void CDirectX::CreateDevice()
{
	assert(!m_SwapChain);

	DXGI_SWAP_CHAIN_DESC swap_chain_desc{};
	swap_chain_desc.BufferCount = 1;
	swap_chain_desc.BufferDesc.Width = static_cast<UINT>(m_WindowWidth);
	swap_chain_desc.BufferDesc.Height = static_cast<UINT>(m_WindowHeight);
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

void CDirectX::BeginRendering(const float(&ClearColorArray)[4])
{
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), ClearColorArray);
}

void CDirectX::EndRendering()
{
	m_SwapChain->Present(0, 0);
}