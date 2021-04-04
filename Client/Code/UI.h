#ifndef UI_h__
#define UI_h__

#include "Define.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END
class CUI : public Engine::CGameObject
{
protected:
	explicit CUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUI();

public:
	virtual HRESULT			Ready_Object(_tchar* pObjTag, const _vec3* pPos, const _vec3* pScale, RESOURCEID eTextureID);
	virtual _int			Update_Object(const _float& fTimeDelta)				override;
	virtual void			LateUpdate_Object(const _float& fTimeDelta)			override;
	virtual void			Render_Object()										override;

protected:
	virtual HRESULT			Add_Component(RESOURCEID eID);

protected:
	Engine::CRcTex*			m_pBufferCom	 = nullptr;
	Engine::CTransform*		m_pTransformCom	 = nullptr;
	Engine::CTexture*		m_pTextureCom	 = nullptr;
	Engine::CRenderer*		m_pRendererCom	 = nullptr;

protected:
	_uint	m_iDrawCnt;
	bool	m_bIsDead;

public:
	void		Free() override;
};

#endif // UI_h__
