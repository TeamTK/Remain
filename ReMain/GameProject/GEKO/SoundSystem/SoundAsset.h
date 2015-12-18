#ifndef _SOUNDASSET_H_
#define _SOUNDASSET_H_

#include "..\System\Window.h"
#include <mmsystem.h>
#include "..\Include\XAudio2.h"

//サウンド情報
class SoundData
{
public:
	SoundData(char *flimeName);
	~SoundData();
	void Relese();

public:
	IXAudio2SourceVoice* m_pSourceVoice;
	BYTE* m_pWavBuffer;							//波形データ（フォーマット等を含まない、純粋に波形データのみ）
	DWORD m_dwWavSize;							//波形データのサイズ
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