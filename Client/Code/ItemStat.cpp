#include "stdafx.h"
#include "ItemStat.h"

#include "Export_Function.h"

USING(Engine)

CItemStat::CItemStat(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
	ZeroMemory(m_iDrawCnt, sizeof(_uint)* 5);
}

CItemStat::~CItemStat()
{

}

void CItemStat::Free()
{
	CGameObject::Free();

	for (int i = 1; i < 4; ++i)
	{
		Safe_Release(m_pTransformCom[i]);
	}
}

HRESULT CItemStat::Ready_Object(_tchar* pObjTag, const _vec3* pPos)
{
	m_pObjTag = pObjTag;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	_float fSize = 0.06f;
	
	for (int i = 0; i < 4; ++i)
	{
		m_pTransformCom[i]->Set_Pos(&_vec3(pPos->x + (i * fSize), pPos->y, pPos->z));
		m_pTransformCom[i]->Set_Scale(&_vec3(fSize, fSize, 0.f));
	}
	if (lstrcmp(L"GemStat", m_pObjTag))
	{
		m_iDrawCnt[0] = 12;
	}
	else
		m_iDrawCnt[0] = 11;

	return S_OK;
}

Engine::_int CItemStat::Update_Object(const _float& fTimeDelta)
{	
	for (int i = 0; i < 4; ++i)
	{
		m_pTransformCom[i]->Update_Component(fTimeDelta);
	}
	m_pRendererCom->Add_RenderGroup(RENDER_UI, this);
	
	return OBJ_NOEVENT;
}

void CItemStat::LateUpdate_Object(const _float & fTimeDelta)
{
	//업뎃 할 필요도 없이 처음에 위치만 잡아주고
	for (int i = 0; i < 4; ++i)
	{
		m_pTransformCom[i]->LateUpdate_Component(fTimeDelta);
	}

}

void CItemStat::Render_Object()
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	for (int i = 0; i < 4; ++i)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[i]->Get_WorldMatrix());
		m_pTextureCom->Set_Texture(m_iDrawCnt[i]);
		m_pBufferCom->Render_Buffers();
	}

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CItemStat::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTransformCom[0] = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_DYNAMIC].emplace(L"Com_Transform", pComponent);

	for (int i = 1; i < 4; ++i)
	{
		pComponent = m_pTransformCom[i] = Engine::CTransform::Create();
		NULL_CHECK_RETURN(pComponent, E_FAIL);
	}

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_PLAYER, L"ItemStat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Texture", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Renderer", pComponent);

	return S_OK;
}

CItemStat * CItemStat::Create(LPDIRECT3DDEVICE9 pGraphicDev, _tchar* pObjTag, const _vec3* pPos)
{
	CItemStat*	pInstance = new CItemStat(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pObjTag, pPos)))
		Safe_Release(pInstance);

	return pInstance;
}