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

CMonsterBullet * CMonsterBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMonsterBullet*	pInstance = new CMonsterBullet(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CMonsterBullet::Free()
{
	CGameObject::Free();
}

HRESULT CMonsterBullet::Ready_Object()
{
	m_pObjTag = L"MonsterBullet";
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//0331_����_�Ѿ� ������ġ�� ������ġ��!
	CTransform*	pMonsterTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(LAYER_LOGIC, LOGIC_MONSTER, L"Monster", L"Com_Transform", COM_DYNAMIC));
	NULL_CHECK_RETURN(pMonsterTransformCom, -1);
	_vec3	vMonsterPos;
	pMonsterTransformCom->Get_Info(INFO_POS, &vMonsterPos);
	m_pTransformCom->Set_Pos(&vMonsterPos);

	//0331_����_�÷��̾� ��ġ �� ���� �޾ƿ���!
	m_pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(LAYER_LOGIC, LOGIC_PLAYER, L"Player", L"Com_Transform", COM_DYNAMIC));
	NULL_CHECK_RETURN(m_pPlayerTransformCom, -1);
	m_pPlayerTransformCom->Get_Info(INFO_POS, &m_vPlayerPos);
	
	return S_OK;
}

_int CMonsterBullet::Update_Object(const _float & fTimeDelta)
{
	if (m_bIsDead)
		return OBJ_DEAD;
	
	//0331_����_�÷��̾� ��ġ�� �Ѿ� �߻�
	_vec3 vDir = m_vPlayerPos - *(m_pTransformCom->Get_Info(INFO_POS));
	m_pTransformCom->Set_Info(INFO_LOOK, D3DXVec3Normalize(&vDir, &vDir));
	m_pTransformCom->Move_Pos(m_pTransformCom->Get_Info(INFO_LOOK), fTimeDelta, 20.f);

	m_pRendererCom->Add_RenderGroup(RENDER_ALPHA, this);

	CGameObject::Update_Object(fTimeDelta);

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
//
//_int CMonsterBullet::Follow_Player(const _float & fTimeDelta)
//{
//	CTransform*	pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(LAYER_LOGIC, LOGIC_PLAYER, L"Player", L"Com_Transform", COM_DYNAMIC));
//	NULL_CHECK_RETURN(pPlayerTransformCom, -1);
//
//	_vec3	vPlayerPos;
//	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);
//
//	m_pTransformCom->Chase_Target(&vPlayerPos, 5.f, fTimeDelta);
//
//	return 0;
//}

void CMonsterBullet::BillBoardYaw()
{
	D3DXMATRIX matScale, matView, matBill;

	//��� �ʱ�ȭ
	D3DXMatrixIdentity(&matScale);
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matBill);
	//ī�޶�(=�÷��̾�) ����� ���ͼ� matView�� ����
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	//������ ��� �ʱ�ȭ
	D3DXMatrixIdentity(&matBill);
	//ī�޶�(=�÷��̾�) ������� Y�� ȸ����İ��� ������ ������ ��� y�� ȸ���� �����Ǵ� ���ҿ� ����
	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;
	//������ ����� ����� ���ϱ�
	D3DXMatrixInverse(&matBill, NULL, &matBill);

	//ī�޶�(=�÷��̾�) ������ ��ġ�� �̵� ó��
	_vec3 BillPos = *(m_pTransformCom->Get_Info(Engine::INFO_POS)); //�÷��̾ �� ī�޶�ϱ�!!!!!
	memcpy(&matBill._41, &BillPos, sizeof(_vec3));

	D3DXMatrixScaling(&matScale, 0.2f, 0.2f, 0.2f);

	//ī�޶�(=�÷��̾�) ������� Y�� ȸ�� ��ĸ�! �ݿ�
	//m_pTransformCom->m_matWorld = matScale * matBill;
	m_pTransformCom->Set_Matrix(matScale * matBill);
}
