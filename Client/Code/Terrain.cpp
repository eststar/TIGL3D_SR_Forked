#include "stdafx.h"
#include "Terrain.h"

#include "Export_Function.h"

USING(Engine)

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CTerrain::~CTerrain()
{

}

CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTerrain*	pInstance = new CTerrain(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CTerrain::Free()
{
	CGameObject::Free();
}

HRESULT CTerrain::Ready_Object()
{
	m_pObjTag = L"Terrain";

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

Engine::_int CTerrain::Update_Object(const _float& fTimeDelta)
{
	CGameObject::Update_Object(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(RENDER_PRIORITY, this);

	return OBJ_NOEVENT;
}

void CTerrain::LateUpdate_Object(const _float & fTimeDelta)
{
	CGameObject::LateUpdate_Object(fTimeDelta);
}

void CTerrain::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffers();		

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CTerrain::Set_Component(COMPONENTID eCOMID, const _tchar* strCOMTag, CComponent* pCom)
{
	auto iter = find_if(m_mapComponent[eCOMID].begin(), m_mapComponent[eCOMID].end(), CTagFinder(strCOMTag));

	if (iter == m_mapComponent[eCOMID].end())
		return;
	Safe_Release(iter->second);
	m_mapComponent[eCOMID].erase(iter);

	if (strCOMTag == L"Com_Transform")
	{
		Safe_Release(m_pTransformCom);
		m_pTransformCom = dynamic_cast<CTransform*>(pCom);
		m_mapComponent[eCOMID].emplace(strCOMTag, pCom);
	}
}

void CTerrain::Set_Resource(COMPONENTID eCOMID, const _tchar* strCOMTag, CResources* pResource)
{
	//동일한 이름의 클론 버퍼가 들어있는가?
	auto iter = find_if(m_mapComponent[eCOMID].begin(), m_mapComponent[eCOMID].end(), CTagFinder(strCOMTag));
	//있으면 기존거 지우기
	if (iter != m_mapComponent[eCOMID].end())
	{
		Safe_Release(iter->second);
		m_mapComponent[eCOMID].erase(iter);
	}

	CComponent* pCOM = nullptr;

	//클론한 리소스를 각 클래스로 다캐, 다캐한걸 컴포넌트 포인터에 넣기
	if (strCOMTag == L"Com_Texture")
	{
		Safe_Release(m_pTextureCom);
		m_pTextureCom = dynamic_cast<CTexture*>(pResource);
		pCOM = m_pTextureCom;
	}
	else if (strCOMTag == L"Com_Buffer")
	{
		Safe_Release(m_pBufferCom);
		m_pBufferCom = dynamic_cast<CTerrainTex*>(pResource);
		pCOM = m_pBufferCom;
	}
	NULL_CHECK(pCOM);
	m_mapComponent[eCOMID].emplace(strCOMTag, pCOM);
}


HRESULT CTerrain::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_TerrainTex_Default"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Buffer", pComponent);

	//pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Terrain"));
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Terrain_Floor"));

	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Texture", pComponent);

	//pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_TerrainY"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Texture", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Renderer", pComponent);

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_DYNAMIC].emplace(L"Com_Transform", pComponent);

	return S_OK;
}
