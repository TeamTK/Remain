#include "GEKO.h"
#include "System//Fps.h"
#include "Collider\ColliderManager.h"

namespace GEKO
{
	bool Init(LPCWSTR WinName, INT WinWidth, INT WinHeight)
	{
		//ウィンドウの初期化
		if (SUCCEEDED(Window::Get()->InitWindow(WinName, WinWidth, WinHeight)))
		{
			//ダイレクトX11の初期化
			if (SUCCEEDED(Direct3D11::Get().InitD3D11(WinWidth, WinHeight)))
			{
				Input::KeyManagement::Get().Init();
				SoundManagement::Get()->Init();
				return true;
			}
		}
		MessageBoxA(0, "GEKO初期化失敗", NULL, MB_OK);
		return false;
	}

	bool Loop()
	{
		Input::KeyManagement::Get().Update();
		WorldMatrixManager::GetInstance()->Update(); //モデルの行列更新
		ColliderManager::GetInstance()->Update();
		Camera::Update();
		return Window::Get()->Loop(); 
	}

	void BackgroundColor(int Red, int Green, int Blue)
	{
		Direct3D11::Get().Clear(Red * RGB, Green * RGB, Blue * RGB);
	}

	void WindowFixing()
	{
		Window::Get()->SetFixing();
	}

	void NoWindowFrame()
	{
		Window::Get()->NoDrawFrame();
	}

	void MouseErasure()
	{
		// マウスカーソルの非表示
		ShowCursor(false);
	}

	void FullScreen(bool isFullScreen)
	{
		Direct3D11::Get().FullScreen(isFullScreen);
	}

	void DrawFps()
	{
		Window::Get()->DrawFps();
	}

	void RenderSpaceLine()
	{
		Fiqure::RenderLine3D(Vector3D(0.0f, 0.0f, 0.0f), Vector3D(100.0f, 0.0f, 0.0f), Vector3D(1.0f, 0.0f, 0.0f));
		Fiqure::RenderLine3D(Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 100.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
		Fiqure::RenderLine3D(Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 0.0f, 100.0f), Vector3D(0.0f, 0.0f, 1.0f));

		float Space = 5.0f;
		float lenge = 50.0f;
		int loopNum = 10;

		for (int i = 0; i < loopNum; i++)
		{
			Fiqure::RenderLine3D(Vector3D(-(Space + Space + loopNum) + i * Space, 0.0f, -lenge), Vector3D(-(Space + Space + loopNum) + i * Space, 0.0f, lenge), Vector3D(1.0f, 1.0f, 1.0f));
			Fiqure::RenderLine3D(Vector3D(-lenge, 0.0f, -(Space + Space + loopNum) + i * Space), Vector3D(lenge, 0.0f, -(Space + Space + loopNum) + i * Space), Vector3D(1.0f, 1.0f, 1.0f));
		}
	}

	void ChangeFps(int fps)
	{
		Fps::SetFps(fps);
	}

	void LoopEnd()
	{
		Window::Get()->WindowEnd();
	}

	void End()
	{
		RenderManager::AllClear();
		ColliderManager::GetInstance()->AllClear();
		WorldMatrixManager::GetInstance()->AllClear();
		ImageAsset::AllClear();
		SoundAsset::AllClear();
		DynamicMeshAsset::AllClear();
		StaticMeshAsset::AllClear();
		Direct3D11::Get().DestroyD3D11();
		Input::KeyManagement::Get().End();
		OutputDebugString(L"GEKO_Systemが正常に終了しました\n");
	};
}