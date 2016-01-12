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
	void SetHearNum(int hearNum);
	void SetDetectionRange(float range);
	void SetPos(Vector3D *pPos);
	void Stop();
	void Start();

private:
	bool m_IsStop;
	int m_HearNum;
	float m_DetectionRange;
	std::function<void(int)> m_func;
	Vector3D *m_pPos;

};

//聴覚探知対象
class AuditoryObject
{
	friend AuditoryManager;

public:
	AuditoryObject();
	~AuditoryObject();
	void SetVolume(int volume);
	void SetDetectionRange(float range);
	void SetPos(Vector3D *pPos);

private:
	int m_Volume;
	float m_DetectionRange;
	Vector3D *m_pPos;
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