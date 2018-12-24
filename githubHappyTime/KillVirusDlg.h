// KillVirusDlg.h : header file
//

#if !defined(AFX_KILLVIRUSDLG_H__4DBB8326_3CD6_11D5_B348_00E04C3E50D1__INCLUDED_)
#define AFX_KILLVIRUSDLG_H__4DBB8326_3CD6_11D5_B348_00E04C3E50D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///////////////////////////////////////////////////////////////////////////
// CKillVirusDlg dialog
#include "hyperlink.h"

class CKillVirusDlg : public CDialog
{
// Construction
public:
	~CKillVirusDlg();
	CKillVirusDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CKillVirusDlg)
	enum { IDD = IDD_KILLVIRUS_DIALOG };
	CButton	m_btr;
	CButton	m_btc;
	//CHyperLink	m_linkHomePage;
	CButton	m_btnPath;
	CEdit	m_editPath;
	BOOL	m_bAllPath;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKillVirusDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CKillVirusDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnPath();
	afx_msg void OnBtnChk();
	afx_msg void OnChkAllpath();
	afx_msg void OnBtnClrreg();
	virtual void OnCancel();
	afx_msg void OnAboutbox();
	afx_msg void OnKillvirusDialog2();
	afx_msg void OnKillvirusDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	CStringArray m_arSuffix;
	char m_dir[MAX_PATH];

public:
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KILLVIRUSDLG_H__4DBB8326_3CD6_11D5_B348_00E04C3E50D1__INCLUDED_)
