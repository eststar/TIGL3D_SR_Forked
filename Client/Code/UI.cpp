#include "stdafx.h"
#include "UI.h"

#include "Export_Function.h"

USING(Engine)

CUI::CUI(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_iDrawCnt(0)
	, m_bIsDead(false)
{

}

CUI::~CUI()
{

}

void CUI::Free()
{
	CGameObject::Free();
}

HRESULT CUI::Ready_Object(_tchar* pObjTag, const _vec3* pPos, const _vec3* pScale, RESOURCEID eTextureID)
{
	m_pObjTag = pObjTag;

	FAILED_CHECK_RETURN(Add_Component(eTextureID), E_FAIL);

	m_pTransformCom->Set_Pos(pPos);
	m_pTransformCom->Set_Scale(pScale);
	return S_OK;
}

Engine::_int CUI::Update_Object(const _float& fTimeDelta)
{	
	if (m_bIsDead)
		return OBJ_DEAD;

	CGameObject::Update_Object(fTimeDelta);


	return OBJ_NOEVENT;
}

void CUI::LateUpdate_Object(const _float & fTimeDelta)
{
	CGameObject::LateUpdate_Object(fTimeDelta);
	m_pRendererCom->Add_RenderGroup(RENDER_UI, this);
}

void CUI::Render_Object()
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pTextureCom->Set_Texture(m_iDrawCnt);
	m_pBufferCom->Render_Buffers();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CUI::Add_Component(RESOURCEID eID)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(eID, m_pObjTag));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Texture", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Renderer", pComponent);

	return S_OK;
}
