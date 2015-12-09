#include "SoundAsset.h"
#include <map>
#include "Sound.h"
#include <iostream>
#include "..\System\Direct3D11.h"

SoundData::SoundData(char *flimeName)
{
	//ZeroMemory(this, sizeof(Sound));
	HMMIO hMmio = NULL;//WindowsマルチメディアAPIのハンドル(WindowsマルチメディアAPIはWAVファイル関係の操作用のAPI)
	DWORD dwWavSize = 0;//WAVファイル内　WAVデータのサイズ（WAVファイルはWAVデータで占められているので、ほぼファイルサイズと同一）
	WAVEFORMATEX* pwfex;//WAVのフォーマット 例）16ビット、44100Hz、ステレオなど
	MMCKINFO ckInfo;//　チャンク情報
	MMCKINFO riffckInfo;// 最上部チャンク（RIFFチャンク）保存用
	PCMWAVEFORMAT pcmWaveForm;

	//WAVファイル内のヘッダー情報（音データ以外）の確認と読み込み
	hMmio = mmioOpenA(flimeName, NULL, MMIO_ALLOCBUF | MMIO_READ);

	//ファイルポインタをRIFFチャンクの先頭にセットする
	mmioDescend(hMmio, &riffckInfo, NULL, 0);

	// ファイルポインタを'f' 'm' 't' ' ' チャンクにセットする
	ckInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmioDescend(hMmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK);

	//フォーマットを読み込む
	mmioRead(hMmio, (HPSTR)&pcmWaveForm, sizeof(pcmWaveForm));
	pwfex = (WAVEFORMATEX*)new CHAR[sizeof(WAVEFORMATEX)];
	memcpy(pwfex, &pcmWaveForm, sizeof(pcmWaveForm));
	pwfex->cbSize = 0;
	mmioAscend(hMmio, &ckInfo, 0);

	// WAVファイル内の音データの読み込み	
	ckInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmioDescend(hMmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK);//データチャンクにセット
	dwWavSize = ckInfo.cksize;
	m_pWavBuffer = new BYTE[dwWavSize];
	DWORD dwOffset = ckInfo.dwDataOffset;
	mmioRead(hMmio, (HPSTR)m_pWavBuffer, dwWavSize);
	//ソースボイスにデータを詰め込む	
	if (FAILED(SoundManagement::Get()->GetAudio2()->CreateSourceVoice(&m_pSourceVoice, pwfex)))
	{
		MessageBox(0, L"ソースボイス作成失敗", 0, MB_OK);
	}
	m_dwWavSize = dwWavSize;

	delete pwfex;
}

SoundData::~SoundData()
{
}

void SoundData::Relese()
{
	if (m_pSourceVoice) m_pSourceVoice->DestroyVoice();
	SAFE_DELETE(m_pWavBuffer);
}

class SoundAsset::SoundPimpl
{
public:
	std::map<const char*, SoundData*> map;
};

SoundAsset::SoundAsset()
{
	m_pSoundPimpl = new SoundPimpl;
}

SoundAsset::~SoundAsset()
{
	delete m_pSoundPimpl;
}

SoundAsset *SoundAsset::GetInstance()
{
	static SoundAsset soundAsset;
	return &soundAsset;
}

SoundData *SoundAsset::GetSound(const char *name)
{
	SoundPimpl *soundPimpl = GetInstance()->m_pSoundPimpl;

	//指定のサウンドデータがない場合警告を出しウインドウ停止
	if (soundPimpl->map.find(name) == soundPimpl->map.end())
	{
		GetInstance()->AllClear();
		MessageBox(0, L"指定のSoundDataはありません", NULL, MB_OK);
		Window::Get()->WindowEnd();
	}

	return soundPimpl->map[name];
}

void SoundAsset::LoadSound(char *flimeName, const char *name)
{
	SoundPimpl *soundPimpl = GetInstance()->m_pSoundPimpl;

	//すでにある場合は警告を出しウィンドウ終了
	if (soundPimpl->map.find(name) != soundPimpl->map.end())
	{
		GetInstance()->AllClear();
		MessageBox(0, L"指定の名前は存在します", NULL, MB_OK);
		Window::Get()->WindowEnd();
	}

	soundPimpl->map[name] = new SoundData(flimeName);
}

void SoundAsset::PartClear(const char *name)
{
	SoundPimpl *soundPimpl = GetInstance()->m_pSoundPimpl;

	auto it = soundPimpl->map.find(name);
	if (it != soundPimpl->map.end())
	{
		std::cout << it->first << "は削除されました" << "\n";
		it->second->Relese();
		delete it->second;
		soundPimpl->map.erase(it);
	}
}

void SoundAsset::AllClear()
{
	SoundPimpl *soundPimpl = GetInstance()->m_pSoundPimpl;

	//SoundDataのデータをリリース
	auto it = soundPimpl->map.begin();
	for (; it != soundPimpl->map.end(); it++)
	{
		std::cout << it->first << "は削除されました\n";
		it->second->Relese();
		delete it->second;
	}

	//全ての要素を削除
	soundPimpl->map.clear();
}

void SoundAsset::DebugDraw()
{
	SoundPimpl *soundPimpl = GetInstance()->m_pSoundPimpl;

	//現在あるサウンドデータを表示
	auto it = soundPimpl->map.begin();
	for (; it != soundPimpl->map.end(); it++)
	{
		std::cout << it->first << "\n";
	}
}