#include "stdafx.h"
#include "KeyMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(KeyMgr)

CKeyMgr::CKeyMgr()
	: m_ulKey(0)
	, m_ulKeyDOWN(0)
	, m_ulKeyUP(0)
{
}

void CKeyMgr::Update_KeyManager()
{
	m_ulKey = 0;

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		m_ulKey |= KEY_LBUTTON;
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		m_ulKey |= KEY_RBUTTON;
	if (GetAsyncKeyState('W') & 0x8000)
		m_ulKey |= KEY_W;
	if (GetAsyncKeyState('S') & 0x8000)
		m_ulKey |= KEY_S;
	if (GetAsyncKeyState('A') & 0x8000)
		m_ulKey |= KEY_A;
	if (GetAsyncKeyState('D') & 0x8000)
		m_ulKey |= KEY_D;


	if (GetAsyncKeyState(VK_TAB) & 0x8000)
		m_ulKey |= KEY_TAB;

	if (GetAsyncKeyState('F') & 0x8000)
		m_ulKey |= KEY_F;

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		m_ulKey |= KEY_SPACE;

	if (GetAsyncKeyState('L') & 0x8000)
		m_ulKey |= KEY_L;

	if (GetAsyncKeyState('Q') & 0x8000)
		m_ulKey |= KEY_Q;

	if (GetAsyncKeyState('E') & 0x8000)
		m_ulKey |= KEY_E;

	if (GetAsyncKeyState('R') & 0x8000)
		m_ulKey |= KEY_R;

	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
		m_ulKey |= KEY_SHIFT;

}

bool CKeyMgr::Key_Up(_ulong _ulKey)
{
	if ((m_ulKey & _ulKey) && !(m_ulKeyUP & _ulKey))
	{
		m_ulKeyUP |= _ulKey;
		return false;
	}
	else if (!(m_ulKey & _ulKey) && (m_ulKeyUP & _ulKey))
	{
		m_ulKeyUP ^= _ulKey;
		return true;
	}
	return false;
}

bool CKeyMgr::Key_Down(_ulong _ulKey)
{
	if (m_ulKey & _ulKey && !(m_ulKeyDOWN & _ulKey))
	{
		m_ulKeyDOWN |= _ulKey;
		return true;
	}
	else if (!(m_ulKey & _ulKey) && (m_ulKeyDOWN & _ulKey))
	{
		m_ulKeyDOWN ^= _ulKey;
		return false;
	}
	return false;
}

bool CKeyMgr::Key_Pressing(_ulong _ulKey)
{
	if (m_ulKey & _ulKey)
		return true;

	return false;
}

bool CKeyMgr::Key_Pressing(_ulong _ulKey1, _ulong _ulKey2)
{
	if ((m_ulKey & _ulKey1) && (m_ulKey & _ulKey2))
		return true;

	return false;
}

bool CKeyMgr::Key_PressingDir()
{
	if ((m_ulKey & KEY_A) || (m_ulKey & KEY_S) || (m_ulKey & KEY_W) || (m_ulKey & KEY_D))
		return true;
	return false;
}
