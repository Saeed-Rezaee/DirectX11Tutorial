#pragma once

#include <windows.h>
#include <cassert>

class CDirectX
{
public:
	CDirectX(HINSTANCE hInstance) : m_hInstance{ hInstance } {};
	~CDirectX() {};

	void CreateMainWindow(LPCTSTR Title, float Width, float Height, WNDPROC WndProc, int nShowCmd);

private:
	HWND		m_hWnd{};
	HINSTANCE	m_hInstance{};
};