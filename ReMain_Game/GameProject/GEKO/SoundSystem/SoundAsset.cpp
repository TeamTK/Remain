#include "SoundAsset.h"
#include <map>
#include "Sound.h"
#include <iostream>
#include <fstream>
#include "..\System\Direct3D11.h"

SoundData::SoundData(std::string flimeName)
{
	//ZeroMemory(this, sizeof(Sound));
	HMMIO hMmio = NULL;//Windows�}���`���f�B�AAPI�̃n���h��(Windows�}���`���f�B�AAPI��WAV�t�@�C���֌W�̑���p��API)
	DWORD dwWavSize = 0;//WAV�t�@�C�����@WAV�f�[�^�̃T�C�Y�iWAV�t�@�C����WAV�f�[�^�Ő�߂��Ă���̂ŁA�قڃt�@�C���T�C�Y�Ɠ���j
	WAVEFORMATEX* pwfex;//WAV�̃t�H�[�}�b�g ��j16�r�b�g�A44100Hz�A�X�e���I�Ȃ�
	MMCKINFO ckInfo;//�@�`�����N���
	MMCKINFO riffckInfo;// �ŏ㕔�`�����N�iRIFF�`�����N�j�ۑ��p
	PCMWAVEFORMAT pcmWaveForm;

	//WAV�t�@�C�����̃w�b�_�[���i���f�[�^�ȊO�j�̊m�F�Ɠǂݍ���
	hMmio = mmioOpenA(const_cast<char*>(flimeName.c_str()), NULL, MMIO_ALLOCBUF | MMIO_READ);

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
		MessageBox(0, TEXT("�\�[�X�{�C�X�쐬���s"), 0, MB_OK);
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
	std::map<std::string, SoundData*> map;
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

SoundData *SoundAsset::GetSound(std::string name)
{
	SoundPimpl *soundPimpl = GetInstance()->m_pSoundPimpl;

	//�w��̃T�E���h�f�[�^���Ȃ��ꍇ�x�����o��
	if (soundPimpl->map.find(name) == soundPimpl->map.end())
	{
		std::string str = name + "��SoundAsset�ɂ���܂���";
		MessageBoxA(0, str.c_str(), NULL, MB_OK);
	}
	else
	{
		return soundPimpl->map[name];
	}
	return nullptr;
}

void SoundAsset::LoadSound(std::string filmeName, std::string name)
{
	SoundPimpl *soundPimpl = GetInstance()->m_pSoundPimpl;

	//���łɂ���ꍇ�͌x�����o��
	if (soundPimpl->map.find(name) != soundPimpl->map.end())
	{
		std::string str = name + "��SoundAsset�Ɋ��ɑ��݂��܂�";
		MessageBoxA(0, str.c_str(), NULL, MB_OK);
		return;
	}
	else
	{
		//wav�t�@�C����������ǂݍ���
		if (filmeName[filmeName.length() - 1] == 'v' &&
			filmeName[filmeName.length() - 2] == 'a' &&
			filmeName[filmeName.length() - 3] == 'w')
		{
			soundPimpl->map[name] = new SoundData(filmeName);
			return;
		}
	}

	std::string str = filmeName + "��SoundAsset���ǂݍ��݂ł���t�@�C���ł͂���܂���";
	MessageBoxA(0, str.c_str(), NULL, MB_OK);
}

void SoundAsset::LoadFile(std::string filmeName)
{
	std::ifstream ifs(filmeName);
	std::string str = filmeName;
	str += "�̓ǂݍ��݂Ɏ��s���܂���";
	if (ifs.fail()) MessageBoxA(0, str.c_str(), NULL, MB_OK);

	while (!ifs.eof())
	{
		ifs >> str;

		if (str == "Sound")
		{
			ifs >> str; std::string fileName = str;
			ifs >> str; std::string Name = str;
			SoundAsset::LoadSound(fileName, str);
		}
	}
}

void SoundAsset::PartClear(std::string name)
{
	SoundPimpl *soundPimpl = GetInstance()->m_pSoundPimpl;

	auto it = soundPimpl->map.find(name);
	if (it != soundPimpl->map.end())
	{
		std::cout << it->first.c_str() << "�͍폜����܂���" << "\n";
		it->second->Relese();
		delete it->second;
		soundPimpl->map.erase(it);
	}
}

void SoundAsset::AllClear()
{
	SoundPimpl *soundPimpl = GetInstance()->m_pSoundPimpl;

	//SoundData�̃f�[�^�������[�X
	for (auto& i : soundPimpl->map)
	{
		std::cout << i.first.c_str() << "�͍폜����܂���\n";
		i.second->Relese();
		delete i.second;
	}

	//�S�Ă̗v�f���폜
	soundPimpl->map.clear();
}

void SoundAsset::DebugDraw()
{
	SoundPimpl *soundPimpl = GetInstance()->m_pSoundPimpl;

	//���݂���T�E���h�f�[�^��\��
	for (auto& i : soundPimpl->map)
	{
		std::cout << i.first.c_str() << "\n";
	}
}