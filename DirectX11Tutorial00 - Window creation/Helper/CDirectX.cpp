#include "CDirectX.h"

void CDirectX::CreateMainWindow(LPCTSTR Title, float Width, float Height, WNDPROC WndProc, int nShowCmd)
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
	window_rect.right = static_cast<LONG>(Width);
	window_rect.bottom = static_cast<LONG>(Height);
	AdjustWindowRect(&window_rect, WS_OVERLAPPEDWINDOW, FALSE);
	
	assert(m_hWnd = CreateWindowEx(0, window_class.lpszClassName, Title, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, window_rect.right - window_rect.left, window_rect.bottom - window_rect.top,
		nullptr, nullptr, m_hInstance, nullptr));

	ShowWindow(m_hWnd, nShowCmd);
	
	UpdateWindow(m_hWnd);
}