#include "CBullet.h"
static void func(Result_Capsule& r) {

}
CBullet::CBullet() : m_isActive(TRUE), m_cnt(0), m_Atk(1){
	m_Sphere.radius = 0.0f;
	m_Collider.Regist_S_vs_C(&m_pos, &m_Sphere.radius, REGIST_FUNC(CBullet::LineSegment_vs_CapsuleCallback));
	m_Collider.SetID(eHITID3, eHITID2);

	m_ColliderMap.Regist_L_vs_SMesh(&m_Oldpos, &m_pos, REGIST_FUNC(CBullet::LineSegment_vs_MeshCallback));
	m_ColliderMap.SetID(eHITID3, eHITID0);

}
CBullet::~CBullet() {
	m_Collider.Release();
	m_ColliderMap.Release();
}
bool CBullet::UpDate(){
	m_Oldpos = m_pos;
	m_pos += m_dir*m_speed;			//移動

	//寿命
	if(++m_cnt>240) m_isActive=false;
	return false;
}
void CBullet::Render(){
	//デプスバッファへの書き込み不許可
	//glDepthMask(GL_FALSE);
	//ライティング無効
	//CLight::setLighting(false);
	//加算ブレンド
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	//モデルの設定
	m_Model.SetTranselate(m_pos);
//	m_Model.SetRotationRadian(asin(-m_dir.y), atan2f(m_dir.x, m_dir.z), 0);
	m_Model.Render();
	//モデルの設定
//	m_pModel[1].SetTranselate(m_pos);
//	m_pModel[1].RenderBuilbord(CCamera::getCurrent()->getBuilbordMatrix());

	//元の設定に戻す
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//CLight::setLighting(true);
	//glDepthMask(GL_TRUE);
	
}
void CBullet::LineSegment_vs_CapsuleCallback(Result_Capsule& r) {
	m_isActive = false;
}
void CBullet::LineSegment_vs_MeshCallback(Result_Porygon& r) {
	m_isActive = false;
}
CBulletManager* CBulletManager::m_Obj = NULL;
CBulletManager::CBulletManager() {
	StaticMeshAsset::LoadMesh("media\\bullet.x", "Bullet");
}
CBulletManager::~CBulletManager(){
	auto it = m_Core.begin();
	while (it != m_Core.end()) {
		//削除処理
		delete *it;
		it = m_Core.erase(it);
	}

}
void CBulletManager::UpDate(){
	auto it=m_Core.begin();
	while(it!=m_Core.end()) {
		CBullet *b = *it;
		//生存していれば
		if(b->m_isActive) {
			if(b->UpDate()) {
				it= m_Core.begin();
				while(b != *it && it!= m_Core.end()) it++;
			}
			it++;
		} else {
			//削除処理
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
/*
void CBulletManager::CollisionMap(CModel *m){
	std::vector<CBullet*>::iterator it = m_core.begin();
	while (it != m_core.end()) {

		if ((*it)->m_isActive) (*it)->CollisionMap(m);
		it++;
	}
}
*/
CBullet *CBulletManager::Add(const Vector3D pos, const Vector3D &dir,const float speed) {
	CBullet *p = new CBullet();
	if(!p) return NULL;
	//各種設定
	p->m_pos = pos;
	p->m_dir = dir;
	p->m_Model.SetAsset("Bullet");
	p->m_Model.SetTranselate(pos);
	p->m_Model.SetScale(0.1f, 0.1f, 0.1f);
	p->m_Model.SetRotationRadian(asin(-dir.y), atan2f(dir.x, dir.z), 0);
//	p->m_Model.UpDateMatrix();
	
	p->m_speed = speed;
	//コンテナへ追加
	m_Core.push_back(p);
	return p;
}

void CBulletManager::AllClear()
{
	auto it = m_Core.begin();
	for (; it != m_Core.end();)
	{
		delete *it;
		it = m_Core.erase(it);
	}
	m_Core.clear();
}