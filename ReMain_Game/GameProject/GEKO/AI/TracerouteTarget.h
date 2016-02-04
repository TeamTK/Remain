#ifndef _TRACEROUTE_TARGET_H_
#define _TRACEROUTE_TARGET_H_

class Vector3D;
class TracerouteManager;

class TracerouteTarget
{
	friend TracerouteManager;

public:
	TracerouteTarget();
	~TracerouteTarget();
	void SetName(const char* name);
	void SetPosition(Vector3D *pPosition);

private:
	Vector3D *m_pPosition;
	const char* m_Name;
};

#endif