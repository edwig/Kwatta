///////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 
// ██╗░░██╗░██╗░░░░░░░██╗░█████╗░████████╗████████╗░█████╗░
// ██║░██╔╝░██║░░██╗░░██║██╔══██╗╚══██╔══╝╚══██╔══╝██╔══██╗
// █████═╝░░╚██╗████╗██╔╝███████║░░░██║░░░░░░██║░░░███████║
// ██╔═██╗░░░████╔═████║░██╔══██║░░░██║░░░░░░██║░░░██╔══██║
// ██║░╚██╗░░╚██╔╝░╚██╔╝░██║░░██║░░░██║░░░░░░██║░░░██║░░██║
// ╚═╝░░╚═╝░░░╚═╝░░░╚═╝░░╚═╝░░╚═╝░░░╚═╝░░░░░░╚═╝░░░╚═╝░░╚═╝
// 
// 
// This product: KWATTA (KWAliTy Test API) Test suite for Command-line SOAP/JSON/HTTP internet API's
// This program: ResultViewer
// This File   : ResultInternetDlg.cpp
// What it does: Shows the total HTTP internet message result
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ResultViewer.h"
#include "ResultInternetDlg.h"
#include <AboutDlg.h>
#include <StdException.h>
#include <afxdialogex.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// CResultInternetDlg dialog

ResultInternetDlg::ResultInternetDlg(CWnd* pParent /*=nullptr*/)
	                :StyleDialog(IDD_RESULTVIEWER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

  m_tab1 = new ValidationsDlg(this);
  m_tab2 = new ResponseDlg(this);
  m_tab3 = new RawResponseDlg(this);
  m_tab4 = new RespHeadersDlg(this);
}

ResultInternetDlg::~ResultInternetDlg()
{
  delete m_tab1;
  delete m_tab2;
  delete m_tab3;
  delete m_tab4;
}

void 
ResultInternetDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);

  DDX_Control(pDX,IDC_NAME,         m_editName,m_name);
  DDX_Control(pDX,IDC_DOCUMENTATION,m_editDocumentation,m_documentation);
  DDX_Control(pDX,IDC_RESULT,       m_picture);
  DDX_Control(pDX,IDC_TABS,         m_tabs);
  DDX_Control(pDX,IDOK,             m_buttonOK);
}

BEGIN_MESSAGE_MAP(ResultInternetDlg, StyleDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
  ON_COMMAND(ID_MENU_THEMELIME,       OnStyleLime)
  ON_COMMAND(ID_MENU_THEMESKYBLUE,    OnStyleSkyblue)
  ON_COMMAND(ID_MENU_THEMEPURPLE,     OnStylePurple)
  ON_COMMAND(ID_MENU_THEMEMUSTARD,    OnStyleMustard)
  ON_COMMAND(ID_MENU_THEMEMODERATE,   OnStyleModerateGray)
  ON_COMMAND(ID_MENU_THEMEPURE,       OnStylePureGray)
  ON_COMMAND(ID_MENU_THEMEBLACK,      OnStyleBlackWhite)
  ON_COMMAND(ID_MENU_ABOUT,           OnAbout)
  ON_COMMAND(ID_MENU_EXIT,            OnExit)
END_MESSAGE_MAP()

// CResultInternetDlg message handlers

BOOL 
ResultInternetDlg::OnInitDialog()
{
	StyleDialog::OnInitDialog();
  SetWindowText(_T("Result Viewer"));
  ShowMinMaxButton();
  SetSysMenu(IDR_MENU);

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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

  InitTabs();
  InitResult();

  // Fill in the variables: After the loading of the XML files
  LoadVariables();
  LoadVariablesTabs();

  SetCanResize();
  UpdateData(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void
ResultInternetDlg::SetupDynamicLayout()
{
  StyleDialog::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif
  manager.AddItem(IDC_GRP_RESULT,   CMFCDynamicLayout::MoveNone(),                        CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_DOCUMENTATION,CMFCDynamicLayout::MoveNone(),                        CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_RESULT,       CMFCDynamicLayout::MoveHorizontal(100),               CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_TABS,         CMFCDynamicLayout::MoveNone(),                        CMFCDynamicLayout::SizeHorizontalAndVertical(100,100));
  manager.AddItem(IDOK,             CMFCDynamicLayout::MoveHorizontalAndVertical(100,100),CMFCDynamicLayout::SizeNone());
}

void
ResultInternetDlg::InitTabs()
{
  m_tab1->Create(IDD_VALIDATIONS, &m_tabs);
  m_tab2->Create(IDD_RESPONSE,    &m_tabs);
  m_tab3->Create(IDD_RAW_RESPONSE,&m_tabs);
  m_tab4->Create(IDD_RESP_HEADERS,&m_tabs);


  m_tabs.InsertItem(0,m_tab1,_T("Validations"));
  m_tabs.InsertItem(1,m_tab2,_T("Response"));
  m_tabs.InsertItem(2,m_tab3,_T("Raw Response"));
  m_tabs.InsertItem(3,m_tab4,_T("Headers"));

  // Position in the parent window
  m_tabs.Init();
}

void
ResultInternetDlg::InitResult()
{
  CString filename = theApp.GetBaseDirectory() + theApp.GetTestDirectory() + theApp.GetResultFilename();

  // Read in the definition file for a test step
  try
  {
    m_stepResult.ReadFromXML(filename);
  }
  catch (StdException& ex)
  {
    StyleMessageBox(this,ex.GetErrorMessage(),PRODUCT_NAME,MB_OK | MB_ICONERROR);
    theApp.GetMainWnd()->PostMessage(WM_QUIT,0,0);
  }
}

void
ResultInternetDlg::LoadVariables()
{
  m_name          = m_stepResult.GetName();
  m_documentation = m_stepResult.GetDocumentation();

  LPTSTR bitmap = m_stepResult.GetTotalResult() ? MAKEINTRESOURCE(IDB_OK) : MAKEINTRESOURCE(IDB_ERROR);
  m_picture.LoadBitmaps(bitmap);
}

void
ResultInternetDlg::LoadVariablesTabs()
{
  m_tab1->LoadVariables(&m_stepResult);
  m_tab2->LoadVariables(&m_stepResult);
  m_tab3->LoadVariables(&m_stepResult);
  m_tab4->LoadVariables(&m_stepResult);
}

void 
ResultInternetDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
// 		CAboutDlg dlgAbout;
// 		dlgAbout.DoModal();
	}
	else
	{
		StyleDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void 
ResultInternetDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		StyleDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR ResultInternetDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void
ResultInternetDlg::OnAbout()
{
  AutoFocus focus;
  AboutDlg dlgAbout(this);
  dlgAbout.DoModal();
}

void
ResultInternetDlg::OnExit()
{
  StyleDialog::OnOK();
}
