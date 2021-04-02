#ifndef Scene_h__
#define Scene_h__

#include "Layer.h"

BEGIN(Engine)

class ENGINE_DLL CScene : public CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScene();

public:
	CComponent*				Get_Component(LAYERID eLayerID, const _int& iIndex, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eType);
	HRESULT					Add_GameObject(LAYERID eLayerID, const _int& iIndex, CGameObject* pGameObject);

public:
	virtual HRESULT			Ready_Scene();
	virtual _int			Update_Scene(const _float& fTimeDelta);
	virtual void			LateUpdate_Scene(const _float& fTimeDelta);
	virtual void			Render_Scene();

protected:
	LPDIRECT3DDEVICE9						m_pGraphicDev;
	map<LAYERID, CLayer*>					m_mapLayer;

public:
	virtual void		Free();
};


END


#endif // Scene_h__
