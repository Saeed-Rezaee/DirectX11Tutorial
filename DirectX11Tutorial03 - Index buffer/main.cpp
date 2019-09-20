#include "Helper\CDirectX.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nShowCmd)
{
	static constexpr XMFLOAT2 KWindowSize{ 800.0f, 600.0f };
	static constexpr float KClearColor[]{ 0.0f, 0.6f, 1.0f, 1.0f };
	static constexpr D3D11_INPUT_ELEMENT_DESC KInputLayout[]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	CDirectX directx{ hInstance, KWindowSize };
	directx.Create(TEXT("Game"), WndProc, nShowCmd);

	CShader* vs{ directx.AddShader(EShaderType::VertexShader, L"HLSL/VertexShader.hlsl", "main", KInputLayout, ARRAYSIZE(KInputLayout)) };
	CShader* ps{ directx.AddShader(EShaderType::PixelShader, L"HLSL/PixelShader.hlsl", "main") };
	
	vector<SVertex2D> vertices
	{
		SVertex2D(XMFLOAT2(-0.5f, 0.5f)),
		SVertex2D(XMFLOAT2(0.5f, 0.5f)),
		SVertex2D(XMFLOAT2(-0.5f, -0.5f)),
		SVertex2D(XMFLOAT2(0.5f, -0.5f)),
	};
	vector<SIndex> indices
	{
		SIndex(0, 1, 2),
		SIndex(1, 3, 2),
	};
	CObject2D* obj_rectangle{ directx.AddObject2D(vertices, indices) };

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

			vs->Use();
			ps->Use();

			obj_rectangle->Draw();

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