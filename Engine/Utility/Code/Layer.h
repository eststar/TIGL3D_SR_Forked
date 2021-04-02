#ifndef Layer_h__
#define Layer_h__

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CLayer : public CBase
{
protected:
	explicit CLayer();
	virtual ~CLayer();

public:
	CComponent*				Get_Component(const _int& iIndex,			//찾고자 하는 대상이 벡터의 몇번째에 있는지 (Enum가능) 
										  const _tchar* pObjTag,		//Index번째의 List에서 해당 객체의 tag값이 무엇인지
										  const _tchar* pComponentTag,	//객체가 가지고 있는 Component의 Tag값이 무엇인지
										  COMPONENTID eType);			//Component의 타입이 무엇인지

	CGameObject*			Get_FirstGameObject(const _int& iIndex);
	list<CGameObject*>		Get_List(const _int& iIndex);

public:
	HRESULT					Add_GameObject(const _int& iIndex, CGameObject* pGameObject);

public:
	virtual HRESULT			Ready_Layer(_int iVecSize);
	virtual  _int			Update_Layer(const _float& fTimeDelta);
	virtual  void			LateUpdate_Layer(const _float& fTimeDelta);

protected:
	vector<list<CGameObject*>>			m_vecObjectList;
	_int								m_iVecSize;

public:
	virtual void		Free();
};

END

#endif // Layer_h__