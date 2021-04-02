#include "stdafx.h"
#include "FirstCamera.h"

#include "Export_Function.h"

USING(Engine)

CFirstCamera::CFirstCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CCamera(pGraphicDev)
{
}


CFirstCamera::~CFirstCamera()
{
}

HRESULT CFirstCamera::Ready_Object(const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const _float & fAspect, const _float & fFovY, const _float & fNear, const _float & fFar)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFovY = fFovY;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	FAILED_CHECK_RETURN(Engine::CCamera::Ready_Object(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

_int CFirstCamera::Update_Object(const _float & fTimeDelta)
{
	//Camera_Move();
	Mouse_Move(fTimeDelta);
	Mouse_Fix();
	CCamera::Update_Object(fTimeDelta);

	return 0;
}

HRESULT CFirstCamera::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;
	return S_OK;
}

void CFirstCamera::Mouse_Fix()
{
	POINT		ptMouse{ WINCX >> 1 , WINCY >> 1 };
	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}

void CFirstCamera::Mouse_Move(const _float & fTimeDelta)
{
	_matrix	matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);

	Engine::CTransform*		pPlayerTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(LAYER_LOGIC, LOGIC_PLAYER, L"Player", L"Com_Transform", Engine::COM_DYNAMIC));
	NULL_CHECK(pPlayerTransform);
	_vec3	vPos;
	pPlayerTransform->Get_Info(Engine::INFO_POS, &vPos);
	m_vEye = vPos;

	//_long		dwMouse = 0;
	//if (dwMouse = Engine::Get_DIMouseMove(Engine::DIMS_Y))
	//{
	//	_vec3		vRight;
	//	memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

	//	_vec3		vLook = m_vAt - m_vEye;

	//	_matrix		matRot;
	//	D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(dwMouse / 10.f));

	//	D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

	//	m_vAt = m_vEye + vLook + vPos;
	//}


	//if (dwMouse = Engine::Get_DIMouseMove(Engine::DIMS_X))
	//{
	//	_vec3		vUp = _vec3(0.f, 1.f, 0.f);

	//	_vec3		vLook = m_vAt - m_vEye;

	//	_matrix		matRot;
	//	D3DXMatrixRotationAxis(&matRot, &vUp, D3DXToRadian(dwMouse / 10.f));

	//	D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

	//	m_vAt = m_vEye + vLook + vPos;;
	//}
}

void CFirstCamera::Camera_Move()
{
	Engine::CTransform*		pPlayerTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(LAYER_LOGIC, LOGIC_PLAYER, L"Player", L"Com_Transform", Engine::COM_DYNAMIC));
	NULL_CHECK(pPlayerTransform);

	_vec3	vLook;
	pPlayerTransform->Get_Info(Engine::INFO_LOOK, &vLook);

	m_vEye = vLook * -1.f;
	D3DXVec3Normalize(&m_vEye, &m_vEye);

	m_vEye *= m_fDistance;

	_vec3	vRight;
	memcpy(&vRight, &pPlayerTransform->m_matWorld.m[0][0], sizeof(_vec3));

	_matrix		matRot;
	D3DXMatrixRotationAxis(&matRot, &vRight, m_fAngle);
	D3DXVec3TransformNormal(&m_vEye, &m_vEye, &matRot);

	_vec3	vPos;
	pPlayerTransform->Get_Info(Engine::INFO_POS, &vPos);

	m_vEye = vPos;
	//m_vAt = vPos;
}

CFirstCamera * CFirstCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const _float & fAspect, const _float & fFovY, const _float & fNear, const _float & fFar)
{
	CFirstCamera*	pInstance = new CFirstCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pEye, pAt, pUp, fAspect, fFovY, fNear, fFar)))
		Safe_Release(pInstance);

	return pInstance;
}

void CFirstCamera::Free(void)
{
	CGameObject::Free();
}
