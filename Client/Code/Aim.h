#ifndef Aim_h__
#define Aim_h__

#include "Define.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END

/* 0405_다영 h & cpp 추가*/

class CAim : public Engine::CGameObject
{
private:
	explicit CAim(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CAim();

public:
	virtual HRESULT			Ready_Object();
	virtual _int			Update_Object(const _float& fTimeDelta)				override;
	virtual void			LateUpdate_Object(const _float& fTimeDelta)			override;
	virtual void			Render_Object()										override;

private:
	virtual HRESULT			Add_Component();

private:
	Engine::CRcTex*			m_pBufferCom	 = nullptr;
	Engine::CTexture*		m_pTextureCom	 = nullptr;
	Engine::CRenderer*		m_pRendererCom	 = nullptr;
	Engine::CTransform*		m_pTransformCom	 = nullptr;

private:
	FRAME					m_tFrame;
	_float					m_fScale;
	_float					m_fGage;

public:
	static CAim*			Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	void					Free() override;
};

#endif // UI_h__