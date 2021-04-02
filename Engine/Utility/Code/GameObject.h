#ifndef GameObject_h__
#define GameObject_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject();

public:
	CComponent*				Get_Component(const _tchar* pComponentTag, COMPONENTID eType);
	const _tchar*			Get_ObjTag() { return m_pObjTag; }
public:
	void					Set_ObjTag(_tchar* pObjTag) { m_pObjTag = pObjTag; }

public:
	virtual HRESULT			Ready_Object();
	virtual _int			Update_Object(const _float& fTimeDelta);
	virtual void			LateUpdate_Object(const _float& fTimeDelta);
	virtual void			Render_Object();

private:
	CComponent*				Find_Component(const _tchar* pComponentTag, COMPONENTID eType);

protected:
	LPDIRECT3DDEVICE9						m_pGraphicDev;
	_tchar*									m_pObjTag;

	map<const _tchar*, CComponent*>			m_mapComponent[COM_END];

public:
	virtual void		Free();
};
END
#endif // GameObject_h__