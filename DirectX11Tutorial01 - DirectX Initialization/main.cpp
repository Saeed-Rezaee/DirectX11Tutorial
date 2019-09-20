#include "Helper\CDirectX.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nShowCmd)
{
	constexpr float KWindowWidth{ 800.0f };
	constexpr float KWindowHeight{ 600.0f };
	constexpr float KClearColor[]{ 0.0f, 0.6f, 1.0f, 1.0f };

	CDirectX directx{ hInstance, KWindowWidth, KWindowHeight };
	directx.Create(TEXT("Game"), WndProc, nShowCmd);

	while (true)
	{
		static MSG msg{};
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) == TRUE)
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);

			DispatchMessage(&msg);
		}
		else
		{
			directx.BeginRendering(KClearColor);

			directx.EndRendering();
		}
	}

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) DestroyWindow(hWnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}