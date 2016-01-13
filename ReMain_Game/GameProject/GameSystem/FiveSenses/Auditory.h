#ifndef _AUDITORY_H_
#define _AUDITORY_H_

#include <functional>

class AuditoryManager;
class Vector3D;

//聴覚探知
class AuditorySense
{
	friend AuditoryManager;

public:
	AuditorySense();
	~AuditorySense();
	void SetFunc(std::function<void(int)> func);
	void SetDetectionRange(float *pRange);
	void SetPos(Vector3D *pPos);
	void Stop();
	void Start();

private:
	bool m_IsStop; //探索を停止
	float *m_pDetectionRange; //聞こえる範囲（平方根をしない長さ）
	std::function<void(int)> m_func; //呼び出す関数ポインタ
	Vector3D *m_pPos; //探索者の位置

};

//聴覚探知対象
class AuditoryObject
{
	friend AuditoryManager;

public:
	AuditoryObject();
	~AuditoryObject();
	void SetVolume(int *pVolume);
	void SetPos(Vector3D *pPos);

private:
	int *m_pVolume; //出す音の量
	Vector3D *m_pPos; //探索対象の位置
};

//聴覚管理
class AuditoryManager
{
public:
	~AuditoryManager();
	static AuditoryManager* GetInstance();
	void AddAuditorySense(AuditorySense *pAuditorySense);
	void AddAuditoryObject(AuditoryObject *pAuditoryObject);
	void ClaerAuditorySense(AuditorySense *pAuditorySense);
	void ClaerAuditoryObject(AuditoryObject *pAuditoryObject);
	void AllClaer();
	void Update();

private:
	AuditoryManager();
	class AuditoryPimpl;
	AuditoryPimpl *m_pAuditoryPimpl;
};

#endif