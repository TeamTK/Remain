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
#define CURSOR_VALUE 0.7f

UI_SelectWeapon* g_pUI_SelectWeapon;

UI_SelectWeapon::UI_SelectWeapon() :
	Task("UI_SelectWeapon", 1),
	m_isSelected(false),
	m_CircleSize(0),
	m_WeponUISize(0),
	m_Selected(1),
	m_ScPos(400.0f, 155.0f, 0.0f)
{
	m_RenderTask.Regist(5, REGIST_RENDER_FUNC(UI_SelectWeapon::Draw));
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
		switch (State)
		{
		case eOpen:
			//円のサイズ変更
			if (m_CircleSize < CIRCLE_SIZE) m_CircleSize += 48;
			m_Circle.SetCenter(m_CircleSize / 2, m_CircleSize / 2);
			m_Circle.SetSize(m_CircleSize, m_CircleSize);
			//各武器UIのサイズ変更
			if (m_WeponUISize < WEAPONUI_SIZE) m_WeponUISize += 32;
			//最大サイズになったらステート変更
			if (m_CircleSize >= CIRCLE_SIZE)	State = eSelect;

			for (int i = 0; i < 4; i++)
			{
				m_UIPos[i] = Vector3D::Lerp(m_UIPos[i], WeaponData[i].pos, 0.6f);
				//各武器UIのサイズ適応
				m_WeaponUI[i].SetSize(m_WeponUISize, m_WeponUISize);
				m_WeaponUI[i].SetCenter(m_WeponUISize / 2, m_WeponUISize / 2);
			}
			break;
		case eSelect:
			//武器選択(方向キー、右パッド)
			if (Input::XInputPad1.GetIsConnection())
			{
				//コントローラー入力
				Vector3D PadDir = Vector3D((float)Input::XInputPad1.ThumbRightX(), (float)Input::XInputPad1.ThumbRightY(), 0.0f);
				PadDir = (PadDir - Vector3D(128, 0.0f, 128)) / 32767;
				if (abs(PadDir.x) < DeadValue && abs(PadDir.y) < DeadValue)
				{
					PadDir = Vector3D(0.0f, 0.0f, 0.0f);
				}
				//if (PadDir.y < -CURSOR_VALUE)	m_Selected = 0;
				if (PadDir.y > CURSOR_VALUE)	m_Selected = 1;
				if (PadDir.x < -CURSOR_VALUE)	m_Selected = 2;
				//if (PadDir.x > CURSOR_VALUE)	m_Selected = 3;
			}
			else
			{
				//if (Input::KeyS.Clicked())	m_Selected = 0;
				if (Input::KeyW.Clicked())	m_Selected = 1;
				if (Input::KeyA.Clicked())	m_Selected = 2;
				//if (Input::KeyD.Clicked())	m_Selected = 3;
			}

			//カーソル移動
			m_ScPos = Vector3D::Lerp(m_ScPos, WeaponData[m_Selected].pos, 0.6f);

			break;
		default:
			break;
		}
	}
	else
	{
		//円のサイズ縮小
		if (m_CircleSize > 0) m_CircleSize -= 32;
		m_Circle.SetCenter(m_CircleSize / 2, m_CircleSize / 2);
		m_Circle.SetSize(m_CircleSize, m_CircleSize);
		//各武器UIのサイズ縮小
		if (m_WeponUISize > 0) m_WeponUISize -= 32;

		for (int i = 0; i < 4; i++)
		{
			if (m_Selected == i)
			{
				//選択している各武器UIの移動
				m_WeaponUI[i].SetSize(100, 100);
				m_UIPos[i] = Vector3D::Lerp(m_UIPos[i], Vector3D(100.0f, 100.0f, 0.0f), 0.6f);
			}
			else
			{
				//選択していない各武器UIの移動
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
	//円
	m_Circle.Draw(400, 300);

	//各武器UI
	for (int i = 0; i < 4; i++)
		m_WeaponUI[i].Draw((int)m_UIPos[i].x, (int)m_UIPos[i].y);

	//カーソル
	if (State == eSelect)
		m_SelectedCursor.Draw((int)m_ScPos.x, (int)m_ScPos.y);
}

bool UI_SelectWeapon::isSelected()
{
	return m_isSelected;
}

bool UI_SelectWeapon::HitCheck(Vector3D pos1, Vector3D pos2, float radius1, float radius2)
{
	if (((pos1.x - pos2.x) * (pos1.x - pos2.x)) + ((pos1.y - pos2.y) * (pos1.y - pos2.y))
		<= (radius1 + radius2) * (radius1 + radius2))
		return true;
	return false;
}