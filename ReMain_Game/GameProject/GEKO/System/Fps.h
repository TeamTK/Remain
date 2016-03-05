#ifndef _FPS_H_
#define _FPS_H_

class Fps
{
private:
	int   m_StartTime; //����J�n����
	int	  m_Count;     //�J�E���^
	int   FPS;		   //�ݒ肵��FPS
	float m_Fps;       //fps
	float m_OneFps;    //�P�t���[��

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