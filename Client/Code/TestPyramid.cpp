#include "stdafx.h"
#include "TestPyramid.h"

#include "Export_Function.h"

USING(Engine)

CTestPyramid::CTestPyramid(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CCreature(pGraphicDev)
{
}

CTestPyramid::~CTestPyramid()
{
}

CTestPyramid* CTestPyramid::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTestPyramid*	pInstance = new CTestPyramid(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CTestPyramid::Free()
{
	CGameObject::Free();
}

HRESULT CTestPyramid::Ready_Object()
{
	m_pObjTag = L"TestPyramid";

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Pos(&_vec3(10.f, 2.f, 10.f));
	m_pTransformCom->Set_Scale(&_vec3(4.f, 4.f, 4.f));
	return S_OK;
}

Engine::_int CTestPyramid::Update_Object(const _float& fTimeDelta)
{
	if (m_bIsDead)
		return OBJ_DEAD;

	CGameObject::Update_Object(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(RENDER_ALPHA, this);

	return OBJ_NOEVENT;
}

void CTestPyramid::LateUpdate_Object(const _float & fTimeDelta)
{
	CGameObject::LateUpdate_Object(fTimeDelta);
}

void CTestPyramid::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffers();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CTestPyramid::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;
	
	pComponent = m_pBufferCom = dynamic_cast<Engine::CTetraTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_TetraTex"));
	//pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	//pComponent = m_pBufferCom = dynamic_cast<Engine::CRcCol*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcCol"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_DYNAMIC].emplace(L"Com_Transform", pComponent);

	//pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_LOGO, L"Texture_Logo"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Texture", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Renderer", pComponent);

	
	return S_OK;
}