#ifndef ItemStat_h__
#define ItemStat_h__

#include "Define.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END
class CItemStat : public Engine::CGameObject
{
private:
	explicit CItemStat(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CItemStat();

public:
	virtual HRESULT			Ready_Object(_tchar* pObjTag, const _vec3* pPos);
	virtual _int			Update_Object(const _float& fTimeDelta)				override;
	virtual void			LateUpdate_Object(const _float& fTimeDelta)			override;
	virtual void			Render_Object()										override;

private:
	virtual HRESULT			Add_Component();

private:
	Engine::CRcTex*			m_pBufferCom	 = nullptr;
	Engine::CRenderer*		m_pRendererCom	 = nullptr;
	Engine::CTexture*		m_pTextureCom	 = nullptr;
	Engine::CTransform*		m_pTransformCom[4];

private:
	_uint					m_iDrawCnt[3];

public:
	static CItemStat*			Create(LPDIRECT3DDEVICE9 pGraphicDev, _tchar* pObjTag, const _vec3* pPos);

public:
	void					Free() override;
};

#endif // UI_h__