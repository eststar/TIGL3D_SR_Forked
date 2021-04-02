#include "stdafx.h"
#include "Monster.h"

#include "Export_Function.h"

USING(Engine)

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CCreature(pGraphicDev)
	, m_fJumpSpeed(0.f)
	, m_fJumpAccel(0.f)
	, m_fTimeDeltaMax(0.f)
	, m_bIsJump(false)
	, m_iTextureCount(0)
	, m_fAnimationTime(0.f)
	, m_fBulletCoolTime(0.f)
{
}

CMonster::~CMonster()
{

}

CMonster* CMonster::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMonster*	pInstance = new CMonster(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CMonster::Free()
{
	CGameObject::Free();
}

HRESULT CMonster::Ready_Object()
{
	m_pObjTag = L"Monster";

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	/* 0330_진원_BillBoardYaw 함수에서 scale값 정해줘야함 */
	//m_pTransformCom->Set_Scale(&_vec3(5.f, 5.f, 0.2f));

	m_pTransformCom->Set_Pos(&_vec3(20.f, 1.f, 20.f));

	m_fJumpSpeed = 20.f;
	m_fJumpAccel = 0.1f;

	return S_OK;
}

Engine::_int CMonster::Update_Object(const _float& fTimeDelta)
{
	if (Engine::Get_DIKeyState(DIK_T) & 0x80)
		m_bIsDead = true;

	if (m_bIsDead)
		return OBJ_DEAD;

	//0330_진원_플레이어 방향으로 점프하며 다가오는 패턴 구현 完
	//Jumping(fTimeDelta);

	CGameObject::Update_Object(fTimeDelta);

	//진원_플레이어 위치 추적 함수
	Follow_Player(fTimeDelta);

	//0330_진원_몬스터 애니메이션 구현 完
	m_fAnimationTime += fTimeDelta;
	if (m_fAnimationTime >= 0.1f) {
		m_iTextureCount = 1;
		if (m_fAnimationTime >= 0.3f) {
			m_iTextureCount = 0;
			m_fAnimationTime = 0.f;
		}
	}

	//0331_진원_대략 2초에 한 번씩 발사되도록 쿨타임 지정
	m_fBulletCoolTime += fTimeDelta;
	if (m_fBulletCoolTime >= 2.f)
	{
		Create_Bullet();
		m_fBulletCoolTime = 0.f;
	}

	m_pRendererCom->Add_RenderGroup(RENDER_ALPHA, this);

	return OBJ_NOEVENT;
}

void CMonster::LateUpdate_Object(const _float & fTimeDelta)
{
	CGameObject::LateUpdate_Object(fTimeDelta);

	//진원_빌보드 함수
	BillBoardYaw();
}

void CMonster::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(m_iTextureCount);
	m_pBufferCom->Render_Buffers();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CMonster::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Monster"));
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

void CMonster::SetUp_OnTerrain()
{
	_float	fHeight = 0.f;
	_vec3	vPos;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);

	Engine::CTerrainTex*		pTerrainBufferCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Get_Component(Engine::LAYER_ENVIR, ENVIR_TERRAIN, L"Terrain", L"Com_Buffer", Engine::COM_STATIC));
	NULL_CHECK(pTerrainBufferCom);

	fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainBufferCom->Get_VtxPos(), VTXCNTX, VTXCNTZ, VTXITV);
	m_pTransformCom->Set_Pos(&_vec3(vPos.x, fHeight + 1.f, vPos.z));
}

void CMonster::BillBoardYaw()
{
	D3DXMATRIX matScale, matView, matBill;

	//행렬 초기화
	D3DXMatrixIdentity(&matView);
	//카메라(=플레이어) 뷰행렬 얻어와서 matView에 저장
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	//빌보드 행렬 초기화
	D3DXMatrixIdentity(&matBill);
	//카메라(=플레이어) 뷰행렬의 Y축 회전행렬값만 가져와 빌보드 행렬 y축 회전에 대응되는 원소에 대입
	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;
	//빌보드 행렬의 역행렬 구하기
	D3DXMatrixInverse(&matBill, NULL, &matBill);

	//카메라(=플레이어) 추적한 위치로 이동 처리
	_vec3 BillPos = *(m_pTransformCom->Get_Info(Engine::INFO_POS)); //플레이어가 곧 카메라니까!!!!!
	memcpy(&matBill._41, &BillPos, sizeof(_vec3));

	D3DXMatrixScaling(&matScale, 0.3f, 0.3f, 0.3f);

	//카메라(=플레이어) 뷰행렬의 Y축 회전 행렬만! 반영
	//m_pTransformCom->m_matWorld = matScale * matBill;
	m_pTransformCom->Set_Matrix(matScale * matBill);
}

HRESULT CMonster::Create_Bullet()
{
	Engine::CGameObject*		pGameObject = nullptr;
	pGameObject = CMonsterBullet::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject(LAYER_LOGIC, LOGIC_MONSTERWEAPON, pGameObject), E_FAIL);
	return S_OK;
}

void CMonster::Jumping(const _float& fTimeDelta)
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

_int CMonster::Follow_Player(const _float& fTimeDelta)
{
	CTransform*	pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(LAYER_LOGIC, LOGIC_PLAYER, L"Player", L"Com_Transform", COM_DYNAMIC));
	NULL_CHECK_RETURN(pPlayerTransformCom, -1);

	_vec3	vPlayerPos;
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);

	m_pTransformCom->Chase_Target(&vPlayerPos, 2.f, fTimeDelta);

	return 0;
}
