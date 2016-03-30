#ifndef _DEFERRED_RENDERING_H_
#define _DEFERRED_RENDERING_H_

enum class ShaderType;

class DeferredRendering
{
public:
	~DeferredRendering();
	static DeferredRendering* GetInstance();
	bool Init();
	void ChangeRenderTarget();
	void SetShaderMesh(ShaderType type);
	void SetShaderSkinMesh(ShaderType type);
	void Rendering();
	void Release();

private:
	DeferredRendering();
	bool InitSheder();

private:
	//イディオムにデータを定義
	struct DeferredPimpl;
	DeferredPimpl *m_pDeferredPimpl;

	enum MultiRenderTargetType
	{
		eAlbedo,
		eNormal,
		ePosition,
	};
};

#endif