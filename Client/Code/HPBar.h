#ifndef HPBar_h__
#define HPBar_h__

#include "Define.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END
class CHPBar : public Engine::CGameObject
{
private:
	explicit CHPBar(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CHPBar();

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

	Engine::CTransform*		m_pTransformCom[5];

private:
	_uint					m_iDrawCnt[5];
	_int					m_iPlayerMaxHP;
	_int					m_iPlayerHP;

public:
	static CHPBar*			Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	void					Free() override;
};

#endif // UI_h__