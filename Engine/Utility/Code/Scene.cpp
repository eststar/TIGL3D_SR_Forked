#include "Scene.h"

USING(Engine)

Engine::CScene::CScene(LPDIRECT3DDEVICE9 pGraphicDev)
	:m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Engine::CScene::~CScene()
{

}

Engine::CComponent* CScene::Get_Component(LAYERID eLayerID, const _int& iIndex, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eType)
{
	auto	iter = m_mapLayer.find(eLayerID);

	if (iter == m_mapLayer.end())
		return nullptr;

	return iter->second->Get_Component(iIndex, pObjTag, pComponentTag, eType);
}

HRESULT Engine::CScene::Add_GameObject(LAYERID eLayerID, const _int& iIndex, CGameObject* pGameObject)
{
	auto	iter = m_mapLayer.find(eLayerID);

	if (iter == m_mapLayer.end())
		return E_FAIL;
	
	if (FAILED(iter->second->Add_GameObject(iIndex, pGameObject)))
		return E_FAIL;
	
	return S_OK;
}


HRESULT Engine::CScene::Ready_Scene()
{
	return S_OK;
}

Engine::_int Engine::CScene::Update_Scene(const _float& fTimeDelta)
{
	_int iExit = 0;

	for (auto& iter : m_mapLayer)
	{
		iExit = iter.second->Update_Layer(fTimeDelta);

		if (iExit & 0x80000000)
			return -1;
	}

	return 0;
}

void CScene::LateUpdate_Scene(const _float & fTimeDelta)
{
	for (auto& iter : m_mapLayer)
	{
		iter.second->LateUpdate_Layer(fTimeDelta);
	}
}

void Engine::CScene::Render_Scene()
{
	
}

void Engine::CScene::Free()
{
	Safe_Release(m_pGraphicDev);

	for_each(m_mapLayer.begin(), m_mapLayer.end(), CDeleteMap());
	m_mapLayer.clear();
}