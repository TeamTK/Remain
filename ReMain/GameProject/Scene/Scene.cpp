#include "Scene.h"
#include "Opening.h"
#include "Title.h"
#include "BaseCamp.h"
#include "MainGame.h"
#include "GameEnd.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

SceneManager::SceneManager() :
	m_IsQuit(true),
	m_QuitCnt(0),
	m_QuitTime(0),
	m_InCnt(0),
	m_IsIn(false),
	m_pScene(nullptr),
	m_EScene(EScene::E_NONE),
	m_IsEnd(false)
{
	m_pScene = new None();
	m_IsQuit = false;
}

SceneManager::~SceneManager()
{
	if (m_pScene != nullptr)
	{
		delete GetInstance()->m_pScene;
		GetInstance()->m_pScene = nullptr;
	}
}

SceneManager* SceneManager::GetInstance()
{
	static SceneManager pSceneM;
	return &pSceneM;
}

void SceneManager::ChangeScene()
{
	SceneManager *ins = SceneManager::GetInstance();

	//シーン削除とシーン切り替え時間を設定
	if (ins->m_pScene != nullptr) delete ins->m_pScene;
	ins->m_pScene = nullptr;

	//シーンの切り替え（クラス）
	switch (ins->m_EScene)
	{
	case EScene::E_OPENING:
		ins->m_pScene = new Opening();
		break;

	case EScene::E_TITLE:
		ins->m_pScene = new Title();
		break;

	case EScene::E_BASE_CAMP:
		ins->m_pScene = new BaseCamp();
		break;

	case EScene::E_MAIN:
		ins->m_pScene = new MainGame();
		break;

	case EScene::E_GAME_END:
		ins->m_pScene = new GameEnd();
		break;

	case EScene::E_NONE:
		ins->m_pScene = new Title();

		break;
	default:
		break;
	}
}

void SceneManager::Quit(int time, EScene scene)
{
	SceneManager *ins = SceneManager::GetInstance();

	//一つのシーンで一回だけ呼べる
	if (!ins->m_IsQuit)
	{
		ins->m_IsQuit = true;	 //シーンを切り替える
		ins->m_QuitTime = time; //シーン切り替え時間
		ins->m_EScene = scene;	 //切り替えるシーン
	}
}

EScene SceneManager::GetScene()
{
	return GetInstance()->m_EScene;
}

void SceneManager::Clear()
{
	if (GetInstance()->m_pScene != nullptr)
	{
		delete GetInstance()->m_pScene;
		GetInstance()->m_pScene = nullptr;
	}
}

void SceneManager::Update()
{
	SceneManager *ins = SceneManager::GetInstance();

	//シーン切り替え判断
	if (ins->m_IsQuit)
	{
		ins->m_QuitCnt += 3;
		//シーン切り替え時間
		if (ins->m_QuitCnt > ins->m_QuitTime)
		{
			//シーン切り替え処理
			ins->ChangeScene();

			//初期化
			ins->m_IsQuit = false;
			ins->m_IsIn = true;
			ins->m_InCnt = ins->m_QuitTime;
			ins->m_QuitCnt = 0;
			ins->m_QuitTime = 0;
		}
	}

	//シーン切り替え後の処理
	if (ins->m_IsIn)
	{
		ins->m_InCnt -= 3;
		if (ins->m_InCnt <= 0)
		{
			ins->m_IsIn = false;
		}
	}

	//画面切り替え中は更新しない
	if (!ins->m_IsQuit && !ins->m_IsIn)
	{
		ins->m_pScene->Update(); //現在のシーン更新
	}
}

void SceneManager::Render()
{
	SceneManager *ins = SceneManager::GetInstance();

	//画面切り替え中は描画しない
	if (!ins->m_IsQuit && !ins->m_IsIn)
	{
		GetInstance()->m_pScene->Render(); //現在のシーンの描画
	}
}