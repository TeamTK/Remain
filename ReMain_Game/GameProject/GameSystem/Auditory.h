#ifndef _AUDITORY_H_
#define _AUDITORY_H_

#include <functional>

class AuditoryManager;
class Vector3D;

//���o�T�m
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

//���o�T�m�Ώ�
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

//���o�Ǘ�
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