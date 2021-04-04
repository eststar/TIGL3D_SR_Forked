#include "stdafx.h"
#include "Terrain.h"

#include "Export_Function.h"

USING(Engine)

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

//CTerrain::CTerrain(const CTerrain & rhs)
//	: Engine::CGameObject(rhs.m_pGraphicDev)
//{
//	Add_Component();
//	m_pBufferCom = nullptr;
//	m_pTextureCom = nullptr;
//	//m_pRendererCom = nullptr;
//	m_pTransformCom = nullptr;
//
//	m_bShowTexture	= false;
//	m_dwFillmode		= D3DFILL_WIREFRAME;
//}

CTerrain::~CTerrain(void)
{
}


CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTerrain*	pInstance = new CTerrain(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CTerrain::Free(void)
{
	CGameObject::Free();
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
}

HRESULT CTerrain::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

Engine::_int CTerrain::Update_Object(const _float& fTimeDelta)
{
	//Safe_Release(m_mapComponent[Engine::COM_DYNAMIC][L"Com_Transform"]);
	//m_mapComponent[Engine::COM_DYNAMIC].erase(L"Com_Transform");

	//m_mapComponent[Engine::COM_DYNAMIC].emplace(L"Com_Transform", m_pTransformCom);

	
	CGameObject::Update_Object(fTimeDelta);
	
	//_int iExit = 0;
	//
	//for (auto& iter : m_mapComponent[COM_DYNAMIC])
	//	iExit = iter.second->Update_Component(fTimeDelta);
	//m_pRendererCom->Add_RenderGroup(RENDER_PRIORITY, this);
	//
	//return iExit;

	//DYNAMIC인 친구를 직접 돌리는 거임
	//m_pTransformCom->Update_Component(fTimeDelta);


	return 0;
}

void CTerrain::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, m_dwFillmode);
	
	if (m_bShowTexture)
		m_pTextureCom->Set_Texture();
	else
	{
		m_pGraphicDev->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	}
	m_pBufferCom->Render_Buffers();		

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CTerrain::Add_Component(void)
{
	//동규_0330 reserve 삭제

	//Engine::Reserve_ContainerSize(RESOURCE_END);

	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_DYNAMIC].emplace(L"Com_Transform", pComponent);


	//FAILED_CHECK_RETURN(Engine::Ready_Buffers(m_pGraphicDev, RESOURCE_STATIC, L"Buffer_TerrainTex", Engine::BUFFER_TERRAINTEX, VTXCNTX, VTXCNTX, VTXITV), E_FAIL);
	
	pComponent = m_pBufferCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_TerrainTex_1_1_1"));
	//pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Grass"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Texture", pComponent);

	//pComponent = m_pRendererCom = Engine::Get_Renderer();
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//pComponent->AddRef();
	//m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Renderer", pComponent);

	return S_OK;
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
