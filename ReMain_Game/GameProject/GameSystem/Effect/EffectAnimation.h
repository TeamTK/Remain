#ifndef _EFFECT_ANIMATION_H_
#define _EFFECT_ANIMATION_H_

#include "..\..\GEKO\Figure\Billboard.h"
#include "..\..\GEKO\Task\RenderTask.h"

//�G�t�F�N�g�A�j���[�V�����N���X�̐ݒ���
struct EffectAnimationInfo
{
	Vector3D pos;   //�ʒu
	int frameNum;  //�A�j���[�V��������
	int sizeW;		//��R�}�̃T�C�Y�i���j
	int sizeH;		//��R�}�̃T�C�Y�i�c�j
	float size;		//�S�̂̃T�C�Y
	float speed;	//�A�j���[�V�����؂�ւ����x
};

//�r���{�[�h���g�����A�j���[�V�����G�t�F�N�g
class EffectAnimation : public Task
{
public:
	EffectAnimation(const std::string &assetName, const EffectAnimationInfo &info);
	~EffectAnimation();
	void Update();
	void Render();

private:
	RenderTask m_Render;
	BillboardAnimation m_Anim;
	Vector3D m_Pos;
	float m_Size;
	float m_Speed;
};

#endif