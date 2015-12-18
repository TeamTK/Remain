#ifndef _SOUNDASSET_H_
#define _SOUNDASSET_H_

#include "..\System\Window.h"
#include <mmsystem.h>
#include "..\Include\XAudio2.h"

//�T�E���h���
class SoundData
{
public:
	SoundData(char *flimeName);
	~SoundData();
	void Relese();

public:
	IXAudio2SourceVoice* m_pSourceVoice;
	BYTE* m_pWavBuffer;							//�g�`�f�[�^�i�t�H�[�}�b�g�����܂܂Ȃ��A�����ɔg�`�f�[�^�̂݁j
	DWORD m_dwWavSize;							//�g�`�f�[�^�̃T�C�Y
};

class SoundAsset
{
public:
	~SoundAsset();
	static SoundData* GetSound(const char* nam);
	static void LoadSound(char *flimeName, const char* name);
	static void PartClear(const char *name);
	static void AllClear();
	static void DebugDraw();

private:
	SoundAsset();
	static SoundAsset *GetInstance();

private:
	class SoundPimpl;
	SoundPimpl *m_pSoundPimpl;
};

#endif