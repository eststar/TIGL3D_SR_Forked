#include "stdafx.h"
#include "CollisionMgr.h"

USING(Engine)

CCollisionMgr::CCollisionMgr()
{
}

CCollisionMgr::~CCollisionMgr()
{
}

//Dest�� Look�������� �и���
void CCollisionMgr::Collision_Push(list<CGameObject*>& rDstListObject, list<CGameObject*>& rSrcListObject)
{
	//�� �� �� ����Ʈ�� ��������� ����ó���� ����
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
				//X�� �Ǵ� Z�� �̵��� �ʿ��� ���
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
				//�÷��̾� ������ �и��� ���
				else
				{
					pDstTrans->Set_Pos(&_vec3(vDstPos.x + vPushDist.x, vDstPos.y, vDstPos.z + vPushDist.z));

				}
			}
		}
	}
}

//(Obj �� �� �� �浹)
void CCollisionMgr::Collision_SphereSphere(list<CGameObject*>& rDstListObject, list<CGameObject*>& rSrcListObject)
{
	//�� �� �� ����Ʈ�� ��������� ����ó���� ����
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
				//Dst ��ü ó�� �κ�
				CCreature* pDstCreature = dynamic_cast<CCreature*>(pDstObject);

				//�ڽİ�ü�� Creature���� Ȯ�����ִ� �κ� -> �½��� �˷��ֽŰ� �� �� ���ս��Ѻ� (�ʼ�X)
				if (nullptr != pDstCreature)
					pDstCreature->Set_IsDead();


			}
		}
	}
}

void CCollisionMgr::Collision_SpherePoint(list<CGameObject*>& rDstListObject, list<CGameObject*>& rSrcListObject)
{
	//�� �� �� ����Ʈ�� ��������� ����ó���� ����
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
				CCreature* pDstCreature = dynamic_cast<CCreature*>(pDstObject);
				//�ڽİ�ü�� Creature���� Ȯ�����ִ� �κ� -> �½��� �˷��ֽŰ� �� �� ���ս��Ѻ� (�ʼ�X)
				if (nullptr != pDstCreature)
					pDstCreature->Set_IsDead();

				//�� ������ �����ؼ� �ʿ信 ���� SrcObject�� 
			}
		}
	}
}

bool CCollisionMgr::Check_SphereSphere(CTransform* pDstTrans, CTransform* pSrcTrans, _vec3 *pOutVec)
{
	//�� �� �ϳ��� null���̸� ����ó���� ����
	if (nullptr == pDstTrans || nullptr == pSrcTrans)
		return false;

	//Dst�� Src���� POS�� �Ÿ�
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






/*����� - 2D �� ����ߴ� �浹 ���*/

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

				//���Ʒ��� �ε���
				if (fX > fY)
				{
					//Dest�� Src���� ������ �ε���
					if (pDestObject->Get_Info()->vPos.y < pSrcObject->Get_Info()->vPos.y)
						fY *= -1.f;

					pDestObject->Set_Pos(fDestX, fDestY + fY);
				}
				//�翷���� �ε���
				else
				{
					//Dest�� Src���� ���ʿ��� �ε���
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
	//Size�� ���ָ� �ȵǰ� ���� ���ϴ� �浹 ������ ���μ��� ������ �ʿ��� ���� �׳� 1�� ��
	float fRadiusSumX = 0.f; //(pDstInfo->vSize.x * 0.5f) + (pSrcInfo->vSize.x * 0.5f);
	float fRadiusSumY = 0.f; // (pDstInfo->vSize.y * 0.5f) + (pSrcInfo->vSize.y * 0.5f);


							 //���� INFOfkd rect �����̶� �ٸ��ٸ� rect ������ �������� ��Ƽ� ����� ����� �Ѵ�!
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