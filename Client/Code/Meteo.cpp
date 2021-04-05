#include "stdafx.h"
#include "Meteo.h"

#include "Export_Function.h"

USING(Engine)
CMeteo::CMeteo(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CCreature(pGraphicDev)
	, m_fTime(0.f)
	, m_iImageCount(0)
{
}


CMeteo::~CMeteo()
{
}

HRESULT CMeteo::Ready_Object()
{
	m_pObjTag = L"PlayerMeteo";
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(LAYER_LOGIC, LOGIC_PLAYER, L"Player", L"Com_Transform", COM_DYNAMIC));
	FAILED_CHECK_RETURN(m_pPlayerTransformCom, E_FAIL);
	//m_pTransformCom->Set_Pos(&_vec3(50.f, 20.f, 20.f));


	return S_OK;
}

_int CMeteo::Update_Object(const _float & fTimeDelta)
{
	if (m_bIsDead)
		return OBJ_DEAD;

	CGameObject::Update_Object(fTimeDelta);
	m_pRendererCom->Add_RenderGroup(RENDER_ALPHA, this);

	m_fTime += fTimeDelta;
	if (m_fTime >= 0.2f) {
		m_iImageCount = 1;
	}
	if (m_fTime >= 0.4f) {
		m_iImageCount = 2;
	}
	if (m_fTime >= 0.6f) {
		m_iImageCount = 3;
	}
	if (m_fTime >= 0.8f) {
		m_iImageCount = 4;
	}
	if (m_fTime >= 1.0f) {
		m_iImageCount = 5;
	}
	//if (m_fTime >= 0.6f) {
	//	m_iImageCount = 0;
	//	//m_fTime = 0.f;
	//}
	m_pTransformCom->Move_Pos(&_vec3(0.f, -1.f, 0.f), fTimeDelta, 50.f);
	return OBJ_NOEVENT;
}

void CMeteo::LateUpdate_Object(const _float & fTimeDelta)
{
	CGameObject::LateUpdate_Object(fTimeDelta);

	BillBoard();// 미정_0330 빌보드 함수 호출
	//m_pTransformCom->Move_Pos(&m_vLook, fTimeDelta, 80.f);
}

void CMeteo::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(m_iImageCount);

	m_pBufferCom->Render_Buffers();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CMeteo::Set_Pos(_vec3 & vPos)
{
	m_pTransformCom->Set_Pos(&vPos);
}

HRESULT CMeteo::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;
	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Meteo"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Texture", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Renderer", pComponent);
	return S_OK;
}

void CMeteo::BillBoard()
{
	_matrix matView, matScale;
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matScale);

	D3DXMatrixScaling(&matScale, 3.f, 6.f, 3.f);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	memset(&matView._41, 0, sizeof(_vec3));
	D3DXMatrixInverse(&matView, 0, &matView);
	_vec3 vPos;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
	_vec3 BillPos = { vPos.x, vPos.y, vPos.z };
	memcpy(&matView._41, &BillPos, sizeof(_vec3));

	m_pTransformCom->Set_Matrix(&(matScale*matView));
}

CMeteo * CMeteo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMeteo*	pInstance = new CMeteo(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CMeteo::Free()
{
	CGameObject::Free();
}
