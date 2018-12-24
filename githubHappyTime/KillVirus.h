// KillVirus.h : main header file for the KILLVIRUS application
//

#if !defined(AFX_KILLVIRUS_H__4DBB8324_3CD6_11D5_B348_00E04C3E50D1__INCLUDED_)
#define AFX_KILLVIRUS_H__4DBB8324_3CD6_11D5_B348_00E04C3E50D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

///////////////////////////////////////////////////////////////////////////
// CKillVirusApp:
// See KillVirus.cpp for the implementation of this class
//

class CKillVirusApp : public CWinApp
{
public:
	CKillVirusApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKillVirusApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CKillVirusApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KILLVIRUS_H__4DBB8324_3CD6_11D5_B348_00E04C3E50D1__INCLUDED_)
