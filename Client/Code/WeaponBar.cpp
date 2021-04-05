#include "stdafx.h"
#include "WeaponBar.h"

#include "Export_Function.h"

USING(Engine)

CWeaponBar::CWeaponBar(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CWeaponBar::~CWeaponBar()
{

}

void CWeaponBar::Free()
{
	CGameObject::Free();

	for (int i = 1; i < 3; ++i)
	{
		Safe_Release(m_pTransformCom[i]);
	}
}

HRESULT CWeaponBar::Ready_Object()
{
	m_pObjTag = L"WeaponBar";

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

Engine::_int CWeaponBar::Update_Object(const _float& fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(RENDER_UI, this);

	return OBJ_NOEVENT;
}

void CWeaponBar::LateUpdate_Object(const _float & fTimeDelta)
{
	//
}

void CWeaponBar::Render_Object()
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[0]->Get_WorldMatrix());
	m_pTextureCom[]->Set_Texture();
	
	m_pBufferCom->Render_Buffers();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CWeaponBar::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTransformCom[0] = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_DYNAMIC].emplace(L"Com_Transform", pComponent);

	for (int i = 1; i < 3; ++i)
	{
		pComponent = m_pTransformCom[i] = Engine::CTransform::Create();
		NULL_CHECK_RETURN(pComponent, E_FAIL);
	}

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"WeaponBar"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Texture", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Renderer", pComponent);

	return S_OK;
}

CWeaponBar * CWeaponBar::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWeaponBar*	pInstance = new CWeaponBar(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}