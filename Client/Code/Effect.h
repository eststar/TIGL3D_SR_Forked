#ifndef Effect_h__
#define Effect_h__

#include "Define.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END

/* 0405_다영 BillBoard 함수 추가 */

class CEffect : public Engine::CGameObject
{
private:
	explicit CEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEffect();

public:
	virtual HRESULT			Ready_Object(_tchar* pObjTag, const _vec3* pPos, const _vec3* pScale, RESOURCEID eTextureID);
	virtual _int			Update_Object(const _float& fTimeDelta)				override;
	virtual void			LateUpdate_Object(const _float& fTimeDelta)			override;
	virtual void			Render_Object()										override;
private:
	HRESULT		Add_Component(RESOURCEID eTextureID);
	void		BillBoard();

private:
	Engine::CRcTex*			m_pBufferCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;

private:
	FRAME	m_tFrame;
	_float	m_fSpeed;
	_bool	m_bIsDead;
	_float	m_fLifeTime;

public:
	static CEffect*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _tchar* pObjTag, const _vec3* pPos, const _vec3* pScale, RESOURCEID eTextureID);

public:
	void		Free() override;
};


#endif // Effect_h__
