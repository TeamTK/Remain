#ifndef _GEKO_H_
#define _GEKO_H_

#include "Mesh\StaticMesh\StaticMesh.h"
#include "Mesh\DynamicMesh\DynamicMesh.h"
#include "ImageSystem\Image.h"
#include "ImageSystem\ImageAnimation.h"
#include "SoundSystem\Sound.h"
#include "System\Camera.h"
#include "System\Input.h"
#include "System\Debug.h"
#include "System\Change.h"
#include "System\Timer.h"
#include "System\Collision.h"
#include "Figure\Fiqure.h"
#include "Figure\Billboard.h"
#include "Collider\Collider.h"
#include "Collider\ColliderManager.h"
#include "Task\RenderTask.h"
#include "Task\FunctionTask.h"
#include "Task\CommandTask.h"
#include "AI\TracerouteManager.h"
#include "AI\TracerouteSearch.h"
#include "AI\TracerouteTarget.h"
#include "Shader\Light\DirectionalLight.h"
#include "Shader\Light\PointLight.h"

namespace GEKO
{
	bool Init(const char* winName, int winWidth, int winHeight, int resolutionWidth, int resolutionHeight);
	bool Loop();
	void BackgroundColor(int red, int green, int blue);
	void ClearColor();
	void ScreenUpdate();
	void ManualRendering();
	void WindowFixing();
	void FullScreen(bool isFullScreen);
	void SetResolution_And_RefreshRate(int width, int height, int refreshRateNum);
	void NoWindowFrame();
	void MouseErasure();
	void UseDepthBuffer(bool isUseDepth);
	void DrawFps();
	void RenderSpaceLine();
	void ChangeFps(int fps);
	void ControlFps(bool isControl);
	void LoopEnd();
	void End();
	const WindowSize *GetWindowSize();
	float GetNowFps();
	float GetOneFps();
}

#endif