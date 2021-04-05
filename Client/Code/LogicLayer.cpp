#include "stdafx.h"
#include "LogicLayer.h"

#include "Export_Function.h"

#include "CollisionMgr.h"

USING(Engine)

CLogicLayer::CLogicLayer()
{
}

CLogicLayer::~CLogicLayer()
{
}

HRESULT CLogicLayer::Ready_Layer(_int iVecSize)
{
	CLayer::Ready_Layer(iVecSize);

	return S_OK;
}

_int CLogicLayer::Update_Layer(const _float & fTimeDelta)
{
	CLayer::Update_Layer(fTimeDelta);

	return 0;
}

void CLogicLayer::LateUpdate_Layer(const _float & fTimeDelta)
{
	CLayer::LateUpdate_Layer(fTimeDelta);

	CCollisionMgr::Collision_SpherePoint(m_vecObjectList[LOGIC_MONSTERWEAPON], m_vecObjectList[LOGIC_PLAYER]);
	CCollisionMgr::Collision_SpherePoint(m_vecObjectList[LOGIC_PLAYERWEAPON], m_vecObjectList[LOGIC_MONSTER]);
	//CCollisionMgr::Collision_SphereSphere(m_vecObjectList[LOGIC_MONSTER], m_vecObjectList[LOGIC_PLAYERWEAPON]);
	CCollisionMgr::Collision_Push(m_vecObjectList[LOGIC_OBJECT], m_vecObjectList[LOGIC_PLAYER]);
}

CLogicLayer * CLogicLayer::Create(_int iVecSize)
{
	CLogicLayer*		pInstance = new CLogicLayer;

	if (FAILED(pInstance->Ready_Layer(iVecSize)))
		Safe_Release(pInstance);

	return pInstance;
}

void CLogicLayer::Free()
{
	CLayer::Free();
}
