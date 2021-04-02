#include "stdafx.h"
#include "Bullet.h"

#include "Export_Function.h"

USING(Engine)

CBullet::CBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CCreature(pGraphicDev)
	, m_iTextureCount(0)
	, m_fTime(0.f)
{
}


CBullet::~CBullet()
{
}

HRESULT CBullet::Ready_Object()
{
	m_pObjTag = L"PlayerBullet";
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	
	m_pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(LAYER_LOGIC, LOGIC_PLAYER, L"Player", L"Com_Transform", COM_DYNAMIC));
	FAILED_CHECK_RETURN(m_pPlayerTransformCom, E_FAIL);
	
	_vec3 vPos, vLook, vRight, vUp;
	m_pPlayerTransformCom->Get_Info(INFO_POS, &vPos);
	m_pPlayerTransformCom->Get_Info(INFO_LOOK, &m_vLook);
	m_pPlayerTransformCom->Get_Info(INFO_RIGHT, &vRight);
	m_pPlayerTransformCom->Get_Info(INFO_UP, &vUp);
	m_pTransformCom->Set_Scale(&_vec3(0.2f, 0.2f, 0.2f));
	
	m_pTransformCom->Set_Info(INFO_LOOK, &m_vLook);


	vRight *= 0.45f;
	vUp *= -1.5f;
	m_vLook *= 2.6f;
	_vec3 vTemp;
	vTemp = vRight + vUp + m_vLook;
	vPos += vTemp;

	m_pTransformCom->Set_Pos(&vPos);
	return S_OK;
}

_int CBullet::Update_Object(const _float & fTimeDelta)
{
	if(m_bIsDead)
		return OBJ_DEAD;

	CGameObject::Update_Object(fTimeDelta);
	m_pRendererCom->Add_RenderGroup(RENDER_ALPHA, this);

	m_fTime += fTimeDelta;

	if (m_fTime >= 0.1f) {
		m_iTextureCount = 1;
		if (m_fTime >= 0.2f) {
			m_iTextureCount = 0;
			m_fTime = 0.f;
		}
	}

	return OBJ_NOEVENT;
}

void CBullet::LateUpdate_Object(const _float & fTimeDelta)
{
	CGameObject::LateUpdate_Object(fTimeDelta);
	BillBoard();// ����_0330 ������ �Լ� ȣ��
	m_pTransformCom->Move_Pos(&m_vLook, fTimeDelta, 80.f);
}

void CBullet::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(m_iTextureCount);

	m_pBufferCom->Render_Buffers();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CBullet::Set_Pos(_vec3 & vPos)
{
	m_pTransformCom->Set_Pos(&vPos);
}

HRESULT CBullet::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;
	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Buffer", pComponent);
	
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_BasicBullet"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Texture", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Renderer", pComponent);
	return S_OK;
}

void CBullet::BillBoard()
{
	_matrix matView, matScale;
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matScale);

	matScale *= 0.05f;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	memset(&matView._41, 0, sizeof(_vec3));
	D3DXMatrixInverse(&matView, 0, &matView);
	_vec3 vPos;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
	_vec3 BillPos = {vPos.x, vPos.y, vPos.z};
	memcpy(&matView._41, &BillPos, sizeof(_vec3));

	m_pTransformCom->Set_Matrix(&(matScale*matView));
}

CBullet * CBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBullet*	pInstance = new CBullet(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CBullet::Free()
{
	CGameObject::Free();
}
