// MapTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "MapTool.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "MFCToolView.h"
#include "Export_Function.h"

#include "FileInfo.h"
#include "MainApp_Tool.h"
#include "Terrain.h"
// CMapTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MAPTOOL, pParent)
	, m_fX(0)
	, m_fY(0)
	, m_fZ(0)
	, m_dwPreOption1(1)
	, m_dwPreOption2(1)
	, m_dwPreOption3(1)
	, m_iComboSelIndex(0)
	, m_strPreOption1(_T(""))
	, m_strPreOption2(_T(""))
	, m_strPreOption3(_T(""))
	, m_pAddName(_T(""))
{
	m_hTerrainItem = NULL;
	m_hLastItem = NULL;
	m_hParentItem = NULL;
	m_hSelItem = NULL;
}

CMapTool::~CMapTool()
{
}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cbComponent);
	DDX_Control(pDX, IDC_LIST1, m_ListBox_Texture);
	DDX_Control(pDX, IDC_TREE_CONTROL1, m_TreeCtrl_OBJ);
	DDX_Text(pDX, IDC_EDIT2, m_fX);
	DDX_Text(pDX, IDC_EDIT3, m_fY);
	DDX_Text(pDX, IDC_EDIT4, m_fZ);
	DDX_Control(pDX, IDC_CHECK_SHOWTEX, m_chkShowTex);
	DDX_Text(pDX, IDC_EDIT1, m_dwPreOption1);
	DDV_MinMaxUInt(pDX, m_dwPreOption1, 1, 1024);
	DDX_Text(pDX, IDC_EDIT5, m_dwPreOption2);
	DDV_MinMaxUInt(pDX, m_dwPreOption2, 1, 1024);
	DDX_Text(pDX, IDC_EDIT6, m_dwPreOption3);
	DDV_MinMaxUInt(pDX, m_dwPreOption3, 1, 1024);
	DDX_CBIndex(pDX, IDC_COMBO1, m_iComboSelIndex);
	DDV_MinMaxInt(pDX, m_iComboSelIndex, 0, 1024);
	DDX_Text(pDX, IDC_TEXT1, m_strPreOption1);
	DDX_Text(pDX, IDC_TEXT2, m_strPreOption2);
	DDX_Text(pDX, IDC_TEXT3, m_strPreOption3);
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CMapTool::OnCbnSelchangeComboComponent)
	ON_BN_CLICKED(IDC_BUTTON1, &CMapTool::OnBnClickedButtonADD)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_CONTROL1, &CMapTool::OnTvnSelchangedTreeControl1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMapTool::OnBnClickedButtonUpdateTile)
	ON_BN_CLICKED(IDC_CHECK_SHOWTEX, &CMapTool::OnBnClickedCheckShowtex)
	ON_BN_CLICKED(IDC_BUTTON4, &CMapTool::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON5, &CMapTool::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON6, &CMapTool::OnBnClickedButtonDelete)
END_MESSAGE_MAP()


// CMapTool 메시지 처리기입니다.

//다이얼로그 초기화
BOOL CMapTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_cbComponent.ResetContent();
	m_cbComponent.InsertString(0, _T("Terrain"));
	m_cbComponent.InsertString(1, _T("Wall"));

	m_ListBox_Texture.ResetContent();
	m_TreeCtrl_OBJ.DeleteAllItems();
	m_hTerrainItem = m_TreeCtrl_OBJ.InsertItem(_T("Terrain"), 0, 0, TVI_ROOT, TVI_LAST);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

//콤보박스 선택으로 어떤것 추가할지 선택.
void CMapTool::OnCbnSelchangeComboComponent()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_ListBox_Texture.ResetContent();
	UpdateData(TRUE);
	m_iComboSelIndex = m_cbComponent.GetCurSel();

	m_dwPreOption1 = 1;
	m_dwPreOption2 = 1;
	m_dwPreOption3 = 1;

	if (CB_ERR != m_iComboSelIndex)
	{
		switch (m_iComboSelIndex)
		{
		case TERRAIN:
		{
			m_pAddName = L"Terrain_";
			m_strPreOption1 = L"가로타일수";
			m_strPreOption2 = L"세로타일수";
			m_strPreOption3 = L"타일간격";

			////size_t iFileCnt = CFileInfo::FileCount(L"../../Client/Bin/Resource/Texture/Terrain");
			//CFileInfo::PathExtraction(L"..\\..\\Client\\Bin\\Resource\\Texture\\Terrain", m_listPathInfo);
			break;
		}
		case WALL:
			break;
		default:
			break;
		}
		UpdateData(FALSE);

	}
}

//ADD 버튼 누를때 마다 tree컨트롤에 추가 및 메인앱툴의 멤버변수인 vector에 추가
void CMapTool::OnBnClickedButtonADD()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	switch (m_iComboSelIndex)
	{
	case TERRAIN:
	{
		//지형 및 저장데이터 클래스 생성
		CGameObject*		pObject = CTerrain::Create(CMainApp_Tool::GetInstance()->m_pGraphicDev);
		NULL_CHECK(pObject);
		CTerrain*				pTerrain = dynamic_cast<CTerrain*>(pObject);
		TERRAININFO*		pTileInfo = new TERRAININFO;
		ZeroMemory(pTileInfo, sizeof(TERRAININFO));

		//벡터에 들은 원소 갯수에 따라 번호 지정
		_tchar szBuf[MAX_PATH] = L"Buffer_TerrainTex";
		_tchar szVtxCNX[MAX_PATH] = L"";
		_tchar szVtxCNZ[MAX_PATH] = L"";
		_tchar szVtxItv[MAX_PATH] = L"";

		_itow_s((_int)m_dwPreOption1,szVtxCNX, 10);
		_itow_s((_int)m_dwPreOption2, szVtxCNZ, 10);
		_itow_s((_int)m_dwPreOption3, szVtxItv, 10);

		lstrcat(szBuf, L"_");
		lstrcat(szBuf, szVtxCNX);
		lstrcat(szBuf, L"_");
		lstrcat(szBuf, szVtxCNZ);
		lstrcat(szBuf, L"_");
		lstrcat(szBuf, szVtxItv);

		//벡터 비어있으면 새로 생성
		if ((CMainApp_Tool::GetInstance()->m_pvecTerrain) == nullptr)
			(CMainApp_Tool::GetInstance()->m_pvecTerrain) = new vector<CGameObject*>;
		_int iNumber = CMainApp_Tool::GetInstance()->m_pvecTerrain->size();

		//메인앱툴의 버퍼추가 함수로 크기 조정한 terraintex 만들어서 맵리소스에 추가해놓기
		FAILED_CHECK_RETURN(CMainApp_Tool::GetInstance()->ADD_Buffer(RESOURCE_STATIC, /*pBufferName*/szBuf
						, Engine::BUFFER_TERRAINTEX, m_dwPreOption1 + 1, m_dwPreOption2 + 1, m_dwPreOption3), );

		//terraintex 리소스 클론 생성
		CResources* pResource = Engine::Clone(RESOURCE_STATIC, /*pBufferName*/szBuf);
		NULL_CHECK(pResource);

		//지형객체의 맵 컴포넌트의 terraintex 변경
		pTerrain->Set_Resource(COM_STATIC, L"Com_Buffer", pResource);

		//트랜스폼 값 변경
		Engine::CComponent*		pComponent = nullptr;
		CTransform* pTransform = Engine::CTransform::Create();
		pComponent = pTransform;
		NULL_CHECK(pComponent);

		pTileInfo->vPos			= *(pTransform->Get_Info(Engine::INFO_POS));
		pTileInfo->vSize		= *(pTransform->Get_Scale());
		pTileInfo->vRotation	= *(pTransform->Get_Angle());

		pTerrain->Set_Component(COM_DYNAMIC, L"Com_Transform", pComponent);

		pTileInfo->dwIndex		= iNumber;
		pTileInfo->dwVtxCNX	= m_dwPreOption1;
		pTileInfo->dwVtxCNZ	= m_dwPreOption2;
		pTileInfo->dwVtxItv		= m_dwPreOption3;
		pTileInfo->byOption		= 0;


		//컨테이너에 추가
		(CMainApp_Tool::GetInstance()->m_pvecTerrain)->emplace_back(pObject);
		(CMainApp_Tool::GetInstance()->m_vecTile).emplace_back(pTileInfo);

		//몇번째인지 이름 결정
		_tchar szNumber[MAX_PATH] = L"";
		_itow_s(iNumber, szNumber, 10);
		CString strName = m_pAddName +szNumber;

		//트리에 지형 하나 추가 Terrain_번호
		HTREEITEM		hTerrain_Options, hTransform;
		hTerrain_Options = m_TreeCtrl_OBJ.InsertItem(strName.GetString(), 0, 0, m_hTerrainItem, TVI_LAST);

		_tchar	szTransform[MAX_PATH] = L"Transform";
		_tchar	szOption[MAX_PATH] = L"Option";
		_tchar	szBuffer[MAX_PATH] = L"Buffer";
		_tchar	szTexture[MAX_PATH] = L"Texture";

		_tchar	szPos[MAX_PATH] = L"Position";
		_tchar	szSize[MAX_PATH] = L"Size";
		_tchar	szRotation[MAX_PATH] = L"Rotation";

		hTransform = m_TreeCtrl_OBJ.InsertItem(szTransform, 0, 0, hTerrain_Options, TVI_LAST);
		m_TreeCtrl_OBJ.InsertItem(szPos, 0, 0, hTransform, TVI_LAST);
		m_TreeCtrl_OBJ.InsertItem(szSize, 0, 0, hTransform, TVI_LAST);
		m_TreeCtrl_OBJ.InsertItem(szRotation, 0, 0, hTransform, TVI_LAST);

		m_TreeCtrl_OBJ.InsertItem(szOption, 0, 0, hTerrain_Options, TVI_LAST);
		m_TreeCtrl_OBJ.InsertItem(szBuffer, 0, 0, hTerrain_Options, TVI_LAST);
		m_TreeCtrl_OBJ.InsertItem(szTexture, 0, 0, hTerrain_Options, TVI_LAST);

		m_hLastItem = hTerrain_Options;
		break;
	}
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	default:
		break;
	}
	UpdateData(FALSE);

}


void CMapTool::OnTvnSelchangedTreeControl1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_hSelItem = m_TreeCtrl_OBJ.GetSelectedItem();
	//트리 아이템 선택

	m_hParentItem = m_hSelItem;
	//GetParentItem은 인자의 부모가 ROOT인경우 NULL반환

	//미리 NULL초기화
	HTREEITEM hTempParent = NULL;

	//부모노드 없으면 NULL그대로 루트노드임
	hTempParent = m_TreeCtrl_OBJ.GetParentItem(m_hSelItem);
	if (hTempParent == NULL)
		return;
	hTempParent = NULL;

	while (true)
	{
		//부모노드의 부모노드 찾았을때 NULL이면 현재 노드가 루트의 하위 항목. 즉 ADD로 추가한 번호를가진 항목
		hTempParent = m_TreeCtrl_OBJ.GetParentItem(m_TreeCtrl_OBJ.GetParentItem(m_hParentItem));
		//만약에 그렇다면 해당 번호 얻기위해 루프 탈출
		if (hTempParent == NULL)
			break;
		//아니면 탐색할 부모노드를 한레벨 올림
		m_hParentItem = m_TreeCtrl_OBJ.GetParentItem(m_hParentItem);
		hTempParent = NULL;
	}
	//해당 아이템 가진 객체 이름 확인
	//이름의 번호 확인
	CString strTile = m_TreeCtrl_OBJ.GetItemText(m_hParentItem);
	_int iNameIndex = 0;
	for (; iNameIndex < strTile.GetLength(); iNameIndex++)
	{
		if (isdigit(strTile[iNameIndex]))
			break;
	}

	strTile.Delete(0, iNameIndex);
	_int iTileIndex = _ttoi(strTile.GetString());

	//아이템과 같은 이름의 컴포넌트 마다 값 변경
 	CString strName = m_TreeCtrl_OBJ.GetItemText(m_hSelItem);

	//루트노드가 아닌 하위 항목 노드를 클릭했으면 일단 사각형 갯수 및 크기는 출력
	m_dwPreOption1 = CMainApp_Tool::GetInstance()->m_vecTile[iTileIndex]->dwVtxCNX;
	m_dwPreOption2 = CMainApp_Tool::GetInstance()->m_vecTile[iTileIndex]->dwVtxCNZ;
	m_dwPreOption3 = CMainApp_Tool::GetInstance()->m_vecTile[iTileIndex]->dwVtxItv;

	//그보다 하위의 옵션 선택했으면 그것 출력
	if (strName == L"Position")
	{
		m_fX = CMainApp_Tool::GetInstance()->m_vecTile[iTileIndex]->vPos.x;
		m_fY = CMainApp_Tool::GetInstance()->m_vecTile[iTileIndex]->vPos.y;
		m_fZ = CMainApp_Tool::GetInstance()->m_vecTile[iTileIndex]->vPos.z;
	}
	else if (strName == L"Size")
	{
		m_fX = CMainApp_Tool::GetInstance()->m_vecTile[iTileIndex]->vSize.x;
		m_fY = CMainApp_Tool::GetInstance()->m_vecTile[iTileIndex]->vSize.y;
		m_fZ = CMainApp_Tool::GetInstance()->m_vecTile[iTileIndex]->vSize.z;
	}
	else if (strName == L"Rotation")
	{
		m_fX = D3DXToDegree(CMainApp_Tool::GetInstance()->m_vecTile[iTileIndex]->vRotation.x);
		m_fY = D3DXToDegree(CMainApp_Tool::GetInstance()->m_vecTile[iTileIndex]->vRotation.y);
		m_fZ = D3DXToDegree(CMainApp_Tool::GetInstance()->m_vecTile[iTileIndex]->vRotation.z);
	}
	else if (strName == L"Option")
	{
	}
	UpdateData(FALSE);

	*pResult = 0;
}


void CMapTool::OnBnClickedButtonUpdateTile()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//마지막으로 선택했던 아이템의 부모의 번호 확인
	CString strTile = m_TreeCtrl_OBJ.GetItemText(m_hParentItem);
	_int iNameIndex = 0;
	for (; iNameIndex < strTile.GetLength(); iNameIndex++)
	{
		if (isdigit(strTile[iNameIndex]))
			break;
	}

	strTile.Delete(0, iNameIndex);
	_int iTileIndex = _ttoi(strTile.GetString());

	//선택되어 있는 아이템 이름 확인
	CString strName = m_TreeCtrl_OBJ.GetItemText(m_hSelItem);

	UpdateData(TRUE);

	
	if (strName == L"Position")
	{
		CMainApp_Tool::GetInstance()->m_vecTile[iTileIndex]->vPos.x = m_fX;
		CMainApp_Tool::GetInstance()->m_vecTile[iTileIndex]->vPos.y = m_fY;
		CMainApp_Tool::GetInstance()->m_vecTile[iTileIndex]->vPos.z = m_fZ;
		
		//번호에 해당하는 벡터의 인덱스의 object 받아와서 다운캐스팅, 해당 객체의 컴포넌트 업데이트
		CTerrain* pTerrain = dynamic_cast<CTerrain*>((*(CMainApp_Tool::GetInstance()->m_pvecTerrain))[iTileIndex]);

		Engine::CComponent*		pComponent = nullptr;
		CTransform* pTransform = Engine::CTransform::Create();
		pComponent = pTransform;
		NULL_CHECK(pComponent);

		pTransform->Set_Pos(&_vec3(m_fX, m_fY, m_fZ));
		pTerrain->Set_Component(COM_DYNAMIC, L"Com_Transform", pTransform);

	}
	else if (strName == L"Size")
	{
		CMainApp_Tool::GetInstance()->m_vecTile[iTileIndex]->vSize.x = m_fX;
		CMainApp_Tool::GetInstance()->m_vecTile[iTileIndex]->vSize.y = m_fY;
		CMainApp_Tool::GetInstance()->m_vecTile[iTileIndex]->vSize.z = m_fZ;

		CTerrain* pTerrain = dynamic_cast<CTerrain*>((*(CMainApp_Tool::GetInstance()->m_pvecTerrain))[iTileIndex]);

		Engine::CComponent*		pComponent = nullptr;
		CTransform* pTransform = Engine::CTransform::Create();
		pComponent = pTransform;
		NULL_CHECK(pComponent);

		pTransform->Set_Scale(&_vec3(m_fX, m_fY, m_fZ));
		pTerrain->Set_Component(COM_DYNAMIC, L"Com_Transform", pTransform);
	}
	else if (strName == L"Rotation")
	{
		CMainApp_Tool::GetInstance()->m_vecTile[iTileIndex]->vRotation.x = D3DXToRadian(m_fX);
		CMainApp_Tool::GetInstance()->m_vecTile[iTileIndex]->vRotation.y = D3DXToRadian(m_fY);
		CMainApp_Tool::GetInstance()->m_vecTile[iTileIndex]->vRotation.z = D3DXToRadian(m_fZ);

		CTerrain* pTerrain = dynamic_cast<CTerrain*>((*(CMainApp_Tool::GetInstance()->m_pvecTerrain))[iTileIndex]);

		Engine::CComponent*		pComponent = nullptr;
		CTransform* pTransform = Engine::CTransform::Create();
		pComponent = pTransform;
		NULL_CHECK(pComponent);

		pTransform->Set_Angle(&_vec3(m_fX, m_fY, m_fZ));
		pTerrain->Set_Component(COM_DYNAMIC, L"Com_Transform", pTransform);
	}

	UpdateData(FALSE);

}

void CMapTool::OnBnClickedCheckShowtex()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_chkShowTex.GetCheck())
	{
		CMainApp_Tool::GetInstance()->m_bShowTex = true;
		CMainApp_Tool::GetInstance()->m_dwFillmode = D3DFILL_SOLID;
	}
	else
	{
		CMainApp_Tool::GetInstance()->m_bShowTex = false;
		CMainApp_Tool::GetInstance()->m_dwFillmode = D3DFILL_WIREFRAME;
	}
}


void CMapTool::OnBnClickedButtonSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(FALSE, L"dat", L"*.dat", OFN_OVERWRITEPROMPT, L"Data File(*.dat) |*.dat||", this);

	TCHAR szPath[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szPath);
	PathRemoveFileSpec(szPath);
	PathRemoveFileSpec(szPath);

	lstrcat(szPath, L"\\Data");

	Dlg.m_ofn.lpstrInitialDir = szPath;
	if (Dlg.DoModal() == IDOK)
	{
		CString strPath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(strPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (hFile == INVALID_HANDLE_VALUE)
			return;

		DWORD dwByte = 0;
		
		vector<TERRAININFO*>& vecTile = CMainApp_Tool::GetInstance()->m_vecTile;
		for (auto& pTile : vecTile)
		{
			WriteFile(hFile, pTile, sizeof(TERRAININFO), &dwByte, nullptr);
		}
		CloseHandle(hFile);
	}

}


void CMapTool::OnBnClickedButtonLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CFileDialog Dlg(TRUE, L"dat", L"*.dat", OFN_OVERWRITEPROMPT, L"Data File(*.dat) |*.dat||", this);

	TCHAR szPath[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szPath);
	PathRemoveFileSpec(szPath);
	PathRemoveFileSpec(szPath);

	lstrcat(szPath, L"\\Data");

	Dlg.m_ofn.lpstrInitialDir = szPath;
	if (Dlg.DoModal() == IDOK)
	{
		CString strPath = Dlg.GetPathName();

		HANDLE hFile = CreateFile(strPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (hFile == INVALID_HANDLE_VALUE)
			return;

		DWORD dwByte;
		TERRAININFO* pTempTile = nullptr;
		vector<TERRAININFO*> vecTempTile;

		//파일 읽기
		while (true)
		{
			pTempTile = new TERRAININFO;
			ZeroMemory(pTempTile, sizeof(TERRAININFO));

			ReadFile(hFile, pTempTile, sizeof(TERRAININFO), &dwByte, nullptr);
			if (dwByte ==0)
			{
				Safe_Delete(pTempTile);
				break;
			}
			vecTempTile.emplace_back(pTempTile);
		}
		CloseHandle(hFile);
		if (vecTempTile.empty())
		{
			ERR_MSG(_T("Load Failed"));
			return;
		}
		//트리 비우기
		m_TreeCtrl_OBJ.DeleteAllItems();
		//일단 terrain만 로드 할거니까 트리에 terrain 루트에 
		m_hTerrainItem = m_TreeCtrl_OBJ.InsertItem(_T("Terrain"), 0, 0, TVI_ROOT, TVI_LAST);
		m_cbComponent.SetCurSel(m_iComboSelIndex);
		m_iComboSelIndex = 0;

		m_pAddName = L"Terrain_";
		m_strPreOption1 = L"가로타일수";
		m_strPreOption2 = L"세로타일수";
		m_strPreOption3 = L"타일간격";

		//기존 타일 벡터 비우기
		vector<TERRAININFO*>* vecTile = &(CMainApp_Tool::GetInstance()->m_vecTile);
		for (auto& pTile : *vecTile)
			Safe_Delete(pTile);
		vecTile->clear();
		vecTile->shrink_to_fit();
		//로드한 타일벡터 추가
		*vecTile = vecTempTile;

		//기존 terrain 지우기
		vector<CGameObject*>* pvecTerrain = CMainApp_Tool::GetInstance()->m_pvecTerrain;
		if (pvecTerrain!= nullptr)
		{
			for (_uint i = 0; i < pvecTerrain->size(); i++)
				Safe_Release((*pvecTerrain)[i]);
			pvecTerrain->clear();
			pvecTerrain->shrink_to_fit();
		}
		else
		{
			pvecTerrain = new vector<CGameObject*>;
			CMainApp_Tool::GetInstance()->m_pvecTerrain = pvecTerrain;
		}
		pvecTerrain->reserve(vecTile->size());

		UpdateData(TRUE);

		//로드한 INFO에 따라 CTerrain 생성후 벡터에 저장
		for (_uint i = 0 ; i < vecTile->size();i++)
		{
			//지형 객체 생성
			CGameObject*		pObject = CTerrain::Create(CMainApp_Tool::GetInstance()->m_pGraphicDev);
			NULL_CHECK(pObject);

			CTerrain*				pTerrain = dynamic_cast<CTerrain*>(pObject);

			// 가진 COM들의 데이터 변경
			//트랜스폼

			Engine::CComponent*		pComponent = nullptr;
			CTransform* pTransform = Engine::CTransform::Create();
			pComponent = pTransform;
			NULL_CHECK(pComponent);

			_vec3 vPos = (*vecTile)[i]->vPos;
			_vec3 vSize = (*vecTile)[i]->vSize;
			_vec3 vRotation = (*vecTile)[i]->vRotation;

			pTransform->Set_Pos(&vPos);
			pTransform->Set_Scale(&vSize);
			pTransform->Set_Angle(&vRotation);

			//CTerrain의 멤버 변수인 컴포넌트들을 여기서 업데이트 해도 object가 가진 맵컨테이너에 들은 컴포넌트들은 안바뀜.
			//그래서 Cterrain의 컴포넌트를 기존의 컴포넌트를 지우고 맵컨테이너에 다시 넣음
			pTerrain->Set_Component(COM_DYNAMIC, L"Com_Transform", pTransform);

			//terraintex
			_tchar szBuf[MAX_PATH] = L"Buffer_TerrainTex";
			_tchar szVtxCNX[MAX_PATH] = L"";
			_tchar szVtxCNZ[MAX_PATH] = L"";
			_tchar szVtxItv[MAX_PATH] = L"";

			_itow_s(_int((*vecTile)[i]->dwVtxCNX), szVtxCNX, 10);
			_itow_s(_int((*vecTile)[i]->dwVtxCNZ), szVtxCNZ, 10);
			_itow_s(_int((*vecTile)[i]->dwVtxItv), szVtxItv, 10);

			lstrcat(szBuf, L"_");
			lstrcat(szBuf, szVtxCNX);
			lstrcat(szBuf, L"_");
			lstrcat(szBuf, szVtxCNZ);
			lstrcat(szBuf, L"_");
			lstrcat(szBuf, szVtxItv);

			FAILED_CHECK_RETURN(CMainApp_Tool::GetInstance()->ADD_Buffer(RESOURCE_STATIC, szBuf, Engine::BUFFER_TERRAINTEX, (*vecTile)[i]->dwVtxCNX + 1, (*vecTile)[i]->dwVtxCNZ + 1, (*vecTile)[i]->dwVtxItv), );

			//리소스 새로 생성
			CResources* pResource = Engine::Clone(RESOURCE_STATIC, szBuf);
			NULL_CHECK(pResource);

			pTerrain->Set_Resource(COM_STATIC, L"Com_Buffer", pResource);

			pvecTerrain->push_back(pObject);

			//트리 박스에 추가
			_tchar szNumber[MAX_PATH] = L"";	
			_itow_s((_int)i, szNumber, 10);
			_tchar szName[MAX_PATH] = L"Terrain_";
			lstrcat(szName, szNumber);

			_tchar	szTransform[MAX_PATH] = L"Transform";
			_tchar	szOption[MAX_PATH] = L"Option";
			_tchar	szBuffer[MAX_PATH] = L"Buffer";
			_tchar	szTexture[MAX_PATH] = L"Texture";

			_tchar	szPos[MAX_PATH] = L"Position";
			_tchar	szSize[MAX_PATH] = L"Size";
			_tchar	szRotation[MAX_PATH] = L"Rotation";

			HTREEITEM		hTerrain_Options, hTransform;
			hTerrain_Options = m_TreeCtrl_OBJ.InsertItem(szName, 0, 0, m_hTerrainItem, TVI_LAST);

			hTransform = m_TreeCtrl_OBJ.InsertItem(szTransform, 0, 0, hTerrain_Options, TVI_LAST);
			m_TreeCtrl_OBJ.InsertItem(szPos, 0, 0, hTransform, TVI_LAST);
			m_TreeCtrl_OBJ.InsertItem(szSize, 0, 0, hTransform, TVI_LAST);
			m_TreeCtrl_OBJ.InsertItem(szRotation, 0, 0, hTransform, TVI_LAST);

			m_TreeCtrl_OBJ.InsertItem(szOption, 0, 0, hTerrain_Options, TVI_LAST);
			m_TreeCtrl_OBJ.InsertItem(szBuffer, 0, 0, hTerrain_Options, TVI_LAST);
			m_TreeCtrl_OBJ.InsertItem(szTexture, 0, 0, hTerrain_Options, TVI_LAST);

			m_hLastItem = hTerrain_Options;
		}
		UpdateData(FALSE);
	}
}


void CMapTool::OnBnClickedButtonDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CString strTile = m_TreeCtrl_OBJ.GetItemText(m_hLastItem);

	_int iNameIndex = 0;
	for (; iNameIndex < strTile.GetLength(); iNameIndex++)
	{
		if (isdigit(strTile[iNameIndex]))
			break;
	}

	strTile.Delete(0, iNameIndex);
	_int iTileIndex = _ttoi(strTile.GetString());

	Safe_Delete(CMainApp_Tool::GetInstance()->m_vecTile.back());
	CMainApp_Tool::GetInstance()->m_vecTile.pop_back();

	Safe_Release(CMainApp_Tool::GetInstance()->m_pvecTerrain->back());
	CMainApp_Tool::GetInstance()->m_pvecTerrain->pop_back();

	//마지막 항목의 자식트리
	HTREEITEM hChild = m_TreeCtrl_OBJ.GetChildItem(m_hLastItem);
	if (hChild != NULL)
	{
		HTREEITEM hNextChild;
		HTREEITEM hNextParent;
		while (true)
		{
			//자식트리의 자식을 탐색하기 전에 부모로 저장해놓기
			hNextParent = hChild;
			//선택자식의 자식이 있냐?
			hNextChild = m_TreeCtrl_OBJ.GetChildItem(hChild);
			//없으면 삭제진행, 있으면 일단 삭제 없이 아래 레벨 자식으로
			if (hNextChild== NULL)
			{
				//선택 자식의 부모를 부모에 저장
				hNextParent = m_TreeCtrl_OBJ.GetParentItem(hChild);
				//같은 레벨의 다음 자식 확인
				hNextChild = m_TreeCtrl_OBJ.GetNextItem(hChild, TVGN_NEXT);
				//선택 자식은 삭제
				m_TreeCtrl_OBJ.DeleteItem(hChild);
				//현재자식의 부모가 root이면 더 올라가면 안되고 걔 하나만 지워야하니까 루프 탈출
				if (hNextParent == NULL)
				{
					m_hLastItem=m_TreeCtrl_OBJ.GetNextItem(m_hLastItem, TVGN_PREVIOUS);
					break;
				}
			}
			//다음 자식을 선택자식으로
			hChild = hNextChild;
			//선택한 다음자식이 없냐?
			if (hChild == NULL)
			{
				//부모를 선택자식으로
				hChild = hNextParent;
			}
		}
	}
	UpdateData(FALSE);
}
