#include "NowLoading.h"
#include "..\GEKO\GEKO.h"
#include "../GameSystem/StageObject/StageObject.h"
#include "MainGame.h"
#include <thread>

#define NOW_LOADING_POS 550, 500

Image g_NowLodingImage;
bool g_isLoad = false;

NowLoading::NowLoading(bool isResource) :
	Task("NowLoading", 0)
{
	g_NowLodingImage.SetAsset("NowLoading");

	//リソース読み込み既に読み込んでいたら実行しない
	if (!isResource)
	{
		auto func = []
		{
			DynamicMeshAsset::LoadFile("TextData\\DynamicMesh.txt");
			SoundAsset::LoadFile("TextData\\Sound.txt");
			g_isLoad = true;
		};

		auto func2 = []
		{
			for (;;)
			{
				if (g_isLoad) break;
				GEKO::ScreenUpdate();
				GEKO::ClearColor(10, 10, 10);

				g_NowLodingImage.Draw(NOW_LOADING_POS);
			}
		};

		std::thread t1(func);
		std::thread t2(func2);

		t1.join();
		t2.join();
	}
	m_Render.Regist(6, REGIST_RENDER_FUNC(NowLoading::Render));
	m_Transfer.Start(3.0f);
}

NowLoading::~NowLoading()
{
}

void NowLoading::Update()
{
	if (m_Transfer.GetIsEndTransfer())
	{
		new MainGame;
		Task::SetKill();
	}
	m_Transfer.Update();
}

void NowLoading::Render()
{
	m_Transfer.Render();
}