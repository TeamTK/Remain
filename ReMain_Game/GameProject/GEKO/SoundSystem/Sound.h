#ifndef _SOUND_H_
#define _SOUND_H_

#include "SoundAsset.h"

class Sound
{
public:
	Sound();
	Sound(const std::string name);

	~Sound();

	void SetAseet(const std::string name);

	/// <summary>
	/// ボリュームを設定
	/// </summary>
	/// <param name="Volume">
	/// ボリューム調整
	/// </param>
	/// <returns>
	/// なし
	/// </returns>
	void SetVolume(float Volume);

	/// <summary>
	/// サウンドの再生をループ設定
	/// </summary>
	/// <param name="isLoop">
	/// true : ループ false : ループしない
	/// </param>
	/// <returns>
	/// なし
	/// </returns>
	void SetLoop(bool isLoop);

	/// <summary>
	/// サウンドを再生
	/// </summary>
	/// <returns>
	/// なし
	/// </returns>
	void Play() const;

	/// <summary>
	/// 再生を停止
	/// </summary>
	/// <returns>
	/// なし
	/// </returns>
	void Stop();

private:
	SoundData *m_pSoundInfo;
	bool m_isLoop;

};

class SoundManagement
{
public:
	~SoundManagement();
	static SoundManagement* const Get();
	IXAudio2* const GetAudio2() const;
	void Init();

private:
	SoundManagement(){};

private:
	IXAudio2* m_pXAudio2;
	IXAudio2MasteringVoice* m_pMasteringVoice;
};

#endif