#ifndef Export_System_h__
#define Export_System_h__

#include "GraphicDev.h"
#include "TimerMgr.h"
#include "FrameMgr.h"
#include "FontMgr.h"
#include "InputDev.h"


BEGIN(Engine)

/* GraphicDev */
// Get
// Set
// General
inline HRESULT		Ready_GraphicDev(HWND hWnd, WINMODE eMode, const _uint& iSizeX, const _uint& iSizeY, CGraphicDev** ppDeviceClass);
inline void			Render_Begin(D3DXCOLOR Color);
inline void			Render_End();

/* TimerMgr */
// Get
inline _float			Get_TimeDelta(TIMERID eTimerID);
// Set
inline void			SetUp_TimeDelta(TIMERID eTimerID);
// General
inline	HRESULT			Ready_Timer(TIMERID eTimerID);


/* FrameMgr */
// Get
inline _bool		IsPermit_Call(FRAMEID eFrameID, const _float& fTimeDelta);
// Set
// General
inline HRESULT		Ready_Frame(FRAMEID eFrameID, const _float& fCallLimit);

/* FontMgr */
// Get
// Set
// General
inline HRESULT	Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev,
							FONTID eFontID,
							const _tchar* pFontType,
							const _uint& iWidth,
							const _uint& iHeight,
							const _uint& iWeight);

inline void	Render_Font(FONTID eFontID,
						const _tchar* pString,
						const _vec2* pPos,
						D3DXCOLOR Color);

// InputDev
// Get
inline _byte	Get_DIKeyState(_ubyte byKeyID);
inline _byte	Get_DIMouseState(MOUSEKEYSTATE eMouse);
inline _long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState);

inline _bool	Key_Up(_ulong ulKey);
inline _bool	Key_Down(_ulong ulKey);
inline _bool	Key_Pressing(_ulong ulKey);
inline _bool	Key_Pressing(_ulong ulKey1, _ulong ulKey2);
inline _bool	Key_PressingDir();
// Set
// General
inline	HRESULT Ready_InputDev(HINSTANCE hInst, HWND hWnd);
inline void	Update_InputDev();

inline void			Release_System();

#include "Export_System.inl"

END

#endif // Export_System_h__
