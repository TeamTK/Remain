#ifndef CHAPTER_MANAGER_H
#define CHAPTER_AMAGER_

#include "../GEKO/GEKO.h"

class ChapterManager
{
public:
	ChapterManager();
	~ChapterManager();
	void Update();
	static ChapterManager* GetInstance();

private:

};


#endif
