#include "ChapterManager.h"
#include "Chapter.h"

ChapterManager::ChapterManager()
{

}

ChapterManager::~ChapterManager()
{

}

void ChapterManager::Update()
{
	
}

ChapterManager* ChapterManager::GetInstance()
{
	static ChapterManager instance;
	return &instance;
}