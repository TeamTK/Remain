#ifndef _COLLIDER_MANAGER_H_
#define _COLLIDER_MANAGER_H_

//球
class SphereCollider;
class Sphere_vs_CapsuleCollider;
class Sphere_vs_LineSegmentCollider;
class Sphere_vs_StaticMeshCollider;

//カプセル
class CapsuleCollider;
class Capsule_vs_SphereCollider;
class Capsule_vs_LineSegmentCollider;

//線分
class LineSegmentCollider;
class LineSegment_vs_SphereCollider;
class LineSegment_vs_CapsuleCollider;
class LineSegment_vs_StaticMeshCollider;

//StaticMesh
class StaticMesh_vs_LineSegmentCollider;
class StaticMesh_vs_SphereCollider;
class StaticMesh;

class Vector3D;

struct SphereHitData;
struct CapsuleHitData;
struct LineSegmentHitData;
struct Result_Porygon;
struct Result_Porygon_Group;

class ColliderManager
{
	//球
	friend SphereCollider;
	friend Sphere_vs_CapsuleCollider;
	friend Sphere_vs_LineSegmentCollider;
	friend Sphere_vs_StaticMeshCollider;

	//カプセル
	friend CapsuleCollider;
	friend Capsule_vs_SphereCollider;
	friend Capsule_vs_LineSegmentCollider;

	//線分
	friend LineSegmentCollider;
	friend LineSegment_vs_SphereCollider;
	friend LineSegment_vs_CapsuleCollider;
	friend LineSegment_vs_StaticMeshCollider;

	//StaticMesh
	friend StaticMesh_vs_LineSegmentCollider;
	friend StaticMesh_vs_SphereCollider;
	friend StaticMesh;

public:
	~ColliderManager();
	static ColliderManager *GetInstance();
	void Update();
	void AllClear();
	bool HitCheckStaticMesh_Line(Result_Porygon *resultPorygon, Vector3D *start, Vector3D *end, unsigned int targetId);

private:
	ColliderManager();
	void Add(SphereCollider *pSphere);
	void Add(Sphere_vs_CapsuleCollider *pSphere);
	void Add(Sphere_vs_LineSegmentCollider *pSphere);
	void Add(Sphere_vs_StaticMeshCollider *pSphere);
	void Add(CapsuleCollider *pCapsule);
	void Add(Capsule_vs_SphereCollider *pCapsule);
	void Add(Capsule_vs_LineSegmentCollider *pCapsule);
	void Add(LineSegmentCollider *pLineSegment);
	void Add(LineSegment_vs_SphereCollider *pLineSegment);
	void Add(LineSegment_vs_CapsuleCollider *pLineSegment);
	void Add(LineSegment_vs_StaticMeshCollider *pLineSegment);
	void Add(StaticMesh_vs_LineSegmentCollider *pStaticMesh);
	void Add(StaticMesh_vs_SphereCollider *pStaticMesh);
	void Clear(SphereCollider *pSphere);
	void Clear(Sphere_vs_CapsuleCollider *pSphere);
	void Clear(Sphere_vs_LineSegmentCollider *pSphere);
	void Clear(Sphere_vs_StaticMeshCollider *pSphere);
	void Clear(CapsuleCollider *pCapsule);
	void Clear(Capsule_vs_SphereCollider *pCapsule);
	void Clear(Capsule_vs_LineSegmentCollider *pCapsule);
	void Clear(LineSegmentCollider *pLineSegment);
	void Clear(LineSegment_vs_SphereCollider *pLineSegment);
	void Clear(LineSegment_vs_CapsuleCollider *pLineSegment);
	void Clear(LineSegment_vs_StaticMeshCollider *pLineSegment);
	void Clear(StaticMesh_vs_LineSegmentCollider *pStaticMesh);
	void Clear(StaticMesh_vs_SphereCollider *pStaticMesh);
	void CheckSphere();
	void CheckSphere_vs_Capsule();
	void CheckSphere_vs_LineSegment();
	void CheckCapsule();
	void CheckCapsule_vs_LineSegment();
	void CheckLineSegment();
	void CheckStaitcMesh_vs_LineSegment();
	void CheckStaitcMesh_vs_Sphere();
	bool HitCheckSphere(SphereHitData &pHitData1, SphereHitData &pHitData2);
	bool HitCheckSphere_vs_Capsule(SphereHitData &pHitData1, CapsuleHitData &pHitData2);
	bool HitCheckSphere_vs_LineSegment(SphereHitData &pHitData1, LineSegmentHitData &pHitData2);
	bool HitCheckCapsule(CapsuleHitData &pHitData1, CapsuleHitData &pHitData2);
	bool HitCheckCapsule_vs_LineSegment(CapsuleHitData &pHitData1, LineSegmentHitData &pHitData2);
	bool HitCheckLineSegment(LineSegmentHitData &pHitData1, LineSegmentHitData &pHitData2);
	bool HitCheckStaticMesh_vs_LineSegment(StaticMesh &hitData1, LineSegmentHitData &hitData2, Result_Porygon *pory);
	bool HitCheckStaticMesh_vs_Sphere(StaticMesh &hitData1, SphereHitData &hitData2, Result_Porygon_Group *pory);

private:
	class ColliderListPimpl;
	ColliderListPimpl *m_pColliderListPimpl;
};

#endif