#include "Lamp.h"

Lamp::Lamp(const Matrix *pParent) :
	Task("Lamp", 2)
{
	m_LampModel.SetRenderingRegister(true, 1, 0);
	m_LampModel.SetAsset("Lamp");
	m_LampModel.SetParentMatirx(pParent);
}

Lamp::~Lamp()
{
}

void Lamp::Update()
{

}