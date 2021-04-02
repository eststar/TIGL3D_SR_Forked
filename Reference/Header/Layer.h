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
	CComponent*				Get_Component(const _int& iIndex,			//ã���� �ϴ� ����� ������ ���°�� �ִ��� (Enum����) 
										  const _tchar* pObjTag,		//Index��°�� List���� �ش� ��ü�� tag���� ��������
										  const _tchar* pComponentTag,	//��ü�� ������ �ִ� Component�� Tag���� ��������
										  COMPONENTID eType);			//Component�� Ÿ���� ��������

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