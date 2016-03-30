#ifndef _RENDER_MANAGER_H_
#define _RENDER_MANAGER_H_

#include <functional>

struct RenderingInfo
{
	unsigned int priorityGroup; //�D�揇�ʃO���[�v���ʁi�����j
	unsigned int priority;		//�D�揇�ʃO���[�v�̒��̏��ʁi�����j
	std::function<void()> func; //�����_�����O�p�֐��o�^
};

//�S�Ă�3D���f���̍s����Ǘ�
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
	//�C�f�B�I���Ƀf�[�^���`
	class ListPimpl;
	ListPimpl *m_pListPimpl;
};

#endif