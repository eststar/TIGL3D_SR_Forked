	#pragma once

#include "Define.h"
#include "afxwin.h"
#include "Engine_Define.h"
#include "afxcmn.h"
// CMapTool ��ȭ �����Դϴ�.

class CMainApp_Tool;
class CMapTool : public CDialog
{
	DECLARE_DYNAMIC(CMapTool)

public:
	CMapTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMapTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboComponent();
	CComboBox m_cbComponent;
	CListBox m_ListBox_Texture;
	//list<PATHINFO*>		m_listPathInfo;

	afx_msg void OnBnClickedButtonADD();
	CTreeCtrl m_TreeCtrl_OBJ;
	afx_msg void OnTvnSelchangedTreeControl1(NMHDR *pNMHDR, LRESULT *pResult);
	HTREEITEM m_hTerrainItem;
	HTREEITEM m_hLastItem;

	HTREEITEM m_hParentItem;
	HTREEITEM m_hSelItem;
	float m_fX;
	float m_fY;
	float m_fZ;
	afx_msg void OnBnClickedButtonUpdateTile();
	CButton m_chkShowTex;
	afx_msg void OnBnClickedCheckShowtex();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonDelete();

	DWORD m_dwPreOption1;
	DWORD m_dwPreOption2;
	DWORD m_dwPreOption3;
	int m_iComboSelIndex;

	CString m_pAddName;

	CString m_strPreOption1;
	CString m_strPreOption2;
	CString m_strPreOption3;
};
