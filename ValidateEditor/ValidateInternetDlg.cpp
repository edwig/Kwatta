﻿///////////////////////////////////////////////////////////////////////////////////////////////////////
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
// This program: ValidateEditor
// This File   : ValidateInternetDlg.cpp
// What it does: Main dialog for the validation of a HTTPMessage call
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "afxdialogex.h"
#include "ValidateEditor.h"
#include "ValidateInternetDlg.h"
#include <AboutDlg.h>
#include "StdException.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// CValidateInternetDlg dialog

ValidateInternetDlg::ValidateInternetDlg(CWnd* pParent /*=nullptr*/)
                    :StyleDialog(IDD_VALIDATEEDITOR,pParent)
{
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

  m_tab1 = new StatusDlg(this);
  m_tab2 = new HeaderDlg(this);
  m_tab3 = new BodyDlg(this);
  m_tab4 = new XMLNodeDlg(this);
  m_tab5 = new JSONPairDlg(this);
}

ValidateInternetDlg::~ValidateInternetDlg()
{
  delete m_tab1;
  delete m_tab2;
  delete m_tab3;
  delete m_tab4;
  delete m_tab5;
}

void ValidateInternetDlg::DoDataExchange(CDataExchange* pDX)
{
  StyleDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_NAME,         m_editName,m_name);
  DDX_Control(pDX,IDC_BOUND,        m_editBound,m_bound);
  DDX_Control(pDX,IDC_DOCUMENTATION,m_editDocumentation,m_documentation);
  DDX_Control(pDX,IDC_TABS,         m_tabs);
  DDX_Control(pDX,IDOK,             m_buttonOK);
  DDX_Control(pDX,IDCANCEL,         m_buttonCancel);
}

BEGIN_MESSAGE_MAP(ValidateInternetDlg,StyleDialog)
  ON_WM_SYSCOMMAND()
  ON_WM_PAINT()
  ON_WM_QUERYDRAGICON()
  ON_EN_CHANGE (IDC_NAME,         &ValidateInternetDlg::OnEnChangeName)
  ON_EN_CHANGE (IDC_DOCUMENTATION,&ValidateInternetDlg::OnEnChangeDocumentation)
  ON_BN_CLICKED(IDOK,             &ValidateInternetDlg::OnBnClickedOk)
  ON_BN_CLICKED(IDCANCEL,         &ValidateInternetDlg::OnBnClickedCancel)

  ON_COMMAND(ID_MENU_THEMELIME,       OnStyleLime)
  ON_COMMAND(ID_MENU_THEMESKYBLUE,    OnStyleSkyblue)
  ON_COMMAND(ID_MENU_THEMEPURPLE,     OnStylePurple)
  ON_COMMAND(ID_MENU_THEMEMODERATE,   OnStyleModerateGray)
  ON_COMMAND(ID_MENU_THEMEPURE,       OnStylePureGray)
  ON_COMMAND(ID_MENU_THEMEBLACK,      OnStyleBlackWhite)
  ON_COMMAND(ID_MENU_ABOUT,           OnAbout)
  ON_COMMAND(ID_MENU_EXIT,            OnExit)
END_MESSAGE_MAP()

// CValidateInternetDlg message handlers

BOOL ValidateInternetDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();
  SetWindowText("Validate Editor (Internet)");
  ShowMinMaxButton();
  SetSysMenu(IDR_MENU);

  // Add "About..." menu item to system menu.

  // IDM_ABOUTBOX must be in the system command range.
  ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
  ASSERT(IDM_ABOUTBOX < 0xF000);

  CMenu* pSysMenu = GetSystemMenu(FALSE);
  if(pSysMenu != nullptr)
  {
    BOOL bNameValid;
    CString strAboutMenu;
    bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
    ASSERT(bNameValid);
    if(!strAboutMenu.IsEmpty())
    {
      pSysMenu->AppendMenu(MF_SEPARATOR);
      pSysMenu->AppendMenu(MF_STRING,IDM_ABOUTBOX,strAboutMenu);
    }
  }

  // Set the icon for this dialog.  The framework does this automatically
  //  when the application's main window is not a dialog
  SetIcon(m_hIcon,TRUE);			// Set big icon
  SetIcon(m_hIcon,FALSE);		// Set small icon

  // Read all the data
  InitTabs();
  InitValidation();
  InitParameters();

  // Effectuate the parameters on the members
  EffectiveParameters();

  // Fill in the variables: After the loading of the XML files
  LoadVariables();
  LoadVariablesTabs();

  m_editBound.SetMutable(false);
  m_editBound.SetBorderSize(2);

  SetCanResize();
  UpdateData(FALSE);

  SetFirstUsedTab();

  return TRUE;  // return TRUE  unless you set the focus to a control
}

void
ValidateInternetDlg::SetupDynamicLayout()
{
  StyleDialog::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif

  manager.AddItem(IDC_GRP_VALID,    CMFCDynamicLayout::MoveNone(),                        CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_BOUND,        CMFCDynamicLayout::MoveHorizontal(100),               CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_DOCUMENTATION,CMFCDynamicLayout::MoveNone(),                        CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_TABS,         CMFCDynamicLayout::MoveNone(),                        CMFCDynamicLayout::SizeHorizontalAndVertical(100,100));
  manager.AddItem(IDOK,             CMFCDynamicLayout::MoveHorizontalAndVertical(100,100),CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDCANCEL,         CMFCDynamicLayout::MoveHorizontalAndVertical(100,100),CMFCDynamicLayout::SizeNone());
}

void
ValidateInternetDlg::InitTabs()
{
   m_tab1->Create(IDD_STATUS,   &m_tabs);
   m_tab2->Create(IDD_HEADER,   &m_tabs);
   m_tab3->Create(IDD_BODY,     &m_tabs);
   m_tab4->Create(IDD_XML_NODE, &m_tabs);
   m_tab5->Create(IDD_JSON_PAIR,&m_tabs);
 
   m_tabs.InsertItem(0,m_tab1,"Status");
   m_tabs.InsertItem(1,m_tab2,"Header");
   m_tabs.InsertItem(2,m_tab3,"Body");
   m_tabs.InsertItem(3,m_tab4,"XML Node");
   m_tabs.InsertItem(4,m_tab5,"JSON Pair");

  m_tabs.Init();
}

void
ValidateInternetDlg::InitValidation()
{
  CString filename = theApp.GetBaseDirectory() + theApp.GetTestDirectory() + theApp.GetValidateFilename();

  // Read in the definition file for a test step
  try
  {
    m_validate.ReadFromXML(filename);
  }
  catch(StdException& ex)
  {
    StyleMessageBox(this,ex.GetErrorMessage(),PRODUCT_NAME,MB_OK | MB_ICONERROR);
    theApp.GetMainWnd()->PostMessage(WM_QUIT,0,0);
  }
}

void
ValidateInternetDlg::InitParameters()
{
  CString global = theApp.GetBaseDirectory() + "Parameters.xpar";
  CString local = theApp.GetParameterFilename();

  // read the definition of the parameters
  try
  {
    m_parameters.ReadFromXML(global);
    m_parameters.ReadFromXML(local,false);
  }
  catch(StdException& ex)
  {
    StyleMessageBox(this,ex.GetErrorMessage(),PRODUCT_NAME,MB_OK | MB_ICONERROR);
    theApp.GetMainWnd()->PostMessage(WM_QUIT,0,0);
  }
}

void
ValidateInternetDlg::EffectiveParameters()
{
  if(m_busy == false)
  {
    m_busy = true;

    m_bound = "Parameters: OK";
    // Effectuate the parameters
    m_unbound = m_validate.EffectiveReplacements(&m_parameters,true);

    if(m_unbound > 0)
    {
      m_bound.Format("Unbound parameters: %d",m_unbound);
    }

    m_editBound.SetTextColor  (m_unbound > 0 ? RGB(255,0,0) : RGB(0,255,0));
    m_editBound.SetBorderColor(m_unbound > 0 ? RGB(255,0,0) : RGB(0,255,0));

    UpdateData(FALSE);
  }
  m_busy = false;

}

void
ValidateInternetDlg::LoadVariables()
{
  m_name = m_validate.GetName();
  m_documentation = m_validate.GetDocumentation();
}

void
ValidateInternetDlg::LoadVariablesTabs()
{
  m_tab1->InitTab(&m_validate,&m_parameters);
  m_tab2->InitTab(&m_validate,&m_parameters);
  m_tab3->InitTab(&m_validate,&m_parameters);
  m_tab4->InitTab(&m_validate,&m_parameters);
  m_tab5->InitTab(&m_validate,&m_parameters);
}

void
ValidateInternetDlg::StoreVariables()
{
  // General variables
  m_validate.SetName(m_name);
  m_validate.SetDocumentation(m_documentation);

  // Take all the changes from the tabs
  m_tab1->StoreVariables();
  m_tab2->StoreVariables();
  m_tab3->StoreVariables();
  m_tab4->StoreVariables();
  m_tab5->StoreVariables();
}

bool
ValidateInternetDlg::SaveValidate()
{
  StoreVariables();

  try
  {
    bool res1 = m_validate.WriteToXML(m_validate.GetFilename());
    bool res2 = m_parameters.WriteToXML();

    return (res1 && res2);
  }
  catch(StdException& ex)
  {
    StyleMessageBox(this,ex.GetErrorMessage(),PRODUCT_NAME,MB_OK | MB_ICONERROR);
  }
  return false;
}

void
ValidateInternetDlg::SetFirstUsedTab()
{
  int tab = 0;

       if (m_validate.GetCheckStatus()) tab = 0;
  else if (m_validate.GetCheckHeader()) tab = 1;
  else if (m_validate.GetCheckBody())   tab = 2;
  else if (m_validate.GetCheckXML())    tab = 3;
  else if (m_validate.GetCheckJSON())   tab = 4;

  m_tabs.SetCurFocus(tab);
}

//////////////////////////////////////////////////////////////////////////
//
// Message Handlers
//
//////////////////////////////////////////////////////////////////////////

void
ValidateInternetDlg::OnAbout()
{
  AboutDlg dlgAbout(this);
  dlgAbout.DoModal();
}

void
ValidateInternetDlg::OnSysCommand(UINT nID,LPARAM lParam)
{
  if((nID & 0xFFF0) == IDM_ABOUTBOX)
  {
//     CAboutDlg dlgAbout;
//     dlgAbout.DoModal();
  }
  else
  {
    StyleDialog::OnSysCommand(nID,lParam);
  }
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void ValidateInternetDlg::OnPaint()
{
  if(IsIconic())
  {
    CPaintDC dc(this); // device context for painting

    SendMessage(WM_ICONERASEBKGND,reinterpret_cast<WPARAM>(dc.GetSafeHdc()),0);

    // Center icon in client rectangle
    int cxIcon = GetSystemMetrics(SM_CXICON);
    int cyIcon = GetSystemMetrics(SM_CYICON);
    CRect rect;
    GetClientRect(&rect);
    int x = (rect.Width() - cxIcon + 1) / 2;
    int y = (rect.Height() - cyIcon + 1) / 2;

    // Draw the icon
    dc.DrawIcon(x,y,m_hIcon);
  }
  else
  {
    StyleDialog::OnPaint();
  }
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR
ValidateInternetDlg::OnQueryDragIcon()
{
  return static_cast<HCURSOR>(m_hIcon);
}

void
ValidateInternetDlg::OnEnChangeName()
{
  UpdateData();
}

void
ValidateInternetDlg::OnEnChangeDocumentation()
{
  UpdateData();
}

void
ValidateInternetDlg::OnBnClickedOk()
{
  if(SaveValidate())
  {
    StyleDialog::OnOK();
  }
}

void
ValidateInternetDlg::OnBnClickedCancel()
{
  StyleDialog::OnCancel();
}

void
ValidateInternetDlg::OnExit()
{
  if (SaveValidate())
  {
    StyleDialog::OnOK();
  }
}