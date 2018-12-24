#include "stdafx.h"
#include "KillVirus.h"
#include "KillVirusDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog) 
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKillVirusDlg dialog

//病毒特征串
const char szVirusMark[]="REM I AM SORRY! HAPPY TIME";
//用来判断是否进入script区
const char szScriptMark[]="SCRIPT";

//判断是否进入script区
BOOL IsScriptStart(CString strLine)
{
	int i=0,iScript,iBack;
	
	strLine.MakeUpper();
	i=strLine.Find('<',0);
	while(i>=0)
	{
		iScript=strLine.Find(szScriptMark,i+1);//script串位置
		iBack=strLine.Find('>',i+1);
		if(iScript>i&& iBack>i&& iScript<iBack)
			return TRUE;
		i=strLine.Find('<',i+1);
	}
	return FALSE;
}

//杀毒
//直接搜索特征串
//发现后直接抛弃后面的内容
//加上</Script>后结束文件

BOOL	KillVirusFile(const CString &filename)
{
	CStdioFile file;

	try{
		file.Open(filename,CFile::modeReadWrite);
	}
	catch(...)
	{
		return FALSE;
	}

	char szTempFile[MAX_PATH];
	GetTempPath(sizeof(szTempFile),szTempFile);
	GetTempFileName(szTempFile,"Temp",1,szTempFile);
	CStdioFile	tempFile(szTempFile,CFile::modeCreate|CFile::modeWrite);
	

	BOOL	bEnterScript=FALSE;//是否在Script段
	BOOL	bEnterBracket=FALSE;//是否进入<>中
	BOOL	bRes;
	CString tmp;
	CString strLine;
	CString strNewLine;
	int i=0;
	CString strScript;
	if(file.m_hFile!=CFile::hFileNull)
	{
		try{
			while(file.ReadString(strLine))
			{  
				strNewLine=strLine;
				strLine.MakeUpper();
				if(strLine.Find(szVirusMark)>=0)
				{  
					AfxMessageBox("发现欢乐时光病毒!\n 病毒脚本已清除!");
					strNewLine="</Script>";
					tempFile.WriteString(strNewLine);
					file.Close();
					tempFile.Close();
					bRes=CopyFile(szTempFile,filename,FALSE);
					
					if(!bRes)
					{
						DWORD att;
						att = GetFileAttributes(filename);
						SetFileAttributes(filename,FILE_ATTRIBUTE_NORMAL);
						bRes=CopyFile(szTempFile,filename,FALSE);
						if(!bRes)
						{
							AfxMessageBox("拷贝文件出错"+filename);
							exit(0);
						}
						SetFileAttributes(filename,att);
					}
					return TRUE;
				}
				else
					tempFile.WriteString(strNewLine);
						
			}
		}
		catch(...)
		{

		}
		
	}
	return FALSE;
}

//arSuffix是目标文件扩展名数组，
BOOL	KillVirusFile(CString filename,const CStringArray& arSuffix)
{
	CString strSuffix;
	int i,j,k;
	
	filename.MakeLower();
	for(i=0;i<arSuffix.GetSize();i++)
	{
		strSuffix=arSuffix[i];
		for(j=filename.GetLength()-1,k=strSuffix.GetLength()-1;j>=0&&k>=0;j--,k--)
		{
			if(filename[j]!=strSuffix[k])
				break;
		}
		if(k<0&&j>0)//后缀匹配
		{
			if(filename[j]=='.')
				KillVirusFile(filename);
		}
		
		
	}

	return FALSE;
}
CStdioFile* plist;


//遍历某一文件夹下所有文件
BOOL	KillVirusDir(CString dirname,const CStringArray & arSuffix)//假定该目录名以"\"结尾
{

	if(dirname[dirname.GetLength()-1]!='\\')
		dirname+='\\';
	CString oldDirName=dirname;
	dirname+="*.*";
	HANDLE hRes;
	WIN32_FIND_DATA wfd;
	CString	strTemp;

	hRes=FindFirstFile(dirname,&wfd);
	while(hRes!=INVALID_HANDLE_VALUE)
	{
		
		if(!(wfd.cFileName==CString(".")||wfd.cFileName==CString("..") ))
		{
			if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				strTemp.Format(wfd.cFileName);
				if(strTemp[strTemp.GetLength()-1]!='\\')
					strTemp=oldDirName+strTemp+"\\";
				plist->WriteString(strTemp);
				plist->WriteString("\n");
				KillVirusDir(strTemp,arSuffix);
				
			}
			else
			{
				strTemp=oldDirName+wfd.cFileName;
				plist->WriteString(strTemp);
				plist->WriteString("\n");
				KillVirusFile(strTemp,arSuffix);
			}
		}
		
			if(FindNextFile( hRes,&wfd)==FALSE)
				break;
	}
	FindClose(hRes);

	return FALSE;
}

//清理注册表
BOOL ClearRegedit()
{
	HKEY hKey;

	//删除注册表中的HKEY_CURRENT_USER\Software\Help
	if(ERROR_SUCCESS==RegOpenKey(HKEY_CURRENT_USER,"Control Panel",&hKey))
	{
		if(ERROR_SUCCESS==RegOpenKey(HKEY_CURRENT_USER,"Software",&hKey))
		{
			RegDeleteKey(hKey,"Help");
			RegCloseKey(hKey);
		}
		else
			RegCloseKey(hKey);
	}
		
	
	
	//改注册表中的背景页面
	if(ERROR_SUCCESS==RegOpenKey(HKEY_CURRENT_USER,"Control Panel",&hKey ) )		
	{
		if(ERROR_SUCCESS==RegOpenKey(hKey,"Desktop",&hKey))
		{
			
			char szWallPaper[MAX_PATH];
			DWORD	dwRes=sizeof(szWallPaper);
			DWORD	dwType;
			if(ERROR_SUCCESS==RegQueryValueEx(hKey,"wallPaper",0,&dwType, (BYTE*)szWallPaper,&dwRes))
			{
				KillVirusFile(szWallPaper);
				RegDeleteValue(hKey,"wallPaper");
			}
			
			
	
	
		}
		RegCloseKey(hKey);
	}
	return TRUE;
}


CKillVirusDlg::CKillVirusDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKillVirusDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKillVirusDlg)
	m_bAllPath = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_arSuffix.Add("htm");
	m_arSuffix.Add("html");
	m_arSuffix.Add("asp");
	m_arSuffix.Add("vbs");
	m_arSuffix.Add("htt");

	ClearRegedit();
//	KillVirusFile("C:\\Windows\\Desktop\\virus.html");
}

void CKillVirusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKillVirusDlg)
	DDX_Control(pDX, IDC_BTN_CLRREG, m_btr);
	DDX_Control(pDX, IDC_BTN_CHK, m_btc);
	//DDX_Control(pDX, IDC_STA_HOMEPAGE, m_linkHomePage);
	DDX_Control(pDX, IDC_BTN_PATH, m_btnPath);
	DDX_Control(pDX, IDC_EDIT_PATH, m_editPath);
	DDX_Check(pDX, IDC_CHK_ALLPATH, m_bAllPath);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CKillVirusDlg, CDialog)
	//{{AFX_MSG_MAP(CKillVirusDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_PATH, OnBtnPath)
	ON_BN_CLICKED(IDC_BTN_CHK, OnBtnChk)
	ON_BN_CLICKED(IDC_CHK_ALLPATH, OnChkAllpath)
	ON_BN_CLICKED(IDC_BTN_CLRREG, OnBtnClrreg)
	ON_BN_CLICKED(IDM_ABOUTBOX, OnAboutbox)
	ON_BN_CLICKED(IDD_KILLVIRUS_DIALOG2, OnKillvirusDialog2)
	ON_BN_CLICKED(IDD_KILLVIRUS_DIALOG, OnKillvirusDialog)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	//ON_BN_CLICKED(IDD_KILLVIRUS_DIALOG, OnKillvirusDialog)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKillVirusDlg message handlers

BOOL CKillVirusDlg::OnInitDialog()
{

	CDialog::OnInitDialog();
    
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CKillVirusDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CKillVirusDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CKillVirusDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CKillVirusDlg::OnBtnPath() 
{
	// TODO: Add your control notification handler code here
	BROWSEINFO bi; 
	
	ITEMIDLIST *pidl; 
	bi.hwndOwner = NULL; 
	bi.pidlRoot = NULL; 
	bi.pszDisplayName = m_dir; 
	bi.lpszTitle = "请选择文件"; 
	bi.ulFlags = BIF_RETURNONLYFSDIRS; 
	bi.lpfn = NULL; 
	bi.lParam = 0; 
	bi.iImage = 0; 
	pidl = SHBrowseForFolder( &bi ); 
	if ( pidl == NULL ) 
		m_dir[0]= 0; 
	if (!SHGetPathFromIDList( pidl, m_dir )) /* Retrieve folder       nam e from ITEMLIST if ( pidl == NULL ) */
		m_dir[0]= 0; 
	m_editPath.SetWindowText(m_dir);
		
}

void CKillVirusDlg::OnBtnChk() 
{   
    m_btc.SetWindowText("请稍后...");
	plist = new CStdioFile("扫描清单.txt",CFile::modeCreate|CFile::modeWrite);

	GetDlgItem(IDC_BTN_CHK)->EnableWindow(false);
	if(m_bAllPath)
	{
	char bufDrive[200];
	GetLogicalDriveStrings(sizeof(bufDrive), bufDrive);
	int i=0;
	char *pOldPos=bufDrive;
	char *pPos=bufDrive;
	char destFile[MAX_PATH];//目标文件名
	WIN32_FIND_DATA result;

	FindFirstFile("C:\\*.*",&result);
	TRACE("find file\n");
	while(*pPos)
	{
		while(*pPos)
			pPos++;
		strcpy(destFile,pOldPos);
		KillVirusDir(destFile,m_arSuffix);
		pPos++;
		pOldPos=pPos;
	}

	}
	else
	{
		CString strDir;
		m_editPath.GetWindowText(strDir);
		if(strDir.GetLength() !=0)
		{
			KillVirusDir(strDir,m_arSuffix);
		}
	}
	MessageBox("检测完毕","欢乐时光专杀");
    m_btc.SetWindowText(" 查毒");
	GetDlgItem(IDC_BTN_CHK)->EnableWindow(true);
	plist->Close();
	delete plist;
}

void CKillVirusDlg::OnChkAllpath() 
{
	UpdateData();
	m_editPath.EnableWindow(!m_bAllPath);
	m_btnPath.EnableWindow(!m_bAllPath);
		
}

void CKillVirusDlg::OnBtnClrreg() 
{   
    m_btr.SetWindowText("请稍后...");
	ClearRegedit();	
    MessageBox("清理注册表完毕!","欢乐时光专杀");
    m_btr.SetWindowText(" 清理注册表");
}

CKillVirusDlg::~CKillVirusDlg()
{
}

void CKillVirusDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	MessageBox("不要忘记定时查毒!", "欢乐时光专杀");
	CDialog::OnCancel();
}

//void CKillVirusDlg::OnCancel2() 
//{
	// TODO: Add your control notification handler code here
	
//}

void CKillVirusDlg::OnAboutbox() 
{
	// TODO: Add your control notification handler code here
	MessageBox("                   欢乐时光暴发时间表\n1月12日、2月11日、3月10日、4月9日、5月8日、6月7日\n7月6日、8日5日、9月4日、10月3日、11月2日、12月1日","欢乐时光专杀");
	
}

void CKillVirusDlg::OnKillvirusDialog2() 
{
	// TODO: Add your control notification handler code here

	MessageBox("金山毒霸：http://www.duba.net\n江民杀毒：http://www.jiangmin.com\n","欢乐时光专杀");
}

void CKillVirusDlg::OnKillvirusDialog() 
{
	// TODO: Add your control notification handler code here
   MessageBox("   欢乐时光（VBS.Happytime.A@mm）是一个VB源程序病毒，.htm/.html/.vbs/.asp/.htt文件将会被其感染。它作为电子邮件的附件，并利用OutlookExoress传播，可以在没有运行任何附件时就运行自己。当其发作后它将自己伪装成看似合法的文件，如help.htm;她会在注册表的HKEY_CURRENT_USER\Software\Help\Count上改变键值更新感染文件数量；当月份和日期相加等于13时，病毒会删除所有的exe和dll文件。","欢乐时光专杀");
	
}

HBRUSH CKillVirusDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if(nCtlColor==CTLCOLOR_STATIC)   
        pDC->SetTextColor(RGB(0,70,250));   
       // pDC->SetBkColor(RGB(0,250,0));
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CAboutDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnOK();
}
