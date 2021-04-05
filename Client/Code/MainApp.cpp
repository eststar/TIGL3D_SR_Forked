#include "stdafx.h"
#include "MainApp.h"

#include "Logo.h"

#include "Export_Function.h"

CMainApp::CMainApp()
{

}

CMainApp::~CMainApp()
{

}

HRESULT CMainApp::Ready_MainApp()
{
	FAILED_CHECK_RETURN(SetUp_Setting(&m_pGraphicDev), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Logo(m_pGraphicDev, &m_pManagementClass), E_FAIL);

	ShowCursor(FALSE);
	//m_pGraphicDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//m_pGraphicDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);


	return S_OK;
}

_int CMainApp::Update_MainApp(const _float& fTimeDelta)
{
	if (nullptr == m_pManagementClass)
		return 0;

	Engine::Update_InputDev();

	m_pManagementClass->Update_Scene(fTimeDelta);

	return 0;
}

_int CMainApp::LateUpdate_MainApp(const _float & fTimeDelta)
{
	//디버깅 확인용 조건부컴파일러
#ifdef _DEBUG
	if (Engine::Get_DIKeyState(DIK_Z) & 0x80)
	{
		int i = 0;
	}
#endif

	m_pManagementClass->LateUpdate_Scene(fTimeDelta);
	return 0;
}

void CMainApp::Render_MainApp()
{
	Engine::Render_Begin(D3DXCOLOR(0.4f, 0.6f, 0.9f, 1.f));

	if (nullptr != m_pManagementClass)
		m_pManagementClass->Render_Scene(m_pGraphicDev);

	Engine::Render_End();
}

HRESULT CMainApp::SetUp_Setting(LPDIRECT3DDEVICE9 * ppGraphicDev)
{
	FAILED_CHECK_RETURN(Engine::Ready_GraphicDev(g_hWnd, Engine::MODE_WIN, WINCX, WINCY, &m_pDeviceClass), E_FAIL);

	m_pDeviceClass->AddRef();

	*ppGraphicDev = m_pDeviceClass->Get_GraphicDev();
	(*ppGraphicDev)->AddRef();

	// Font
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, Engine::FONT_DEFAULT, L"돋움", 15, 20, FW_HEAVY), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, Engine::FONT_ACCENT, L"궁서", 30, 60, FW_BOLD), E_FAIL);

	// InputDev
	FAILED_CHECK_RETURN(Engine::Ready_InputDev(g_hInst, g_hWnd), E_FAIL);

	return S_OK;
}

HRESULT CMainApp::Ready_Logo(LPDIRECT3DDEVICE9 & pGraphicDev, Engine::CManagement ** ppManagement)
{
	Engine::CScene*		pScene = nullptr;

	if (FAILED(Engine::Ready_Management(ppManagement)))
		return E_FAIL;

	(*ppManagement)->AddRef();

	pScene = CLogo::Create(pGraphicDev);
	if (nullptr == pScene)
		return E_FAIL;
	
	if (FAILED(Engine::SetUp_Scene(pScene)))
		return E_FAIL;

	return S_OK;
}

CMainApp* CMainApp::Create()
{
	CMainApp*	pInstance = new CMainApp;

	if (FAILED(pInstance->Ready_MainApp()))
	{
		MSG_BOX("MainApp Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{
	Safe_Release(m_pGraphicDev);
	Safe_Release(m_pDeviceClass);


	Safe_Release(m_pManagementClass);

	Engine::Release_Utility();
	Engine::Release_Resources();
	Engine::Release_System();
}

