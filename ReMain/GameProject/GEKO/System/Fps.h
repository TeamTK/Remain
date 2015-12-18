#ifndef _FPS_H_
#define _FPS_H_

class Fps
{
private:
	int   m_StartTime; //����J�n����
	int	  m_Count;     //�J�E���^
	float m_Fps;       //fps
	int   N;           //���ς����T���v����
	int   FPS;		   //�ݒ肵��FPS

private:
	Fps();
	static Fps* const GetInstance();

public:
	static void SetFps(int fps);
	static void Update();
	static void Wait();
};

#endif