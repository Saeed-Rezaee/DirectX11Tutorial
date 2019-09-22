#include "Helper\CDirectX.h"
#include "Game\CEntityPool.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nShowCmd)
{
	static constexpr XMFLOAT2 KWindowSize{ 800.0f, 600.0f };
	static constexpr float KClearColor[]{ 0.0f, 0.6f, 1.0f, 1.0f };
	static constexpr D3D11_INPUT_ELEMENT_DESC KInputLayout[]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	static const string KAssetDir{ "Asset/" };

	CDirectX directx{ hInstance, KWindowSize };
	directx.Create(TEXT("Game"), WndProc, nShowCmd, KAssetDir + "dotumche_10_korean.spritefont");

	CShader* vs{ directx.AddShader(EShaderType::VertexShader, L"HLSL/VertexShader.hlsl", "main", KInputLayout, ARRAYSIZE(KInputLayout)) };
	CShader* ps{ directx.AddShader(EShaderType::PixelShader, L"HLSL/PixelShader.hlsl", "main") };
	
	CEntityPool entity_pool{ directx };
	CEntity* entity_bg{ entity_pool.AddEntity() };
	{
		entity_bg->AddTexture(KAssetDir + "bg.png");
		entity_bg->CreateRectangle(entity_bg->GetTexture(0)->GetTextureSize());
		entity_bg->Sampler = ESampler::Point;
	}

	CEntity* entity_sprite{ entity_pool.AddEntity() };
	{
		entity_sprite->AddTexture(KAssetDir + "rocky_brown_100x100_lexasset.png");
		entity_sprite->CreateRectangle(entity_sprite->GetTexture(0)->GetTextureSize());
		entity_sprite->Sampler = ESampler::Linear;
	}

	steady_clock clock{};
	long long time_prev{ clock.now().time_since_epoch().count() };
	long long time_now{};
	float delta_time{};

	while (true)
	{
		static MSG msg{};
		static char key_down{};
		time_now = clock.now().time_since_epoch().count();

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) == TRUE)
		{
			if (msg.message == WM_QUIT) break;

			if (msg.message == WM_KEYDOWN) key_down = MapVirtualKey(static_cast<UINT>(msg.wParam), MAPVK_VK_TO_CHAR);

			TranslateMessage(&msg);

			DispatchMessage(&msg);
		}
		else
		{
			if (GetKeyState(VK_RIGHT) < 0)
			{
				entity_sprite->WorldPosition.x += 1.0f;
			}
			if (GetKeyState(VK_LEFT) < 0)
			{
				entity_sprite->WorldPosition.x -= 1.0f;
			}
			if (GetKeyState(VK_UP) < 0)
			{
				entity_sprite->WorldPosition.y += 1.0f;
			}
			if (GetKeyState(VK_DOWN) < 0)
			{
				entity_sprite->WorldPosition.y -= 1.0f;
			}

			delta_time = 0.000'000'001f * (time_now - time_prev);
			
			directx.BeginRendering(KClearColor);

			vs->Use();
			ps->Use();

			entity_pool.DrawEntities();

			directx.RenderText(L"Delta Time: " + to_wstring(delta_time) + L" √ ", XMFLOAT2(0, 0), Colors::Black);

			directx.EndRendering();

			time_prev = time_now;
			key_down = 0;
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