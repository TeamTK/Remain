#include "Lamp.h"

Lamp::Lamp(const Matrix *pParent) :
	Task("Lamp", 2),
	m_pParentMatrix(pParent)
{
	m_LampModel.SetRenderingRegister(true, 1, 0);
	m_LampModel.SetAsset("Lamp");
	m_LampModel.SetRotationDegree(-90, 0, 0);
	m_PointLight.SetRange(10.0f);
	m_PointLight.SetIntensity(2.0f);
}

Lamp::~Lamp()
{
}

void Lamp::Update()
{
	m_LampModel.SetTranselate(0.2f, -0.3f, 0.15f);
	m_LampModel.SetParentMatirx(*m_pParentMatrix);

	Matrix mat = *m_LampModel.GetModelMatrix();
	m_PointLight.SetPosition(mat._41, mat._42, mat._43);
}