#ifndef _SCENE_H_
#define _SCENE_H_

#include <iostream>

enum class EScene
{
	E_OPENING,
	E_TITLE,
	E_BASE_CAMP,
	E_MAIN,
	E_GAME_END,
	E_NONE
};

class Scene
{
public:
	Scene();
	virtual ~Scene();
	virtual void Update() = 0;
	virtual void Render() = 0;

private:

};

class SceneManager
{
public:
	~SceneManager();
	static void Quit(int time, EScene scene);
	static EScene GetScene();
	static void Clear();
	static void Update();
	static void Render();

private:
	SceneManager();
	inline static SceneManager* GetInstance();
	static void ChangeScene();

private:
	Scene *m_pScene;
	EScene m_EScene;
	bool m_IsQuit;
	int  m_QuitCnt;
	int  m_QuitTime;
	bool m_IsIn;
	int  m_InCnt;
	bool m_IsEnd;
};

class None : public Scene
{
public:
	None(){};
	~None(){};
	void Render() { std::cout << "No" << "\n"; };
	void Update(){};

private:

};

#endif