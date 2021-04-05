#include "stdafx.h"
#include "Player.h"

#include "Export_Function.h"
#include "Scene.h"
#include "Terrain.h"

USING(Engine)

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CCreature(pGraphicDev)
	, m_bIsJump(false)
	, m_fTime(0.f)
	, m_bHasGun(true)
	, m_eGunSkill(FASTBULLET)
	, m_bSkillOn(false)
{

}

CPlayer::~CPlayer()
{

}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer*	pInstance = new CPlayer(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CPlayer::Free()
{
	CGameObject::Free();
}

HRESULT CPlayer::Ready_Object()
{
	m_pObjTag = L"Player";
	
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	
	m_pTransformCom->Set_Pos(&_vec3(1.f, 0.f, 0.f));
	
	return S_OK;
}

Engine::_int CPlayer::Update_Object(const _float& fTimeDelta)
{
	if (m_bIsDead)
	{
		/*죽었을 때 따로 이벤트 必*/
	}

	// 미정_0331 총 생성
	if (m_bHasGun) {
		m_pGun = CGun::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(m_pGun, E_FAIL);
		FAILED_CHECK_RETURN(Engine::Add_GameObject(LAYER_LOGIC, LOGIC_PLAYERWEAPON, m_pGun), E_FAIL);
		m_bHasGun = false;
	}

	SetUp_OnTerrain();
	Key_Input(fTimeDelta);
	CGameObject::Update_Object(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(RENDER_ALPHA, this);

	return OBJ_NOEVENT;
}

void CPlayer::LateUpdate_Object(const _float & fTimeDelta)
{
	CGameObject::LateUpdate_Object(fTimeDelta);
}

void CPlayer::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	//플레이어가 큐브 형태여서 큐브 안에 카메라를 박아놔서 일단 후면 추려내기 꺼둠
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffers();

	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CPlayer::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CCubeTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_CubeTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Cube"));
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

void CPlayer::Key_Input(const _float& fTimeDelta)
{
	_vec3 vRight;

	m_pTransformCom->Get_Info(Engine::INFO_LOOK, &m_vLook);
	m_pTransformCom->Get_Info(Engine::INFO_RIGHT, &vRight);

	D3DXVec3Normalize(&m_vLook, &m_vLook);
	_long		dwMouse = 0;

	//시점 상하 전환
	if (dwMouse = Engine::Get_DIMouseMove(Engine::DIMS_Y))
		m_pTransformCom->Rotation(Engine::ROT_X, D3DXToRadian(dwMouse / 10.f));
	//시점 좌우 전환
	if (dwMouse = Engine::Get_DIMouseMove(Engine::DIMS_X))
		m_pTransformCom->Rotation(Engine::ROT_Y, D3DXToRadian(dwMouse / 10.f));

	dynamic_cast<CGun*>(m_pGun)->Set_State(CGun::IDLE);

	//전진
	if (Engine::Key_Pressing(KEY_W)) {
		dynamic_cast<CGun*>(m_pGun)->Set_State(CGun::WALKING);
		m_pTransformCom->Move_Pos(&m_vLook, fTimeDelta, 15.f);
	}
	//후진
	if (Engine::Key_Pressing(KEY_S)) {
		dynamic_cast<CGun*>(m_pGun)->Set_State(CGun::WALKING);
		m_pTransformCom->Move_Pos(&m_vLook, fTimeDelta, -15.f);
	}
		//왼쪽 게걸음
	if (Engine::Key_Pressing(KEY_A)) {
		dynamic_cast<CGun*>(m_pGun)->Set_State(CGun::WALKING);
		m_pTransformCom->Move_Pos(&vRight, fTimeDelta, -15.f);
	}
		//오른족 게걸음
	if (Engine::Key_Pressing(KEY_D)) {
		dynamic_cast<CGun*>(m_pGun)->Set_State(CGun::WALKING);
		m_pTransformCom->Move_Pos(&vRight, fTimeDelta, 15.f);
	}
	//총알

	// 미정_0401
	m_fTime += fTimeDelta;
	if (!m_bSkillOn&&Engine::Key_Pressing(VK_LBUTTON)) { // 기본 총알
		if (m_fTime >= 0.2f) {
			dynamic_cast<CGun*>(m_pGun)->Set_State(CGun::SHOOTING);
			Create_Bullet();// 미정_0330 인자지움
			m_fTime = 0.f;
		}
	}

	if (!m_bSkillOn&&Engine::Key_Down(VK_RBUTTON)) { // 토글버튼. 총 껐다 켰다 할 수 있음
		m_bSkillOn = true;
		dynamic_cast<CGun*>(m_pGun)->Gun_On();
	}
	if (m_bSkillOn&&Engine::Key_Down(VK_RBUTTON)) { // 토글버튼
		m_bSkillOn = false;
		dynamic_cast<CGun*>(m_pGun)->Gun_Off();
	}

	// 0401_미정
	// 버튼 짜기 귀찮아서 무식하게 박았읍니다 머리 박겠읍니다 죄송합니다 히ㅣㅎ....
	if (m_eGunSkill == FASTBULLET&&::Key_Down(KEY_Q)) {
		m_eGunSkill = ICE;
	}
	if (m_eGunSkill == ICE&&::Key_Down(KEY_Q)) {
		m_eGunSkill = BOMB;
	}
	if (m_eGunSkill == BOMB&&::Key_Down(KEY_Q)) {
		m_eGunSkill = BARRIER;
	}
	if (m_eGunSkill == BARRIER&&::Key_Down(KEY_Q)) {
		m_eGunSkill = SHOTGUN;
	}
	if (m_eGunSkill == SHOTGUN&&::Key_Down(KEY_Q)) {
		m_eGunSkill = FIRE;
	}
	if (m_eGunSkill == FIRE&&::Key_Down(KEY_Q)) {
		m_eGunSkill = RAY;
	}
	if (m_eGunSkill == RAY&&::Key_Down(KEY_Q)) {
		m_eGunSkill = BIGBULLET;
	}
	if (m_eGunSkill == BIGBULLET&&::Key_Down(KEY_Q)) {
		m_eGunSkill = FASTBULLET;
	}

	if (m_eGunSkill == FASTBULLET&&::Key_Down(KEY_E)) {
		m_eGunSkill = BIGBULLET;
	}
	if (m_eGunSkill == BIGBULLET&&::Key_Down(KEY_E)) {
		m_eGunSkill = RAY;
	}
	if (m_eGunSkill == RAY&&::Key_Down(KEY_E)) {
		m_eGunSkill = FIRE;
	}
	if (m_eGunSkill == FIRE&&::Key_Down(KEY_E)) {
		m_eGunSkill = SHOTGUN;
	}
	if (m_eGunSkill == SHOTGUN&&::Key_Down(KEY_E)) {
		m_eGunSkill = BARRIER;
	}
	if (m_eGunSkill == BARRIER&&::Key_Down(KEY_E)) {
		m_eGunSkill = BOMB;
	}
	if (m_eGunSkill == BOMB&&::Key_Down(KEY_E)) {
		m_eGunSkill = ICE;
	}
	if (m_eGunSkill == ICE&&::Key_Down(KEY_E)) {
		m_eGunSkill = FASTBULLET;
	}

	if (m_bSkillOn) {
		switch (m_eGunSkill) {
		case FASTBULLET:
			if (Engine::Key_Pressing(VK_LBUTTON)) {
				if (m_fTime >= 0.1f) {
					dynamic_cast<CGun*>(m_pGun)->Set_State(CGun::SHOOTING);
					Create_Bullet();
					m_fTime = 0.f;
				}
			}
			break;
		case BIGBULLET:
			if (Engine::Key_Pressing(VK_LBUTTON)) {
				if (m_fTime >= 0.7f) {
					dynamic_cast<CGun*>(m_pGun)->Set_State(CGun::SHOOTING);
					Create_BigBullet();
					m_fTime = 0.f;
				}
			}
			break;
		case RAY: // 미래의 궁극기
			if (Engine::Key_Pressing(VK_LBUTTON)) {
				dynamic_cast<CGun*>(m_pGun)->Set_State(CGun::SHOOTING);
				Create_Ray();
			}
			break;
		case FIRE:
			break;
		case SHOTGUN:
			break;
		case BARRIER:
			break;
		case BOMB:
			break;
		case ICE:
			if (Engine::Key_Pressing(VK_LBUTTON)) {
				if (m_fTime >= 0.7f) {
					dynamic_cast<CGun*>(m_pGun)->Set_State(CGun::SHOOTING);
					Create_IceBullet();
					m_fTime = 0.f;
				}
			}
			break;
		}
	}
	//cout << m_bSkillOn << endl;
	//cout << m_eGunSkill << endl;
	Mouse_Fix();
}

void CPlayer::SetUp_OnTerrain()
{
	_float	fHeight = 0.f;
	_vec3	vPos;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);

	//0406_동규_플레이어가 서있는 Terrain의 컴포넌트를 얻어오는 방식으로 변경. vector 컨테이너 2가지는 Scene에 넣어둠.
	_ulong dwIndex = m_pCalculatorCom->Compute_IndexOnTerrain(&vPos, Engine::Get_Scene()->Get_VecTerrainInfo());
	if (dwIndex ==-1)
		return;
	CTerrain* pTerrain = dynamic_cast<CTerrain*>(Engine::Get_Scene()->Get_VecTerrain()[dwIndex]);
	
	Engine::CTerrainTex*		pTerrainBufferCom = dynamic_cast<Engine::CTerrainTex*>(pTerrain->Get_Component(L"Com_Buffer", Engine::COM_STATIC));
	//Engine::CTerrainTex*		pTerrainBufferCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Get_Component(Engine::LAYER_ENVIR, ENVIR_TERRAIN, L"Terrain", L"Com_Buffer", Engine::COM_STATIC));
	NULL_CHECK(pTerrainBufferCom);
	_ulong dwVTXCNTX =  Engine::Get_Scene()->Get_VecTerrainInfo()[dwIndex]->dwVtxCNX;
	_ulong dwVTXCNTZ = Engine::Get_Scene()->Get_VecTerrainInfo()[dwIndex]->dwVtxCNZ;
	_ulong dwVTXITV = Engine::Get_Scene()->Get_VecTerrainInfo()[dwIndex]->dwVtxItv;


	fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainBufferCom->Get_VtxPos(), dwVTXCNTX, dwVTXCNTZ, dwVTXITV);
	m_pTransformCom->Set_Pos(&_vec3(vPos.x, fHeight +5.f, vPos.z));
}

void CPlayer::Mouse_Fix()
{
	POINT		ptMouse{ WINCX >> 1 , WINCY >> 1 };
	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}

HRESULT CPlayer::CreateBullet()
{
	return E_NOTIMPL;
}

HRESULT CPlayer::Create_Bullet() // 미정_0330 인자지움
{
	Engine::CGameObject*		pGameObject = nullptr;
	pGameObject = CBullet::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject(LAYER_LOGIC, LOGIC_PLAYERWEAPON, pGameObject), E_FAIL);
	return S_OK;
}

HRESULT CPlayer::Create_BigBullet()
{
	Engine::CGameObject*		pGameObject = nullptr;
	pGameObject = CBigBullet::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject(LAYER_LOGIC, LOGIC_PLAYERWEAPON, pGameObject), E_FAIL);
	return S_OK;
}

HRESULT CPlayer::Create_Ray()
{
	Engine::CGameObject*		pGameObject = nullptr;
	pGameObject = CRay::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject(LAYER_LOGIC, LOGIC_PLAYERWEAPON, pGameObject), E_FAIL);
	return S_OK;
}

HRESULT CPlayer::Create_IceBullet()
{
	Engine::CGameObject*		pGameObject = nullptr;
	pGameObject = CIceBullet::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject(LAYER_LOGIC, LOGIC_PLAYERWEAPON, pGameObject), E_FAIL);
	return S_OK;
}
