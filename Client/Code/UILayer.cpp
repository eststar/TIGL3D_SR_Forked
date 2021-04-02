#include "stdafx.h"
#include "UILayer.h"

#include "Export_Function.h"

#include "CollisionMgr.h"

USING(Engine)

CUILayer::CUILayer()
{
}

CUILayer::~CUILayer()
{
}

HRESULT CUILayer::Ready_Layer(_int iVecSize)
{
	CLayer::Ready_Layer(iVecSize);

	return S_OK;
}

_int CUILayer::Update_Layer(const _float & fTimeDelta)
{
	CLayer::Update_Layer(fTimeDelta);

	return 0;
}

void CUILayer::LateUpdate_Layer(const _float & fTimeDelta)
{
	CLayer::LateUpdate_Layer(fTimeDelta);
}

CUILayer * CUILayer::Create(_int iVecSize)
{
	CUILayer*		pInstance = new CUILayer;

	if (FAILED(pInstance->Ready_Layer(iVecSize)))
		Safe_Release(pInstance);

	return pInstance;
}

void CUILayer::Free()
{
	CLayer::Free();
}
