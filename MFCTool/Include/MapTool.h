	#pragma once

#include "afxwin.h"
#include "afxcmn.h"
// CMapTool ��ȭ �����Դϴ�.
#include "Engine_Define.h"
#include "Define.h"

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
	CComboBox m_cbOption;
	CComboBox m_cbType;

	CListBox m_ListBox_Texture;

	CTreeCtrl m_TreeCtrl_OBJ;
	HTREEITEM m_hTerrainItem;
	HTREEITEM m_hLastItem;
	HTREEITEM m_hParentItem;
	HTREEITEM m_hSelItem;
	float m_fX;
	float m_fY;
	float m_fZ;
	CButton m_chkShowTex;
	DWORD m_dwPreOption1;
	DWORD m_dwPreOption2;
	DWORD m_dwPreOption3;
	int m_iComboSelIndex;
	_int m_iTerrainNumber;
	CString m_pAddName;
	CString m_strPreOption1;
	CString m_strPreOption2;
	CString m_strPreOption3;

	afx_msg void OnBnClickedButtonADD();
	afx_msg void OnTvnSelchangedTreeControl1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonUpdateTile();
	afx_msg void OnBnClickedCheckShowtex();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonDelete();

	int m_iOption;
	int m_iType;
	afx_msg void OnCbnSelchangeComboOption();
	afx_msg void OnCbnSelchangeComboType();
};
