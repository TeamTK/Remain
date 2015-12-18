#include "Title.h"

Title::Title()
{

}

Title::~Title()
{
}

void Title::Update()
{
	SceneManager::Quit(0, EScene::E_MAIN);
}

void Title::Render()
{
	std::cout << "Title" << '\n';
}