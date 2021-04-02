#include "Camera.h"

USING(Engine)

Engine::CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_vEye(0.f, 0.f, 0.f)
	, m_vAt(0.f, 0.f, 0.f)
	, m_vUp(0.f, 0.f, 0.f)
	, m_fAspect(0.f), m_fFovY(0.f), m_fNear(0.f), m_fFar(0.f)
{
}

Engine::CCamera::~CCamera()
{
}

HRESULT Engine::CCamera::Ready_Object()
{
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFovY, m_fAspect, m_fNear, m_fFar);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	return S_OK;
}

void Engine::CCamera::LateUpdate_Object(const _float& fTimeDelta)
{
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
}

void Engine::CCamera::Free()
{
	CGameObject::Free();
}