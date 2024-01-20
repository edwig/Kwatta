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
// This File   : StepWindowsDlg.cpp
// What it does: Main editor for one database SQL test call
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "StepEditor.h"
#include "StepWindowsDlg.h"
#include "DatabaseDlg.h"
#include "TimingDlg.h"
#include "ScriptDlg.h"
#include "WinActionsDlg.h"
#include "WinResultDlg.h"
#include <AboutDlg.h>
#include <SearchVarDlg.h>
#include <TestStepWIN.h>
#include <StdException.h>
#include "afxdialogex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// StepWindowsDlg dialog

IMPLEMENT_DYNAMIC(StepWindowsDlg, StyleDialog)

StepWindowsDlg::StepWindowsDlg(CWnd* p_parent)
               :StyleDialog(IDD_STEPEDITOR_WIN,p_parent)
{
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

  m_page1 = new WinActionsDlg(this);
  m_page2 = new TimingDlg(this);
  m_page3 = new ScriptDlg(this);
  m_page4 = new WinResultDlg(this);
}

StepWindowsDlg::~StepWindowsDlg()
{
  delete m_page1;
  delete m_page2;
  delete m_page3;
  delete m_page4;
  
  if(m_testStep)
  {
    delete m_testStep;
    m_testStep = nullptr;
  }
}

void 
StepWindowsDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);
  DDX_Control (pDX,IDC_STEPNAME, m_editName,   m_name);
  DDX_Control (pDX,IDC_GLOBAL,   m_buttonGlobal);
  DDX_Control (pDX,IDC_COMMENT , m_editComment,m_comment);
  DDX_Control (pDX,IDC_BOUND,    m_editBound,  m_bound);
  DDX_Control (pDX,IDC_TABS,     m_tabs);
  DDX_Control (pDX,IDC_GO,       m_buttonGO);
  DDX_Control (pDX,IDOK,         m_buttonOK);
  DDX_Control (pDX,IDCANCEL,     m_buttonCancel);

  if(pDX->m_bSaveAndValidate == FALSE)
  {
    m_buttonGlobal.EnableWindow(!theApp.GetGlobal());
  }
}

BEGIN_MESSAGE_MAP(StepWindowsDlg, StyleDialog)
  ON_EN_KILLFOCUS (IDC_STEPNAME,&StepWindowsDlg::OnEnChangeStepname)
  ON_BN_CLICKED   (IDC_GLOBAL,  &StepWindowsDlg::OnBnClickedGlobal)
  ON_EN_KILLFOCUS (IDC_COMMENT, &StepWindowsDlg::OnEnChangeComment)
  ON_BN_CLICKED   (IDC_GO,      &StepWindowsDlg::OnBnClickedGO)
  ON_BN_CLICKED   (IDOK,        &StepWindowsDlg::OnBnClickedOk)

  ON_COMMAND(ID_MENU_THEMELIME,       OnStyleLime)
  ON_COMMAND(ID_MENU_THEMESKYBLUE,    OnStyleSkyblue)
  ON_COMMAND(ID_MENU_THEMEPURPLE,     OnStylePurple)
  ON_COMMAND(ID_MENU_THEMEMODERATE,   OnStyleModerateGray)
  ON_COMMAND(ID_MENU_THEMEPURE,       OnStylePureGray)
  ON_COMMAND(ID_MENU_THEMEBLACK,      OnStyleBlackWhite)
  ON_COMMAND(ID_MENU_THEMEDARK,       OnStyleDark)
  ON_COMMAND(ID_MENU_ABOUT,           OnAbout)
  ON_COMMAND(ID_MENU_EXIT,            OnExit)
END_MESSAGE_MAP()

BOOL
StepWindowsDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();
  SetWindowText("StepEditor Windows UI");
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
StepWindowsDlg::SetupDynamicLayout()
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
  manager.AddItem(IDC_TABS,       CMFCDynamicLayout::MoveVertical(20),                  CMFCDynamicLayout::SizeHorizontalAndVertical(100,80));
  manager.AddItem(IDOK,           CMFCDynamicLayout::MoveHorizontalAndVertical(100,100),CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDCANCEL,       CMFCDynamicLayout::MoveHorizontalAndVertical(100,100),CMFCDynamicLayout::SizeNone());
}

void
StepWindowsDlg::InitTabs()
{
  m_page1->Create(IDD_WIN_ACTIONS,&m_tabs);
  m_page2->Create(IDD_TIMING,     &m_tabs);
  m_page3->Create(IDD_SCRIPT,     &m_tabs);
  m_page4->Create(IDD_WINRESULT,  &m_tabs);
 
  m_tabs.InsertItem(1,m_page1,"Win UI Actions");
  m_tabs.InsertItem(2,m_page2,"Timing");
  m_tabs.InsertItem(3,m_page3,"Script");
  m_tabs.InsertItem(4,m_page4,"Result");

  m_tabs.Init();
}

void
StepWindowsDlg::InitStep()
{
  CString filename = theApp.GetEffectiveStep();

  // Read in the definition file for a test step
  try
  {
    m_testStep = new TestStepWIN();
    m_testStep->ReadFromXML(filename);
  }
  catch (StdException& ex)
  {
    StyleMessageBox(this, ex.GetErrorMessage(), PRODUCT_NAME, MB_OK | MB_ICONERROR);
    theApp.GetMainWnd()->PostMessage(WM_QUIT, 0, 0);
  }
}

void
StepWindowsDlg::InitGlobalParameters()
{
  CString filename = theApp.GetBaseDirectory() + "Parameters.xpar";

  ReadParameters(filename);
}

void
StepWindowsDlg::InitParameters()
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
StepWindowsDlg::ReadParameters(CString p_file, bool p_global /*= true*/)
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
StepWindowsDlg::EffectiveParameters()
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
    // m_page4->InitTab(m_testStep);

    UpdateData(FALSE);
    m_busy = false;
  }
}

void
StepWindowsDlg::LoadVariables()
{
  m_name    = m_testStep->GetName();
  m_comment = m_testStep->GetDocumentation();
}

void
StepWindowsDlg::LoadVariablesTabs()
{
  // Initialize all the tabs
  m_page1->InitTab(m_testStep,&m_parameters);
  m_page2->InitTab(m_testStep,&m_parameters);
  m_page3->InitTab(m_testStep,&m_parameters);

  // ResetStepResult();
}

void
StepWindowsDlg::ResetStepResult()
{
  m_page4->InitTab();
}

void 
StepWindowsDlg::SetStepResult(StepResultWIN* p_result)
{
  m_page4->SetResult(p_result);
}

void
StepWindowsDlg::RefreshEffective()
{
  m_page4->InitTab();
}

bool
StepWindowsDlg::SaveStep()
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
StepWindowsDlg::StoreVariables()
{
  // General variables above the tabs
  m_testStep->SetName(m_name);
  m_testStep->SetDocumentation(m_comment);

  // Take all the changes from the tabs
  m_page1->StoreVariables();
  m_page2->StoreVariables();
  m_page3->StoreVariables();
}

// StepWindowsDlg message handlers

void
StepWindowsDlg::OnAbout()
{
  AutoFocus focus;
  AboutDlg dlgAbout(this);
  dlgAbout.DoModal();
}

void 
StepWindowsDlg::OnEnChangeStepname()
{
  UpdateData();
}

void
StepWindowsDlg::OnBnClickedGlobal()
{
  if(StyleMessageBox(this,"Promote this test step to a global teststep?",PRODUCT_NAME,MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION) == IDYES)
  {
    theApp.PromoteTestStep(this);
  }
}

void 
StepWindowsDlg::OnEnChangeComment()
{
  UpdateData();
}

void
StepWindowsDlg::OnBnClickedGO()
{
  CWaitCursor sigh;
  m_buttonGO.SetBkColor(RGB(255, 0, 0));
  m_buttonGO.Invalidate();
  Redraw();
  if(SaveStep())
  {
    theApp.StartTheWINRunner(this);
  }
  m_buttonGO.SetBkColor(RGB(0, 255, 0));
  m_buttonGO.Invalidate();
  Redraw();
}

void 
StepWindowsDlg::OnBnClickedOk()
{
  CWaitCursor sigh;
  if(SaveStep())
  {
    StyleDialog::OnOK();
  }
}

void
StepWindowsDlg::OnExit()
{
  CWaitCursor sigh;
  if(SaveStep())
  {
    StyleDialog::OnOK();
  }
}

void
StepWindowsDlg::Redraw()
{
  // Handle all paint messages for a short period of time
  MSG msg;
  UINT ticks = GetTickCount();
  while(GetTickCount() - ticks < 500 && PeekMessage(&msg,NULL,WM_MOVE,WM_USER,PM_REMOVE))
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
