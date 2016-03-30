#ifndef _FPS_H_
#define _FPS_H_

class Fps
{
public:
	static float GetFPS();
	static float GetOneFPS();
	static int GetSettingFPS();
	static void SetFps(int fps);
	static void SetControl(bool isControl);
	static void Update();
	static void Wait();

private:
	Fps();
	static Fps* const GetInstance();

private:
	bool  m_IsControl; //fps制御判断
	int   m_StartTime; //測定開始時刻
	int	  m_Count;     //カウンタ
	int   FPS;		   //設定したFPS
	float m_Fps;       //fps
	float m_OneFps;    //１フレーム
};

#endif