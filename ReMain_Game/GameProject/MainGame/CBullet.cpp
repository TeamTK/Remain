#include "CBullet.h"

CBullet::CBullet() : m_isActive(TRUE), m_cnt(0), m_Atk(1){
	//, m_LineSegmentCollider(&m_Oldpos,&m_pos, std::bind(&CBullet::LineSegment_vs_CapsuleCallback, std::ref(*this), std::placeholders::_1)){
//	m_LineSegmentCollider.SetID(eHITID3, eHITID0| eHITID2);
//	ColliderManager::GetInstance()->Add(&m_LineSegmentCollider);

}
CBullet::~CBullet() {
//	ColliderManager::GetInstance()->Clear(&m_LineSegmentCollider);
}
bool CBullet::UpDate(){
	m_Oldpos = m_pos;
	m_pos += m_dir*m_speed;			//�ړ�

	//����
	if(++m_cnt>240) m_isActive=false;
	return false;
}
void CBullet::Render(){
	//�f�v�X�o�b�t�@�ւ̏������ݕs����
	//glDepthMask(GL_FALSE);
	//���C�e�B���O����
	//CLight::setLighting(false);
	//���Z�u�����h
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	//���f���̐ݒ�
	m_Model.SetTranselate(m_pos);
//	m_Model.SetRotationRadian(asin(-m_dir.y), atan2f(m_dir.x, m_dir.z), 0);
	m_Model.Render();
	//���f���̐ݒ�
//	m_pModel[1].SetTranselate(m_pos);
//	m_pModel[1].RenderBuilbord(CCamera::getCurrent()->getBuilbordMatrix());

	//���̐ݒ�ɖ߂�
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//CLight::setLighting(true);
	//glDepthMask(GL_TRUE);
	
}
/*
void CBullet::LineSegment_vs_CapsuleCallback(Result_Capsule& r) {
	m_isActive = false;
}
*/
CBulletManager* CBulletManager::m_Obj = NULL;
CBulletManager::CBulletManager() {
	StaticMeshAsset::LoadMesh("media\\bullet.x", "Bullet");
}
CBulletManager::~CBulletManager(){
	auto it = m_Core.begin();
	while (it != m_Core.end()) {
		//�폜����
		delete *it;
		it = m_Core.erase(it);
	}

}
void CBulletManager::UpDate(){
	auto it=m_Core.begin();
	while(it!=m_Core.end()) {
		CBullet *b = *it;
		//�������Ă����
		if(b->m_isActive) {
			if(b->UpDate()) {
				it= m_Core.begin();
				while(b != *it && it!= m_Core.end()) it++;
			}
			it++;
		} else {
			//�폜����
			delete *it;
			it = m_Core.erase(it);
		}
	}
}
void CBulletManager::Render(){
	auto it = m_Core.begin();
	while(it!= m_Core.end()) {
		(*it)->Render();
		it++;
	}
}
CBullet *CBulletManager::Add(const Vector3D pos, const Vector3D &dir,const float speed) {
	CBullet *p = new CBullet();
	if(!p) return NULL;
	//�e��ݒ�
	p->m_pos = pos;
	p->m_dir = dir;
	p->m_Model.SetAsset("Bullet");
	p->m_Model.SetTranselate(pos);
	p->m_Model.SetScale(0.1f, 0.1f, 0.1f);
	p->m_Model.SetRotationRadian(asin(-dir.y), atan2f(dir.x, dir.z), 0);
//	p->m_Model.UpDateMatrix();
	
	p->m_speed = speed;
	//�R���e�i�֒ǉ�
	m_Core.push_back(p);
	return p;
}