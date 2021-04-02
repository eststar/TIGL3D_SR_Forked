#include "stdafx.h"
#include "Stage.h"

#include "Export_Function.h"

/* Layer */
#include "EnvirLayer.h"
#include "LogicLayer.h"
#include "UILayer.h"

/* GameObject */
#include "SkyCube.h"
#include "BackGround.h"
#include "Terrain.h"
#include "StaticCamera.h"
#include "Player.h"
#include "Monster.h"
#include "Bullet.h"
#include "TestPyramid.h"


USING(Engine)

CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{

}

CStage::~CStage()
{

}

HRESULT CStage::Ready_Scene()
{
	if (FAILED(Engine::CScene::Ready_Scene()))
		return E_FAIL;

	if (FAILED(Ready_Resource(m_pGraphicDev)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Enviroment(LAYER_ENVIR)))
		return E_FAIL;

	if (FAILED(Ready_Layer_GameLogic(LAYER_LOGIC)))
		return E_FAIL;

	//if (FAILED(Ready_Layer_UI(LAYER_UI)))
	//	return E_FAIL;

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

Engine::_int CStage::Update_Scene(const _float& fTimeDelta)
{
	_int iExit = Engine::CScene::Update_Scene(fTimeDelta);

	return iExit;
}

void CStage::LateUpdate_Scene(const _float & fTimeDelta)
{
	Engine::CScene::LateUpdate_Scene(fTimeDelta);
}

void CStage::Render_Scene()
{
	// Debug용
}

HRESULT CStage::Ready_Resource(LPDIRECT3DDEVICE9& pGraphicDev)
{
	FAILED_CHECK_RETURN(Engine::Ready_Buffers(pGraphicDev, RESOURCE_STATIC, L"Buffer_TerrainTex", Engine::BUFFER_TERRAINTEX, VTXCNTX, VTXCNTZ, VTXITV), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Buffers(pGraphicDev, RESOURCE_STATIC, L"Buffer_CubeTex", Engine::BUFFER_CUBETEX), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Buffers(pGraphicDev, RESOURCE_STATIC, L"Buffer_TetraTex", Engine::BUFFER_TETRATEX), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Texture_Cube", Engine::TEX_CUBE, L"../../Resource/Test/Texture/SkyBox/burger%d.dds", 4), E_FAIL);

	//Terrain 이미지
	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Texture_Terrain", Engine::TEX_NORMAL, L"../../Resource/Test/Texture/Terrain/Grass_%d.tga", 2), E_FAIL);
	
	//플레이어 이미지
	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Texture_Player", Engine::TEX_CUBE, L"../../Resource/Test/Texture/SkyBox/burger%d.dds", 4), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Texture_PlayerGun", Engine::TEX_NORMAL, L"../../Resource/Texture/Player/Gun/%d.png", 2), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Texture_BasicBullet", Engine::TEX_NORMAL, L"../../Resource/Texture/Player/Bullet/Basic_Bullet%d.png", 2), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Texture_BigBullet", Engine::TEX_NORMAL, L"../../Resource/Texture/Player/Bullet/Big_Bullet%d.png", 2), E_FAIL); 
	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Texture_Ray", Engine::TEX_NORMAL, L"../../Resource/Texture/Player/Bullet/Ray.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Texture_IceBullet", Engine::TEX_NORMAL, L"../../Resource/Texture/Player/Bullet/Ice_Bullet%d.png", 4), E_FAIL);
	
	//몬스터 이미지
	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Texture_Monster", Engine::TEX_NORMAL, L"../../Resource/Texture/Monster/MiniOctopus/png/%d.png", 2), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Texture_MonsterBullet", Engine::TEX_NORMAL, L"../../Resource/Texture/Monster/Bullet/png/%d.png", 2), E_FAIL);

	return S_OK;
}

HRESULT CStage::Ready_Layer_Enviroment(LAYERID eLayerID)
{
	Engine::CLayer*		pLayer = CEnvirLayer::Create(ENVIR_END);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(ENVIR_TERRAIN, pGameObject), E_FAIL);

	pGameObject = CSkyCube::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(ENVIR_SKYBOX, pGameObject), E_FAIL);
	
	m_mapLayer.emplace(eLayerID, pLayer);

	return S_OK;
}

HRESULT CStage::Ready_Layer_GameLogic(LAYERID eLayerID)
{
	Engine::CLayer*		pLayer = CLogicLayer::Create(LOGIC_END);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(LOGIC_PLAYER, pGameObject), E_FAIL);

	pGameObject = CMonster::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(LOGIC_MONSTER, pGameObject), E_FAIL);

	pGameObject = CTestPyramid::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(LOGIC_OBJECT, pGameObject), E_FAIL);

	pGameObject = CStaticCamera::Create(m_pGraphicDev, &_vec3(0.f, 0.f, 0.f),
										&_vec3(0.f, 0.f, 1.f), &_vec3(0.f, 1.f, 0.f),
										D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);

	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(LOGIC_CAMERA, pGameObject), E_FAIL);

	m_mapLayer.emplace(eLayerID, pLayer);

	return S_OK;
}

HRESULT CStage::Ready_Layer_UI(LAYERID eLayerID)
{
	Engine::CLayer*		pLayer = CUILayer::Create(UI_END);
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	
	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CGun::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(UI_PLAYER, pGameObject), E_FAIL);
	
	m_mapLayer.emplace(eLayerID, pLayer);

	return S_OK;
}

CStage* CStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage*	pInstance = new CStage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}

void CStage::Free()
{
	CScene::Free();
}