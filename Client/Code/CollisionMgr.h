#ifndef CollisionMgr_h__
#define CollisionMgr_h__

#include "Base.h"

#include "Define.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Engine)

class CGameObject;
class CTransform;

END

class CCollisionMgr : public CBase
{
public:
	CCollisionMgr();
	~CCollisionMgr();

//Obj List���� �浹 Ȯ�� �Լ���
//�ʿ信 ���� �Ʒ� �Լ����� �����Ͽ� ���ϴ� �Լ��� �����ؼ� ����� �� ����

public:
	/*���� �� -> ���� �� �� ����*/
	static void Collision_Push(list<CGameObject*>& rDstListObject, list<CGameObject*>& rSrcListObject);

	//��(Dst) vs ��(Src) �浹
	static void Collision_SphereSphere(list<CGameObject*>& rDstListObject, list<CGameObject*>& rSrcListObject);
	
	//��(Dst) vs ��(Src) �浹
	static void Collision_SpherePoint(list<CGameObject*>& rDstListObject, list<CGameObject*>& rSrcListObject);

//�浹 Ȯ�ο� bool ��ȯ �Լ���
public:
	static bool Check_SphereSphere(CTransform* pDstTrans, CTransform* pSrcTrans, _vec3 *pOutVec);
	static bool Check_SpherePoint(CTransform* pDstTrans, CTransform* pSrcTrans, _vec3 *pOutVec);

// .cpp �� �Ʒ��� ��������� 2D �� ����ߴ� �浹 �Լ� ��
};


#endif // CollisionMgr_h__
