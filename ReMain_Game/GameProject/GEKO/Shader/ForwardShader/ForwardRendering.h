#ifndef _FORWARD_RENDERING_H_
#define _FORWARD_RENDERING_H_

#include "..\ShaderInfo.h"

//StaticMesh�^�̐ÓI��3D���f���̃V�F�[�_�[���Ǘ�
class ForwardRendering
{
public:
	~ForwardRendering();
	static ForwardRendering* GetInstance();
	bool Init();
	void SetStaticMeshShader(ID3D11DeviceContext *pDeviceContext, unsigned int state);
	void SetDynamicMeshShader(ID3D11DeviceContext *pDeviceContext, unsigned int state);
	void Rendering();
	void Release();	

private:
	ForwardRendering();
	bool InitStaticMesh(ID3DBlob *pCompiledShader, ID3DBlob *pErrors);
	bool InitDynamicMesh(ID3DBlob *pCompiledShader, ID3DBlob *pErrors);
	bool InitBoard(ID3DBlob *pCompiledShader, ID3DBlob *pErrors);
	void ShaderType(int *typeVS, int *typePS, unsigned int state, ID3D11DeviceContext *pDeviceContext);

private:
	//���b�V���p
	ID3D11VertexShader*  m_pVertexShader[SHADER_TYPE_TOTAL_VS];	//���_�V�F�[�_�[
	ID3D11PixelShader*	 m_pPixelShader[SHADER_TYPE_TOTAL_PS];	//�s�N�Z���V�F�[�_�[
	ID3D11InputLayout*	 m_pVertexLayout[SHADER_TYPE_TOTAL_VS];	//���_���C�A�E�g

	//�X�L�����b�V���p
	ID3D11VertexShader*  m_pSkinVertexShader[SHADER_TYPE_TOTAL_VS];	//���_�V�F�[�_�[
	ID3D11PixelShader*	 m_pSkinPixelShader[SHADER_TYPE_TOTAL_PS];	//�s�N�Z���V�F�[�_�[
	ID3D11InputLayout*	 m_pSkinVertexLayout[SHADER_TYPE_TOTAL_VS];	//���_���C�A�E�g

	ID3D11VertexShader *m_pVertexShaderBoard;
	ID3D11PixelShader *m_pPixelShaderBoard;
	ID3D11InputLayout *m_pVertexLayoutBoard;
	ID3D11Buffer *m_pVertexBuffer;
	ID3D11SamplerState *m_pSampleLinear;
};

#endif