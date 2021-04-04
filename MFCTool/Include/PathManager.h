#pragma once
#include "afxwin.h"


// CPathManager 대화 상자입니다.

class CPathManager : public CDialog
{
	DECLARE_DYNAMIC(CPathManager)

public:
	CPathManager(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPathManager();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PATHMANAGER };
#endif
public:
	list<PATHINFO*> m_ListPathInfo;
public:
	void HorizontalScroll();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSave();
	CListBox m_ListBox_Path;
	afx_msg void OnDropFiles(HDROP hDropInfo);


	afx_msg void OnBnClickedLoadTexture();
};
