#ifndef _SOUND_ASSET_H_
#define _SOUND_ASSET_H_

#include "..\System\Window.h"
#include <mmsystem.h>
#include "..\Include\XAudio2.h"
#include <string>

//サウンド情報
class SoundData
{
public:
	SoundData(std::string flimeName);
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
	static SoundData* GetSound(std::string name);
	static void LoadSound(std::string filmeName, std::string name);
	static void LoadFile(std::string filmeName);
	static void PartClear(std::string name);
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