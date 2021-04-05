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

/* 0405_진원_몬스터 GameObject 추가 */
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
	// Debug용
}
//0406_지형로드 함수
HRESULT CStage::Load_Terrain(LPDIRECT3DDEVICE9& rpGraphicDev, CLayer*&	pLayer, const _tchar * pPath)
{
	HANDLE hFile = CreateFile(pPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (hFile == INVALID_HANDLE_VALUE)
		return E_FAIL;

	_ulong dwByte;
	TERRAININFO* pTempTile = nullptr;
	vector<TERRAININFO*> vecTempTile;

	//파일 읽기
	while (true)
	{
		pTempTile = new TERRAININFO;
		ZeroMemory(pTempTile, sizeof(TERRAININFO));

		ReadFile(hFile, pTempTile, sizeof(TERRAININFO), &dwByte, nullptr);
		if (dwByte == 0)
		{
			Safe_Delete(pTempTile);
			break;
		}
		vecTempTile.emplace_back(pTempTile);
	}
	CloseHandle(hFile);
	if (vecTempTile.empty())
	{
		ERR_MSG(L"Load Failed");
		return E_FAIL;
	}

	//기존 Info 벡터 비우기
	for (auto& pInfo : m_VecTerrainInfo)
		Safe_Delete(pInfo);
	m_VecTerrainInfo.clear();
	m_VecTerrainInfo.shrink_to_fit();
	//로드한 Info벡터 추가
	m_VecTerrainInfo = vecTempTile;

	//기존 terrain 지우기
	if (!m_VecTerrain.empty())
	{
		for (_uint i = 0; i < m_VecTerrain.size(); i++)
			Safe_Release(m_VecTerrain[i]);
		m_VecTerrain.clear();
		m_VecTerrain.shrink_to_fit();
	}
	m_VecTerrain.reserve(m_VecTerrainInfo.size());


	//로드한 INFO에 따라 CTerrain 생성후 벡터에 저장
	for (_uint i = 0; i < m_VecTerrainInfo.size(); i++)
	{
		//지형 객체 생성
		CGameObject*		pObject = CTerrain::Create(rpGraphicDev);
		NULL_CHECK_RETURN(pObject, E_FAIL);

		CTerrain*				pTerrain = dynamic_cast<CTerrain*>(pObject);

		// 가진 COM들의 데이터 변경
		//트랜스폼

		pTerrain->Set_Index(m_VecTerrainInfo[i]->dwIndex);
		pTerrain->Set_Option(m_VecTerrainInfo[i]->byOption);
		pTerrain->Set_Type(m_VecTerrainInfo[i]->byType);

		Engine::CComponent*		pComponent = nullptr;
		CTransform* pTransform = Engine::CTransform::Create();
		pComponent = pTransform;
		NULL_CHECK_RETURN(pComponent, E_FAIL);

		pTransform->Set_Pos(&(m_VecTerrainInfo[i]->vPos));
		pTransform->Set_Scale(&(m_VecTerrainInfo[i]->vSize));
		pTransform->Set_Angle(&(m_VecTerrainInfo[i]->vRotation));

		//CTerrain의 멤버 변수인 컴포넌트들을 여기서 업데이트 해도 object가 가진 맵컨테이너에 들은 컴포넌트들은 안바뀜.
		//그래서 Cterrain의 컴포넌트를 기존의 컴포넌트를 지우고 맵컨테이너에 다시 넣음
		pTerrain->Set_Component(COM_DYNAMIC, L"Com_Transform", pTransform);

		//terraintex
		_tchar szBuf[MAX_PATH] = L"Buffer_TerrainTex";
		_tchar szVtxCNX[MAX_PATH] = L"";
		_tchar szVtxCNZ[MAX_PATH] = L"";
		_tchar szVtxItv[MAX_PATH] = L"";

		_itow_s(_int(m_VecTerrainInfo[i]->dwVtxCNX), szVtxCNX, 10);
		_itow_s(_int(m_VecTerrainInfo[i]->dwVtxCNZ), szVtxCNX, 10);
		_itow_s(_int(m_VecTerrainInfo[i]->dwVtxItv), szVtxItv, 10);

		lstrcat(szBuf, L"_");
		lstrcat(szBuf, szVtxCNX);
		lstrcat(szBuf, L"_");
		lstrcat(szBuf, szVtxCNZ);
		lstrcat(szBuf, L"_");
		lstrcat(szBuf, szVtxItv);

		FAILED_CHECK_RETURN(Engine::ADD_Buffer(rpGraphicDev, RESOURCE_STATIC, szBuf, Engine::BUFFER_TERRAINTEX
			, m_VecTerrainInfo[i]->dwVtxCNX + 1, m_VecTerrainInfo[i]->dwVtxCNZ + 1, m_VecTerrainInfo[i]->dwVtxItv), E_FAIL);

		//리소스 클론 새로 생성
		CResources* pResource = Engine::Clone(RESOURCE_STATIC, szBuf);
		NULL_CHECK_RETURN(pResource, E_FAIL);

		pTerrain->Set_Resource(COM_STATIC, L"Com_Buffer", pResource);

		NULL_CHECK_RETURN(pObject, E_FAIL);

		m_VecTerrain.push_back(pObject);

		//환경 레이어에 추가
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(ENVIR_TERRAIN, pObject), E_FAIL);
	}

	return S_OK;
}


HRESULT CStage::Ready_Resource(LPDIRECT3DDEVICE9& pGraphicDev)
{
	FAILED_CHECK_RETURN(Engine::Ready_Buffers(pGraphicDev, RESOURCE_STATIC, L"Buffer_TerrainTex_Default", Engine::BUFFER_TERRAINTEX, VTXCNTX, VTXCNTZ, VTXITV), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Buffers(pGraphicDev, RESOURCE_STATIC, L"Buffer_CubeTex", Engine::BUFFER_CUBETEX), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Buffers(pGraphicDev, RESOURCE_STATIC, L"Buffer_TetraTex", Engine::BUFFER_TETRATEX), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Texture_Cube", Engine::TEX_CUBE, L"../../Resource/Test/Texture/SkyBox/burger%d.dds", 4), E_FAIL);

	//Terrain 이미지
	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Texture_Terrain", Engine::TEX_NORMAL, L"../../Resource/Test/Texture/Terrain/Grass_%d.tga", 2), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Texture_Terrain_Floor", Engine::TEX_NORMAL, L"../../Resource/Texture/Terrain/Floor/Floor_%d.tga", 2), E_FAIL);

	
	//플레이어 이미지
	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Texture_Player", Engine::TEX_CUBE, L"../../Resource/Test/Texture/SkyBox/burger%d.dds", 4), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Texture_PlayerGun", Engine::TEX_NORMAL, L"../../Resource/Texture/Player/Gun/%d.png", 2), E_FAIL);

	//총알 이미지
	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Texture_BasicBullet", Engine::TEX_NORMAL, L"../../Resource/Texture/Player/Bullet/Basic_Bullet%d.png", 2), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Texture_BigBullet", Engine::TEX_NORMAL, L"../../Resource/Texture/Player/Bullet/Big_Bullet%d.png", 2), E_FAIL); 
	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Texture_Ray", Engine::TEX_NORMAL, L"../../Resource/Texture/Player/Bullet/Ray%d.png", 5), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Texture_IceBullet", Engine::TEX_NORMAL, L"../../Resource/Texture/Player/Bullet/Ice_Bullet%d.png", 4), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Texture_FireBullet", Engine::TEX_NORMAL, L"../../Resource/Texture/Player/Bullet/Fire_Bullet%d.png", 4), E_FAIL);

	//몬스터 이미지
	/* 0405_진원_몬스터 이미지 추가 */
	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Texture_MiniOctopus", Engine::TEX_NORMAL, L"../../Resource/Texture/Monster/MiniOctopus/png/%d.png", 2), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Texture_Cannon", Engine::TEX_NORMAL, L"../../Resource/Texture/Monster/Cannon/png/%d.png", 2), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Texture_MonsterBullet", Engine::TEX_NORMAL, L"../../Resource/Texture/Monster/Bullet/png/%d.png", 2), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Texture_OctopusBoss", Engine::TEX_NORMAL, L"../../Resource/Texture/Monster/OctopusBoss/png/%d.png", 4), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Texture_WalkCannon", Engine::TEX_NORMAL, L"../../Resource/Texture/Monster/WalkCannon/png/%d.png", 4), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Texture_Spider", Engine::TEX_NORMAL, L"../../Resource/Texture/Monster/Spider/png/%d.png", 6), E_FAIL);

	//궁극기 이미지
	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Texture_Meteo", Engine::TEX_NORMAL, L"../../Resource/Texture/Player/Ultimate/Meteo%d.png", 6), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Textures(pGraphicDev, RESOURCE_STAGE, L"Texture_Thunder", Engine::TEX_NORMAL, L"../../Resource/Texture/Player/Ultimate/Thunder/%d.png", 5), E_FAIL);

	/* 0405_다영 이미지 리소스 변경 및 추가 */
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

	//pGameObject = CTerrain::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(Load_Terrain(m_pGraphicDev, pLayer, L"../../Data/Terrain_Stage0_Test.dat"), E_FAIL);

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

	/* 0405_진원_일반 몬스터 추가 */
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

	/* 0405_진원_보스 몬스터 추가 */
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

	/* 0405_다영 ItemStat(Gem, Disk) 추가 */
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

	for (_uint i = 0; i < m_VecTerrain.size(); i++)
	{
		Safe_Release(m_VecTerrain[i]);
	}
	m_VecTerrain.clear();
	m_VecTerrain.shrink_to_fit();

	for (_uint i = 0; i < m_VecTerrainInfo.size(); i++)
	{
		Safe_Delete(m_VecTerrainInfo[i]);
	}
	m_VecTerrainInfo.clear();
	m_VecTerrainInfo.shrink_to_fit();

}