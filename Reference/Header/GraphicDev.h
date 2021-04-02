#ifndef GraphicDev_h__
#define GraphicDev_h__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CGraphicDev : public CBase
{
	DECLARE_SINGLETON(CGraphicDev)

private:
	explicit CGraphicDev();
	virtual ~CGraphicDev();

public:
	LPDIRECT3DDEVICE9		Get_GraphicDev() { return m_pGraphicDev; }

public:
	HRESULT		Ready_GraphicDev(HWND hWnd, 
								WINMODE eMode,
								const _uint& iSizeX, 
								const _uint& iSizeY, 
								CGraphicDev** ppDeviceClass);

	void		Render_Begin(D3DXCOLOR Color);
	void		Render_End();


private:
	LPDIRECT3D9					m_pSDK;
	LPDIRECT3DDEVICE9			m_pGraphicDev;

public:
	virtual void Free();

};

END
#endif // GraphicDev_h__