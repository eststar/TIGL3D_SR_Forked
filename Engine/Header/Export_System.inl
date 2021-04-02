#include "Export_System.h"
// GraphicDev
// Get
// Set
// General
HRESULT		Ready_GraphicDev(HWND hWnd, Engine::WINMODE eMode, const _uint& iSizeX, const _uint& iSizeY, Engine::CGraphicDev** ppDeviceClass)
{
	return CGraphicDev::GetInstance()->Ready_GraphicDev(hWnd, eMode, iSizeX, iSizeY, ppDeviceClass);
}
void			Render_Begin(D3DXCOLOR Color)
{
	CGraphicDev::GetInstance()->Render_Begin(Color);
}
void			Render_End()
{
	CGraphicDev::GetInstance()->Render_End();
}

// Timer
// Get
_float			Get_TimeDelta(TIMERID eTimerID)
{
	return CTimerMgr::GetInstance()->Get_TimeDelta(eTimerID);
}

// Set
void			SetUp_TimeDelta(TIMERID eTimerID)
{
	CTimerMgr::GetInstance()->SetUp_TimeDelta(eTimerID);
}
// General
HRESULT			Ready_Timer(TIMERID eTimerID)
{
	return CTimerMgr::GetInstance()->Ready_Timer(eTimerID);
}

// FrameMgr
// Get
_bool		IsPermit_Call(FRAMEID eFrameID, const _float& fTimeDelta)
{
	return CFrameMgr::GetInstance()->IsPermit_Call(eFrameID, fTimeDelta);
}
// Set
// General
HRESULT		Ready_Frame(FRAMEID eFrameID, const _float& fCallLimit)
{
	return CFrameMgr::GetInstance()->Ready_Frame(eFrameID, fCallLimit);
}

// FontMgr
// Get
// Set
// General
HRESULT	Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev,
					FONTID eFontID,
					const _tchar* pFontType,
					const _uint& iWidth,
					const _uint& iHeight,
					const _uint& iWeight)
{
	return CFontMgr::GetInstance()->Ready_Font(pGraphicDev, eFontID, pFontType, iWidth, iHeight, iWeight);
}

void	Render_Font(FONTID eFontID,
					const _tchar* pString,
					const _vec2* pPos,
					D3DXCOLOR Color)
{
	CFontMgr::GetInstance()->Render_Font(eFontID, pString, pPos, Color);
}

// InputDev
// Get
_byte	Get_DIKeyState(_ubyte byKeyID)
{
	return CInputDev::GetInstance()->Get_DIKeyState(byKeyID);
}
_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse)
{
	return CInputDev::GetInstance()->Get_DIMouseState(eMouse);
}
_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
{
	return CInputDev::GetInstance()->Get_DIMouseMove(eMouseState);
}

_bool	Key_Up(_ulong ulKey)
{
	return CInputDev::GetInstance()->Key_Up(ulKey);
}

_bool	Key_Down(_ulong ulKey)
{
	return CInputDev::GetInstance()->Key_Down(ulKey);
}

_bool	Key_Pressing(_ulong ulKey)
{
	return CInputDev::GetInstance()->Key_Pressing(ulKey);
}

_bool	Key_Pressing(_ulong ulKey1, _ulong ulKey2)
{
	return CInputDev::GetInstance()->Key_Pressing(ulKey1, ulKey2);
}

_bool	Key_PressingDir()
{
	return CInputDev::GetInstance()->Key_PressingDir();
}


// Set
// General
HRESULT Ready_InputDev(HINSTANCE hInst, HWND hWnd)
{
	return CInputDev::GetInstance()->Ready_InputDev(hInst, hWnd);
}
void	Update_InputDev()
{
	CInputDev::GetInstance()->Update_InputDev();
}



void			Release_System()
{
	CInputDev::GetInstance()->DestroyInstance();
	CFontMgr::GetInstance()->DestroyInstance();
	CFrameMgr::GetInstance()->DestroyInstance();
	CTimerMgr::GetInstance()->DestroyInstance();
	CGraphicDev::GetInstance()->DestroyInstance();
}