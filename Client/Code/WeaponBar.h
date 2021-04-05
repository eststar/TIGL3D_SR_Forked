#ifndef WeaponBar_h__
#define WeaponBar_h__

#include "Define.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END

/* 0405_다영 h & cpp 추가*/

class CWeaponBar : public Engine::CGameObject
{
private:
	explicit CWeaponBar(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CWeaponBar();

public:
	virtual HRESULT			Ready_Object();
	virtual _int			Update_Object(const _float& fTimeDelta)				override;
	virtual void			LateUpdate_Object(const _float& fTimeDelta)			override;
	virtual void			Render_Object()										override;

private:
	virtual HRESULT			Add_Component();

private:
	Engine::CRcTex*			m_pBufferCom	 = nullptr;

	Engine::CTexture*		m_pTexBarCom	 = nullptr;
	Engine::CTexture*		m_pTexWeaponCom  = nullptr;
	//Engine::CTexture*		m_pTexGageCom	 = nullptr;

	Engine::CTransform*		m_pTransformCom[3];

	Engine::CRenderer*		m_pRendererCom	 = nullptr;

public:
	static CWeaponBar*			Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	void					Free() override;
};

#endif // UI_h__