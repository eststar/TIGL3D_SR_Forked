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

//Obj List별로 충돌 확인 함수들
//필요에 따라 아래 함수들을 참고하여 원하는 함수들 생성해서 사용할 수 있음

public:
	/*진행 중 -> 아직 쓸 수 없음*/
	static void Collision_Push(list<CGameObject*>& rDstListObject, list<CGameObject*>& rSrcListObject);

	//구(Dst) vs 구(Src) 충돌
	static void Collision_SphereSphere(list<CGameObject*>& rDstListObject, list<CGameObject*>& rSrcListObject);
	
	//구(Dst) vs 점(Src) 충돌
	static void Collision_SpherePoint(list<CGameObject*>& rDstListObject, list<CGameObject*>& rSrcListObject);

//충돌 확인용 bool 반환 함수들
public:
	static bool Check_SphereSphere(CTransform* pDstTrans, CTransform* pSrcTrans, _vec3 *pOutVec);
	static bool Check_SpherePoint(CTransform* pDstTrans, CTransform* pSrcTrans, _vec3 *pOutVec);

// .cpp 맨 아래에 참고용으로 2D 때 사용했던 충돌 함수 有
};


#endif // CollisionMgr_h__
