#ifndef _SHADER_SHADOW_MAP_H_
#define _SHADER_SHADOW_MAP_H_

class StaticMesh;

struct ID3D11Device;
struct ID3D11DeviceContext;
struct D3DXMATRIX;

class ShaderShadowMap
{
public:
	~ShaderShadowMap();
	static ShaderShadowMap* GetInstance();
	D3DXMATRIX *GetViewMatrix();
	D3DXMATRIX *GetProjMatrix();
	void SetShadowMap(ID3D11DeviceContext *pDeviceContext);
	void SetResolution(float width, float height);
	void SetPosition(float x, float y, float z);
	bool Init();
	void Release();
	void Update();
	void Add(StaticMesh *pStaticMesh);
	void Clear(StaticMesh *pStaticMesh);

private:
	ShaderShadowMap();
	bool InitVertexShader(ID3D11Device *pDevice);
	bool InitPixelShader(ID3D11Device *pDevice);

private:
	class ShadowMapPimpl;
	ShadowMapPimpl *m_pShadowMaPimpl;
	float m_Width;
	float m_Height;
	float m_x;
	float m_y;
	float m_z;
};

#endif