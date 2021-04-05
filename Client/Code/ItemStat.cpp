#include "stdafx.h"
#include "ItemStat.h"

#include "Export_Function.h"

USING(Engine)

CItemStat::CItemStat(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_fSpeed(0.f)
{
	ZeroMemory(m_iDrawCnt, sizeof(_uint) * 4);
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}

CItemStat::~CItemStat()
{

}

void CItemStat::Free()
{
	CGameObject::Free();

	for (int i = 1; i < 4; ++i)
	{
		Safe_Release(m_pTransformCom[i]);
	}
}

HRESULT CItemStat::Ready_Object(_tchar* pObjTag, const _vec3* pPos)
{
	m_pObjTag = pObjTag;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	_float fSize = 0.06f;

	m_pTransformCom[0]->Set_Pos(&_vec3(pPos->x, pPos->y, pPos->z));
	m_pTransformCom[0]->Set_Scale(&_vec3(fSize * ((float)WINCY / WINCX), fSize, 0.f));

	for (int i = 1; i < 4; ++i)
	{
		m_pTransformCom[i]->Set_Pos(&_vec3((pPos->x + 0.02f) +(i * (fSize * 1.2f)), pPos->y, pPos->z));
		m_pTransformCom[i]->Set_Scale(&_vec3(fSize * ((float)WINCY / WINCX), fSize, 0.f));
	}

	if (!lstrcmp(L"Disk_S", m_pObjTag))
	{
		m_iDrawCnt[2] = 10;
	}

	m_fSpeed = 10.f;

	m_tFrame.fFrameStart = 0;
	m_tFrame.fFrameEnd = m_pTexIconCom->Get_TextureSize() - 1.f;


	return S_OK;
}

Engine::_int CItemStat::Update_Object(const _float& fTimeDelta)
{
	for (int i = 0; i < 4; ++i)
	{
		m_pTransformCom[i]->Update_Component(fTimeDelta);
	}
	m_pRendererCom->Add_RenderGroup(RENDER_UI, this);

	return OBJ_NOEVENT;
}

void CItemStat::LateUpdate_Object(const _float & fTimeDelta)
{
	//Gem_S인 경우 DrawCnt 1, 2, 3 을 바꿔줘야 하고
	//플레이어의 Get_GemCnt를 받아서 10보다 작으면 3만 바꿔주고 뭐 이런 식으로 변경

	//Disk_S인 경우 Drawcnt 1, 3 을 바꿔줘야 함
	//1 : Player 한테 받아오고
	//3 : Scene에서 받아오고

	m_tFrame.fFrameStart += m_fSpeed * fTimeDelta;

	if (m_tFrame.fFrameStart > m_tFrame.fFrameEnd)
	{
		m_tFrame.fFrameStart = 0;
	}

}

void CItemStat::Render_Object()
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[0]->Get_WorldMatrix());
	m_pTexIconCom->Set_Texture(_uint(m_tFrame.fFrameStart));
	m_pBufferCom->Render_Buffers();

	for (int i = 1; i < 4; ++i)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[i]->Get_WorldMatrix());
		m_pTexNumberCom->Set_Texture(m_iDrawCnt[i]);
		m_pBufferCom->Render_Buffers();
	}

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CItemStat::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTransformCom[0] = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_DYNAMIC].emplace(L"Com_Transform", pComponent);

	for (int i = 1; i < 4; ++i)
	{
		pComponent = m_pTransformCom[i] = Engine::CTransform::Create();
		NULL_CHECK_RETURN(pComponent, E_FAIL);
	}

	pComponent = m_pTexNumberCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Number"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_TexNumber", pComponent);

	pComponent = m_pTexIconCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, m_pObjTag));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_TexIcon", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::COM_STATIC].emplace(L"Com_Renderer", pComponent);

	return S_OK;
}

CItemStat * CItemStat::Create(LPDIRECT3DDEVICE9 pGraphicDev, _tchar* pObjTag, const _vec3* pPos)
{
	CItemStat*	pInstance = new CItemStat(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pObjTag, pPos)))
		Safe_Release(pInstance);

	return pInstance;
}