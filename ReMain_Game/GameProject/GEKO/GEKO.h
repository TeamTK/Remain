#ifndef _GEKO_H_
#define _GEKO_H_

#include "Mesh\StaticMesh.h"
#include "AnimationMesh\DynamicMesh.h"
#include "ImageSystem\Image.h"
#include "SoundSystem\Sound.h"
#include "System\Camera.h"
#include "System\DirectionalLight.h"
#include "System\Input.h"
#include "System\Debug.h"
#include "System\Change.h"
#include "System\Collision.h"
#include "Figure\Fiqure.h"
#include "Collider\Collider.h"
#include "Collider\ColliderManager.h"
#include "Task\RenderTask.h"
#include "Task\FunctionTask.h"
#include "System\Timer.h"

namespace GEKO
{
	bool Init(const char* winName, int winWidth, int winHeight, int resolutionWidth, int resolutionHeight);
	bool Loop();
	void BackgroundColor(int red, int green, int blue);
	void ClearColor(int red, int green, int blue);
	void ScreenUpdate();
	void WindowFixing();
	void FullScreen(bool isFullScreen);
	void SetResolution_And_RefreshRate(int width, int height, int refreshRateNum);
	void NoWindowFrame();
	void MouseErasure();
	void DrawFps();
	void RenderSpaceLine();
	void ChangeFps(int fps);
	void LoopEnd();
	void End();
}

#endif