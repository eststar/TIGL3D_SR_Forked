#include "stdafx.h"
#include "EnvirLayer.h"

#include "Export_Function.h"

#include "CollisionMgr.h"

USING(Engine)

CEnvirLayer::CEnvirLayer()
{
}

CEnvirLayer::~CEnvirLayer()
{
}

HRESULT CEnvirLayer::Ready_Layer(_int iVecSize)
{
	CLayer::Ready_Layer(iVecSize);

	return S_OK;
}

_int CEnvirLayer::Update_Layer(const _float & fTimeDelta)
{
	CLayer::Update_Layer(fTimeDelta);

	return 0;
}

void CEnvirLayer::LateUpdate_Layer(const _float & fTimeDelta)
{
	CLayer::LateUpdate_Layer(fTimeDelta);
}

CEnvirLayer * CEnvirLayer::Create(_int iVecSize)
{
	CEnvirLayer*		pInstance = new CEnvirLayer;

	if (FAILED(pInstance->Ready_Layer(iVecSize)))
		Safe_Release(pInstance);

	return pInstance;
}

void CEnvirLayer::Free()
{
	CLayer::Free();
}
