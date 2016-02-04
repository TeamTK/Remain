#include "UI_SelectWeapon.h"
#include "../../MainGame/Weapon/WeaponInfo.h"

enum EState
{
	eOpen,
	eSelect,
} State;

struct SData
{
	Vector3D pos;
	EWeapons weapons;
};

static SData WeaponData[] =
{
	{ Vector3D(400.0f, 445.0f, 0.0f), eHand		},	//Down
	{ Vector3D(400.0f, 155.0f, 0.0f), eShotgun	},	//Up
	{ Vector3D(255.0f, 300.0f, 0.0f), eHandgun	},	//Left
	{ Vector3D(545.0f, 300.0f, 0.0f), eNone		},	//Right
};

#define CIRCLE_SIZE 260
#define WEAPONUI_SIZE 192

UI_SelectWeapon::UI_SelectWeapon() :
	m_isSelected(false), m_CircleSize(0), m_WeponUISize(0),
	m_Selected(1), m_ScPos(400.0f, 155.0f, 0.0f)
{
	m_RenderTask.Regist(0, REGIST_RENDER_FUNC(UI_SelectWeapon::Draw));
	State = eOpen;
	m_Circle.SetAsset("SelectCircle");
	m_SelectedCursor.SetAsset("SelectedCursor");
	m_SelectedCursor.SetSize(256, 256);
	m_SelectedCursor.SetCenter(128, 128);

	m_WeaponUI[0].SetAsset("UI_FreeHand");
	m_WeaponUI[1].SetAsset("UI_Shotgun");
	m_WeaponUI[2].SetAsset("UI_Handgun");
	m_WeaponUI[3].SetAsset("UI_None");
	for (int i = 0; i < 4; i++) {
		m_WeaponUI[i].SetCenter(96, 96);
		m_UIPos[i] = Vector3D(400.0f, 300.0f, 0.0f);
	}
}

UI_SelectWeapon::~UI_SelectWeapon()
{

}

void UI_SelectWeapon::Update()
{
	if (m_isSelected)
	{
		//����I�𒆂̓����X�^�[�̓�����~
		TaskManager::Stop("Monster_A");
		TaskManager::Stop("Monster_B");

		switch (State)
		{
		case eOpen:
			//�~�̃T�C�Y�ύX
			if (m_CircleSize < CIRCLE_SIZE) m_CircleSize += 32;
			m_Circle.SetCenter(m_CircleSize / 2, m_CircleSize / 2);
			m_Circle.SetSize(m_CircleSize, m_CircleSize);
			//�e����UI�̃T�C�Y�ύX
			if (m_WeponUISize < WEAPONUI_SIZE) m_WeponUISize += 32;
			//�ő�T�C�Y�ɂȂ�����X�e�[�g�ύX
			if (m_CircleSize >= CIRCLE_SIZE)	State = eSelect;

			for (int i = 0; i < 4; i++)
			{
				m_UIPos[i] = Vector3D::Lerp(m_UIPos[i], WeaponData[i].pos, 0.6f);
				//�e����UI�̃T�C�Y�K��
				m_WeaponUI[i].SetSize(m_WeponUISize, m_WeponUISize);
				m_WeaponUI[i].SetCenter(m_WeponUISize / 2, m_WeponUISize / 2);
			}
			break;
		case eSelect:
			//����I��(�����L�[�A�\���L�[)
			/*
			if (Input::KeyDown.Clicked() || Input::XInputPad1.DownClicked())	m_Selected = 0;
			if (Input::KeyUp.Clicked() || Input::XInputPad1.UpClicked())		m_Selected = 1;
			if (Input::KeyLeft.Clicked() || Input::XInputPad1.LeftClicked())	m_Selected = 2;
			if (Input::KeyRight.Clicked() || Input::XInputPad1.RightClicked())	m_Selected = 3;
			*/
			if (Input::KeyS.Clicked() || Input::XInputPad1.DownClicked())	m_Selected = 0;
			if (Input::KeyW.Clicked() || Input::XInputPad1.UpClicked())		m_Selected = 1;
			if (Input::KeyA.Clicked() || Input::XInputPad1.LeftClicked())	m_Selected = 2;
			if (Input::KeyD.Clicked() || Input::XInputPad1.RightClicked())	m_Selected = 3;
			//�J�[�\���ړ�
			m_ScPos = Vector3D::Lerp(m_ScPos, WeaponData[m_Selected].pos, 0.6f);

			break;
		default:
			break;
		}
	}
	else
	{
		//����I���O�̓����X�^�[�̓����N��
		TaskManager::Start("Monster_A");
		TaskManager::Start("Monster_B");

		//�~�̃T�C�Y�k��
		if (m_CircleSize > 0) m_CircleSize -= 32;
		m_Circle.SetCenter(m_CircleSize / 2, m_CircleSize / 2);
		m_Circle.SetSize(m_CircleSize, m_CircleSize);
		//�e����UI�̃T�C�Y�k��
		if (m_WeponUISize > 0) m_WeponUISize -= 32;

		for (int i = 0; i < 4; i++)
		{
			if (m_Selected == i)
			{
				//�I�����Ă���e����UI�̈ړ�
				m_WeaponUI[i].SetSize(100, 100);
				m_UIPos[i] = Vector3D::Lerp(m_UIPos[i], Vector3D(100.0f, 100.0f, 0.0f), 0.6f);
			}
			else
			{
				//�I�����Ă��Ȃ��e����UI�̈ړ�
				m_UIPos[i] = Vector3D::Lerp(m_UIPos[i], Vector3D(400.0f, 300.0f, 0.0f), 0.6f);
				m_WeaponUI[i].SetSize(m_WeponUISize, m_WeponUISize);
				m_WeaponUI[i].SetCenter(m_WeponUISize / 2, m_WeponUISize / 2);
			}
		}
		State = eOpen;
	}
}

EWeapons UI_SelectWeapon::Select()
{
	m_isSelected = !m_isSelected;
	return WeaponData[m_Selected].weapons;
}

void UI_SelectWeapon::Draw()
{
	//�J�[�\��
	if (State == eSelect)
		m_SelectedCursor.Draw((int)m_ScPos.x, (int)m_ScPos.y);

	//�e����UI
	for (int i = 0; i < 4; i++)	{
		m_WeaponUI[i].Draw((int)m_UIPos[i].x, (int)m_UIPos[i].y);
	}

	//�~
	m_Circle.Draw(400, 300);
}

bool UI_SelectWeapon::isSelected()
{
	return m_isSelected;
}