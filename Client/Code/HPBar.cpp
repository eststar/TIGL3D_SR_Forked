#include "stdafx.h"
#include "HPBar.h"

#include "Export_Function.h"

USING(Engine)

CHPBar::CHPBar(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
	ZeroMemory(m_iDrawCnt, sizeof(_uint)* 5);
}

CHPBar::~CHPBar()
{

}

void CHPBar::Free()
{
	CGameObject::Free();

	for (int i = 1; i < 5; ++i)
	{
		Safe_Release(m_pTransformCom[i]);
	}
}

HRESULT CHPBar::Ready_Object()
{
	m_pObjTag = L"HPBar";

	m_iPlayerHP = 120;
	m_iPlayerMaxHP = 120;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	_float fSize = 0.15f;
	
	for (int i = 0; i < 5; ++i)
	{
		m_pTransformCom[i]->Set_Pos(&_vec3(-0.9f + (i * fSize), 0.85f, 0.f));
		m_pTransformCom[i]->Set_Scale(&_vec3(fSize * ((float)WINCY / WINCX), fSize, 0.f));
		m_iDrawCnt[i] = 4;
	}

	return S_OK;
}

Engine::_int CHPBar::Update_Object(const _float& fTimeDelta)
{	
	for (int i = 0; i < 5; ++i)
	{
		m_pTransformCom[i]->Update_Component(fTimeDelta);
	}


	m_pRendererCom->Add_RenderGroup(RENDER_UI, this);
	return OBJ_NOEVENT;
}

void CHPBar::LateUpdate_Object(const _float & fTimeDelta)
{
	//업뎃 할 필요도 없이 처음에 위치만 잡아주고
	for (int i = 0; i < 5; ++i)
	{
		m_pTransformCom[i]->LateUpdate_Component(fTimeDelta);
	}

}

void CHPBar::Render_Object()
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	for (int i = 0; i < m_iPlayerMaxHP / 40; ++i)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[i]->Get_WorldMatrix());
		m_pTextureCom->Set_Texture(m_iDrawCnt[i]);
		m_pBufferCom->Render_Buffers();
	}


	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CHPBar::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTransformCom[0] = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_DYNAMIC].emplace(L"Com_Transform", pComponent);

	for (int i = 1; i < 5; ++i)
	{
		pComponent = m_pTransformCom[i] = Engine::CTransform::Create();
		NULL_CHECK_RETURN(pComponent, E_FAIL);
	}

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_PLAYER, L"PlayerHP"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Texture", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Renderer", pComponent);

	return S_OK;
}

CHPBar * CHPBar::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CHPBar*	pInstance = new CHPBar(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}