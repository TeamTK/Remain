#ifndef _SHADER_SHADOW_MAP_H_
#define _SHADER_SHADOW_MAP_H_

class StaticMesh;
class DynamicMesh;
class Vector3D;
class Matrix;

struct ID3D11Device;
struct ID3D11DeviceContext;

//シャドウマップで影を生成と管理
class ShaderShadowMap
{
public:
	~ShaderShadowMap();
	static ShaderShadowMap* GetInstance();
	Matrix *GetViewProjMatrix();
	void SetShadowMap(ID3D11DeviceContext *pDeviceContext);
	void SetResolution(float width, float height);
	void SetLookat(float x, float y, float z);
	void SetLookat(const Vector3D &lookatPt);
	void SetNearFar(float nearZ, float farZ);
	void SetViewAngle(float viewAngle);
	void SetDistance(float distance);
	bool Init();
	void Release();
	void Update();
	void Add(StaticMesh *pStaticMesh);
	void Add(DynamicMesh *pDynamicMesh);
	void Clear(StaticMesh *pStaticMesh);
	void Clear(DynamicMesh *pDynamicMesh);

private:
	ShaderShadowMap();
	bool InitVertexShader(ID3D11Device *pDevice);
	bool InitPixelShader(ID3D11Device *pDevice);
	void StaticMeshUpdate(ID3D11Device *pDevice, ID3D11DeviceContext *pDeviceContext);
	void DynaimcMeshUpdate(ID3D11Device *pDevice, ID3D11DeviceContext *pDeviceContext);

private:
	//イディオムにデータを定義
	class ShadowMapPimpl;
	ShadowMapPimpl *m_pShadowMaPimpl;
	float m_Width;	   //幅の解像度
	float m_Height;	   //高さの解像度
	float m_Near;	   //カメラが描画する最小距離
	float m_Far;	   //カメラが描画する最大距離
	float m_ViewAngle; //視野角度（ラジアン）
	float m_Distance;  //中心的からの距離
};

#endif