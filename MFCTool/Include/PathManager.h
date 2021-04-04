#pragma once
#include "afxwin.h"


// CPathManager ��ȭ �����Դϴ�.

class CPathManager : public CDialog
{
	DECLARE_DYNAMIC(CPathManager)

public:
	CPathManager(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPathManager();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PATHMANAGER };
#endif
public:
	list<PATHINFO*> m_ListPathInfo;
public:
	void HorizontalScroll();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSave();
	CListBox m_ListBox_Path;
	afx_msg void OnDropFiles(HDROP hDropInfo);


	afx_msg void OnBnClickedLoadTexture();
};
