#ifndef _TRACEROUTE_TARGET_H_
#define _TRACEROUTE_TARGET_H_

#include <string>

class Vector3D;
class TracerouteManager;

class TracerouteTarget
{
	friend TracerouteManager;

public:
	TracerouteTarget();
	~TracerouteTarget();
	void SetName(std::string name);
	void SetPosition(Vector3D *pPosition);

private:
	Vector3D *m_pPosition;
	std::string m_Name;
};

#endif