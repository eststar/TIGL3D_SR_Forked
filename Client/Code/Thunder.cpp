#include "stdafx.h"
#include "Thunder.h"

#include "Export_Function.h"

USING(Engine)
CThunder::CThunder(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CCreature(pGraphicDev)
	, m_fTime(0.f)
	, m_iImageCount(0)
{
}


CThunder::~CThunder()
{
}

HRESULT CThunder::Ready_Object()
{
	m_pObjTag = L"PlayerThunder";
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(LAYER_LOGIC, LOGIC_PLAYER, L"Player", L"Com_Transform", COM_DYNAMIC));
	FAILED_CHECK_RETURN(m_pPlayerTransformCom, E_FAIL);

	m_pTransformCom->Set_Pos(&_vec3(50.f, 40.f, 50.f));

	return S_OK;
}

_int CThunder::Update_Object(const _float & fTimeDelta)
{
	if (m_bIsDead)
		return OBJ_DEAD;

	CGameObject::Update_Object(fTimeDelta);
	m_pRendererCom->Add_RenderGroup(RENDER_ALPHA, this);

	m_fTime += fTimeDelta;
	if (m_fTime >= 0.1f) {
		m_iImageCount = 1;
	}
	if (m_fTime >= 0.2f) {
		m_iImageCount = 2;
	}
	if (m_fTime >= 0.3f) {
		m_iImageCount = 3;
	}
	if (m_fTime >= 0.4f) {
		m_iImageCount = 4;
	}
	if (m_fTime >= 0.5f) {
		m_bIsDead = true;
	}
	//cout << m_fTime << endl;
	return OBJ_NOEVENT;
}

void CThunder::LateUpdate_Object(const _float & fTimeDelta)
{
	CGameObject::LateUpdate_Object(fTimeDelta);

	BillBoard();// 미정_0330 빌보드 함수 호출
	//m_pTransformCom->Move_Pos(&m_vLook, fTimeDelta, 80.f);

}

void CThunder::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(m_iImageCount);

	m_pBufferCom->Render_Buffers();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CThunder::Set_Pos(_vec3 & vPos)
{
	m_pTransformCom->Set_Pos(&vPos);
}

HRESULT CThunder::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;
	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Thunder"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Texture", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Renderer", pComponent);
	return S_OK;
}

void CThunder::BillBoard()
{
	_matrix matView, matScale;
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matScale);

	D3DXMatrixScaling(&matScale, 40.f, 80.f, 0.f);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	memset(&matView._41, 0, sizeof(_vec3));
	D3DXMatrixInverse(&matView, 0, &matView);
	_vec3 vPos;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
	_vec3 BillPos = { vPos.x, vPos.y, vPos.z };
	memcpy(&matView._41, &BillPos, sizeof(_vec3));

	m_pTransformCom->Set_Matrix(&(matScale*matView));
}

CThunder * CThunder::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CThunder*	pInstance = new CThunder(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CThunder::Free()
{
	CGameObject::Free();
}
