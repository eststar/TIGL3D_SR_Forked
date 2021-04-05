#include "stdafx.h"
#include "RayBullet.h"
#include "Export_Function.h"

USING(Engine)

CRayBullet::CRayBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CCreature(pGraphicDev)
	, m_fTime(0.f)
{
}

CRayBullet::~CRayBullet()
{
}

HRESULT CRayBullet::Ready_Object()
{
	m_pObjTag = L"PlayerRayBullet";
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(LAYER_LOGIC, LOGIC_PLAYER, L"Player", L"Com_Transform", COM_DYNAMIC));
	FAILED_CHECK_RETURN(m_pPlayerTransformCom, E_FAIL);

	_vec3 vPos, vLook, vRight, vUp;
	m_pPlayerTransformCom->Get_Info(INFO_POS, &vPos);
	m_pPlayerTransformCom->Get_Info(INFO_LOOK, &vLook);
	m_pPlayerTransformCom->Get_Info(INFO_LOOK, &m_vLook);
	m_pPlayerTransformCom->Get_Info(INFO_RIGHT, &vRight);
	m_pPlayerTransformCom->Get_Info(INFO_UP, &vUp);
	m_pTransformCom->Set_Scale(&_vec3(1.f, 1.f, 1.f));

	m_pTransformCom->Set_Info(INFO_LOOK, &m_vLook);
	vRight *= 0.45f;
	vUp *= -1.5f;
	vLook *= 12.f;
	_vec3 vTemp;
	vTemp = vRight + vUp + vLook;
	vPos += vTemp;

	m_vDir = m_vLook - vPos;
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	_float fAngleX = acosf(D3DXVec3Dot(&m_vDir, m_pPlayerTransformCom->Get_Info(INFO_RIGHT)));
	_float fAngleY = acosf(D3DXVec3Dot(&m_vDir, m_pPlayerTransformCom->Get_Info(INFO_UP)));
	_float fAngleZ = acosf(D3DXVec3Dot(&m_vDir, m_pPlayerTransformCom->Get_Info(INFO_LOOK)));
	//m_pTransformCom->Rotation(ROT_X, fAngleX);
	//m_pTransformCom->Rotation(ROT_Y, fAngleY);
	//m_pTransformCom->Rotation(ROT_Z, fAngleZ);

	//_vec3 vPosNormal;
	//D3DXVec3Normalize(&vPosNormal, &vPos);
	//_float fAngleX = acosf(D3DXVec3Dot(&vPosNormal, m_pPlayerTransformCom->Get_Info(INFO_RIGHT)));
	//_float fAngleY = acosf(D3DXVec3Dot(&vPosNormal, m_pPlayerTransformCom->Get_Info(INFO_UP)));
	//_float fAngleZ = acosf(D3DXVec3Dot(&vPosNormal, m_pPlayerTransformCom->Get_Info(INFO_LOOK)));

	//m_pTransformCom->Rotation(ROT_X, fAngleX);
	//m_pTransformCom->Rotation(ROT_Y, fAngleY);
	//m_pTransformCom->Rotation(ROT_Z, fAngleZ);
	m_pTransformCom->Set_Pos(&vPos);
	return S_OK;
}

_int CRayBullet::Update_Object(const _float & fTimeDelta)
{
	if (m_bIsDead)
		return OBJ_DEAD;

	CGameObject::Update_Object(fTimeDelta);
	m_pRendererCom->Add_RenderGroup(RENDER_ALPHA, this);

	//m_pTransformCom->Rotation(ROT_X, 30.f);
	//m_pTransformCom->Rotation(ROT_Z, 15.f);
	return OBJ_NOEVENT;
}

void CRayBullet::LateUpdate_Object(const _float & fTimeDelta)
{
	CGameObject::LateUpdate_Object(fTimeDelta);

	BillBoard();// 미정_0330 빌보드 함수 호출
	m_pTransformCom->Move_Pos(&m_vLook, fTimeDelta, 120.f);
}

void CRayBullet::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(2);

	m_pBufferCom->Render_Buffers();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CRayBullet::Set_Pos(_vec3 & vPos)
{
	m_pTransformCom->Set_Pos(&vPos);
}

HRESULT CRayBullet::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;
	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Ray"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Texture", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Renderer", pComponent);
	return S_OK;
}

void CRayBullet::BillBoard()
{
	_matrix matView, matScale, matRotX, matRotY, matRotZ, matWorld;
	//초기화
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matScale);
	D3DXMatrixIdentity(&matRotX);
	D3DXMatrixIdentity(&matRotY);
	D3DXMatrixIdentity(&matRotZ);
	D3DXMatrixIdentity(&matWorld);
	//크기
	D3DXMatrixScaling(&matScale, 0.3f, 10.f, 1.f);

	//자전
	_float fAngleX = acosf(D3DXVec3Dot(&m_vDir, m_pPlayerTransformCom->Get_Info(INFO_RIGHT)));
	_float fAngleY = acosf(D3DXVec3Dot(&m_vDir, m_pPlayerTransformCom->Get_Info(INFO_UP)));
	_float fAngleZ = acosf(D3DXVec3Dot(&m_vDir, m_pPlayerTransformCom->Get_Info(INFO_LOOK)));
	D3DXMatrixRotationX(&matRotX, D3DXToRadian(90.f));
	//D3DXMatrixRotationY(&matRotY, D3DXToRadian(10.f));
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(15.f));

	//이동
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	memset(&matView._41, 0, sizeof(_vec3));
	D3DXMatrixInverse(&matView, 0, &matView);
	_vec3 vPos;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
	_vec3 BillPos = { vPos.x, vPos.y, vPos.z};
	memcpy(&matView._41, &BillPos, sizeof(_vec3));

	matWorld = matScale*matRotX*matRotY*matRotZ*matView;

	m_pTransformCom->Set_Matrix(&matWorld);
}

CRayBullet * CRayBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRayBullet*	pInstance = new CRayBullet(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CRayBullet::Free()
{
	CGameObject::Free();
}
