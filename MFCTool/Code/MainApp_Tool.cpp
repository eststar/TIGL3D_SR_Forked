#include "stdafx.h"
#include "MainApp_Tool.h"

#include "Export_Function.h"

#include "Terrain.h"
#include "DynamicCamera.h"

USING(Engine)
IMPLEMENT_SINGLETON(CMainApp_Tool)

CMainApp_Tool::CMainApp_Tool(void)
{

}

CMainApp_Tool::~CMainApp_Tool(void)
{
	Free();
}

HRESULT CMainApp_Tool::Ready_MainApp_Tool(void)
{
	
	FAILED_CHECK_RETURN(SetUp_Setting(&m_pGraphicDev), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Resource(/*m_pGraphicDev*/), E_FAIL);

	//FAILED_CHECK_RETURN(Ready_Buffer(RESOURCE_STATIC, L"Buffer_TerrainTex_1_1_1", Engine::BUFFER_TERRAINTEX, 1+1, 1+1, 1), E_FAIL);
	//FAILED_CHECK_RETURN(Ready_Texture(RESOURCE_STAGE, L"Texture_Grass", Engine::TEX_NORMAL, L"../../Client/Bin/Resource/Test/Texture/Terrain/Grass_%d.tga", 2), E_FAIL);

	m_pDynamicCamera = CDynamicCamera::Create(m_pGraphicDev, &_vec3(10.f, 10.f, -1.f),
		&_vec3(0.f, 0.f, 1.f), &_vec3(0.f, 1.f, 0.f),
		D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	NULL_CHECK_RETURN(m_pDynamicCamera, E_FAIL);

	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);


	return S_OK;
}

_int CMainApp_Tool::Update_MainApp_Tool(const _float& fTimeDelta)
{
	Engine::Update_InputDev();

	m_pDynamicCamera->Update_Object(fTimeDelta);
	if (m_pvecTerrain != nullptr)
	{
		for (_uint i = 0; i < m_pvecTerrain->size(); i++)
		{
			(*m_pvecTerrain)[i]->Update_Object(fTimeDelta);
		}
	}

	return 0;
}

_int CMainApp_Tool::Late_Update_MainApp_Tool(const _float & fTimeDelta)
{
	m_pDynamicCamera->LateUpdate_Object(fTimeDelta);

	return 0;
}

void CMainApp_Tool::Render_MainApp_Tool(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	//동규_0330 렌더 대상 변경
	Engine::Render_Begin(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f));

	if (m_pvecTerrain != nullptr)
	{
		for (_uint i = 0; i < m_pvecTerrain->size(); i++)
		{
			CTerrain* pTerrain = dynamic_cast<CTerrain*>((*m_pvecTerrain)[i]);
			pTerrain->m_dwFillmode = m_dwFillmode;
			pTerrain->m_bShowTexture = m_bShowTex;
			pTerrain->Render_Object();
		}
	}

	Engine::Render_End();
}

//HRESULT CMainApp_Tool::ReadImagePath()
//{
//	wifstream fin;
//	fin.open("../Data/ImagePathInfo.txt");
//	if (fin.fail())
//	{
//		ERR_MSG(L"File Open Failed");
//		return E_FAIL;
//	}
//
//	TCHAR szPath[MAX_PATH] = L"";
//	TCHAR szObjectKey[MAX_PATH] = L"";
//	TCHAR szStateKey[MAX_PATH] = L"";
//	TCHAR szCount[MAX_PATH] = L"";
//
//	while (true)
//	{
//		fin.getline(szPath, MAX_PATH, '|');
//		fin.getline(szObjectKey, MAX_PATH, '|');
//		fin.getline(szStateKey, MAX_PATH, '|');
//		fin.getline(szCount, MAX_PATH);
//		if (fin.eof())
//			break;
//		int iCount = _ttoi(szCount);
//		//Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGE, szPath,, szObjectKey, szStateKey, iCount)
//		//if (FAILED(Insert_Texture(TEX_MULTI, szPath, szObjectKey, szStateKey, iCount)))
//		//{
//		//	return E_FAIL;
//		//}
//	}
//	fin.close();
//	return S_OK;
//}

HRESULT CMainApp_Tool::SetUp_Setting(LPDIRECT3DDEVICE9 * ppGraphicDev)
{
	FAILED_CHECK_RETURN(Engine::Ready_GraphicDev(g_hWnd, Engine::MODE_WIN, WINCX, WINCY, &m_pDeviceClass), E_FAIL);

	m_pDeviceClass->AddRef();

	*ppGraphicDev = m_pDeviceClass->Get_GraphicDev();
	(*ppGraphicDev)->AddRef();

	// 폰트추가
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, FONT_DEFAULT, L"바탕", 15, 20, FW_HEAVY), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, FONT_ACCENT, L"궁서", 30, 30, FW_BOLD), E_FAIL);

	// inputdev
	FAILED_CHECK_RETURN(Engine::Ready_InputDev(GetModuleHandle(NULL), g_hWnd), E_FAIL);


	return S_OK;
}

//동규_0330 Reserve_ContainerSize 추가 MainApp_Tool 싱글턴으로 변경하면서 create 
HRESULT CMainApp_Tool::Ready_Resource(/*LPDIRECT3DDEVICE9& pGraphicDev*/)
{
	Engine::Reserve_ContainerSize(RESOURCE_END);

	FAILED_CHECK_RETURN(Engine::Ready_Buffers(m_pGraphicDev, RESOURCE_STATIC, L"Buffer_TerrainTex_1_1_1", Engine::BUFFER_TERRAINTEX, 1 + 1, 1 + 1, 1), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGE, L"Texture_Terrain_Floor", Engine::TEX_NORMAL, L"../../Resource/Texture/Terrain/Floor/Floor_%d.tga", 2), E_FAIL);

	return S_OK;
}



//HRESULT CMainApp_Tool::Ready_Buffer(RESOURCEID eResourceID, const _tchar* pBufferTag
//																	, Engine::BUFFERID eBufferID, DWORD dwVTXCNTX, DWORD dwVTXCNTZ, DWORD dwVTXITV)
//{
//
//	FAILED_CHECK_RETURN(Engine::Ready_Buffers(m_pGraphicDev, eResourceID, pBufferTag, eBufferID, dwVTXCNTX, dwVTXCNTZ, dwVTXITV), E_FAIL);
//	//FAILED_CHECK_RETURN(Engine::Ready_Buffers(m_pGraphicDev, RESOURCE_STATIC, L"Buffer_RcTex", Engine::BUFFER_RCTEX), E_FAIL);
//
//	return S_OK;
//}

HRESULT CMainApp_Tool::ADD_Buffer(RESOURCEID eResourceID, const _tchar* pBufferTag
	, Engine::BUFFERID eBufferID, DWORD dwVTXCNTX, DWORD dwVTXCNTZ, DWORD dwVTXITV)
{

	FAILED_CHECK_RETURN(Engine::ADD_Buffer(m_pGraphicDev, eResourceID, pBufferTag, eBufferID, dwVTXCNTX, dwVTXCNTZ, dwVTXITV), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Buffers(m_pGraphicDev, RESOURCE_STATIC, L"Buffer_RcTex", Engine::BUFFER_RCTEX), E_FAIL);

	return S_OK;
}


//HRESULT CMainApp_Tool::Ready_Texture(RESOURCEID eResourceID, const _tchar* pTextureTag
//	, Engine::TEXTUREID eTextureID, const _tchar* pPath, _uint iCnt)
//{
//	// texture
//	FAILED_CHECK_RETURN(Engine::Ready_Textures(m_pGraphicDev, eResourceID, pTextureTag, eTextureID, pPath, iCnt), E_FAIL);
//
//	return S_OK;
//}

void CMainApp_Tool::Free(void)
{

	Safe_Release(m_pGraphicDev);
	Safe_Release(m_pDeviceClass);


	//Safe_Release(m_pManagementClass);


	//동규_0330 멤버 벡터컨테이너 삭제 구문
	if (m_pvecTerrain != nullptr)
	{
		for (_uint i = 0; i < m_pvecTerrain->size(); i++)
		{
			Safe_Release((*m_pvecTerrain)[i]);
		}
		m_pvecTerrain->clear();
		m_pvecTerrain->shrink_to_fit();
		Safe_Delete(m_pvecTerrain);
		m_pvecTerrain = nullptr;
	}

	for (_uint i = 0; i < m_vecTile.size(); i++)
	{
		Safe_Delete(m_vecTile[i]);
	}
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();

	Safe_Release(m_pDynamicCamera);

	Engine::Release_Utility();
	Engine::Release_Resources();
	Engine::Release_System();
}

