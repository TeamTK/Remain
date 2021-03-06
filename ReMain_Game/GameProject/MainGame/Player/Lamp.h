#ifndef _LAMP_H_
#define _LAMP_H_

#include "../../GEKO/GEKO.h"

class Lamp : public Task
{
public:
	Lamp(const Matrix *pParent);
	~Lamp();
	void Update();

private:
	StaticMesh m_LampModel;
	const Matrix *m_pParentMatrix;
	PointLight m_PointLight;
};

#endif