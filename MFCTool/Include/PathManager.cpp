// PathManager.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "PathManager.h"
#include "afxdialogex.h"

#include "FileInfo.h"
// CPathManager 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPathManager, CDialog)

CPathManager::CPathManager(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PATHMANAGER, pParent)
{

}

CPathManager::~CPathManager()
{
}

void CPathManager::HorizontalScroll()
{
	CString strName = L"";
	CSize TextSize;

	int iCX = 0;

	CDC* pDC = m_ListBox_Path.GetDC();

	for (int i = 0; i < m_ListBox_Path.GetCount(); i++)
	{
		m_ListBox_Path.GetText(i, strName);
		TextSize = pDC->GetTextExtent(strName);

		if (TextSize.cx > iCX)
			iCX = TextSize.cx;
	}
	m_ListBox_Path.ReleaseDC(pDC);

	m_ListBox_Path.SetHorizontalExtent(iCX);
}

void CPathManager::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox_Path);
}


BEGIN_MESSAGE_MAP(CPathManager, CDialog)
	ON_BN_CLICKED(IDC_BUTTON3, &CPathManager::OnBnClickedSave)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON1, &CPathManager::OnBnClickedLoadTexture)
END_MESSAGE_MAP()


// CPathManager 메시지 처리기입니다.


void CPathManager::OnBnClickedSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	wofstream fout;
	fout.open("../Data/ImagePathInfo.txt");
	if (!fout.fail())
	{
		for (auto& rPathInfo : m_ListPathInfo)
			fout << rPathInfo->pPath << L"|" << rPathInfo->pObjectKey << L"|" << rPathInfo->pStateKey << L"|" << rPathInfo->iCount << endl;
		fout.close();
	}

}


void CPathManager::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	UpdateData(TRUE);
	TCHAR szPath[MAX_PATH] = L"";

	size_t iFileCount = DragQueryFile(hDropInfo, -1, nullptr, 0);

	for (size_t i = 0; i < iFileCount; ++i)
	{
		DragQueryFile(hDropInfo, i, szPath, MAX_PATH);
		CFileInfo::PathExtraction(szPath, m_ListPathInfo);
	}
	m_ListBox_Path.ResetContent();

	wstring wstrCombine = L"";
	TCHAR szBuf[MAX_PATH] = L"";
	for (auto& pImagePath : m_ListPathInfo)
	{
		_itow_s(pImagePath->iCount, szBuf, 10);
		wstring wstrPath = pImagePath->pPath;
		wstring wstrObjectKey = pImagePath->pObjectKey;
		wstring wstrStateKey = pImagePath->pStateKey;

		wstrCombine = wstrPath + L"|" + wstrObjectKey + L"|" + wstrStateKey + L"|" + szBuf;
		m_ListBox_Path.AddString(wstrCombine.c_str());

	}
	HorizontalScroll();
	UpdateData(FALSE);
	CDialog::OnDropFiles(hDropInfo);
}


void CPathManager::OnBnClickedLoadTexture()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//UpdateData(TRUE);
	//if (FAILED(Engine::Get_Instance()->ReadImagePath()))
	//{
	//	ERR_MSG(L"Read Image Failed-PathExtraction");
	//	CTexture_Manager::Get_Instance()->Release();
	//}
	//UpdateData(FALSE);

}
