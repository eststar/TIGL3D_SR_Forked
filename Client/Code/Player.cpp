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
	, m_fMeteoTime(0.f)
	, m_bHasGun(true)
	, m_eGunSkill(FASTBULLET)
	, m_bSkillOn(false)
	, m_bUltimateOn(false)
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
		/*�׾��� �� ���� �̺�Ʈ ��*/
	}

	// ����_0331 �� ����
	if (m_bHasGun) {
		m_pGun = CGun::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(m_pGun, E_FAIL);
		FAILED_CHECK_RETURN(Engine::Add_GameObject(LAYER_LOGIC, LOGIC_PLAYERWEAPON, m_pGun), E_FAIL);
		m_bHasGun = false;
	}

	SetUp_OnTerrain();
	Key_Input(fTimeDelta);
	Ultimate(fTimeDelta);
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

	//�÷��̾ ť�� ���¿��� ť�� �ȿ� ī�޶� �ھƳ��� �ϴ� �ĸ� �߷����� ����
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

	//���� ���� ��ȯ
	if (dwMouse = Engine::Get_DIMouseMove(Engine::DIMS_Y))
		m_pTransformCom->Rotation(Engine::ROT_X, D3DXToRadian(dwMouse / 10.f));
	//���� �¿� ��ȯ
	if (dwMouse = Engine::Get_DIMouseMove(Engine::DIMS_X))
		m_pTransformCom->Rotation(Engine::ROT_Y, D3DXToRadian(dwMouse / 10.f));

	dynamic_cast<CGun*>(m_pGun)->Set_State(CGun::IDLE);

	//����
	if (Engine::Key_Pressing(KEY_W)) {
		dynamic_cast<CGun*>(m_pGun)->Set_State(CGun::WALKING);
		m_pTransformCom->Move_Pos(&m_vLook, fTimeDelta, 15.f);
	}
	//����
	if (Engine::Key_Pressing(KEY_S)) {
		dynamic_cast<CGun*>(m_pGun)->Set_State(CGun::WALKING);
		m_pTransformCom->Move_Pos(&m_vLook, fTimeDelta, -15.f);
	}
	//���� �԰���
	if (Engine::Key_Pressing(KEY_A)) {
		dynamic_cast<CGun*>(m_pGun)->Set_State(CGun::WALKING);
		m_pTransformCom->Move_Pos(&vRight, fTimeDelta, -15.f);
	}
	//������ �԰���
	if (Engine::Key_Pressing(KEY_D)) {
		dynamic_cast<CGun*>(m_pGun)->Set_State(CGun::WALKING);
		m_pTransformCom->Move_Pos(&vRight, fTimeDelta, 15.f);
	}
	//�Ѿ�

	// ����_0401
	m_fTime += fTimeDelta;
	if (!m_bSkillOn&&Engine::Key_Pressing(VK_LBUTTON)) { // �⺻ �Ѿ�
		if (m_fTime >= 0.2f) {
			dynamic_cast<CGun*>(m_pGun)->Set_State(CGun::SHOOTING);
			Create_Bullet();// ����_0330 ��������
			m_fTime = 0.f;
		}
	}

	if (!m_bSkillOn&&Engine::Key_Down(VK_RBUTTON)) { // ��۹�ư. �� ���� �״� �� �� ����
		m_bSkillOn = true;
		dynamic_cast<CGun*>(m_pGun)->Gun_On();
	}
	if (m_bSkillOn&&Engine::Key_Down(VK_RBUTTON)) { // ��۹�ư
		m_bSkillOn = false;
		dynamic_cast<CGun*>(m_pGun)->Gun_Off();
	}

	if (Engine::Key_Pressing(KEY_C)) {  // �ñر� ��ư
		m_bUltimateOn = true;
	}
	if (Engine::Key_Up(KEY_C)) {  // �ñر� ��ư
		m_bUltimateOn = false;
	}

	if (m_eGunSkill == FASTBULLET&&Engine::Key_Down(KEY_Q)) {
		m_eGunSkill = ICE;
	}
	if (m_eGunSkill == ICE&&Engine::Key_Down(KEY_Q)) {
		m_eGunSkill = BARRIER;
	}
	if (m_eGunSkill == BARRIER&&Engine::Key_Down(KEY_Q)) {
		m_eGunSkill = SHOTGUN;
	}
	if (m_eGunSkill == SHOTGUN&&Engine::Key_Down(KEY_Q)) {
		m_eGunSkill = FIRE;
	}
	if (m_eGunSkill == FIRE&&Engine::Key_Down(KEY_Q)) {
		m_eGunSkill = RAY;
	}
	if (m_eGunSkill == RAY&&Engine::Key_Down(KEY_Q)) {
		m_eGunSkill = BIGBULLET;
	}
	if (m_eGunSkill == BIGBULLET&&Engine::Key_Down(KEY_Q)) {
		m_eGunSkill = FASTBULLET;
	}
	if (m_eGunSkill == FASTBULLET&&Engine::Key_Down(KEY_E)) {
		m_eGunSkill = BIGBULLET;
	}
	if (m_eGunSkill == BIGBULLET&&Engine::Key_Down(KEY_E)) {
		m_eGunSkill = RAY;
	}
	if (m_eGunSkill == RAY&&Engine::Key_Down(KEY_E)) {
		m_eGunSkill = FIRE;
	}
	if (m_eGunSkill == FIRE&&Engine::Key_Down(KEY_E)) {
		m_eGunSkill = SHOTGUN;
	}
	if (m_eGunSkill == SHOTGUN&&Engine::Key_Down(KEY_E)) {
		m_eGunSkill = BARRIER;
	}
	if (m_eGunSkill == BARRIER&&Engine::Key_Down(KEY_E)) {
		m_eGunSkill = ICE;
	}
	if (m_eGunSkill == ICE&&Engine::Key_Down(KEY_E)) {
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
		case RAY:
			if (Engine::Key_Pressing(VK_LBUTTON)) {
				if (m_fTime >= 1.2f) {
					dynamic_cast<CGun*>(m_pGun)->Set_State(CGun::SHOOTING);
					Create_RayBullet();
					m_fTime = 0.f;
				}
			}
			break;
		case FIRE:
			if (Engine::Key_Pressing(VK_LBUTTON))
				if (m_fTime >= 0.05f) {
					Create_FireBullet();
					m_fTime = 0.f;
				}
			break;
		case SHOTGUN:
			if (Engine::Key_Pressing(VK_LBUTTON)) {
				if (m_fTime >= 0.4f) {
					dynamic_cast<CGun*>(m_pGun)->Set_State(CGun::SHOOTING);
					Create_SpreadBullet(0.6f);
					Create_SpreadBullet(0.4f);
					Create_SpreadBullet(0.2f);
					Create_SpreadBullet(0.f);
					Create_SpreadBullet(-0.2f);
					Create_SpreadBullet(-0.4f);
					Create_SpreadBullet(-0.6f);
					m_fTime = 0.f;
				}
			}
			break;
		case BARRIER:
			break;
		case ICE:
			if (Engine::Key_Pressing(VK_LBUTTON)) {
				if (m_fTime >= 0.4f) {
					dynamic_cast<CGun*>(m_pGun)->Set_State(CGun::SHOOTING);
					Create_IceBullet();
					m_fTime = 0.f;
				}
			}
			break;
		}
	}


	//cout << m_eGunSkill << endl;
	Mouse_Fix();
}

void CPlayer::SetUp_OnTerrain()
{
	_float	fHeight = 0.f;
	_vec3	vPos;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);

	//0406_����_�÷��̾ ���ִ� Terrain�� ������Ʈ�� ������ ������� ����. vector �����̳� 2������ Scene�� �־��.
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

HRESULT CPlayer::Create_Bullet() // ����_0330 ��������
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
	// �ñر�� ��뿹��
	Engine::CGameObject*		pGameObject = nullptr;
	pGameObject = CRay::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject(LAYER_LOGIC, LOGIC_PLAYERULTIMATE, pGameObject), E_FAIL);
	return S_OK;
}

HRESULT CPlayer::Create_Meteo(_vec3& vPos)
{
	Engine::CGameObject*		pGameObject = nullptr;
	pGameObject = CMeteo::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CMeteo*>(pGameObject)->Set_Pos(vPos);
	FAILED_CHECK_RETURN(Engine::Add_GameObject(LAYER_LOGIC, LOGIC_PLAYERULTIMATE, pGameObject), E_FAIL);
	return S_OK;
}

HRESULT CPlayer::Create_Thunder(_vec3 & vPos)
{
	Engine::CGameObject*		pGameObject = nullptr;
	pGameObject = CThunder::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CThunder*>(pGameObject)->Set_Pos(vPos);
	FAILED_CHECK_RETURN(Engine::Add_GameObject(LAYER_LOGIC, LOGIC_PLAYERULTIMATE, pGameObject), E_FAIL);
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

HRESULT CPlayer::Create_RayBullet()
{
	Engine::CGameObject*		pGameObject = nullptr;
	pGameObject = CRayBullet::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject(LAYER_LOGIC, LOGIC_PLAYERWEAPON, pGameObject), E_FAIL);
	return S_OK;
}

HRESULT CPlayer::Create_SpreadBullet(const _float fAngle)
{
	Engine::CGameObject*		pGameObject = nullptr;
	pGameObject = CSpreadBullet::Create(m_pGraphicDev, fAngle);
	dynamic_cast<CSpreadBullet*>(pGameObject)->Set_Angle(fAngle);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject(LAYER_LOGIC, LOGIC_PLAYERWEAPON, pGameObject), E_FAIL);
	return S_OK;
}

HRESULT CPlayer::Create_FireBullet()
{
	Engine::CGameObject*		pGameObject = nullptr;
	pGameObject = CFireBullet::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject(LAYER_LOGIC, LOGIC_PLAYERWEAPON, pGameObject), E_FAIL);
	return S_OK;
}

HRESULT CPlayer::Ultimate(const _float& fTimeDelta)
{
	if (m_bUltimateOn) {
		switch (m_eGunSkill) {
		case FASTBULLET:
			break;
		case BIGBULLET:
			if (m_fTime >= 0.2f) {
				Create_Thunder(_vec3(50.f, 55.f, 50.f));
				m_fTime = 0.f;
			}
			break;
		case RAY:
			m_fMeteoTime += fTimeDelta;
			if (m_fMeteoTime >= 0.1f)
				Create_Ray();
			if (m_fMeteoTime >= 0.05f)
				Create_Ray();
			if (m_fMeteoTime >= 0.1f)
				Create_Ray();
			if (m_fMeteoTime >= 0.15f)
				Create_Ray();
			if (m_fMeteoTime >= 0.2f)
				Create_Ray();
			if (m_fMeteoTime >= 0.25f)
				Create_Ray();
			if (m_fMeteoTime >= 0.3f)
				Create_Ray();
			if (m_fMeteoTime >= 0.35f)
				Create_Ray();
			if (m_fMeteoTime >= 0.4f)
				Create_Ray();
			if (m_fMeteoTime >= 0.45f)
				Create_Ray();
			if (m_fMeteoTime >= 0.5f)
				Create_Ray();
			if (m_fMeteoTime >= 0.55f)
				Create_Ray();
			if (m_fMeteoTime >= 0.6f)
				Create_Ray();
			if (m_fMeteoTime >= 0.65f)
				Create_Ray();
			if (m_fMeteoTime >= 0.7f)
				Create_Ray();
			if (m_fMeteoTime >= 0.75f)
				Create_Ray();
			break;
		case FIRE:
			m_fMeteoTime += fTimeDelta;
			if (m_fTime >= 1.2f) {
				if (m_fMeteoTime >= 0.2f)
					Create_Meteo(_vec3(50.f, 30.f, 50.f));
				if (m_fMeteoTime >= 0.2f)
					Create_Meteo(_vec3(52.f, 32.f, 42.f));
				if (m_fMeteoTime >= 0.3f)
					Create_Meteo(_vec3(48.f, 28.f, 49.f));
				if (m_fMeteoTime >= 0.4f)
					Create_Meteo(_vec3(50.f, 35.f, 50.f));
				if (m_fMeteoTime >= 0.4f)
					Create_Meteo(_vec3(48.f, 28.f, 50.f));
				if (m_fMeteoTime >= 0.5f)
					Create_Meteo(_vec3(50.f, 35.f, 50.f));
				if (m_fMeteoTime >= 0.6f)
					Create_Meteo(_vec3(51.f, 31.f, 52.f));
				if (m_fMeteoTime >= 0.6f)
					Create_Meteo(_vec3(49.f, 30.f, 48.f));
				if (m_fMeteoTime >= 0.7f)
					Create_Meteo(_vec3(50.f, 34.f, 46.f));
				if (m_fMeteoTime >= 0.8f) {
					Create_Meteo(_vec3(46.f, 27.f, 52.f));
				}
				m_fTime = 0.f;
			}
			break;
		case SHOTGUN:
			break;
		case BARRIER:
			break;
		case ICE:
			break;
		}
		m_bUltimateOn = false;
	}
	return S_OK;
}
