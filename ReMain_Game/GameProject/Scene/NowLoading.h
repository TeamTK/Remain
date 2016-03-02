#ifndef _NOW_LOADING_H_
#define _NOW_LOADING_H_

#include "../GEKO/Task/RenderTask.h"
#include "../GameSystem/ScreenTransfer.h"

class NowLoading : public Task
{
public:
	NowLoading(bool isResource);
	~NowLoading();
	void Update();
	void Render();

private:
	ScreenTransfer_Out m_Transfer;
	RenderTask m_Render;
};

#endif