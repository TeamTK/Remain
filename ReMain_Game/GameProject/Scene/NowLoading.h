#ifndef _NOW_LOADING_H_
#define _NOW_LOADING_H_

#include "../GameSystem/ScreenTransfer.h"
#include "MainGame.h"

class NowLoading : public Task
{
public:
	NowLoading(ChapterType type, bool isResource);
	~NowLoading();
	void Update();

private:
	ScreenTransfer_Out m_Transfer;
	ChapterType m_ChapterType;
};

#endif