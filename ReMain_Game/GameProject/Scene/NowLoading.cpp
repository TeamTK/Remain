#include "NowLoading.h"
#include <thread>

#define NOW_LOADING_POS 500, 500

ImageAnimation g_NowLodingImage;
bool g_isLoad = false;

NowLoading::NowLoading(ChapterType type, bool isResource) :
	Task("NowLoading", 0)
{
	m_ChapterType = type;
	GEKO::BackgroundColor(0, 0, 0);
	g_NowLodingImage.SetDrawRegister(true, 11, 0);

	//リソース読み込み既に読み込んでいたら実行しない
	if (!isResource)
	{
		g_NowLodingImage.FrameDivision("NowLoading", 12, 3, 4);
		g_NowLodingImage.SetPosition(NOW_LOADING_POS);

		auto func = []
		{
			ImageAsset::LoadFile("TextData\\Image2.txt");
			StaticMeshAsset::LoadMesh("media\\Traceroute_1_1.x", "Chapter_1_1_Traceroute");
			StaticMeshAsset::LoadMesh("media\\Traceroute_1_2.x", "Chapter_1_2_Traceroute");
			StaticMeshAsset::LoadFile("TextData\\StaticMesh2.txt");
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
				GEKO::ClearColor();
				GEKO::ManualRendering();
				g_NowLodingImage.PlayFrame(0.007f);
				g_NowLodingImage.Update();
			}
		};

		std::thread t1(func);
		std::thread t2(func2);

		t1.join();
		t2.join();

		m_Transfer.Start(3.0f);
	}
	else
	{
		m_Transfer.Start(1.0f);
	}
}

NowLoading::~NowLoading()
{
}

void NowLoading::Update()
{
	if (m_Transfer.GetIsEndTransfer())
	{
		new MainGame(m_ChapterType);
		g_NowLodingImage.SetDrawRegister(false, 11, 0);
		Task::SetKill();
	}
	m_Transfer.Update();
	g_NowLodingImage.PlayFrame(0.2f);
	g_NowLodingImage.Update();
}