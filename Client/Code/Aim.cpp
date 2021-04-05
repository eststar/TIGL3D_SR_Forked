#include "stdafx.h"
#include "Aim.h"

#include "Export_Function.h"

USING(Engine)

CAim::CAim(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_fScale(0.f)
	, m_fGage(0.1f)
{
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}

CAim::~CAim()
{

}

void CAim::Free()
{
	CGameObject::Free();
}

HRESULT CAim::Ready_Object()
{
	m_pObjTag = L"Aim";

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->Set_Pos(&_vec3(0.f, -0.01f, 0.f));

	m_tFrame.fFrameStart = 0;
	m_tFrame.fFrameEnd = m_pTextureCom->Get_TextureSize() - 1.f;

	return S_OK;
}

Engine::_int CAim::Update_Object(const _float& fTimeDelta)
{
	m_fScale += m_fGage * fTimeDelta;

	if (m_fScale >= 0.07f || m_fScale <= 0.02f)
		m_fGage *= -1.f;

	m_pTransformCom->Set_Scale(&_vec3(m_fScale * ((float)WINCY / WINCX), m_fScale, 0.f));
	m_pTransformCom->Update_Component(fTimeDelta);
	m_pRendererCom->Add_RenderGroup(RENDER_UI, this);

	return OBJ_NOEVENT;
}

void CAim::LateUpdate_Object(const _float & fTimeDelta)
{
	//여기서 프레임도 돌려야 함
	m_tFrame.fFrameStart += 6.f * fTimeDelta;

	if (m_tFrame.fFrameStart > m_tFrame.fFrameEnd)
	{
		m_tFrame.fFrameStart = 0;
	}
}

void CAim::Render_Object()
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pTextureCom->Set_Texture(_uint(m_tFrame.fFrameStart));
	
	m_pBufferCom->Render_Buffers();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CAim::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Aim"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Texture", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Renderer", pComponent);

	return S_OK;
}

CAim * CAim::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CAim*	pInstance = new CAim(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}