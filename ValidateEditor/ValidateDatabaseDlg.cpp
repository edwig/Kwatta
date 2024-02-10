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
// This program: ValidateEditor
// This File   : ValidateDatabaseDlg.cpp
// What it does: Main dialog for the validation of a SQL Database call
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "afxdialogex.h"
#include "ValidateEditor.h"
#include "ValidateDatabaseDlg.h"
#include "SQLSucceededDlg.h"
#include "SQLRowsDlg.h"
#include "SQLColsDlg.h"
#include "SQLSQLStateDlg.h"
#include "SQLNativeDlg.h"
#include "SQLDataDlg.h"
#include <AboutDlg.h>
#include "StdException.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// CValidateDatabaseDlg dialog

ValidateDatabaseDlg::ValidateDatabaseDlg(CWnd* pParent /*=nullptr*/)
                    :StyleDialog(IDD_VALIDATEEDITOR,pParent)
{
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

  m_tab1 = new SQLSucceededDlg(this);
  m_tab2 = new SQLRowsDlg(this);
  m_tab3 = new SQLColsDlg(this);
  m_tab4 = new SQLSQLStateDlg(this);
  m_tab5 = new SQLNativeDlg(this);
  m_tab6 = new SQLDataDlg(this);
}

ValidateDatabaseDlg::~ValidateDatabaseDlg()
{
  delete m_tab1;
  delete m_tab2;
  delete m_tab3;
  delete m_tab4;
  delete m_tab5;
  delete m_tab6;
}

void ValidateDatabaseDlg::DoDataExchange(CDataExchange* pDX)
{
  StyleDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_NAME,         m_editName,m_name);
  DDX_Control(pDX,IDC_GLOBAL,       m_buttonGlobal);
  DDX_Control(pDX,IDC_BOUND,        m_editBound,m_bound);
  DDX_Control(pDX,IDC_DOCUMENTATION,m_editDocumentation,m_documentation);
  DDX_Control(pDX,IDC_TABS,         m_tabs);
  DDX_Control(pDX,IDOK,             m_buttonOK);
  DDX_Control(pDX,IDCANCEL,         m_buttonCancel);

  if(pDX->m_bSaveAndValidate == FALSE)
  {
    m_buttonGlobal.EnableWindow(!theApp.GetGlobal());
  }
}

BEGIN_MESSAGE_MAP(ValidateDatabaseDlg,StyleDialog)
  ON_WM_SYSCOMMAND()
  ON_WM_PAINT()
  ON_WM_QUERYDRAGICON()
  ON_EN_CHANGE (IDC_NAME,         &ValidateDatabaseDlg::OnEnChangeName)
  ON_BN_CLICKED(IDC_GLOBAL,       &ValidateDatabaseDlg::OnBnClckedGlobal)
  ON_EN_CHANGE (IDC_DOCUMENTATION,&ValidateDatabaseDlg::OnEnChangeDocumentation)
  ON_BN_CLICKED(IDOK,             &ValidateDatabaseDlg::OnBnClickedOk)
  ON_BN_CLICKED(IDCANCEL,         &ValidateDatabaseDlg::OnBnClickedCancel)

  ON_COMMAND(ID_MENU_THEMELIME,       OnStyleLime)
  ON_COMMAND(ID_MENU_THEMESKYBLUE,    OnStyleSkyblue)
  ON_COMMAND(ID_MENU_THEMEPURPLE,     OnStylePurple)
  ON_COMMAND(ID_MENU_THEMEMUSTARD,    OnStyleMustard)
  ON_COMMAND(ID_MENU_THEMEMODERATE,   OnStyleModerateGray)
  ON_COMMAND(ID_MENU_THEMEPURE,       OnStylePureGray)
  ON_COMMAND(ID_MENU_THEMEBLACK,      OnStyleBlackWhite)
  ON_COMMAND(ID_MENU_THEMEDARK,       OnStyleDark)
  ON_COMMAND(ID_MENU_ABOUT,           OnAbout)
  ON_COMMAND(ID_MENU_EXIT,            OnExit)
END_MESSAGE_MAP()

// CValidateDatabaseDlg message handlers

BOOL ValidateDatabaseDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();
  SetWindowText("Validate Editor (Internet)");
  ShowMinMaxButton();
  SetSysMenu(IDR_MENU);
  SetAboutBoxAndIcon(IDM_ABOUTBOX,IDS_ABOUTBOX);

  // Read all the data
  InitTabs();
  InitButtons();
  InitValidation();
  InitParameters();

  // Effectuate the parameters on the members
  EffectiveParameters();

  // Fill in the variables: After the loading of the XML files
  LoadVariables();
  LoadVariablesTabs();

  m_editBound.SetMutable(false);
  m_editBound.SetBorderSize(2);

  if(theApp.GetGlobal())
  {
    m_editName.SetBkColor(GLOBAL_COLOR);
    m_editDocumentation.SetBkColor(GLOBAL_COLOR);
  }

  SetCanResize();
  UpdateData(FALSE);

  SetFirstUsedTab();

  return TRUE;  // return TRUE  unless you set the focus to a control
}

void
ValidateDatabaseDlg::SetupDynamicLayout()
{
  StyleDialog::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif

  manager.AddItem(IDC_GRP_VALID,    CMFCDynamicLayout::MoveNone(),                        CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_NAME,         CMFCDynamicLayout::MoveNone(),                        CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_GLOBAL,       CMFCDynamicLayout::MoveHorizontal(100),               CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_BOUND,        CMFCDynamicLayout::MoveHorizontal(100),               CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_DOCUMENTATION,CMFCDynamicLayout::MoveNone(),                        CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_TABS,         CMFCDynamicLayout::MoveNone(),                        CMFCDynamicLayout::SizeHorizontalAndVertical(100,100));
  manager.AddItem(IDOK,             CMFCDynamicLayout::MoveHorizontalAndVertical(100,100),CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDCANCEL,         CMFCDynamicLayout::MoveHorizontalAndVertical(100,100),CMFCDynamicLayout::SizeNone());
}

void
ValidateDatabaseDlg::InitTabs()
{
   m_tab1->Create(IDD_SQL_RETURN,  &m_tabs);
   m_tab2->Create(IDD_SQL_ROWS,    &m_tabs);
   m_tab3->Create(IDD_SQL_COLS,    &m_tabs);
   m_tab4->Create(IDD_SQL_SQLSTATE,&m_tabs);
   m_tab5->Create(IDD_SQL_NATIVE,  &m_tabs);
   m_tab6->Create(IDD_SQL_DATA,    &m_tabs);
//  
   m_tabs.InsertItem(0,m_tab1,"Succeeded");
   m_tabs.InsertItem(1,m_tab2,"Rows");
   m_tabs.InsertItem(2,m_tab3,"Columns");
   m_tabs.InsertItem(3,m_tab4,"SQLSTATE");
   m_tabs.InsertItem(4,m_tab5,"Error");
   m_tabs.InsertItem(5,m_tab6,"Data");

  m_tabs.Init();
}

void
ValidateDatabaseDlg::InitButtons()
{
  m_buttonGlobal.SetIconImage(IDI_EARTH);
  m_buttonOK.SetStyle("ok");
  m_buttonCancel.SetStyle("can");

  EnableToolTips();
  RegisterTooltip(m_buttonGlobal,"Promote validation to a global validation");
}

void
ValidateDatabaseDlg::InitValidation()
{
  CString filename = theApp.GetEffectiveValidation();

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
ValidateDatabaseDlg::InitParameters()
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
ValidateDatabaseDlg::EffectiveParameters()
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
ValidateDatabaseDlg::LoadVariables()
{
  m_name = m_validate.GetName();
  m_documentation = m_validate.GetDocumentation();
}

void
ValidateDatabaseDlg::LoadVariablesTabs()
{
  m_tab1->InitTab(&m_validate,&m_parameters);
  m_tab2->InitTab(&m_validate,&m_parameters);
  m_tab3->InitTab(&m_validate,&m_parameters);
  m_tab4->InitTab(&m_validate,&m_parameters);
  m_tab5->InitTab(&m_validate,&m_parameters);
  m_tab6->InitTab(&m_validate,&m_parameters);
}

void
ValidateDatabaseDlg::StoreVariables()
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
  m_tab6->StoreVariables();
}

bool
ValidateDatabaseDlg::SaveValidate()
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
ValidateDatabaseDlg::SetFirstUsedTab()
{
  int tab = 0;

       if (m_validate.GetCheckSucceeded())    tab = 0;
  else if (m_validate.GetCheckRows())         tab = 1;
  else if (m_validate.GetCheckCols())         tab = 2;
  else if (m_validate.GetCheckSQLState())     tab = 3;
  else if (m_validate.GetCheckNativeStatus()) tab = 4;
  else if (m_validate.GetCheckData())         tab = 5;

  m_tabs.SetCurFocus(tab);
}

//////////////////////////////////////////////////////////////////////////
//
// Message Handlers
//
//////////////////////////////////////////////////////////////////////////

void
ValidateDatabaseDlg::OnAbout()
{
  AutoFocus focus;
  AboutDlg dlgAbout(this);
  dlgAbout.DoModal();
}

void
ValidateDatabaseDlg::OnSysCommand(UINT nID,LPARAM lParam)
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

void ValidateDatabaseDlg::OnPaint()
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
ValidateDatabaseDlg::OnQueryDragIcon()
{
  return static_cast<HCURSOR>(m_hIcon);
}

void
ValidateDatabaseDlg::OnEnChangeName()
{
  UpdateData();
}

void 
ValidateDatabaseDlg::OnBnClckedGlobal()
{
  if(StyleMessageBox(this,"Promote this validation to a global validation?",PRODUCT_NAME,MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION) == IDYES)
  {
    theApp.PromoteValidation(this);
  }
}

void
ValidateDatabaseDlg::OnEnChangeDocumentation()
{
  UpdateData();
}

void
ValidateDatabaseDlg::OnBnClickedOk()
{
  if(SaveValidate())
  {
    StyleDialog::OnOK();
  }
}

void
ValidateDatabaseDlg::OnBnClickedCancel()
{
  StyleDialog::OnCancel();
}

void
ValidateDatabaseDlg::OnExit()
{
  if (SaveValidate())
  {
    StyleDialog::OnOK();
  }
}
