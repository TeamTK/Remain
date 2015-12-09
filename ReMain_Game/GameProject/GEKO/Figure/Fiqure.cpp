#include "Fiqure.h"
#include "Line3D.h"
#include "Triangle3D.h"

class Fiqure::FiqurePimpl
{
public:
	Line3D m_Line3D;
	Triangle3D m_Triangle3D;
};

Fiqure::Fiqure()
{
	m_Fiqure = new FiqurePimpl();
}

Fiqure::~Fiqure()
{
	delete m_Fiqure;
}

Fiqure* Fiqure::GetInstance()
{
	static Fiqure fiqure;
	return &fiqure;
}

void Fiqure::RenderLine3D(const Vector3D &Spos, const Vector3D &Epos, const Vector3D &color)
{
	GetInstance()->m_Fiqure->m_Line3D.Render(Spos, Epos, color);
}

void Fiqure::RenderTriangle3D(const Vector3D &pos1, const Vector3D &pos2, const Vector3D &pos3, const Vector3D &color)
{
	GetInstance()->m_Fiqure->m_Triangle3D.Render(pos1, pos2, pos3, color);
}