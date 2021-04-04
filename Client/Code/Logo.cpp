#include "stdafx.h"
#include "Logo.h"
#include "Stage.h"

#include "EnvirLayer.h"
#include "UILayer.h"

#include "BackGround.h"

#include "Export_Function.h"

USING(Engine)

CLogo::CLogo(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
}

CLogo::~CLogo()
{
}

HRESULT CLogo::Ready_Scene()
{
	if (FAILED(Engine::CScene::Ready_Scene()))
		return E_FAIL;

	if (FAILED(Ready_Resource(m_pGraphicDev, RESOURCE_END)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Enviroment(LAYER_ENVIR)))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(LAYER_UI)))
		return E_FAIL;

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

Engine::_int CLogo::Update_Scene(const _float& fTimeDelta)
{
	_int iExit = Engine::CScene::Update_Scene(fTimeDelta);

	if (true /*== �ε� �Ϸ��*/)
	{
		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		{
			//Engine::Release_Resources();
			CScene*		pScene = nullptr;

			pScene = CStage::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pScene, E_FAIL);
			FAILED_CHECK_RETURN(SetUp_Scene(pScene), E_FAIL);
			return iExit;
		}
	}

	return iExit;
}

void CLogo::LateUpdate_Scene(const _float & fTimeDelta)
{
	Engine::CScene::LateUpdate_Scene(fTimeDelta);
}

void CLogo::Render_Scene()
{
	// Debug��
}

HRESULT CLogo::Ready_Resource(LPDIRECT3DDEVICE9& pGraphicDev, RESOURCEID eMax)
{
	Engine::Reserve_ContainerSize(eMax);

	FAILED_CHECK_RETURN(Engine::Ready_Buffers(pGraphicDev, RESOURCE_STATIC, L"Buffer_TriCol", Engine::BUFFER_TRICOL), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Buffers(pGraphicDev, RESOURCE_STATIC, L"Buffer_RcCol", Engine::BUFFER_RCCOL), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Buffers(pGraphicDev, RESOURCE_STATIC, L"Buffer_RcTex", Engine::BUFFER_RCTEX), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_LOGO, L"Texture_Logo", Engine::TEX_NORMAL, L"../../Resource/Test/Texture/Logo/sana.jpg"), E_FAIL);

	return S_OK;
}

HRESULT CLogo::Ready_Layer_Enviroment(LAYERID eLayerID)
{
	//���̾� �������� vector�� ����� �Բ� �Ѱ��ֱ� -> ���̾�ID_END�� ��ŭ ���� resize
	Engine::CLayer*		pLayer = CEnvirLayer::Create(ENVIR_END);
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	
	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CBackGround::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	
	//Tag ���ڿ� ��� ENVIR_�׸� enum������ �ش� ��ü�� ��ü�� �߰�
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(ENVIR_BACKGROUND, pGameObject), E_FAIL);


	m_mapLayer.emplace(eLayerID, pLayer);

	return S_OK;
	
	// : Logo�� ��������� map<LAYERID, Layer* >���ٰ� LAYER_ENVIR Ű���� pLayer�� �־��ִµ�
	//   pLayer�� ��������� vec<list<GameObj*>> �� ���µ� vec[ENVIR_BACKGROUND]��° ����(list)�� pBackGround�� �־��ְڴٴ� ����
}

HRESULT CLogo::Ready_Layer_UI(LAYERID eLayerID)
{
	Engine::CLayer*		pLayer = CUILayer::Create(UI_END);
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	
	Engine::CGameObject*		pGameObject = nullptr;
	
	m_mapLayer.emplace(eLayerID, pLayer);

	return S_OK;
}

CLogo* CLogo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLogo*	pInstance = new CLogo(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}

void CLogo::Free()
{
	CScene::Free();
}

