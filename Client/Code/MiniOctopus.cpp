#include "stdafx.h"
#include "MiniOctopus.h"

#include "Export_Function.h"

USING(Engine)

CMiniOctopus::CMiniOctopus(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CCreature(pGraphicDev)
	, m_fJumpSpeed(0.f)
	, m_fJumpAccel(0.f)
	, m_fTimeDeltaMax(0.f)
	, m_bIsJump(false)
{
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}

CMiniOctopus::~CMiniOctopus()
{

}

CMiniOctopus* CMiniOctopus::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMiniOctopus*	pInstance = new CMiniOctopus(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CMiniOctopus::Free()
{
	CGameObject::Free();
}

HRESULT CMiniOctopus::Ready_Object()
{
	m_pObjTag = L"MiniOctopus";

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	/* 0330_진원_BillBoardYaw 함수에서 scale값 정해줘야함 */
	//m_pTransformCom->Set_Scale(&_vec3(0.2f, 0.2f, 0.2f));

	/* 0405_진원_10~50까지의 실수형 난수 생성 */
	random_device rd;
	mt19937_64 randNum(rd());
	uniform_real_distribution<float> dist(10.f, 50.f);

	m_pTransformCom->Set_Pos(&_vec3(dist(randNum), 0.f, dist(randNum)));

	m_fJumpSpeed = 20.f;
	m_fJumpAccel = 0.1f;

	m_tFrame.fFrameStart = 0;
	m_tFrame.fFrameEnd = 2;

	return S_OK;
}

Engine::_int CMiniOctopus::Update_Object(const _float& fTimeDelta)
{
	if (Engine::Get_DIKeyState(DIK_T) & 0x80)
		m_bIsDead = true;

	if (m_bIsDead)
		return OBJ_DEAD;

	//0330_진원_플레이어 방향으로 점프하며 다가오는 패턴 구현 完
	Jumping(fTimeDelta);

	CGameObject::Update_Object(fTimeDelta);

	//진원_플레이어 위치 추적 함수
	Follow_Player(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(RENDER_ALPHA, this);

	return OBJ_NOEVENT;
}

void CMiniOctopus::LateUpdate_Object(const _float & fTimeDelta)
{
	CGameObject::LateUpdate_Object(fTimeDelta);

	//진원_빌보드 함수
	BillBoardYaw();
	//0402_진원_몬스터 텍스처 애니메이션 함수 적용 完
	Frame_Move(fTimeDelta);
}

void CMiniOctopus::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(static_cast<_uint>(m_tFrame.fFrameStart));
	m_pBufferCom->Render_Buffers();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CMiniOctopus::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_MiniOctopus"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Texture", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Renderer", pComponent);

	pComponent = m_pCalculatorCom = Engine::CCalculator::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Calculator", pComponent);

	return S_OK;
}

void CMiniOctopus::Frame_Move(const _float & fTimeDelta)
{
	m_tFrame.fFrameStart += m_tFrame.fFrameEnd * fTimeDelta * 1.2f;

	if (m_tFrame.fFrameStart >= m_tFrame.fFrameEnd)
	{
		m_tFrame.fFrameStart = 0.f;
	}
}

void CMiniOctopus::SetUp_OnTerrain()
{
	_float	fHeight = 0.f;
	_vec3	vPos;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);

	Engine::CTerrainTex*		pTerrainBufferCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Get_Component(Engine::LAYER_ENVIR, ENVIR_TERRAIN, L"Terrain", L"Com_Buffer", Engine::COM_STATIC));
	NULL_CHECK(pTerrainBufferCom);

	fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainBufferCom->Get_VtxPos(), VTXCNTX, VTXCNTZ, VTXITV);
	m_pTransformCom->Set_Pos(&_vec3(vPos.x, fHeight + 1.f, vPos.z));
}

void CMiniOctopus::BillBoardYaw()
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

void CMiniOctopus::Jumping(const _float& fTimeDelta)
{
	m_pTransformCom->Get_Info(Engine::INFO_UP, &m_vUp);

	//시간값 누적 후, 대략 1.5초?에 한 번 점프
	m_fTimeDeltaMax += fTimeDelta;
	if (m_fTimeDeltaMax > 1.5f)
		m_bIsJump = true;

	if (m_bIsJump)
	{
		m_fJumpAccel = 0.f;
		m_fTimeDeltaMax = 0.f; //누적한 시간값 초기화
		m_bIsJump = false;
	}

	if (!m_bIsJump)
	{
		D3DXVec3Normalize(&m_vUp, &m_vUp);
		m_pTransformCom->Move_Pos(&m_vUp, fTimeDelta, m_fJumpSpeed - g_Gravity * m_fJumpAccel);
		m_fJumpAccel += 0.2f;
	}

	if (m_pTransformCom->Get_Info(Engine::INFO_POS)->y <= 1)
	{
		SetUp_OnTerrain();
	}
}

_int CMiniOctopus::Follow_Player(const _float& fTimeDelta)
{
	CTransform*	pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(LAYER_LOGIC, LOGIC_PLAYER, L"Player", L"Com_Transform", COM_DYNAMIC));
	NULL_CHECK_RETURN(pPlayerTransformCom, -1);

	_vec3	vPlayerPos;
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);

	m_pTransformCom->Chase_Target(&vPlayerPos, 2.f, fTimeDelta);

	return 0;
}
