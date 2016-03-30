#ifndef _CONSTANT_SHADER_H_
#define _CONSTANT_SHADER_H_

#include "ShaderInfo.h"
#include <vector>

struct CopyBone;

//シェーダーに送るコンスタントバッファーを管理(StaticMesh・DynamicMeshのみ)
class ConstantShader
{
public:
	~ConstantShader();
	static ConstantShader* GetInstance();
	bool Init();
	void SetCommonInfoConstantBuffer();
	void SetTransformMatrixConstantBuffer(ID3D11DeviceContext *pDeviceContext, Matrix &mat, bool isShadow);
	void SetMaterialConstantBuffer(ID3D11DeviceContext *pDeviceContext, 
								   const Vector4D &diffuse, const Vector4D &specular, const Vector4D &ambient);
	void SetBoneConstantBuffer(ID3D11DeviceContext *pDeviceContext, int bornAllNum, std::vector<CopyBone*> &pCopyBornArray);
	void SetForwardLightConstantBuffer(ID3D11DeviceContext *pDeviceContext);
	void SetDeferredLightConstantBuffer(ID3D11DeviceContext *pDeviceContext);
	void Release();

private:
	ConstantShader();

private:
	ID3D11Buffer* m_pCommonInfoConstantBuffer;		 //定数バッファー（共通情報）
	ID3D11Buffer* m_pTransformMatrixConstantBuffer;  //定数バッファー（変換行列）
	ID3D11Buffer* m_pMaterialConstantBuffer;		 //定数バッファー（マテリアル）
	ID3D11Buffer* m_pBoneConstantBuffer;			 //定数バッファー（ボーン）
	ID3D11Buffer* m_pForwardConstantBuffer;			 //定数バッファー（前方レンダリング用ライト）
	ID3D11Buffer* m_pDeferredConstantBuffer;		 //定数バッファー（後方レンダリング用ライト）
};

#endif