#include "stdafx.h"
#include "Spider.h"

#include "Export_Function.h"

USING(Engine)

CSpider::CSpider(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CCreature(pGraphicDev)
{
}

CSpider::~CSpider()
{
}

CSpider * CSpider::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSpider* pInstance = new CSpider(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CSpider::Free()
{
	CGameObject::Free();
}

HRESULT CSpider::Ready_Object()
{
	m_pObjTag = L"Spider";
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	/* 0405_진원_10~50까지의 실수형 난수 생성 */
	random_device rd;
	mt19937_64 randNum(rd());
	uniform_real_distribution<float> dist(10.f, 50.f);

	m_pTransformCom->Set_Pos(&_vec3(dist(randNum), 0.f, dist(randNum)));

	m_tFrame.fFrameStart = 0;
	m_tFrame.fFrameEnd = 4;
	/* 죽었을 때 애니메이션 변경해줘야ㅕ 함 */

	return S_OK;
}

_int CSpider::Update_Object(const _float & fTimeDelta)
{
	if (m_bIsDead)
		return OBJ_DEAD;

	SetUp_OnTerrain();
	Follow_Player(fTimeDelta);

	CGameObject::Update_Object(fTimeDelta);
	m_pRendererCom->Add_RenderGroup(RENDER_ALPHA, this);

	return OBJ_NOEVENT;
}

void CSpider::LateUpdate_Object(const _float & fTimeDelta)
{
	CGameObject::LateUpdate_Object(fTimeDelta);
	BillBoardYaw();
	Frame_Move(fTimeDelta);
}

void CSpider::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(static_cast<_uint>(m_tFrame.fFrameStart));
	m_pBufferCom->Render_Buffers();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CSpider::Add_Component()
{
	Engine::CComponent*	pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Spider"));
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

void CSpider::SetUp_OnTerrain()
{
	_float	fHeight = 0.f;
	_vec3	vPos;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);

	Engine::CTerrainTex* pTerrainBufferCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Get_Component(Engine::LAYER_ENVIR, ENVIR_TERRAIN, L"Terrain", L"Com_Buffer", Engine::COM_STATIC));
	NULL_CHECK(pTerrainBufferCom);

	fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainBufferCom->Get_VtxPos(), VTXCNTX, VTXCNTZ, VTXITV);
	m_pTransformCom->Set_Pos(&_vec3(vPos.x, fHeight + 1.f, vPos.z));
}

void CSpider::BillBoardYaw()
{
	D3DXMATRIX matScale, matView, matBill;

	D3DXMatrixIdentity(&matView);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixIdentity(&matBill);
	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;
	D3DXMatrixInverse(&matBill, NULL, &matBill);

	_vec3 BillPos = *(m_pTransformCom->Get_Info(Engine::INFO_POS));
	memcpy(&matBill._41, &BillPos, sizeof(_vec3));

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);

	m_pTransformCom->Set_Matrix(matScale * matBill);
}

void CSpider::Frame_Move(const _float & fTimeDelta)
{
	m_tFrame.fFrameStart += m_tFrame.fFrameEnd * fTimeDelta;

	if (m_tFrame.fFrameStart >= m_tFrame.fFrameEnd)
	{
		m_tFrame.fFrameStart = 0.f;
	}
}

_int CSpider::Follow_Player(const _float & fTimeDelta)
{
	m_pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(LAYER_LOGIC, LOGIC_PLAYER, L"Player", L"Com_Transform", COM_DYNAMIC));
	m_pPlayerTransformCom->Get_Info(INFO_POS, &m_vPlayerPos);

	_vec3 vDir = m_vPlayerPos - *(m_pTransformCom->Get_Info(INFO_POS));
	m_pTransformCom->Set_Info(INFO_LOOK, D3DXVec3Normalize(&vDir, &vDir));
	m_pTransformCom->Move_Pos(m_pTransformCom->Get_Info(INFO_LOOK), fTimeDelta, 7.f);

	return 0;
}


