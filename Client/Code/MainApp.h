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
	// ������, �Ҹ���
private:
	explicit CMainApp();
	virtual ~CMainApp();

	// public �Լ�, ����
public:
	HRESULT		Ready_MainApp();
	_int		Update_MainApp(const _float& fTimeDelta);
	_int		LateUpdate_MainApp(const _float& fTimeDelta);
	void		Render_MainApp();

	// protected �Լ�, ����
	// private �Լ�, ����
private:
	HRESULT				SetUp_Setting(LPDIRECT3DDEVICE9* ppGraphicDev);
	HRESULT				Ready_Logo(LPDIRECT3DDEVICE9& pGraphicDev, Engine::CManagement** ppManagement);


private:
	Engine::CGraphicDev*				m_pDeviceClass		= nullptr;
	Engine::CManagement*				m_pManagementClass	= nullptr;

	LPDIRECT3DDEVICE9					m_pGraphicDev		= nullptr;

	// ���� �Լ�, �Ҹ� �Լ�
public:
	static CMainApp*					Create();
private:
	virtual void						Free() override;
};


#endif // MainApp_h__