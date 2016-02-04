#ifndef _COLLIDER_H_
#define _COLLIDER_H_

#include "BaseCollider.h"

class Collider
{
public:
	Collider();
	~Collider();
	bool GetSleep() const;
	unsigned int GetMyID() const;
	unsigned int GetTargetID() const;
	const char* GetName() const;
	void Regist_S_vs_S(Vector3D *pPos, float *pRadius, std::function<void(Result_Sphere &)> func);
	void Regist_S_vs_C(Vector3D *pPos, float *pRadius, std::function<void(Result_Capsule &)> func);
	void Regist_S_vs_L(Vector3D *pPos, float *pRadius, std::function<void(Result_LineSegment &)> func);
	void Regist_S_vs_SMesh(Vector3D *pPos, float *pRadius, std::function<void(Result_Porygon_Group_Sphere &)> func);
	void Regist_C_vs_C(Vector3D *pStart, Vector3D *pEnd, float *pRadius, std::function<void(Result_Capsule &)> func);
	void Regist_C_vs_S(Vector3D *pStart, Vector3D *pEnd, float *pRadius, std::function<void(Result_Sphere &)> func);
	void Regist_C_vs_L(Vector3D *pPos, Vector3D *pEnd, float *pRadius, std::function<void(Result_LineSegment &)> func);
	void Regist_L_vs_L(Vector3D *pStart, Vector3D *pEnd, std::function<void(Result_LineSegment &)> func);
	void Regist_L_vs_S(Vector3D *pStart, Vector3D *pEnd, std::function<void(Result_Sphere &)> func);
	void Regist_L_vs_C(Vector3D *pStart, Vector3D *pEnd, std::function<void(Result_Capsule &)> func);
	void Regist_L_vs_SMesh(Vector3D *pStart, Vector3D *pEnd, std::function<void(Result_Porygon &)> func);
	void Regist_LGroup_vs_SMesh(Vector3D *pStart, Vector3D *pEnd, std::function<void(Result_Porygon_Group_LineSegment &)> func);
	void Regist_SMesh_vs_L(StaticMesh *pStaticMesh, bool isGPU);
	void Regist_SMesh_vs_S(StaticMesh *pStaticMesh);
	void SetID(unsigned int myId, unsigned int targetId);
	void SetName(const char *name);
	void Sleep();
	void Awake();
	void Release();

private:
	BaseCollider *m_pBaseCollider;

};

#endif