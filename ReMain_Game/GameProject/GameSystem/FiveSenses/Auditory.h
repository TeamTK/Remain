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
	void SetDetectionRange(float *pRange);
	void SetPos(Vector3D *pPos);
	void Stop();
	void Start();

private:
	bool m_IsStop; //�T�����~
	float *m_pDetectionRange; //��������͈́i�����������Ȃ������j
	std::function<void(int)> m_func; //�Ăяo���֐��|�C���^
	Vector3D *m_pPos; //�T���҂̈ʒu

};

//���o�T�m�Ώ�
class AuditoryObject
{
	friend AuditoryManager;

public:
	AuditoryObject();
	~AuditoryObject();
	void SetVolume(int *pVolume);
	void SetPos(Vector3D *pPos);

private:
	int *m_pVolume; //�o�����̗�
	Vector3D *m_pPos; //�T���Ώۂ̈ʒu
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