#include "Number.h"
#include <math.h>

Number::Number()
{
	m_Img.SetAsset("Number");
}

Number::~Number()
{

}

void Number::NumDraw(Vector2D pos, int num)
{
	m_Num = num;

	if (m_Num <= 0)m_Num = 0;
	if (m_Num >= 99999999) m_Num = 99999999;

	//åÖêî
	int digits = (int)log10((double)m_Num) + 1;

	if (num == 0)
	{
		m_Img.SetDrawPos(0, 0, NUM_WIDTH, NUM_HEIGHT);
		m_Img.SetSize(NUM_WIDTH, NUM_HEIGHT);
		m_Img.Draw((int)(pos.x - NUM_WIDTH * 8) + NUM_WIDTH * 8, (int)pos.y);
		return;
	}

	m_x = (int)pos.x + (digits - 1) * NUM_WIDTH;
	for (int i = 0; i < digits; i++)
	{
		int DrawNum = m_Num % 10;

		m_Img.SetDrawPos(DrawNum * NUM_WIDTH, 0, (DrawNum + 1) * NUM_WIDTH, NUM_HEIGHT);
		m_Img.SetSize(NUM_WIDTH, NUM_HEIGHT);
		m_Img.Draw(m_x, (int)pos.y);
		m_x -= NUM_WIDTH;
		m_Num /= 10;

	}
}
