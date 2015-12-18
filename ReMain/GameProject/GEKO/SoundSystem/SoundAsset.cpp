#include "SoundAsset.h"
#include <map>
#include "Sound.h"
#include <iostream>
#include "..\System\Direct3D11.h"

SoundData::SoundData(char *flimeName)
{
	//ZeroMemory(this, sizeof(Sound));
	HMMIO hMmio = NULL;//Windows�}���`���f�B�AAPI�̃n���h��(Windows�}���`���f�B�AAPI��WAV�t�@�C���֌W�̑���p��API)
	DWORD dwWavSize = 0;//WAV�t�@�C�����@WAV�f�[�^�̃T�C�Y�iWAV�t�@�C����WAV�f�[�^�Ő�߂��Ă���̂ŁA�قڃt�@�C���T�C�Y�Ɠ���j
	WAVEFORMATEX* pwfex;//WAV�̃t�H�[�}�b�g ��j16�r�b�g�A44100Hz�A�X�e���I�Ȃ�
	MMCKINFO ckInfo;//�@�`�����N���
	MMCKINFO riffckInfo;// �ŏ㕔�`�����N�iRIFF�`�����N�j�ۑ��p
	PCMWAVEFORMAT pcmWaveForm;

	//WAV�t�@�C�����̃w�b�_�[���i���f�[�^�ȊO�j�̊m�F�Ɠǂݍ���
	hMmio = mmioOpenA(flimeName, NULL, MMIO_ALLOCBUF | MMIO_READ);

	//�t�@�C���|�C���^��RIFF�`�����N�̐擪�ɃZ�b�g����
	mmioDescend(hMmio, &riffckInfo, NULL, 0);

	// �t�@�C���|�C���^��'f' 'm' 't' ' ' �`�����N�ɃZ�b�g����
	ckInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmioDescend(hMmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK);

	//�t�H�[�}�b�g��ǂݍ���
	mmioRead(hMmio, (HPSTR)&pcmWaveForm, sizeof(pcmWaveForm));
	pwfex = (WAVEFORMATEX*)new CHAR[sizeof(WAVEFORMATEX)];
	memcpy(pwfex, &pcmWaveForm, sizeof(pcmWaveForm));
	pwfex->cbSize = 0;
	mmioAscend(hMmio, &ckInfo, 0);

	// WAV�t�@�C�����̉��f�[�^�̓ǂݍ���	
	ckInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmioDescend(hMmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK);//�f�[�^�`�����N�ɃZ�b�g
	dwWavSize = ckInfo.cksize;
	m_pWavBuffer = new BYTE[dwWavSize];
	DWORD dwOffset = ckInfo.dwDataOffset;
	mmioRead(hMmio, (HPSTR)m_pWavBuffer, dwWavSize);
	//�\�[�X�{�C�X�Ƀf�[�^���l�ߍ���	
	if (FAILED(SoundManagement::Get()->GetAudio2()->CreateSourceVoice(&m_pSourceVoice, pwfex)))
	{
		MessageBox(0, L"�\�[�X�{�C�X�쐬���s", 0, MB_OK);
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

	//�w��̃T�E���h�f�[�^���Ȃ��ꍇ�x�����o���E�C���h�E��~
	if (soundPimpl->map.find(name) == soundPimpl->map.end())
	{
		GetInstance()->AllClear();
		MessageBox(0, L"�w���SoundData�͂���܂���", NULL, MB_OK);
		Window::Get()->WindowEnd();
	}

	return soundPimpl->map[name];
}

void SoundAsset::LoadSound(char *flimeName, const char *name)
{
	SoundPimpl *soundPimpl = GetInstance()->m_pSoundPimpl;

	//���łɂ���ꍇ�͌x�����o���E�B���h�E�I��
	if (soundPimpl->map.find(name) != soundPimpl->map.end())
	{
		GetInstance()->AllClear();
		MessageBox(0, L"�w��̖��O�͑��݂��܂�", NULL, MB_OK);
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
		std::cout << it->first << "�͍폜����܂���" << "\n";
		it->second->Relese();
		delete it->second;
		soundPimpl->map.erase(it);
	}
}

void SoundAsset::AllClear()
{
	SoundPimpl *soundPimpl = GetInstance()->m_pSoundPimpl;

	//SoundData�̃f�[�^�������[�X
	auto it = soundPimpl->map.begin();
	for (; it != soundPimpl->map.end(); it++)
	{
		std::cout << it->first << "�͍폜����܂���\n";
		it->second->Relese();
		delete it->second;
	}

	//�S�Ă̗v�f���폜
	soundPimpl->map.clear();
}

void SoundAsset::DebugDraw()
{
	SoundPimpl *soundPimpl = GetInstance()->m_pSoundPimpl;

	//���݂���T�E���h�f�[�^��\��
	auto it = soundPimpl->map.begin();
	for (; it != soundPimpl->map.end(); it++)
	{
		std::cout << it->first << "\n";
	}
}