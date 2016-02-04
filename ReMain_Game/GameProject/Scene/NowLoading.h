#ifndef _NOW_LOADING_H_
#define _NOW_LOADING_H_

#include "../GEKO/Task/Task.h"

class NowLoading : public Task
{
public:
	NowLoading(bool isResource);
	~NowLoading();
	void Update();

private:
};

#endif