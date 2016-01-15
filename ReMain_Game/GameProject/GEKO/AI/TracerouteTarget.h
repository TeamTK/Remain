#ifndef _TRACEROUTE_TARGET_H_
#define _TRACEROUTE_TARGET_H_

class Vector3D;

class TracerouteTarget
{
public:
	TracerouteTarget();
	~TracerouteTarget();
	void SetPosition(Vector3D *pPosition);

private:
	Vector3D *m_pPosition;
};

#endif