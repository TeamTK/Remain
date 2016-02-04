#include "GameOvar.h"
#include <iostream>

GameOvar::GameOvar() :
	Task("GameOvar", 0)
{
}

GameOvar::~GameOvar()
{
}

void GameOvar::Update()
{
	std::cout << "GameOvar" << "\n";
}

void GameOvar::Render()
{
}