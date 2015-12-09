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
#include "System\Math.h"
#include "System\Change.h"
#include "System\Collision.h"
#include "Figure\Fiqure.h"
#include "Collider\Collider.h"
#include "Task\RenderTask.h"

namespace GEKO
{
	bool Init(LPCWSTR WinName, INT WinWidth, INT WinHeight);
	bool Loop();
	void BackgroundColor(int Red, int Green, int Blue);
	void WindowFixing();
	void FullScreen(bool isFullScreen);
	void NoWindowFrame();
	void MouseErasure();
	void DrawFps();
	void RenderSpaceLine();
	void ChangeFps(int fps);
	void LoopEnd();
	void End();
}

#endif