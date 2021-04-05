#include "stdafx.h"
#include "Effect.h"

#include "Export_Function.h"

USING(Engine)

CEffect::CEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_fSpeed(0.f)
	, m_bIsDead(false)
	, m_fLifeTime(0.f)
{
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}

CEffect::~CEffect()
{

}

CEffect * CEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, _tchar * pObjTag, const _vec3 * pPos, const _vec3 * pScale, RESOURCEID eTextureID)
{
	CEffect*	pInstance = new CEffect(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pObjTag, pPos, pScale, eTextureID)))
		Safe_Release(pInstance);

	return pInstance;
}

void CEffect::Free()
{
	CGameObject::Free();
}


HRESULT CEffect::Ready_Object(_tchar* pObjTag, const _vec3* pPos, const _vec3* pScale, RESOURCEID eTextureID)
{
	m_pObjTag = pObjTag;

	FAILED_CHECK_RETURN(Add_Component(eTextureID), E_FAIL);

	m_pTransformCom->Set_Pos(pPos);
	m_pTransformCom->Set_Scale(pScale);

	//Tag 값에 따라서 Frame 값을 다르게 초기화해주면 됨
	//어차피 End값이 되면 죽어야 하니까 effect는

	m_fSpeed = 10.f;
	m_tFrame.fFrameStart = 0;
	m_tFrame.fFrameEnd = m_pTextureCom->Get_TextureSize() - 1;


	return S_OK;
}

Engine::_int CEffect::Update_Object(const _float& fTimeDelta)
{	
	if (m_bIsDead)
		return OBJ_DEAD;

	CGameObject::Update_Object(fTimeDelta);

	BillBoard();

	return OBJ_NOEVENT;
}

void CEffect::LateUpdate_Object(const _float & fTimeDelta)
{
	CGameObject::LateUpdate_Object(fTimeDelta);

	m_tFrame.fFrameStart += m_fSpeed * fTimeDelta;

	if (m_tFrame.fFrameStart > m_tFrame.fFrameEnd)
	{
		m_tFrame.fFrameStart = m_tFrame.fFrameEnd;
		m_bIsDead = true;
	}
	m_pRendererCom->Add_RenderGroup(RENDER_ALPHA, this);
}

void CEffect::Render_Object()
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pTextureCom->Set_Texture(_uint(m_tFrame.fFrameStart));
	m_pBufferCom->Render_Buffers();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CEffect::Add_Component(RESOURCEID eTextureID)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(eTextureID, m_pObjTag));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Texture", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Renderer", pComponent);

	return S_OK;
}

void CEffect::BillBoard()
{
	_matrix matView, matScale;
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matScale);

	_vec3 vScale = *m_pTransformCom->Get_Scale();

	D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	memset(&matView._41, 0, sizeof(_vec3));
	D3DXMatrixInverse(&matView, 0, &matView);
	_vec3 vPos;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
	_vec3 BillPos = { vPos.x, vPos.y, vPos.z };
	memcpy(&matView._41, &BillPos, sizeof(_vec3));

	m_pTransformCom->Set_Matrix(&(matScale*matView));
}
