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
#include "Bullet.h"
#include "TestPyramid.h"

#include "HPBar.h"
#include "ItemStat.h"

/* 0405_����_���� GameObject �߰� */
#include "MiniOctopus.h"
#include "Cannon.h"
#include "WalkCannon.h"
#include "Spider.h"
#include "OctopusBoss.h"
#include "Aim.h"

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

	if (FAILED(Ready_Layer_UI(LAYER_UI)))
		return E_FAIL;

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
	// Debug��
}

HRESULT CStage::Ready_Resource(LPDIRECT3DDEVICE9& pGraphicDev)
{
	FAILED_CHECK_RETURN(Engine::Ready_Buffers(pGraphicDev, RESOURCE_STATIC, L"Buffer_TerrainTex", Engine::BUFFER_TERRAINTEX, VTXCNTX, VTXCNTZ, VTXITV), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Buffers(pGraphicDev, RESOURCE_STATIC, L"Buffer_CubeTex", Engine::BUFFER_CUBETEX), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Buffers(pGraphicDev, RESOURCE_STATIC, L"Buffer_TetraTex", Engine::BUFFER_TETRATEX), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Texture_Cube", Engine::TEX_CUBE, L"../../Resource/Test/Texture/SkyBox/burger%d.dds", 4), E_FAIL);

	//Terrain �̹���
	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Texture_Terrain", Engine::TEX_NORMAL, L"../../Resource/Test/Texture/Terrain/Grass_%d.tga", 2), E_FAIL);
	
	//�÷��̾� �̹���
	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Texture_Player", Engine::TEX_CUBE, L"../../Resource/Test/Texture/SkyBox/burger%d.dds", 4), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Texture_PlayerGun", Engine::TEX_NORMAL, L"../../Resource/Texture/Player/Gun/%d.png", 2), E_FAIL);

	//�Ѿ� �̹���
	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Texture_BasicBullet", Engine::TEX_NORMAL, L"../../Resource/Texture/Player/Bullet/Basic_Bullet%d.png", 2), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Texture_BigBullet", Engine::TEX_NORMAL, L"../../Resource/Texture/Player/Bullet/Big_Bullet%d.png", 2), E_FAIL); 
	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Texture_Ray", Engine::TEX_NORMAL, L"../../Resource/Texture/Player/Bullet/Ray%d.png", 5), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Texture_IceBullet", Engine::TEX_NORMAL, L"../../Resource/Texture/Player/Bullet/Ice_Bullet%d.png", 4), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Texture_FireBullet", Engine::TEX_NORMAL, L"../../Resource/Texture/Player/Bullet/Fire_Bullet%d.png", 4), E_FAIL);

	//���� �̹���
	/* 0405_����_���� �̹��� �߰� */
	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Texture_MiniOctopus", Engine::TEX_NORMAL, L"../../Resource/Texture/Monster/MiniOctopus/png/%d.png", 2), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Texture_Cannon", Engine::TEX_NORMAL, L"../../Resource/Texture/Monster/Cannon/png/%d.png", 2), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Texture_MonsterBullet", Engine::TEX_NORMAL, L"../../Resource/Texture/Monster/Bullet/png/%d.png", 2), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Texture_OctopusBoss", Engine::TEX_NORMAL, L"../../Resource/Texture/Monster/OctopusBoss/png/%d.png", 4), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Texture_WalkCannon", Engine::TEX_NORMAL, L"../../Resource/Texture/Monster/WalkCannon/png/%d.png", 4), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Texture_Spider", Engine::TEX_NORMAL, L"../../Resource/Texture/Monster/Spider/png/%d.png", 6), E_FAIL);

	//�ñر� �̹���
	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Texture_Meteo", Engine::TEX_NORMAL, L"../../Resource/Texture/Player/Ultimate/Meteo%d.png", 6), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Texture_Thunder", Engine::TEX_NORMAL, L"../../Resource/Texture/Player/Ultimate/Thunder/%d.png", 5), E_FAIL);

	/* 0405_�ٿ� �̹��� ���ҽ� ���� �� �߰� */
	//Item
	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Heart", Engine::TEX_NORMAL, L"../../Resource/Texture/Item/Heart/%d.png", 5), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Disk_S", Engine::TEX_NORMAL, L"../../Resource/Texture/Item/Disk_S/%d.png", 4), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Gem_S", Engine::TEX_NORMAL, L"../../Resource/Texture/Item/Gem_S/%d.png", 6), E_FAIL);

	//UI
	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Number", Engine::TEX_NORMAL, L"../../Resource/Texture/UI/Number/%d.png", 11), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Aim", Engine::TEX_NORMAL, L"../../Resource/Texture/UI/Aim/%d.png", 4), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Weapon", Engine::TEX_NORMAL, L"../../Resource/Texture/UI/Weapon/%d.png", 7), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"WeaponBar", Engine::TEX_NORMAL, L"../../Resource/Texture/UI/WeaponBar/%d.png", 2), E_FAIL);
	
	//Effect
	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Explosion", Engine::TEX_NORMAL, L"../../Resource/Texture/UI/Effect/Explosion/%d.png", 5), E_FAIL);

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

	/* 0405_����_�Ϲ� ���� �߰� */
	for (int i = 0; i < 5; ++i) {
		pGameObject = CMiniOctopus::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(LOGIC_MONSTER, pGameObject), E_FAIL);

		pGameObject = CCannon::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(LOGIC_MONSTER, pGameObject), E_FAIL);

		pGameObject = CWalkCannon::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(LOGIC_MONSTER, pGameObject), E_FAIL);

		pGameObject = CSpider::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(LOGIC_MONSTER, pGameObject), E_FAIL);
	}

	/* 0405_����_���� ���� �߰� */
	pGameObject = COctopusBoss::Create(m_pGraphicDev);
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

	pGameObject = CHPBar::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(UI_PLAYER, pGameObject), E_FAIL);

	/* 0405_�ٿ� ItemStat(Gem, Disk) �߰� */
	pGameObject = CItemStat::Create(m_pGraphicDev, L"Gem_S", &_vec3(0.65f, 0.85f, 0.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(UI_PLAYER, pGameObject), E_FAIL);

	pGameObject = CItemStat::Create(m_pGraphicDev, L"Disk_S", &_vec3(0.65f, 0.7f, 0.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(UI_PLAYER, pGameObject), E_FAIL);

	pGameObject = CAim::Create(m_pGraphicDev);
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