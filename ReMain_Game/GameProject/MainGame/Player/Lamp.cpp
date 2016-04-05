#include "Lamp.h"

Lamp::Lamp(const Matrix *pParent) :
	Task("Lamp", 2),
	m_pParentMatrix(pParent)
{
	m_LampModel.SetRenderingRegister(true, 1, 0);
	m_LampModel.SetAsset("Lamp");
}

Lamp::~Lamp()
{
}

void Lamp::Update()
{
	m_LampModel.SetParentMatirx(*m_pParentMatrix);
}