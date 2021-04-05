#include "stdafx.h"
#include "Gun.h"
#include "Export_Function.h"

USING(Engine)

CGun::CGun(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_fAngle(0.f)
	, m_fSpeed(0.f)
	, m_fTime(0.f)
	, m_fShootAngle(0.f)
	, m_fShootTime(0.f)
	, m_bIsOn(false)
{
}


CGun::~CGun()
{
}

HRESULT CGun::Ready_Object()
{
	m_pObjTag = L"PlayerGun";
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	_vec3 vPlayerRight, vPlayerLook;
	m_pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(LAYER_LOGIC, LOGIC_PLAYER, L"Player", L"Com_Transform", COM_DYNAMIC));
	m_pPlayerTransformCom->Get_Info(INFO_RIGHT, &vPlayerRight);
	m_pPlayerTransformCom->Get_Info(INFO_LOOK, &vPlayerLook);

	vPlayerRight *= 0.5f;
	vPlayerRight.x -= 0.1f;
	vPlayerRight.y -= 0.75f;
	vPlayerRight.z += 0.5f;
	m_pTransformCom->Set_Scale(&_vec3(0.3f, 0.3f, 0.3f));
	m_pTransformCom->Set_Pos(&vPlayerRight);

	_vec3 vDir, vPos;
	vPos = 0.52f * vPlayerRight;
	vDir = vPlayerLook - vPos;
	D3DXVec3Normalize(&m_vDir, &vPos);
	m_fSpeed = 1.f;
	return S_OK;
}

_int CGun::Update_Object(const _float & fTimeDelta)
{
	CGameObject::Update_Object(fTimeDelta);
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_RIGHT, &vPos);
	D3DXVec3Normalize(&vPos, &vPos);
	switch (m_eState) {
	case IDLE:
		m_fAngle = 0.f;
		m_fShootAngle = 0.f;
		m_fTime = 0;
		m_fSpeed = 1.f;
		m_fShootAngle = 0.f;
		break;
	case WALKING:
		m_fTime += fTimeDelta;
		m_fAngle += (0.5f *m_fSpeed);
		if (m_fTime > 0.5f&& m_fTime < 1.f) {
			m_fSpeed = -1.5;
		}
		if (m_fTime > 1.f) {
			m_fSpeed = 1.5;
			m_fTime = 0;
		}
		m_pTransformCom->RotationAxis(&m_vDir, D3DXToRadian(m_fAngle));
		break;
	case SHOOTING:
		m_fShootAngle = -10;
		static float a = 0.f;
		a -= 45.f * fTimeDelta;
		m_pTransformCom->RotationX(D3DXToRadian(m_fShootAngle));
		break;
	}


	m_pRendererCom->Add_RenderGroup(RENDER_ALPHA, this);
	return 0;
}

void CGun::LateUpdate_Object(const _float & fTimeDelta)
{
	CGameObject::LateUpdate_Object(fTimeDelta);
}

void CGun::Render_Object()
{
	_matrix matView, matProj, matOne;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);


	D3DXMatrixIdentity(&matOne);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOne);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOne);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if(m_bIsOn) // ¹ÌÁ¤_0401
		m_pTextureCom->Set_Texture(1);
	else
		m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffers();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);

}

void CGun::Gun_On()
{
	m_bIsOn = true;
}

void CGun::Gun_Off()
{
	m_bIsOn = false;
}

HRESULT CGun::Add_Component()
{
	Engine::CComponent*      pComponent = nullptr;
	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_PlayerGun"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Texture", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Renderer", pComponent);
	return S_OK;
}

void CGun::Set_State(STATE state)
{
	m_eState = state;
}

CGun * CGun::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGun*   pInstance = new CGun(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CGun::Free()
{
	CGameObject::Free();
}