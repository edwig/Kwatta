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
// This program: StepEditor
// This File   : StepDatabaseDlg.cpp
// What it does: Main editor for one database SQL test call
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "StepEditor.h"
#include "StepDatabaseDlg.h"
#include "SQLDlg.h"
#include "DatabaseDlg.h"
#include "TimingDlg.h"
#include "RawSQLDlg.h"
#include "SQLResultDlg.h"
#include "SQLOutputDlg.h"
#include "ScriptDlg.h"
#include <AboutDlg.h>
#include <SearchVarDlg.h>
#include <TestStepSQL.h>
#include <StepResultSQL.h>
#include <StdException.h>
#include "afxdialogex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// StepDatabaseDlg dialog

IMPLEMENT_DYNAMIC(StepDatabaseDlg, StyleDialog)

StepDatabaseDlg::StepDatabaseDlg(CWnd* p_parent)
                :StyleDialog(IDD_STEPEDITOR_SQL,p_parent)
{
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

  m_page1 = new SQLDlg(this);
  m_page2 = new DatabaseDlg(this);
  m_page3 = new TimingDlg(this);
  m_page4 = new RawSQLDlg(this);
  m_page5 = new ScriptDlg(this);
// 
  m_page10 = new SQLResultDlg(this);
  m_page11 = new SQLOutputDlg(this);
}

StepDatabaseDlg::~StepDatabaseDlg()
{
  delete m_page1;
  delete m_page2;
  delete m_page3;
  delete m_page4;
  delete m_page5;
  delete m_page10;
  delete m_page11;

  if(m_testStep)
  {
    delete m_testStep;
    m_testStep = nullptr;
  }
}

void 
StepDatabaseDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);
  DDX_Control (pDX,IDC_STEPNAME, m_editName,   m_name);
  DDX_Control (pDX,IDC_GLOBAL,   m_buttonGlobal);
  DDX_Control (pDX,IDC_COMMENT , m_editComment,m_comment);
  DDX_Control (pDX,IDC_BOUND,    m_editBound,  m_bound);
  DDX_Control (pDX,IDC_TAB_REQ,  m_tabsRequest);
  DDX_Control (pDX,IDC_TAB_RES,  m_tabsResponse);
  DDX_Control (pDX,IDC_GO,       m_buttonGO);
  DDX_Control (pDX,IDOK,         m_buttonOK);
  DDX_Control (pDX,IDCANCEL,     m_buttonCancel);

  if(pDX->m_bSaveAndValidate == FALSE)
  {
    m_buttonGlobal.EnableWindow(!theApp.GetGlobal());
  }
}

BEGIN_MESSAGE_MAP(StepDatabaseDlg, StyleDialog)
  ON_EN_KILLFOCUS (IDC_STEPNAME,&StepDatabaseDlg::OnEnChangeStepname)
  ON_BN_CLICKED   (IDC_GLOBAL,  &StepDatabaseDlg::OnBnClickedGlobal)
  ON_EN_KILLFOCUS (IDC_COMMENT, &StepDatabaseDlg::OnEnChangeComment)
  ON_BN_CLICKED   (IDC_GO,      &StepDatabaseDlg::OnBnClickedGO)
  ON_BN_CLICKED   (IDOK,        &StepDatabaseDlg::OnBnClickedOk)

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

BOOL
StepDatabaseDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();
  SetWindowText("StepEditor SQL");
  ShowMinMaxButton();
  SetSysMenu(IDR_MENU);

  // Add "About..." menu item to system menu.

  // IDM_ABOUTBOX must be in the system command range.
  ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
  ASSERT (IDM_ABOUTBOX < 0xF000);

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
      pSysMenu->AppendMenu(MF_STRING,IDM_ABOUTBOX,strAboutMenu);
    }
  }

  // Set the icon for this dialog.  The framework does this automatically
  //  when the application's main window is not a dialog
  SetIcon(m_hIcon, TRUE);			// Set big icon
  SetIcon(m_hIcon, FALSE);		// Set small icon

  InitTabs();
  InitStep();
  InitGlobalParameters();
  InitParameters();

  // Fill in the variables: After the loading of the XML files
  LoadVariables();
  LoadVariablesTabs();

  m_buttonOK.SetStyle("ok");
  m_buttonCancel.SetStyle("can");
  m_editBound.SetMutable(false);
  m_editBound.SetBorderColor(RGB(0, 255, 0));
  m_buttonGO.SetIconImage(IDI_RUN);
  m_buttonGlobal.SetIconImage(IDI_EARTH);

  if(theApp.GetGlobal())
  {
    m_editName.SetBkColor(GLOBAL_COLOR);
    m_editComment.SetBkColor(GLOBAL_COLOR);
  }

  EnableToolTips();
  RegisterTooltip(m_buttonGlobal, "Promote teststep to a global teststep");

  EffectiveParameters();

  SetCanResize();
  UpdateData(FALSE);
  return TRUE;
}

void
StepDatabaseDlg::SetupDynamicLayout()
{
  StyleDialog::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif

  manager.AddItem(IDC_GRP_STEP,   CMFCDynamicLayout::MoveNone(),                        CMFCDynamicLayout::SizeHorizontalAndVertical(100, 20));
  manager.AddItem(IDC_STEPNAME,   CMFCDynamicLayout::MoveNone(),                        CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_GLOBAL,     CMFCDynamicLayout::MoveHorizontal(100),               CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_BOUND,      CMFCDynamicLayout::MoveHorizontal(100),               CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_COMMENT,    CMFCDynamicLayout::MoveNone(),                        CMFCDynamicLayout::SizeHorizontalAndVertical(100,20));
  manager.AddItem(IDC_GO,         CMFCDynamicLayout::MoveVertical(20),                  CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_TAB_REQ,    CMFCDynamicLayout::MoveVertical(20),                  CMFCDynamicLayout::SizeHorizontalAndVertical(50,80));
  manager.AddItem(IDC_TAB_RES,    CMFCDynamicLayout::MoveHorizontalAndVertical( 50, 20),CMFCDynamicLayout::SizeHorizontalAndVertical(50,80));
  manager.AddItem(IDOK,           CMFCDynamicLayout::MoveHorizontalAndVertical(100,100),CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDCANCEL,       CMFCDynamicLayout::MoveHorizontalAndVertical(100,100),CMFCDynamicLayout::SizeNone());
}

void
StepDatabaseDlg::InitTabs()
{
  m_page1->Create(IDD_SQL,      &m_tabsRequest);
  m_page2->Create(IDD_DATABASE, &m_tabsRequest);
  m_page3->Create(IDD_TIMING,   &m_tabsRequest);
  m_page4->Create(IDD_RAW_SQL,  &m_tabsRequest);
  m_page5->Create(IDD_SCRIPT,   &m_tabsRequest);
 
  m_tabsRequest.InsertItem(0,m_page1,"SQL");
  m_tabsRequest.InsertItem(1,m_page2,"Database");
  m_tabsRequest.InsertItem(2,m_page3,"Timing");
  m_tabsRequest.InsertItem(3,m_page4,"Raw SQL");
  m_tabsRequest.InsertItem(4,m_page5,"Script");

  m_tabsRequest.Init();
 
  m_page10->Create(IDD_SQLRESULT,&m_tabsResponse);
  m_page11->Create(IDD_SQLOUTPUT,&m_tabsResponse);

  m_tabsResponse.InsertItem(0,m_page10,"SQL Result");
  m_tabsResponse.InsertItem(1,m_page11,"Output parameters");

  m_tabsResponse.Init();
}

void
StepDatabaseDlg::InitStep()
{
  CString filename = theApp.GetEffectiveStep();

  // Read in the definition file for a test step
  try
  {
    m_testStep = new TestStepSQL();
    m_testStep->ReadFromXML(filename);
  }
  catch (StdException& ex)
  {
    StyleMessageBox(this, ex.GetErrorMessage(), PRODUCT_NAME, MB_OK | MB_ICONERROR);
    theApp.GetMainWnd()->PostMessage(WM_QUIT, 0, 0);
  }
}

void
StepDatabaseDlg::InitGlobalParameters()
{
  CString filename = theApp.GetBaseDirectory() + "Parameters.xpar";

  ReadParameters(filename);
}

void
StepDatabaseDlg::InitParameters()
{
  CString filename = theApp.GetBaseDirectory() + theApp.GetTestDirectory() + theApp.GetParametersFilename();

  // Possibly no parameter file bound to the step!
  if(filename.IsEmpty())
  {
    return;
  }
  ReadParameters(filename, false);
}

void
StepDatabaseDlg::ReadParameters(CString p_file, bool p_global /*= true*/)
{
  // read the definition of the parameters
  try
  {
    m_parameters.ReadFromXML(p_file,p_global);
  }
  catch(StdException& ex)
  {
    StyleMessageBox(this,ex.GetErrorMessage(),PRODUCT_NAME,MB_OK | MB_ICONERROR);
    theApp.GetMainWnd()->PostMessage(WM_QUIT, 0, 0);
  }
}

void
StepDatabaseDlg::EffectiveParameters()
{
  if(m_busy == false && m_testStep)
  {
    m_busy = true;

    CString bound("Parameters: OK");

    // Effectuate the parameters
    StoreVariables();
    m_unbound = m_testStep->EffectiveReplacements(&m_parameters,true);

    if(m_unbound > 0)
    {
      bound.Format("Unbound parameters: %d", m_unbound);
    }
    m_bound = bound;

    m_editBound.SetTextColor  (m_unbound > 0 ? RGB(255,0,0) : RGB(0,255,0));
    m_editBound.SetBorderColor(m_unbound > 0 ? RGB(255,0,0) : RGB(0,255,0));

    // Set changes on tab 4 (Raw SQL)
    m_page4->InitTab(m_testStep);

    UpdateData(FALSE);
    m_busy = false;
  }
}

void
StepDatabaseDlg::LoadVariables()
{
  m_name    = m_testStep->GetName();
  m_comment = m_testStep->GetDocumentation();
}

void
StepDatabaseDlg::LoadVariablesTabs()
{
  // Initialize all the tabs
  m_page1->InitTab(m_testStep,&m_parameters);
  m_page2->InitTab(m_testStep,&m_parameters);
  m_page3->InitTab(m_testStep,&m_parameters);
  m_page4->InitTab(m_testStep);
  m_page5->InitTab(m_testStep,&m_parameters);

  ResetStepResult();
}

void
StepDatabaseDlg::ResetStepResult()
{
  m_page10->InitTab();
  m_page11->InitTab();
}

void 
StepDatabaseDlg::SetStepResult(StepResultSQL* p_result)
{
  m_page10->SetResult(p_result);
  m_page11->SetResult(p_result);
}

void
StepDatabaseDlg::RefreshEffective()
{
  m_page4->InitTab(m_testStep);
}

bool
StepDatabaseDlg::SaveStep()
{
  StoreVariables();

  CString filenameStep = theApp.GetEffectiveStep();
  CString filenameParm = theApp.GetBaseDirectory() + theApp.GetTestDirectory() + theApp.GetParametersFilename();

  try
  {
    bool res1 = m_testStep->WriteToXML(filenameStep);
    bool res2 = m_parameters.WriteToXML();

    if(!res1 || !res2)
    {
      StyleMessageBox(this,"Could not save the test step",PRODUCT_NAME,MB_OK|MB_ICONWARNING);
      return false;
    }
    return true;
  }
  catch (StdException& ex)
  {
    StyleMessageBox(this, ex.GetErrorMessage(), PRODUCT_NAME, MB_OK | MB_ICONERROR);
  }
  return false;
}

void
StepDatabaseDlg::StoreVariables()
{
  // General variables above the tabs
  m_testStep->SetName(m_name);
  m_testStep->SetDocumentation(m_comment);

  // Take all the changes from the tabs
  m_page1->StoreVariables();
  m_page2->StoreVariables();
  m_page3->StoreVariables();
  m_page4->StoreVariables();
  m_page5->StoreVariables();
}

// StepDatabaseDlg message handlers

void
StepDatabaseDlg::OnAbout()
{
  AutoFocus focus;
  AboutDlg dlgAbout(this);
  dlgAbout.DoModal();
}

void 
StepDatabaseDlg::OnEnChangeStepname()
{
  UpdateData();
}

void
StepDatabaseDlg::OnBnClickedGlobal()
{
  if(StyleMessageBox(this,"Promote this test step to a global teststep?",PRODUCT_NAME,MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION) == IDYES)
  {
    theApp.PromoteTestStep(this);
  }
}

void 
StepDatabaseDlg::OnEnChangeComment()
{
  UpdateData();
}

void
StepDatabaseDlg::OnBnClickedGO()
{
  CWaitCursor sigh;
  m_buttonGO.SetBkColor(RGB(255, 0, 0));
  m_buttonGO.Invalidate();
  Redraw();
  if(SaveStep())
  {
    theApp.StartTheSQLRunner(this);
  }
  m_buttonGO.SetBkColor(RGB(0, 255, 0));
  m_buttonGO.Invalidate();
  Redraw();
}

void 
StepDatabaseDlg::OnBnClickedOk()
{
  CWaitCursor sigh;
  if(SaveStep())
  {
    StyleDialog::OnOK();
  }
}

void
StepDatabaseDlg::OnExit()
{
  CWaitCursor sigh;
  if(SaveStep())
  {
    StyleDialog::OnOK();
  }
}

void
StepDatabaseDlg::Redraw()
{
  // Handle all paint messages for a short period of time
  MSG msg;
  UINT ticks = GetTickCount();
  while (GetTickCount() - ticks < 500 && PeekMessage(&msg, NULL, WM_MOVE, WM_USER, PM_REMOVE))
  {
    try
    {
      ::TranslateMessage(&msg);
      ::DispatchMessage(&msg);
    }
    catch (...)
    {
      // How now, brown cow?
    }
  }
}