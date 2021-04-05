#include "stdafx.h"
#include "CollisionMgr.h"

USING(Engine)

CCollisionMgr::CCollisionMgr()
{
}

CCollisionMgr::~CCollisionMgr()
{
}

//Dest가 Look방향으로 밀리게
void CCollisionMgr::Collision_Push(list<CGameObject*>& rDstListObject, list<CGameObject*>& rSrcListObject)
{
	//둘 중 한 리스트라도 비어있으면 예외처리로 종료
	if (rDstListObject.empty() || rSrcListObject.empty())
		return;

	_vec3 vPushDist;

	for (auto& pDstObject : rDstListObject)
	{
		for (auto& pSrcObject : rSrcListObject)
		{
			CTransform* pDstTrans = dynamic_cast<CTransform*>(pDstObject->Get_Component(L"Com_Transform", Engine::COM_DYNAMIC));
			CTransform* pSrcTrans = dynamic_cast<CTransform*>(pSrcObject->Get_Component(L"Com_Transform", Engine::COM_DYNAMIC));
			if (nullptr == pDstTrans || nullptr == pSrcTrans)
				continue;

			_vec3 vDstPos = *pDstTrans->Get_Info(INFO_POS);
			_vec3 vSrcPos = *pSrcTrans->Get_Info(INFO_POS);

			if (Check_SpherePoint(pDstTrans, pSrcTrans, &vPushDist))
			{
				//X축 또는 Z축 이동만 필요한 경우
				if (!lstrcmp(L"TestPyramid", pDstObject->Get_ObjTag()))
				{
					if ((vPushDist.x*vPushDist.x) > (vPushDist.z*vPushDist.z))
					{
						pDstTrans->Set_Pos(&_vec3(vDstPos.x + vPushDist.x, vDstPos.y, vDstPos.z));
					}
					else
					{
						pDstTrans->Set_Pos(&_vec3(vDstPos.x, vDstPos.y, vDstPos.z + vPushDist.z));

					}
				}
				//플레이어 방향대로 밀리는 경우
				else
				{
					pDstTrans->Set_Pos(&_vec3(vDstPos.x + vPushDist.x, vDstPos.y, vDstPos.z + vPushDist.z));

				}
			}
		}
	}
}

//(Obj 둘 다 구 충돌)
void CCollisionMgr::Collision_SphereSphere(list<CGameObject*>& rDstListObject, list<CGameObject*>& rSrcListObject)
{
	//둘 중 한 리스트라도 비어있으면 예외처리로 종료
	if (rDstListObject.empty() || rSrcListObject.empty())
		return;

	_vec3 vPushDist;

	for (auto& pDstObject : rDstListObject)
	{
		for (auto& pSrcObject : rSrcListObject)
		{
			if (Check_SphereSphere(dynamic_cast<CTransform*>(pDstObject->Get_Component(L"Com_Transform", Engine::COM_DYNAMIC)),
								   dynamic_cast<CTransform*>(pSrcObject->Get_Component(L"Com_Transform", Engine::COM_DYNAMIC)), &vPushDist))
			{
				//Dst 객체 처리 부분
				CCreature* pDstCreature = dynamic_cast<CCreature*>(pDstObject);

				//자식객체가 Creature인지 확인해주는 부분 -> 태쌤이 알려주신거 한 번 적합시켜봄 (필수X)
				if (nullptr != pDstCreature)
					pDstCreature->Set_IsDead();


			}
		}
	}
}

void CCollisionMgr::Collision_SpherePoint(list<CGameObject*>& rDstListObject, list<CGameObject*>& rSrcListObject)
{
	//둘 중 한 리스트라도 비어있으면 예외처리로 종료
	if (rDstListObject.empty() || rSrcListObject.empty())
		return;

	_vec3 vPushDist;

	for (auto& pDstObject : rDstListObject)
	{
		for (auto& pSrcObject : rSrcListObject)
		{
			if (Check_SpherePoint(dynamic_cast<CTransform*>(pDstObject->Get_Component(L"Com_Transform", Engine::COM_DYNAMIC)),
								  dynamic_cast<CTransform*>(pSrcObject->Get_Component(L"Com_Transform", Engine::COM_DYNAMIC)), &vPushDist))
			{
				CCreature* pCreature = dynamic_cast<CCreature*>(pDstObject);
				//자식객체가 Creature인지 확인해주는 부분 -> 태쌤이 알려주신거 한 번 적합시켜봄 (필수X)
				if (nullptr != pCreature)
					pCreature->Set_IsDead();

				/* 0405_다영 충돌처리 추가 */
				pCreature = dynamic_cast<CCreature*>(pSrcObject);
				if (nullptr != pCreature)
					pCreature->Set_IsDead();
			}
		}
	}
}

bool CCollisionMgr::Check_SphereSphere(CTransform* pDstTrans, CTransform* pSrcTrans, _vec3 *pOutVec)
{
	//둘 중 하나라도 null값이면 예외처리로 종료
	if (nullptr == pDstTrans || nullptr == pSrcTrans)
		return false;

	//Dst와 Src에서 POS의 거리
	_vec3 vDist = *(pDstTrans->Get_Info(Engine::INFO_POS)) - *(pSrcTrans->Get_Info(Engine::INFO_POS));
	_float fDist = D3DXVec3Length(&vDist);
	
	_vec3 vDstLength = { 1.f, 1.f, 0.f };
	D3DXVec3TransformNormal(&vDstLength, &vDstLength, pDstTrans->Get_WorldMatrix());
	_float fDstLength = D3DXVec3Length(&vDstLength);

	_vec3 vSrcLength = { 1.f, 1.f, 0.f };
	D3DXVec3TransformNormal(&vSrcLength, &vSrcLength, pSrcTrans->Get_WorldMatrix());
	_float fSrcLength = D3DXVec3Length(&vSrcLength);
	
	float fRadiusSum = fDstLength + fSrcLength;

	if (fRadiusSum >= fDist)
	{
		float fDistance = fRadiusSum - fDist;
		D3DXVec3Normalize(pOutVec, &vDist);
		*pOutVec *= fDistance;

		return true;
	}

	return false;
}

bool CCollisionMgr::Check_SpherePoint(CTransform * pDstTrans, CTransform * pSrcTrans, _vec3 * pOutVec)
{
	if (nullptr == pDstTrans || nullptr == pSrcTrans)
		return false;

	_vec3 vDist = *(pDstTrans->Get_Info(Engine::INFO_POS)) - *(pSrcTrans->Get_Info(Engine::INFO_POS));
	_float fDist = D3DXVec3Length(&vDist);

	_vec3 vDstLength = { 1.f, 1.f, 0.f };
	D3DXVec3TransformNormal(&vDstLength, &vDstLength, pDstTrans->Get_WorldMatrix());
	_float fDstLength = D3DXVec3Length(&vDstLength);

	if (fDstLength >= fDist)
	{
		float fDistance = fDstLength - fDist;
		D3DXVec3Normalize(pOutVec, &vDist);
		*pOutVec *= fDistance;
		return true;
	}

	return false;
}






/*참고용 - 2D 때 사용했던 충돌 방식*/

/*
void CCollisionMgr::CollisionRectPush(list<CGameObject*>& rDstListObject, list<CGameObject*>& rSrcListObject)
{
	float fX = 0.f, fY = 0.f;

	for (auto& pDestObject : rDstListObject)
	{
		for (auto& pSrcObject : rSrcListObject)
		{
			if (CheckRect(pDestObject->Get_Info(), pSrcObject->Get_Info(), &fX, &fY))
			{
				float fDestX = pDestObject->Get_Info()->vPos.x;
				float fDestY = pDestObject->Get_Info()->vPos.y;

				//위아래로 부딪힘
				if (fX > fY)
				{
					//Dest가 Src보다 위에서 부딪힘
					if (pDestObject->Get_Info()->vPos.y < pSrcObject->Get_Info()->vPos.y)
						fY *= -1.f;

					pDestObject->Set_Pos(fDestX, fDestY + fY);
				}
				//양옆으로 부딪힘
				else
				{
					//Dest가 Src보다 왼쪽에서 부딪힘
					if (pDestObject->Get_Info()->vPos.x < pSrcObject->Get_Info()->vPos.x)
						fX *= -1.f;
					pDestObject->Set_Pos(fDestX + fX, fDestY);
				}
			}
		}
	}
}
bool CCollisionMgr::CheckRect(float * pOutX, float * pOutY)
{
	//Size로 해주면 안되고 내가 원하는 충돌 범위의 가로세로 범위가 필요함 저건 그냥 1임 ㅠ
	float fRadiusSumX = 0.f; //(pDstInfo->vSize.x * 0.5f) + (pSrcInfo->vSize.x * 0.5f);
	float fRadiusSumY = 0.f; // (pDstInfo->vSize.y * 0.5f) + (pSrcInfo->vSize.y * 0.5f);


							 //만약 INFOfkd rect 중점이랑 다르다면 rect 중점을 기준으로 잡아서 계산을 해줘야 한다!
	float fDistX = 0.f;//fabs(pDstInfo->vPos.x - pSrcInfo->vPos.x);
	float fDistY = 0.f;//fabs(pDstInfo->vPos.y - pSrcInfo->vPos.y);

	if (fRadiusSumX > fDistX && fRadiusSumY > fDistY)
	{
		*pOutX = fRadiusSumX - fDistX;
		*pOutY = fRadiusSumY - fDistY;

		return true;
	}
	return false;
}

*/