#include "Export_Utility.h"
#include "Management.h"

USING(Engine)
IMPLEMENT_SINGLETON(CManagement)


Engine::CManagement::CManagement()
	:m_pScene(nullptr)
{
}

Engine::CManagement::~CManagement()
{
	Free();
}

Engine::CComponent* CManagement::Get_Component(LAYERID eLayerID, const _int& iIndex, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eType)
{
	if (nullptr == m_pScene)
		return nullptr;

	return m_pScene->Get_Component(eLayerID, iIndex, pObjTag, pComponentTag, eType);
}

HRESULT Engine::CManagement::Add_GameObject(LAYERID eLayerID, const _int & iIndex, CGameObject * pGameObject)
{
	if (nullptr == m_pScene)
		return E_FAIL;

	if (FAILED(m_pScene->Add_GameObject(eLayerID, iIndex, pGameObject)))
		return E_FAIL;

	return S_OK;
}

HRESULT Engine::CManagement::SetUp_Scene(CScene* pScene)
{
	if (nullptr != m_pScene)
		Safe_Release(m_pScene);

	Engine::Clear_RenderGroup();		// 씬을 교체하면서 전에 그리던 오브젝트들을 모두 지운다.

	m_pScene = pScene;

	return S_OK;
}

Engine::_int Engine::CManagement::Update_Scene(const _float& fTimeDelta)
{
	if (nullptr == m_pScene)
		return -1;

	m_pScene->Update_Scene(fTimeDelta);

	return 0;
}

void CManagement::LateUpdate_Scene(const _float & fTimeDelta)
{
	if (nullptr == m_pScene)
		return;

	m_pScene->LateUpdate_Scene(fTimeDelta);
}

void Engine::CManagement::Render_Scene(LPDIRECT3DDEVICE9& pGraphicDev)
{
	Engine::Render_GameObject(pGraphicDev);

	if (nullptr == m_pScene)
		return;

	m_pScene->Render_Scene();
}

void Engine::CManagement::Free()
{
	Safe_Release(m_pScene);
}

