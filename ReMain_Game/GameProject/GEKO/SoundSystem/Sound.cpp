#include "Sound.h"
#include "..\System\Direct3D11.h"

Sound::Sound() :
	m_isLoop(false)
{
}

Sound::~Sound()
{
}

void Sound::SetAsset(SoundData *soundData)
{
	m_pSoundInfo = soundData;
}

void Sound::SetAseet(const char *name)
{
	m_pSoundInfo = SoundAsset::GetSound(name);
}

void Sound::SetVolume(float Volume = 1.0f)
{
	m_pSoundInfo->m_pSourceVoice->SetVolume(Volume);
}

void Sound::SetLoop(bool isLoop)
{
	m_isLoop = isLoop;
}

void Sound::Play()
{
	m_pSoundInfo->m_pSourceVoice->Stop(0, XAUDIO2_COMMIT_NOW);
	m_pSoundInfo->m_pSourceVoice->FlushSourceBuffers();

	XAUDIO2_BUFFER buffer = { 0 };
	buffer.pAudioData = m_pSoundInfo->m_pWavBuffer;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.AudioBytes = m_pSoundInfo->m_dwWavSize;

	if (m_isLoop)
	{
		buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	}
	if (FAILED(m_pSoundInfo->m_pSourceVoice->SubmitSourceBuffer(&buffer)))
	{
		MessageBox(0, TEXT("ソースボイスにサブミット失敗"), 0, MB_OK);
		return;
	}
	m_pSoundInfo->m_pSourceVoice->Start(0, XAUDIO2_COMMIT_NOW);
}

void Sound::Stop()
{
	m_pSoundInfo->m_pSourceVoice->Stop(0, XAUDIO2_COMMIT_NOW);
}

SoundManagement::~SoundManagement()
{
	if (m_pMasteringVoice) m_pMasteringVoice->DestroyVoice();
	SAFE_RELEASE(m_pXAudio2);
	CoUninitialize();
	OutputDebugString(TEXT("SoundManagementが正常に終了しました\n"));
}

SoundManagement* const SoundManagement::Get()
{
	static SoundManagement soundManagement;
	return &soundManagement;
}

IXAudio2* const SoundManagement::GetAudio2() const
{
	return m_pXAudio2;
}

void SoundManagement::Init()
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	if (FAILED(XAudio2Create(&m_pXAudio2, 0)))
	{
		CoUninitialize();
		MessageBox(0, TEXT("XAudio2初期化失敗"), 0, MB_OK);
	}
	if (FAILED(m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice)))
	{
		CoUninitialize();
		MessageBox(0, TEXT("マスターリング失敗"), 0, MB_OK);
	}
}