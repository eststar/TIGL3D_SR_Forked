#include "InputDev.h"

USING(Engine)
IMPLEMENT_SINGLETON(CInputDev)

Engine::CInputDev::CInputDev()
	: m_ulKey(0)
	, m_ulKeyDOWN(0)
	, m_ulKeyUP(0)
{

}

Engine::CInputDev::~CInputDev()
{
	Free();
}

HRESULT Engine::CInputDev::Ready_InputDev(HINSTANCE hInst, HWND hWnd)
{
	// DInput 컴객체를 생성하는 함수
	FAILED_CHECK_RETURN(DirectInput8Create(hInst,
											DIRECTINPUT_VERSION,
											IID_IDirectInput8,
											(void**)&m_pInputSDK,
											NULL), E_FAIL);

	// 키보드 객체 생성
	FAILED_CHECK_RETURN(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr), E_FAIL);

	// 생성된 키보드 객체의 대한 정보를 컴 객체에게 전달하는 함수
	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);

	// 장치에 대한 독점권을 설정해주는 함수, (클라이언트가 떠있는 상태에서 키 입력을 받을지 말지를 결정하는 함수)
	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// 장치에 대한 access 버전을 받아오는 함수
	m_pKeyBoard->Acquire();


	// 마우스 객체 생성
	FAILED_CHECK_RETURN(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr), E_FAIL);

	// 생성된 마우스 객체의 대한 정보를 컴 객체에게 전달하는 함수
	m_pMouse->SetDataFormat(&c_dfDIMouse);

	// 장치에 대한 독점권을 설정해주는 함수, 클라이언트가 떠있는 상태에서 키 입력을 받을지 말지를 결정하는 함수
	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// 장치에 대한 access 버전을 받아오는 함수
	m_pMouse->Acquire();


	return S_OK;
}

void Engine::CInputDev::Update_InputDev()
{
	m_pKeyBoard->GetDeviceState(256, m_byKeyState);
	m_pMouse->GetDeviceState(sizeof(m_tMouseState), &m_tMouseState);

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
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		m_ulKey |= KEY_SPACE;
	if (GetAsyncKeyState(VK_TAB) & 0x8000)
		m_ulKey |= KEY_TAB;
}

_bool CInputDev::Key_Up(_ulong ulKey)
{
	if ((m_ulKey & ulKey) && !(m_ulKeyUP & ulKey))
	{
		m_ulKeyUP |= ulKey;
		return false;
	}
	else if (!(m_ulKey & ulKey) && (m_ulKeyUP & ulKey))
	{
		m_ulKeyUP ^= ulKey;
		return true;
	}
	return false;
}

_bool CInputDev::Key_Down(_ulong ulKey)
{
	if (m_ulKey & ulKey && !(m_ulKeyDOWN & ulKey))
	{
		m_ulKeyDOWN |= ulKey;
		return true;
	}
	else if (!(m_ulKey & ulKey) && (m_ulKeyDOWN & ulKey))
	{
		m_ulKeyDOWN ^= ulKey;
		return false;
	}
	return false;
}

_bool CInputDev::Key_Pressing(_ulong ulKey)
{
	if (m_ulKey & ulKey)
		return true;

	return false;
}

_bool CInputDev::Key_Pressing(_ulong ulKey1, _ulong ulKey2)
{
	if ((m_ulKey & ulKey1) && (m_ulKey & ulKey2))
		return true;

	return false;
}

_bool CInputDev::Key_PressingDir()
{
	if ((m_ulKey & KEY_A) || (m_ulKey & KEY_S) || (m_ulKey & KEY_W) || (m_ulKey & KEY_D))
		return true;
	return false;
}

void Engine::CInputDev::Free()
{
	Engine::Safe_Release(m_pKeyBoard);
	Engine::Safe_Release(m_pMouse);
	Engine::Safe_Release(m_pInputSDK);
}

