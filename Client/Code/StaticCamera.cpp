#include "stdafx.h"
#include "StaticCamera.h"

#include "Export_Function.h"

USING(Engine)

CStaticCamera::CStaticCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CCamera(pGraphicDev)
{

}

CStaticCamera::~CStaticCamera()
{

}

CStaticCamera* CStaticCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev,
	const _vec3* pEye, const _vec3* pAt, const _vec3* pUp,
	const _float& fAspect, const _float& fFovY,
	const _float& fNear, const _float& fFar)
{
	CStaticCamera*	pInstance = new CStaticCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pEye, pAt, pUp, fAspect, fFovY, fNear, fFar)))
		Safe_Release(pInstance);

	return pInstance;
}

void CStaticCamera::Free()
{
	CGameObject::Free();
}


HRESULT CStaticCamera::Ready_Object(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fAspect, const _float& fFovY, const _float& fNear, const _float& fFar)
{
	m_pObjTag = L"StaticCamera";

	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFovY = fFovY;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	FAILED_CHECK_RETURN(Engine::CCamera::Ready_Object(), E_FAIL);


	return S_OK;
}

Engine::_int CStaticCamera::Update_Object(const _float& fTimeDelta)
{
	Target_Renewal();
	Key_Input(fTimeDelta);

	CCamera::Update_Object(fTimeDelta);

	return OBJ_NOEVENT;
}

void CStaticCamera::LateUpdate_Object(const _float & fTimeDelta)
{
	//LateUpdate()에서 뷰행렬 변환 수행
	CCamera::LateUpdate_Object(fTimeDelta);
}


HRESULT CStaticCamera::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;
	return S_OK;
}



void CStaticCamera::Target_Renewal()
{
	Engine::CTransform*		pPlayerTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(LAYER_LOGIC, LOGIC_PLAYER, L"Player", L"Com_Transform", Engine::COM_DYNAMIC));
	NULL_CHECK(pPlayerTransform);

	_vec3	vLook;
	pPlayerTransform->Get_Info(Engine::INFO_LOOK, &vLook);

	m_vEye = vLook * -1.f;
	D3DXVec3Normalize(&m_vEye, &m_vEye);

	_vec3	vRight;
	//memcpy(&vRight, &pPlayerTransform->m_matWorld.m[0][0], sizeof(_vec3));
	pPlayerTransform->Get_Info(Engine::INFO_RIGHT, &vRight); // 다영_ Transform의 private 멤버변수들 호출을 위해 함수사용으로 변경

	_matrix		matRot;
	D3DXMatrixRotationAxis(&matRot, &vRight, m_fAngle);
	D3DXVec3TransformNormal(&m_vEye, &m_vEye, &matRot);

	_vec3	vPos;
	pPlayerTransform->Get_Info(Engine::INFO_POS, &vPos);

	m_vEye += vPos;
	m_vAt = vPos;
}

void CStaticCamera::Key_Input(const _float& fTimeDelta)
{
	if (Engine::Get_DIKeyState(DIK_U) & 0x80)
		m_fAngle += D3DXToRadian(45.f) * fTimeDelta;

	if (Engine::Get_DIKeyState(DIK_J) & 0x80)
		m_fAngle -= D3DXToRadian(45.f) * fTimeDelta;

	if (Engine::Get_DIKeyState(DIK_Y) & 0x80)
		m_fDistance += fTimeDelta * m_fSpeed;

	if (Engine::Get_DIKeyState(DIK_I) & 0x80)
		m_fDistance -= fTimeDelta * m_fSpeed;
}

