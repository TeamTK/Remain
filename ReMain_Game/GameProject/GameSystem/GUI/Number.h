#ifndef NUMBER_H
#define NUMBER_H

#define NUM_WIDTH 22
#define NUM_HEIGHT 32

#include "../../GEKO/GEKO.h"

class Number
{
public:
	Number();
	~Number();

	/// <summary>
	/// ������`�悵�܂�
	/// </summary>
	/// <param name="x">
	/// �`�悵��������X���W
	/// </param>
	/// <param name="y">
	/// �`�悵��������Y���W
	/// </param>
	/// <param name="y">
	/// �`�悵��������
	/// </param>
	/*
	/// <param name="scroll">
	/// �������������A1���ڂ���ɍ��E�ǂ���Ɍ��𑝂₷��
	/// true = �E���Ɍ��𑝂₷
	/// false = �����Ɍ��𑝂₷
	/// </param>
	*/
	void NumDraw(int x, int y, int num);

private:
	Image m_Img;
	int m_Num;
	int m_x;
	int m_y;
};

#endif