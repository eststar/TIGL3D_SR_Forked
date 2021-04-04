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

	for (_uint i = 0; i < m_VecTerrain.size(); i++)
	{
		Safe_Delete(m_VecTerrain[i]);
	}
	m_VecTerrain.clear();
	m_VecTerrain.shrink_to_fit();

	for (_uint i = 0; i < m_VecTerrainInfo.size(); i++)
	{
		Safe_Delete(m_VecTerrainInfo[i]);
	}
	m_VecTerrainInfo.clear();
	m_VecTerrainInfo.shrink_to_fit();

}
