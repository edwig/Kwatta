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
// This File   : ResultViewerDlg.cpp
// What it does: Shows the result of a command line test step.
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ResultViewer.h"
#include "ResultViewerDlg.h"
#include "StdException.h"
#include <AboutDlg.h>
#include "afxdialogex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// CResultViewerDlg dialog

ResultViewerDlg::ResultViewerDlg(CWnd* pParent /*=nullptr*/)
	              :StyleDialog(IDD_RESULTVIEWER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

  m_tab1 = new ValidationsDlg(this);
  m_tab2 = new OutputDlg(this);
}

ResultViewerDlg::~ResultViewerDlg()
{
  delete m_tab1;
  delete m_tab2;
}

void 
ResultViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);

  DDX_Control(pDX,IDC_NAME,         m_editName,m_name);
  DDX_Control(pDX,IDC_DOCUMENTATION,m_editDocumentation,m_documentation);
  DDX_Control(pDX,IDC_RESULT,       m_picture);
  DDX_Control(pDX,IDC_TABS,         m_tabs);
  DDX_Control(pDX,IDOK,             m_buttonOK);
}

BEGIN_MESSAGE_MAP(ResultViewerDlg, StyleDialog)
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

// CResultViewerDlg message handlers

BOOL 
ResultViewerDlg::OnInitDialog()
{
	StyleDialog::OnInitDialog();
  SetWindowText(_T("Step Result Viewer"));
  ShowMinMaxButton();
  SetSysMenu(IDR_MENU);
  SetAboutBoxAndIcon(IDM_ABOUTBOX,IDS_ABOUTBOX);

	// initialization 
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
ResultViewerDlg::SetupDynamicLayout()
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
ResultViewerDlg::InitTabs()
{
  m_tab1->Create(IDD_VALIDATIONS,&m_tabs);
  m_tab2->Create(IDD_OUTPUT,     &m_tabs);

  m_tabs.InsertItem(0,m_tab1,_T("Validations"));
  m_tabs.InsertItem(1,m_tab2,_T("Program output"));

  // Position in the parent window
  m_tabs.Init();
}

void
ResultViewerDlg::InitResult()
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
ResultViewerDlg::LoadVariables()
{
  m_name          = m_stepResult.GetName();
  m_documentation = m_stepResult.GetDocumentation();
  m_documentation.Replace(_T("\n"),_T("\r\n"));

  LPTSTR bitmap = m_stepResult.GetTotalResult() ? MAKEINTRESOURCE(IDB_OK) : MAKEINTRESOURCE(IDB_ERROR);
  m_picture.LoadBitmaps(bitmap);
}

void
ResultViewerDlg::LoadVariablesTabs()
{
  m_tab1->LoadVariables(&m_stepResult);
  m_tab2->LoadVariables(&m_stepResult);
}

void 
ResultViewerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
    AutoFocus focus;
    AboutDlg dlgAbout;
		dlgAbout.DoModal();
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
ResultViewerDlg::OnPaint()
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
HCURSOR ResultViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void
ResultViewerDlg::OnAbout()
{
  AutoFocus focus;
  AboutDlg dlgAbout(this);
  dlgAbout.DoModal();
}

void
ResultViewerDlg::OnExit()
{
  StyleDialog::OnOK();
}
