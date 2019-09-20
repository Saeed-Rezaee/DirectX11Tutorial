#pragma once

#include <windows.h>
#include <cassert>

class CDirectX
{
public:
	CDirectX(HINSTANCE hInstance, float WindowWidth, float WindowHeight) : 
		m_hInstance{ hInstance }, m_WindowWidth{ WindowWidth }, m_WindowHeight{ WindowHeight }  {};
	~CDirectX() {};

	void CreateWin32Window(LPCTSTR Title, WNDPROC WndProc, int nShowCmd);

private:
	HWND		m_hWnd{};
	HINSTANCE	m_hInstance{};
	float		m_WindowWidth{};
	float		m_WindowHeight{};
};