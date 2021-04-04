// MapTool.cpp : ���� �����Դϴ�.
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
// CMapTool ��ȭ �����Դϴ�.

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


// CMapTool �޽��� ó�����Դϴ�.

//���̾�α� �ʱ�ȭ
BOOL CMapTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_cbComponent.ResetContent();
	m_cbComponent.InsertString(0, _T("Terrain"));
	m_cbComponent.InsertString(1, _T("Wall"));
	m_cbComponent.SetCurSel(m_iComboSelIndex);

	m_ListBox_Texture.ResetContent();
	m_TreeCtrl_OBJ.DeleteAllItems();
	m_hTerrainItem = m_TreeCtrl_OBJ.InsertItem(_T("Terrain"), 0, 0, TVI_ROOT, TVI_LAST);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

//�޺��ڽ� �������� ��� �߰����� ����.
void CMapTool::OnCbnSelchangeComboComponent()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
			m_strPreOption1 = L"����Ÿ�ϼ�";
			m_strPreOption2 = L"����Ÿ�ϼ�";
			m_strPreOption3 = L"Ÿ�ϰ���";

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

//ADD ��ư ������ ���� tree��Ʈ�ѿ� �߰� �� ���ξ����� ��������� vector�� �߰�
void CMapTool::OnBnClickedButtonADD()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	switch (m_iComboSelIndex)
	{
	case TERRAIN:
	{
		//���� �� ���嵥���� Ŭ���� ����
		CGameObject*		pObject = CTerrain::Create(CMainApp_Tool::GetInstance()->m_pGraphicDev);
		NULL_CHECK(pObject);
		CTerrain*				pTerrain = dynamic_cast<CTerrain*>(pObject);
		TERRAININFO*		pTileInfo = new TERRAININFO;
		ZeroMemory(pTileInfo, sizeof(TERRAININFO));

		//���Ϳ� ���� ���� ������ ���� ��ȣ ����
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

		//���� ��������� ���� ����
		if ((CMainApp_Tool::GetInstance()->m_pvecTerrain) == nullptr)
			(CMainApp_Tool::GetInstance()->m_pvecTerrain) = new vector<CGameObject*>;
		_int iNumber = CMainApp_Tool::GetInstance()->m_pvecTerrain->size();

		//���ξ����� �����߰� �Լ��� ũ�� ������ terraintex ���� �ʸ��ҽ��� �߰��س���
		FAILED_CHECK_RETURN(CMainApp_Tool::GetInstance()->ADD_Buffer(RESOURCE_STATIC, /*pBufferName*/szBuf
						, Engine::BUFFER_TERRAINTEX, m_dwPreOption1 + 1, m_dwPreOption2 + 1, m_dwPreOption3), );

		//terraintex ���ҽ� Ŭ�� ����
		CResources* pResource = Engine::Clone(RESOURCE_STATIC, /*pBufferName*/szBuf);
		NULL_CHECK(pResource);

		//������ü�� �� ������Ʈ�� terraintex ����
		pTerrain->Set_Resource(COM_STATIC, L"Com_Buffer", pResource);

		//Ʈ������ �� ����
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


		//�����̳ʿ� �߰�
		(CMainApp_Tool::GetInstance()->m_pvecTerrain)->emplace_back(pObject);
		(CMainApp_Tool::GetInstance()->m_vecTile).emplace_back(pTileInfo);

		//���°���� �̸� ����
		_tchar szNumber[MAX_PATH] = L"";
		_itow_s(iNumber, szNumber, 10);
		CString strName = m_pAddName +szNumber;

		//Ʈ���� ���� �ϳ� �߰� Terrain_��ȣ
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	m_hSelItem = m_TreeCtrl_OBJ.GetSelectedItem();
	//Ʈ�� ������ ����

	m_hParentItem = m_hSelItem;
	//GetParentItem�� ������ �θ� ROOT�ΰ�� NULL��ȯ

	//�̸� NULL�ʱ�ȭ
	HTREEITEM hTempParent = NULL;

	//�θ��� ������ NULL�״�� ��Ʈ�����
	hTempParent = m_TreeCtrl_OBJ.GetParentItem(m_hSelItem);
	if (hTempParent == NULL)
		return;
	hTempParent = NULL;

	while (true)
	{
		//�θ����� �θ��� ã������ NULL�̸� ���� ��尡 ��Ʈ�� ���� �׸�. �� ADD�� �߰��� ��ȣ������ �׸�
		hTempParent = m_TreeCtrl_OBJ.GetParentItem(m_TreeCtrl_OBJ.GetParentItem(m_hParentItem));
		//���࿡ �׷��ٸ� �ش� ��ȣ ������� ���� Ż��
		if (hTempParent == NULL)
			break;
		//�ƴϸ� Ž���� �θ��带 �ѷ��� �ø�
		m_hParentItem = m_TreeCtrl_OBJ.GetParentItem(m_hParentItem);
		hTempParent = NULL;
	}
	//�ش� ������ ���� ��ü �̸� Ȯ��
	//�̸��� ��ȣ Ȯ��
	CString strTile = m_TreeCtrl_OBJ.GetItemText(m_hParentItem);
	_int iNameIndex = 0;
	for (; iNameIndex < strTile.GetLength(); iNameIndex++)
	{
		if (isdigit(strTile[iNameIndex]))
			break;
	}

	strTile.Delete(0, iNameIndex);
	_int iTileIndex = _ttoi(strTile.GetString());

	//�����۰� ���� �̸��� ������Ʈ ���� �� ����
 	CString strName = m_TreeCtrl_OBJ.GetItemText(m_hSelItem);

	//��Ʈ��尡 �ƴ� ���� �׸� ��带 Ŭ�������� �ϴ� �簢�� ���� �� ũ��� ���
	m_dwPreOption1 = CMainApp_Tool::GetInstance()->m_vecTile[iTileIndex]->dwVtxCNX;
	m_dwPreOption2 = CMainApp_Tool::GetInstance()->m_vecTile[iTileIndex]->dwVtxCNZ;
	m_dwPreOption3 = CMainApp_Tool::GetInstance()->m_vecTile[iTileIndex]->dwVtxItv;

	//�׺��� ������ �ɼ� ���������� �װ� ���
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	//���������� �����ߴ� �������� �θ��� ��ȣ Ȯ��
	CString strTile = m_TreeCtrl_OBJ.GetItemText(m_hParentItem);
	_int iNameIndex = 0;
	for (; iNameIndex < strTile.GetLength(); iNameIndex++)
	{
		if (isdigit(strTile[iNameIndex]))
			break;
	}

	strTile.Delete(0, iNameIndex);
	_int iTileIndex = _ttoi(strTile.GetString());

	//���õǾ� �ִ� ������ �̸� Ȯ��
	CString strName = m_TreeCtrl_OBJ.GetItemText(m_hSelItem);

	UpdateData(TRUE);

	
	if (strName == L"Position")
	{
		CMainApp_Tool::GetInstance()->m_vecTile[iTileIndex]->vPos.x = m_fX;
		CMainApp_Tool::GetInstance()->m_vecTile[iTileIndex]->vPos.y = m_fY;
		CMainApp_Tool::GetInstance()->m_vecTile[iTileIndex]->vPos.z = m_fZ;
		
		//��ȣ�� �ش��ϴ� ������ �ε����� object �޾ƿͼ� �ٿ�ĳ����, �ش� ��ü�� ������Ʈ ������Ʈ
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

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

		//���� �б�
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

		//Ʈ�� ����
		m_TreeCtrl_OBJ.DeleteAllItems();
		//�ϴ� terrain�� �ε� �ҰŴϱ� Ʈ���� terrain ��Ʈ�� 
		m_hTerrainItem = m_TreeCtrl_OBJ.InsertItem(_T("Terrain"), 0, 0, TVI_ROOT, TVI_LAST);
		m_iComboSelIndex = 0;
		//���� Ÿ�� ���� ����
		vector<TERRAININFO*>* vecTile = &(CMainApp_Tool::GetInstance()->m_vecTile);
		for (auto& pTile : *vecTile)
			Safe_Delete(pTile);
		vecTile->clear();
		vecTile->shrink_to_fit();
		//�ε��� Ÿ�Ϻ��� �߰�
		*vecTile = vecTempTile;

		//���� terrain �����
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

		//�ε��� INFO�� ���� CTerrain ������ ���Ϳ� ����
		for (_uint i = 0 ; i < vecTile->size();i++)
		{
			//���� ��ü ����
			CGameObject*		pObject = CTerrain::Create(CMainApp_Tool::GetInstance()->m_pGraphicDev);
			NULL_CHECK(pObject);

			CTerrain*				pTerrain = dynamic_cast<CTerrain*>(pObject);

			// ���� COM���� ������ ����
			//Ʈ������

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

			//CTerrain�� ��� ������ ������Ʈ���� ���⼭ ������Ʈ �ص� object�� ���� �������̳ʿ� ���� ������Ʈ���� �ȹٲ�.
			//�׷��� Cterrain�� ������Ʈ�� ������ ������Ʈ�� ����� �������̳ʿ� �ٽ� ����
			pTerrain->Set_Component(COM_DYNAMIC, L"Com_Transform", pTransform);

			//terraintex
			_tchar szBuf[MAX_PATH] = L"Buffer_TerrainTex";
			_tchar szVtxCNX[MAX_PATH] = L"";
			_tchar szVtxCNZ[MAX_PATH] = L"";
			_tchar szVtxItv[MAX_PATH] = L"";

			_itow_s((_int)m_dwPreOption1, szVtxCNX, 10);
			_itow_s((_int)m_dwPreOption2, szVtxCNZ, 10);
			_itow_s((_int)m_dwPreOption3, szVtxItv, 10);

			lstrcat(szBuf, L"_");
			lstrcat(szBuf, szVtxCNX);
			lstrcat(szBuf, L"_");
			lstrcat(szBuf, szVtxCNZ);
			lstrcat(szBuf, L"_");
			lstrcat(szBuf, szVtxItv);

			FAILED_CHECK_RETURN(CMainApp_Tool::GetInstance()->ADD_Buffer(RESOURCE_STATIC, szBuf, Engine::BUFFER_TERRAINTEX, (*vecTile)[i]->dwVtxCNX + 1, (*vecTile)[i]->dwVtxCNZ + 1, (*vecTile)[i]->dwVtxItv), );

			//���ҽ� ���� ����
			CResources* pResource = Engine::Clone(RESOURCE_STATIC, /*pBufferName*/szBuf);
			NULL_CHECK(pResource);

			pTerrain->Set_Resource(COM_STATIC, L"Com_Buffer", pResource);

			pvecTerrain->push_back(pObject);

			//Ʈ�� �ڽ��� �߰�
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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

	//������ �׸��� �ڽ�Ʈ��
	HTREEITEM hChild = m_TreeCtrl_OBJ.GetChildItem(m_hLastItem);
	if (hChild != NULL)
	{
		HTREEITEM hNextChild;
		HTREEITEM hNextParent;
		while (true)
		{
			//�ڽ�Ʈ���� �ڽ��� Ž���ϱ� ���� �θ�� �����س���
			hNextParent = hChild;
			//�����ڽ��� �ڽ��� �ֳ�?
			hNextChild = m_TreeCtrl_OBJ.GetChildItem(hChild);
			//������ ��������, ������ �ϴ� ���� ���� �Ʒ� ���� �ڽ�����
			if (hNextChild== NULL)
			{
				//���� �ڽ��� �θ� �θ� ����
				hNextParent = m_TreeCtrl_OBJ.GetParentItem(hChild);
				//���� ������ ���� �ڽ� Ȯ��
				hNextChild = m_TreeCtrl_OBJ.GetNextItem(hChild, TVGN_NEXT);
				//���� �ڽ��� ����
				m_TreeCtrl_OBJ.DeleteItem(hChild);
				//�����ڽ��� �θ� root�̸� �� �ö󰡸� �ȵǰ� �� �ϳ��� �������ϴϱ� ���� Ż��
				if (hNextParent == NULL)
				{
					m_hLastItem=m_TreeCtrl_OBJ.GetNextItem(m_hLastItem, TVGN_PREVIOUS);
					break;
				}
			}
			//���� �ڽ��� �����ڽ�����
			hChild = hNextChild;
			//������ �����ڽ��� ����?
			if (hChild == NULL)
			{
				//�θ� �����ڽ�����
				hChild = hNextParent;
			}
		}
	}
	UpdateData(FALSE);
}
