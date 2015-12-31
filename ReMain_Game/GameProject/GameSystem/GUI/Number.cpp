#include "Number.h"
#include <math.h>

Number::Number()
{
	m_Img.SetAsset("Number");
}

Number::~Number()
{

}

void Number::NumDraw(int x, int y, int num)
{
	m_Num = num;
	m_y = y;

	if (m_Num <= 0)m_Num = 0;
	if (m_Num >= 99999999) m_Num = 99999999;

	//Œ…”
	int digits = (int)log10((double)m_Num) + 1;

	m_x = x + (digits - 1) * NUM_WIDTH;
	for (int i = 0; i < digits; i++)
	{
		int DrawNum = m_Num % 10;

		m_Img.SetDrawPos(DrawNum * NUM_WIDTH, 0, (DrawNum + 1) * NUM_WIDTH, NUM_HEIGHT);
		m_Img.SetSize(NUM_WIDTH, NUM_HEIGHT);
		m_Img.Draw(m_x, m_y);
		m_x -= NUM_WIDTH;
		m_Num /= 10;

	}
}
