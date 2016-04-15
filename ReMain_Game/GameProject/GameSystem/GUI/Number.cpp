#include "Number.h"
#include <math.h>

Number::Number() :
	m_Digit2Avtive(false)
{
	m_Img1.SetDrawRegister(true, 0, 0);
	m_Img1.SetAsset("Number");
	m_Img2.SetDrawRegister(true, 0, 0);
	m_Img2.SetAsset("Number");
}

Number::~Number()
{

}

void Number::NumDraw(const Vector2D pos, int number, const bool leftShift)
{
	if (number < 0) number = 0;
	if (99 < number) number = 99;

	if (number == 0)
	{
		m_Img1.SetPosition(pos.x + NUM_WIDTH, pos.y);
		m_Img1.SetDrawArea(0, 0, NUM_WIDTH, NUM_HEIGHT);
		m_Img1.SetSize(NUM_WIDTH, NUM_HEIGHT);
		return;
	}

	int digit1 = number % 10;
	int digit2 = number / 10;

	//ˆêŒ…–Ú
	m_Img1.SetPosition(pos.x + NUM_WIDTH, pos.y);
	m_Img1.SetDrawArea(NUM_WIDTH * digit1, 0, NUM_WIDTH * digit1 + NUM_WIDTH, NUM_HEIGHT);
	m_Img1.SetSize(NUM_WIDTH, NUM_HEIGHT);

	//“ñŒ…–Ú
	if (digit2 == 0)
	{
		//0‚ÌŽž‚Í“§–¾•”•ª‚ð•`‰æ
		m_Img2.SetPosition(pos.x, pos.y);
		m_Img2.SetDrawArea(260, 0, 281, 31);
		m_Img2.SetSize(NUM_WIDTH, NUM_HEIGHT);
	}
	else
	{
		//‚OˆÈŠO‚ÌŽž
		m_Img2.SetPosition(pos.x, pos.y);
		m_Img2.SetDrawArea(NUM_WIDTH * digit2, 0, NUM_WIDTH * digit2 + NUM_WIDTH, NUM_HEIGHT);
		m_Img2.SetSize(NUM_WIDTH, NUM_HEIGHT);
	}
}