#ifndef _STATIC_MESH_SHADER_H_
#define _STATIC_MESH_SHADER_H_

#include "..\System\Direct3D11.h"

class StaticMeshShader
{
public:
	~StaticMeshShader();
	static StaticMeshShader* GetInstance();
	HRESULT Init();
	void Begin();
	void End();
	void Release();

private:
	StaticMeshShader();

private:
	ID3D11InputLayout*	 m_pVertexLayout;	 //頂点レイアウト
	ID3D11VertexShader*  m_pVertexShader;	 //頂点シェーダー
	ID3D11PixelShader*	 m_pPixelShader;	 //ピクセルシェーダー
	ID3D11ClassLinkage*  m_pPSClassLinkage;  //動的シェーダーリンク
	ID3D11ClassInstance* m_pClassInstance;   //ピクセルシェーダーに設定時に使用するクラスインスタンス
	ID3D11Buffer*		 m_pConstantBuffer0; //コンスタントバッファー（変換行列）
	ID3D11Buffer*		 m_pConstantBuffer1; //コンスタントバッファー（マテリアル）
	ID3D11Buffer*		 m_pConstantBuffer2; //コンスタントバッファー（動的リンク）
	std::string			 m_ClassInstanceName; //クラスインスタンス名
};

#endif