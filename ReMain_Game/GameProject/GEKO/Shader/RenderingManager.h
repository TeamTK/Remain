#ifndef _RENDER_MANAGER_H_
#define _RENDER_MANAGER_H_

#include <functional>

struct RenderingInfo
{
	unsigned int priorityGroup; //優先順位グループ順位（昇順）
	unsigned int priority;		//優先順位グループの中の順位（昇順）
	std::function<void()> func; //レンダリング用関数登録
};

//全ての3Dモデルの行列を管理
class RenderingManager
{
public:
	~RenderingManager();
	static RenderingManager *GetInstance();
	void AddForward(unsigned int priorityGroup, RenderingInfo *pRenderingInfo);
	void AddDeferred(unsigned int priorityGroup, RenderingInfo *pRenderingInfo);
	void AddImage(unsigned int priorityGroup, RenderingInfo *pRenderingInfo);
	void Render();
	void SortForward(unsigned int priorityGroup);
	void SortDeferred(unsigned int priorityGroup);
	void SortImage(unsigned int priorityGroup);
	void ClearForward(unsigned int priorityGroup, RenderingInfo *pRenderingInfo);
	void ClearDeferred(unsigned int priorityGroup, RenderingInfo *pRenderingInfo);
	void ClearImage(unsigned int priorityGroup, RenderingInfo *pRenderingInfo);
	void AllClear();

private:
	RenderingManager();

private:
	//イディオムにデータを定義
	class ListPimpl;
	ListPimpl *m_pListPimpl;
};

#endif