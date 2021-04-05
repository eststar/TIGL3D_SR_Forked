#include "stdafx.h"
#include "OctopusBoss.h"
#include "MiniOctopus.h"

#include "Export_Function.h"

USING(Engine)

COctopusBoss::COctopusBoss(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CCreature(pGraphicDev)
	, m_iJumpCnt(0)
	, m_iJumpCntMax(0)
	, m_fJumpSpeed(0.f)
	, m_fJumpAccel(0.f)
	, m_bIsJump(false)
	, m_fTimeDeltaMax(0.f)
	, m_fPatternTime(0.f)
	
{
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}

COctopusBoss::~COctopusBoss()
{
}

COctopusBoss * COctopusBoss::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	COctopusBoss*	pInstance = new COctopusBoss(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void COctopusBoss::Free()
{
	CGameObject::Free();
}

HRESULT COctopusBoss::Ready_Object()
{
	m_pObjTag = L"OctopusBoss";
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Pos(&_vec3(50.f, 0.f, 50.f));

	/* 0401_����_�������ú��� */
	m_fJumpSpeed = 20.f;
	m_fJumpAccel = 0.1f;

	m_iJumpCntMax = 2; //���� �ִ� Ƚ��

	m_tFrame.fFrameStart = 0;
	m_tFrame.fFrameEnd = 4;

	return S_OK;
}

_int COctopusBoss::Update_Object(const _float & fTimeDelta)
{
	if (m_bIsDead)
		return OBJ_DEAD;
	
	m_fPatternTime += fTimeDelta;
	if (m_fPatternTime <= 5.f ) {
		/* 0401_����_1��° ����_�÷��̾� �������� �ٰ��� */
		SetUp_OnTerrain();
		Follow_Player(fTimeDelta);
		m_pTransformCom->Move_Pos(m_pTransformCom->Get_Info(INFO_LOOK), fTimeDelta, 5.f);
	}
	else if (m_fPatternTime > 5.f && m_fPatternTime <= 8.5f)
	{
		/* 0401_����_2��° ����_������ �����ϸ鼭 �ٰ��� */
		Jumping(fTimeDelta);
		if (m_iJumpCnt == 0)
		{
			SetUp_OnTerrain();
			Follow_Player(fTimeDelta);
			m_pTransformCom->Move_Pos(m_pTransformCom->Get_Info(INFO_LOOK), fTimeDelta, 30.f);
		}
	}
	/* 0401_����_3��° ����_�� ���� ���� �� ���� ����! - ���� �� */
	//�� ������ �� �����ؾߵȴ� ������.............
	//�ϴ� �� ������ ������.... �ٵ� ��� ������� ������ �־�� �ϴ°� �ƴ϶�
	//���� ������������ �ֱ������� ������ �Ǿ�� �ϴ°� ���� 
	//else if (m_fPatternTime > 8.5f)
	//{
	//	Create_Monster();
	//	m_fPatternTime = 0;
	//}
	else
		m_fPatternTime = 0;

	CGameObject::Update_Object(fTimeDelta);
	m_pRendererCom->Add_RenderGroup(RENDER_ALPHA, this);

	return OBJ_NOEVENT;
}

void COctopusBoss::LateUpdate_Object(const _float & fTimeDelta)
{
	CGameObject::LateUpdate_Object(fTimeDelta);
	/* �ؽ�ó �ִϸ��̼� */
	Frame_Move(fTimeDelta);
	BillBoardYaw();
}

void COctopusBoss::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(static_cast<_uint>(m_tFrame.fFrameStart));

	m_pBufferCom->Render_Buffers();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT COctopusBoss::Add_Component()
{
	Engine::CComponent*	pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_OctopusBoss"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Texture", pComponent);

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Renderer", pComponent);

	pComponent = m_pCalculatorCom = Engine::CCalculator::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Calculator", pComponent);

	return S_OK;
}

void COctopusBoss::Frame_Move(const _float& fTimeDelta)
{
	m_tFrame.fFrameStart += m_tFrame.fFrameEnd * fTimeDelta * 2.f;

	if (m_tFrame.fFrameStart >= m_tFrame.fFrameEnd)
	{
		m_tFrame.fFrameStart = 0.f;
	}
}

void COctopusBoss::Jumping(const _float & fTimeDelta)
{
	m_pTransformCom->Get_Info(Engine::INFO_UP, &m_vUp);

	//�ð��� ���� ��, �뷫 1.5��?�� �� �� ����
	m_fTimeDeltaMax += fTimeDelta;
	if (m_fTimeDeltaMax > 0.7f)
	{
		m_bIsJump = true;
		++m_iJumpCnt;
	}

	if (m_iJumpCnt > m_iJumpCntMax) {
		m_iJumpCnt = 0;
	}

	if (m_bIsJump)
	{
		m_fJumpAccel = 0.f;
		m_fTimeDeltaMax = 0.f; //������ �ð��� �ʱ�ȭ
		m_bIsJump = false;
	}

	if (!m_bIsJump && m_iJumpCnt != 0)
	{
		/* ���� */
		Follow_Player(fTimeDelta);
		m_pTransformCom->Move_Pos(m_pTransformCom->Get_Info(INFO_LOOK), fTimeDelta, -5.f);

		D3DXVec3Normalize(&m_vUp, &m_vUp);
		m_pTransformCom->Move_Pos(&m_vUp, fTimeDelta, m_fJumpSpeed - g_Gravity * m_fJumpAccel);

		m_fJumpAccel += 0.2f;
	}

	if (m_pTransformCom->Get_Info(Engine::INFO_POS)->y <= 1)
	{
		SetUp_OnTerrain();
	}

}

_int COctopusBoss::Follow_Player(const _float & fTimeDelta)
{
	m_pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(LAYER_LOGIC, LOGIC_PLAYER, L"Player", L"Com_Transform", COM_DYNAMIC));
	m_pPlayerTransformCom->Get_Info(INFO_POS, &m_vPlayerPos);

	_vec3 vDir = m_vPlayerPos - *(m_pTransformCom->Get_Info(INFO_POS));
	m_pTransformCom->Set_Info(INFO_LOOK, D3DXVec3Normalize(&vDir, &vDir));

	return 0;
}

void COctopusBoss::BillBoardYaw()
{
	D3DXMATRIX matScale, matView, matBill;

	D3DXMatrixIdentity(&matView);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixIdentity(&matBill);
	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, NULL, &matBill);

	_vec3 BillPos = *(m_pTransformCom->Get_Info(Engine::INFO_POS));
	memcpy(&matBill._41, &BillPos, sizeof(_vec3));

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);

	m_pTransformCom->Set_Matrix(matScale * matBill);
}

void COctopusBoss::SetUp_OnTerrain()
{
	_float	fHeight = 0.f;
	_vec3	vPos;

	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);

	Engine::CTerrainTex* pTerrainBufferCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Get_Component(Engine::LAYER_ENVIR, ENVIR_TERRAIN, L"Terrain", L"Com_Buffer", Engine::COM_STATIC));
	NULL_CHECK(pTerrainBufferCom);

	fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainBufferCom->Get_VtxPos(), VTXCNTX, VTXCNTZ, VTXITV);

	m_pTransformCom->Set_Pos(&_vec3(vPos.x, fHeight + 1.f, vPos.z));
}

/* ���� ���� �Լ� */
HRESULT COctopusBoss::Create_Monster()
{
	Engine::CGameObject* pGameObject = nullptr;

	pGameObject = CMiniOctopus::Create(m_pGraphicDev);

	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject(LAYER_LOGIC, LOGIC_MONSTER, pGameObject), E_FAIL);

	
	return S_OK;
}
