#include "stdafx.h"
#include "MonsterBullet.h"

#include "Export_Function.h"

USING(Engine)

CMonsterBullet::CMonsterBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CCreature(pGraphicDev)
	, m_fTimeDeltaMax(0)
{
}

CMonsterBullet::~CMonsterBullet()
{
}

CMonsterBullet * CMonsterBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev, CTransform* pTrans)
{
	CMonsterBullet*	pInstance = new CMonsterBullet(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pTrans)))
		Safe_Release(pInstance);

	return pInstance;
}

void CMonsterBullet::Free()
{
	CGameObject::Free();
}

HRESULT CMonsterBullet::Ready_Object(CTransform* pTrans)
{
	m_pObjTag = L"MonsterBullet";
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//0405_진원_총알 생성위치를 몬스터위치로! -> 다영이가 해쥼!!!!!!!!!!!!!!!!!!
	m_pMonsterTransformCom = pTrans;
	NULL_CHECK_RETURN(m_pMonsterTransformCom, -1);
	m_pMonsterTransformCom->Get_Info(INFO_POS, &m_vMonsterPos);
	m_pTransformCom->Set_Pos(&m_vMonsterPos);

	//0405_진원_플레이어 위치를 한 번만 받아와서 방향 계산
	m_pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(LAYER_LOGIC, LOGIC_PLAYER, L"Player", L"Com_Transform", COM_DYNAMIC));
	NULL_CHECK_RETURN(m_pPlayerTransformCom, -1);
	m_pPlayerTransformCom->Get_Info(INFO_POS, &m_vPlayerPos);
	
	m_vDir = m_vPlayerPos - *(m_pTransformCom->Get_Info(INFO_POS)) - _vec3(0.f, 2.f, 0.f); //0405_진원_플레이어보다 아래 위치로 쏘도록
	D3DXVec3Normalize(&m_vDir, &m_vDir);

	return S_OK;
}

_int CMonsterBullet::Update_Object(const _float & fTimeDelta)
{
	if (m_bIsDead)
		return OBJ_DEAD;
	
	//0405_진원_플레이어 위치로 총알 발사
	m_pTransformCom->Move_Pos(&m_vDir, fTimeDelta, 50.f);

	CGameObject::Update_Object(fTimeDelta);
	m_pRendererCom->Add_RenderGroup(RENDER_ALPHA, this);

	return OBJ_NOEVENT;
}

void CMonsterBullet::LateUpdate_Object(const _float & fTimeDelta)
{
	CGameObject::LateUpdate_Object(fTimeDelta);
	BillBoardYaw();
}

void CMonsterBullet::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture();

	m_pBufferCom->Render_Buffers();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CMonsterBullet::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_MonsterBullet"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Texture", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Renderer", pComponent);

	return S_OK;
}

void CMonsterBullet::BillBoardYaw()
{
	D3DXMATRIX matScale, matView, matBill;

	D3DXMatrixIdentity(&matScale);
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matBill);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixIdentity(&matBill);
	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;
	D3DXMatrixInverse(&matBill, NULL, &matBill);

	_vec3 BillPos = *(m_pTransformCom->Get_Info(Engine::INFO_POS)); //플레이어가 곧 카메라니까!!!!!
	memcpy(&matBill._41, &BillPos, sizeof(_vec3));

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);

	m_pTransformCom->Set_Matrix(matScale * matBill);
}
