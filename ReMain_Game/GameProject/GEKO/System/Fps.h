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
	bool  m_IsControl; //fps���䔻�f
	int   m_StartTime; //����J�n����
	int	  m_Count;     //�J�E���^
	int   FPS;		   //�ݒ肵��FPS
	float m_Fps;       //fps
	float m_OneFps;    //�P�t���[��
};

#endif