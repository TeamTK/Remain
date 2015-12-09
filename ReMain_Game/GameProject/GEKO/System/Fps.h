#ifndef _FPS_H_
#define _FPS_H_

class Fps
{
private:
	int   m_StartTime; //測定開始時刻
	int	  m_Count;     //カウンタ
	float m_Fps;       //fps
	int   N;           //平均を取るサンプル数
	int   FPS;		   //設定したFPS

private:
	Fps();
	static Fps* const GetInstance();

public:
	static void SetFps(int fps);
	static void Update();
	static void Wait();
};

#endif