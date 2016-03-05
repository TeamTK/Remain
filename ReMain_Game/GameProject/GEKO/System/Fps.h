#ifndef _FPS_H_
#define _FPS_H_

class Fps
{
private:
	int   m_StartTime; //測定開始時刻
	int	  m_Count;     //カウンタ
	int   FPS;		   //設定したFPS
	float m_Fps;       //fps
	float m_OneFps;    //１フレーム

private:
	Fps();
	static Fps* const GetInstance();

public:
	static float GetFPS();
	static float GetOneFPS();
	static int GetSettingFPS();
	static void SetFps(int fps);
	static void Update();
	static void Wait();
};

#endif