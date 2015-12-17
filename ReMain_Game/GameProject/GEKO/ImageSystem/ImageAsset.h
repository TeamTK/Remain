#ifndef _IMAGE_ASSET_H_
#define _IMAGE_ASSET_H_

#include "..\System\Direct3D11.h"

//�V�F�[�_�[�p�̃R���X�^���g�o�b�t�@�[�̃A�v�����\����
struct SHADER_CONSTANT_BUFFER
{
	ALIGN16 D3DXMATRIX mW;
	ALIGN16 float ViewPortWidth;
	ALIGN16 float ViewPortHeight;
	ALIGN16 D3DXVECTOR4 Color;
};

struct ImageInfo
{
	ID3D11InputLayout*  pVertexLayout;	//���_���C�A�E�g
	ID3D11VertexShader* pVertexShader;	//���_�V�F�[�_�[
	ID3D11PixelShader*	pPixelShader;		//�s�N�Z���V�F�[�_�[
	ID3D11Buffer*		pConstantBuffer;	//�R���X�^���g�o�b�t�@�[
	ID3D11Buffer*		pVertexBuffer;	//���_�o�b�t�@�[
	ID3D11SamplerState* pSampler;			//�e�N�X�`���[�̃T���v���[
	ID3D11ShaderResourceView* pTexture;	//�e�N�X�`���[���\�[�X�r���[
	int Width;  //�O�̉摜�̕�
	int Height; //�O�̉摜�̍���
};

class ImageData
{
public:
	ImageData(std::string fileName);
	~ImageData();
	ImageInfo *GetImageInfo();
	void Relese();

private:
	ImageInfo m_ImageInfo;
};

class ImageAsset
{
public:
	~ImageAsset();
	static ImageData* GetImage(std::string name);
	static void LoadAsset(std::string filmeName, std::string name);
	static void LoadFile(std::string filmeName);
	static void PartClear(std::string name);
	static void AllClear();
	static void DebugDraw();

private:
	ImageAsset();
	static ImageAsset *GetInstance();

private:
	class ImagePimpl;
	ImagePimpl *m_pImagePimpl;
};

#endif