#ifndef MainApp_h__
#define MainApp_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CGraphicDev;
class CManagement;

END

class CMainApp final : public CBase
{
	// 생성자, 소멸자
private:
	explicit CMainApp();
	virtual ~CMainApp();

	// public 함수, 변수
public:
	HRESULT		Ready_MainApp();
	_int		Update_MainApp(const _float& fTimeDelta);
	_int		LateUpdate_MainApp(const _float& fTimeDelta);
	void		Render_MainApp();

	// protected 함수, 변수
	// private 함수, 변수
private:
	HRESULT				SetUp_Setting(LPDIRECT3DDEVICE9* ppGraphicDev);
	HRESULT				Ready_Logo(LPDIRECT3DDEVICE9& pGraphicDev, Engine::CManagement** ppManagement);


private:
	Engine::CGraphicDev*				m_pDeviceClass		= nullptr;
	Engine::CManagement*				m_pManagementClass	= nullptr;

	LPDIRECT3DDEVICE9					m_pGraphicDev		= nullptr;

	// 생성 함수, 소멸 함수
public:
	static CMainApp*					Create();
private:
	virtual void						Free() override;
};


#endif // MainApp_h__