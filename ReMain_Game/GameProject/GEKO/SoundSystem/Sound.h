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
	/// �{�����[����ݒ�
	/// </summary>
	/// <param name="Volume">
	/// �{�����[������
	/// </param>
	/// <returns>
	/// �Ȃ�
	/// </returns>
	void SetVolume(float Volume);

	/// <summary>
	/// �T�E���h�̍Đ������[�v�ݒ�
	/// </summary>
	/// <param name="isLoop">
	/// true : ���[�v false : ���[�v���Ȃ�
	/// </param>
	/// <returns>
	/// �Ȃ�
	/// </returns>
	void SetLoop(bool isLoop);

	/// <summary>
	/// �T�E���h���Đ�
	/// </summary>
	/// <returns>
	/// �Ȃ�
	/// </returns>
	void Play() const;

	/// <summary>
	/// �Đ����~
	/// </summary>
	/// <returns>
	/// �Ȃ�
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