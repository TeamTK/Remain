#include "Number.h"
#include <math.h>

Number::Number()
{
	m_Img.SetDrawRegister(true, 0, 0);
	m_Img.SetAsset("Number");
}

Number::~Number()
{

}

void Number::NumDraw(const Vector2D pos, int number, const bool leftShift)
{
	if (number < 0) number = 0;
	if (9999999 < number) number = 9999999;

	if (number == 0)
	{
		m_Img.SetDrawArea(0, 0, NUM_WIDTH, NUM_HEIGHT);
		m_Img.SetSize(NUM_WIDTH, NUM_HEIGHT);
		m_Img.SetPosition((pos.x - NUM_WIDTH * 8) + NUM_WIDTH * 8, pos.y);
		return;
	}

	if (leftShift)
	{
		bool shift = false;

		for (int i = 7; 0 <= i; i--)
		{
			int num = (int)(number / std::pow(10, i)) % 10;

			if (num == 0 && !shift) continue;
			else shift = true;

			m_Img.SetDrawArea(NUM_WIDTH * num, 0, NUM_WIDTH * num + NUM_WIDTH, NUM_HEIGHT);
			m_Img.SetSize(NUM_WIDTH, NUM_HEIGHT);
			m_Img.SetPosition((pos.x - NUM_WIDTH * 8) + NUM_WIDTH * (8 - i), pos.y);
		}
	}
	else
	{
		int shift = -1;

		for (int i = 7; 0 <= i; i--)
		{
			int num = (int)(number / std::pow(10, i)) % 10;

			if (num == 0 && shift == -1) continue;
			else shift++;

			m_Img.SetDrawArea(NUM_WIDTH * num, 0, NUM_WIDTH * num + NUM_WIDTH, NUM_HEIGHT);
			m_Img.SetSize(NUM_WIDTH, NUM_HEIGHT);
			m_Img.SetPosition(pos.x + NUM_WIDTH * shift, pos.y);
		}
	}

}