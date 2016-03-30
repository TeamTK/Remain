#include "GEKO.h"
#include "System//Fps.h"
#include "Figure\FiqureShaderManager.h"
#include "Shader\CopmuteShader\BaseCopmute.h"
#include "Shader\StaticMeshShader\StaticMeshShader.h"
#include "Shader\DynamicMeshShader\DynamicMeshShader.h"
#include "Shader\ShadowMap\ShaderShadowMap.h"
#include "Shader\ConstantShader.h"
#include "Shader\Light\PointLightManager.h"
#include "Shader\DeferredShader\DeferredRendering.h"

namespace GEKO
{
	bool Init(const char* winName, int winWidth, int winHeight, int resolutionWidth, int resolutionHeight)
	{
		//ウィンドウの初期化
		if (SUCCEEDED(Window::Get()->InitWindow(winName, winWidth, winHeight)))
		{
			//ダイレクトX11の初期化
			if (SUCCEEDED(Direct3D11::GetInstance()->InitD3D11(resolutionWidth, resolutionHeight)))
			{
				Direct3D11::GetInstance()->Clear();
				Direct3D11::GetInstance()->Present();
				CopmuteManager::GetInstance()->Init();
				
				Input::KeyManagement::Get().Init();
				SoundManagement::Get()->Init();

				bool isInit[] = {false, false, false, false, false};

				isInit[0] = StaticMeshShader::GetInstance()->Init();
				isInit[1] = DynamicMeshShader::GetInstance()->Init();
				isInit[2] = ShaderShadowMap::GetInstance()->Init();
				isInit[3] = ConstantShader::GetInstance()->Init();
				isInit[4] = FiqureShaderManager::GetInstance()->Init();

				if (!isInit[0] && !isInit[1] && !isInit[2] && !isInit[3] && !isInit[4])
				{
					GEKO::LoopEnd();
					return false;
				}

				//DeferredRendering::GetInstance()->Init();

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
		RenderingManager::GetInstance()->Render();
		return Window::Get()->Loop(); 
	}

	void BackgroundColor(int red, int green, int blue)
	{
		Direct3D11::GetInstance()->SetClearColor(red * RGB, green * RGB, blue * RGB);
	}

	void ClearColor()
	{
		Direct3D11::GetInstance()->Clear();
	}

	void ScreenUpdate()
	{
		Direct3D11::GetInstance()->Present();
	}

	void ManualRendering()
	{
		RenderingManager::GetInstance()->Render();
	}

	void WindowFixing()
	{
		Window::Get()->SetFixing();
	}

	void FullScreen(bool isFullScreen)
	{
		Direct3D11::GetInstance()->FullScreen(isFullScreen);
	}

	void SetResolution_And_RefreshRate(int width, int height, int refreshRateNum)
	{
		Direct3D11::GetInstance()->SetResolution_And_RefreshRate(width, height, refreshRateNum);
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

	void UseDepthBuffer(bool isUseDepth)
	{
		if (isUseDepth)
		{
			Direct3D11::GetInstance()->DepthOn();
		}
		else
		{
			Direct3D11::GetInstance()->DepthOff();
		}
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

	void ControlFps(bool isControl)
	{
		Fps::SetControl(isControl);
	}

	void LoopEnd()
	{
		Window::Get()->WindowEnd();
	}

	void End()
	{
		RenderingManager::GetInstance()->AllClear();
		TaskManager::AllClear();
		RenderManager::AllClear();
		ColliderManager::GetInstance()->AllClear();
		CopmuteManager::GetInstance()->Release();
		WorldMatrixManager::GetInstance()->AllClear();
		ImageAsset::AllClear();
		SoundAsset::AllClear();
		DynamicMeshAsset::AllClear();
		StaticMeshAsset::AllClear();
		StaticMeshShader::GetInstance()->Release();
		DynamicMeshShader::GetInstance()->Release();
		ShaderShadowMap::GetInstance()->Release();
		ConstantShader::GetInstance()->Release();
		//DeferredRendering::GetInstance()->Release();
		PointLightManager::GetInstance()->Release();
		Direct3D11::GetInstance()->DestroyD3D11();
		Input::KeyManagement::Get().End();
		OutputDebugString(TEXT("GEKO_Systemが正常に終了しました\n"));
	};

	const WindowSize* GetWindowSize()
	{
		return Window::Get()->GetWindowSize();
	}

	float GetNowFps()
	{
		return Fps::GetFPS();
	}

	float GetOneFps()
	{
		return Fps::GetOneFPS();
	}
}