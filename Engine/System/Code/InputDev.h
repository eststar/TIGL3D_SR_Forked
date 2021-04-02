#ifndef InputDev_h__
#define InputDev_h__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)


/* 비트값 맞춰서 원하는 키 추가 */
#define KEY_LBUTTON		0X00000001
#define KEY_RBUTTON		0X00000002

#define KEY_W			0X00000004
#define KEY_S			0X00000008
#define KEY_A			0X00000010
#define KEY_D			0X00000020

#define KEY_SPACE		0X00000040
#define KEY_TAB			0X00000080

#define KEY_Q			0x00000100
#define KEY_E			0x00000200
#define KEY_C			0x00000400


class ENGINE_DLL CInputDev : public CBase
{
	DECLARE_SINGLETON(CInputDev)

private:
	explicit CInputDev();
	virtual ~CInputDev();

public:
	_byte	Get_DIKeyState(_ubyte byKeyID) { return m_byKeyState[byKeyID]; }

	_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse) 
	{ return m_tMouseState.rgbButtons[eMouse]; }
	
	_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
	{
		return *(((_long*)&m_tMouseState) + eMouseState);
	}


public:
	HRESULT Ready_InputDev(HINSTANCE hInst, HWND hWnd);
	void	Update_InputDev();

public:
	_bool	Key_Up(_ulong ulKey);
	_bool	Key_Down(_ulong ulKey);
	_bool	Key_Pressing(_ulong ulKey);

	_bool	Key_Pressing(_ulong ulKey1, _ulong ulKey2);
	_bool	Key_PressingDir();

private:
	LPDIRECTINPUT8			m_pInputSDK = nullptr;

private:
	LPDIRECTINPUTDEVICE8	m_pKeyBoard = nullptr;
	LPDIRECTINPUTDEVICE8	m_pMouse = nullptr;

private:					/*따로 #define 저장해준 키값에 대한 변수들*/
	_ulong					m_ulKey;				
	_ulong					m_ulKeyUP;				
	_ulong					m_ulKeyDOWN;			

							/*기존에 있던 변수들*/
	DIMOUSESTATE			m_tMouseState;	
	_byte					m_byKeyState[256];
public:
	virtual void	Free();

};
END
#endif // InputDev_h__
