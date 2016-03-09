#ifndef _MESHBASE_H_
#define _MESHBASE_H_

#include "StaticMesh\StaticMeshAsset.h"
#include "WorldMatrixManager.h"

class Image;

//StaticMesh�EDynamicMesh�^�̊�ꃁ�b�V����̋��ʍ��ڂ��`
class MeshBase
{
public:
	MeshBase();
	~MeshBase();
	void SetScale(float x, float y, float z);
	void SetRotationRadian(float x, float y, float z);
	void SetRotationDegree(int x, int y, int z);
	void SetTranselate(float x, float y, float z);
	void SetTranselate(const Vector3D &pos);
	void SetDiffuse(int r, int g, int b, int materialIndex);
	void SetSpecular(int r, int g, int b, int materialIndex);
	void SetAmbient(int r, int g, int b, int materialIndex);
	void SetAlpha(int alpha, int materialIndex);
	void SetAlphaAll(int alpha);
	void SetTexture(Image *pImage);
	Vector3D GetScale() const;
	Vector3D GetRotation() const;
	Vector3D GetTranselate() const;
	Vector3D GetDiffuse(int materialIndex) const;
	Vector3D GetSpecular(int materialIndex) const;
	Vector3D GetAmbient(int materialIndex) const;
	Vector3D GetAxisX(float length) const;
	Vector3D GetAxisX(const Matrix &matrix, float length) const;
	Vector3D GetAxisY(float length) const;
	Vector3D GetAxisY(const Matrix &matrix, float length) const;
	Vector3D GetAxisZ(float length) const;
	Vector3D GetAxisZ(const Matrix &matrix, float length) const;
	const Matrix *GetWorldMatrix() const;
	const Matrix *GetSynthesisMatrix() const;
	void DebugAxis(const Matrix &matrix);
	void DebugAxis();

protected:
	Matrix m_WorldMatrix;			   //���f���̃��[���h�s��
	Matrix m_SynthesisMatrix;		   //���f���̃��[�J�� �~ ���[���h�s��
	Vector3D m_Rotation;			   //���f���̉�]�l�iX���EY���EZ���j
	Vector3D m_Scale;				   //���f���̊g�嗦�iX���EY���EZ���j
	Vector3D m_Transelate;			   //���f���̕��s�ړ��iX���EY���EZ���j
	WorldMatrixInfo m_WorldMatrixInfo; //���f���̉�]�E�g��E���s�ړ��𑼂̏ꏊ�Ōv�Z���邽�߂̏��
	std::vector<Vector4D> m_Diffuse;   //���f���̊g�U���˗�
	std::vector<Vector3D> m_Specular;  //���f���̋��ʔ��˗�
	std::vector<Vector3D> m_Ambient;   //���f���̊�����
	Image *m_pImage;				   //�}���`�e�N�X�`���K�p�̉摜
};

#endif