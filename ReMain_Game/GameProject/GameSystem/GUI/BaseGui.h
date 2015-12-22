#ifndef BASE_GUI_H
#define BASE_GUI_H

#include "../../GEKO/GEKO.h"

class BaseGui : public Task
{
public:
	BaseGui();
	virtual ~BaseGui();
	void Update();
	void Draw();

protected:

	Image m_Image;
	Vector2D m_Pos;
	Vector2D m_Center;

	RenderTask m_RenderTask;
};

#endif