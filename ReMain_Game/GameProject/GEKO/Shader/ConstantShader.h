#ifndef _CONSTANT_SHADER_H_
#define _CONSTANT_SHADER_H_

#include "ShaderInfo.h"
#include <vector>

struct CopyBorn;

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
	void SetBornConstantBuffer(ID3D11DeviceContext *pDeviceContext, int bornAllNum, std::vector<CopyBorn*> &pCopyBornArray);
	void Release();

private:
	ConstantShader();

private:
	ID3D11Buffer* m_pCommonInfoConstantBuffer;		 //コンスタントバッファー（共通情報）
	ID3D11Buffer* m_pTransformMatrixConstantBuffer;  //コンスタントバッファー（変換行列）
	ID3D11Buffer* m_pMaterialConstantBuffer;		 //コンスタントバッファー（マテリアル）
	ID3D11Buffer* m_pBornConstantBuffer;			 //コンスタントバッファー（ボーン）
};

#endif