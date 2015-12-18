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

	//�V�[���폜�ƃV�[���؂�ւ����Ԃ�ݒ�
	if (ins->m_pScene != nullptr) delete ins->m_pScene;
	ins->m_pScene = nullptr;

	//�V�[���̐؂�ւ��i�N���X�j
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

	//��̃V�[���ň�񂾂��Ăׂ�
	if (!ins->m_IsQuit)
	{
		ins->m_IsQuit = true;	 //�V�[����؂�ւ���
		ins->m_QuitTime = time; //�V�[���؂�ւ�����
		ins->m_EScene = scene;	 //�؂�ւ���V�[��
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

	//�V�[���؂�ւ����f
	if (ins->m_IsQuit)
	{
		ins->m_QuitCnt += 3;
		//�V�[���؂�ւ�����
		if (ins->m_QuitCnt > ins->m_QuitTime)
		{
			//�V�[���؂�ւ�����
			ins->ChangeScene();

			//������
			ins->m_IsQuit = false;
			ins->m_IsIn = true;
			ins->m_InCnt = ins->m_QuitTime;
			ins->m_QuitCnt = 0;
			ins->m_QuitTime = 0;
		}
	}

	//�V�[���؂�ւ���̏���
	if (ins->m_IsIn)
	{
		ins->m_InCnt -= 3;
		if (ins->m_InCnt <= 0)
		{
			ins->m_IsIn = false;
		}
	}

	//��ʐ؂�ւ����͍X�V���Ȃ�
	if (!ins->m_IsQuit && !ins->m_IsIn)
	{
		ins->m_pScene->Update(); //���݂̃V�[���X�V
	}
}

void SceneManager::Render()
{
	SceneManager *ins = SceneManager::GetInstance();

	//��ʐ؂�ւ����͕`�悵�Ȃ�
	if (!ins->m_IsQuit && !ins->m_IsIn)
	{
		GetInstance()->m_pScene->Render(); //���݂̃V�[���̕`��
	}
}