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
// This File   : StepInternetDlg.cpp
// What it does: Main editor for one internet HTTP test call
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "StepEditor.h"
#include "StepInternetDlg.h"
#include "RequestDlg.h"
#include "URLParameterDlg.h"
#include "UrlHeaderDlg.h"
#include "AuthenticateDlg.h"
#include "RawRequestDlg.h"
#include "ResponseDlg.h"
#include "RespHeadersDlg.h"
#include "RawResponseDlg.h"
#include <AboutDlg.h>
#include <SearchVarDlg.h>
#include <TestStepIN.h>
#include <StepResultIN.h>
#include <StdException.h>
#include "afxdialogex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// StepInternetDlg dialog

IMPLEMENT_DYNAMIC(StepInternetDlg, StyleDialog)

StepInternetDlg::StepInternetDlg(CWnd* pParent /*=nullptr*/)
                :StyleDialog(IDD_STEPEDITOR_INET, pParent)
{
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

  m_page1 = new RequestDlg(this);
  m_page2 = new URLParameterDlg(this);
  m_page3 = new UrlHeaderDlg(this);
  m_page4 = new AuthenticateDlg(this);
  m_page5 = new RawRequestDlg(this);

  m_page10 = new ResponseDlg(this);
  m_page11 = new RespHeadersDlg(this);
  m_page12 = new RawResponseDlg(this);
}

StepInternetDlg::~StepInternetDlg()
{
  delete m_page1;
  delete m_page2;
  delete m_page3;
  delete m_page4;
  delete m_page5;

  delete m_page10;
  delete m_page11;
  delete m_page12;

  if(m_testStep)
  {
    delete m_testStep;
    m_testStep = nullptr;
  }
}

void 
StepInternetDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);
  DDX_Control (pDX,IDC_STEPNAME, m_editName,   m_name);
  DDX_Control (pDX,IDC_GLOBAL,   m_buttonGlobal);
  DDX_Control (pDX,IDC_COMMENT , m_editComment,m_comment);
  DDX_Control (pDX,IDC_BOUND,    m_editBound,  m_bound);
  DDX_CBString(pDX,IDC_VERB,     m_comboVerb,  m_verb);
  DDX_Control (pDX,IDC_URL,      m_editUrl,    m_url);
  DDX_Control (pDX,IDC_URL_PARM, m_buttonUrlParm);
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

BEGIN_MESSAGE_MAP(StepInternetDlg, StyleDialog)
  ON_EN_KILLFOCUS (IDC_STEPNAME,&StepInternetDlg::OnEnChangeStepname)
  ON_BN_CLICKED   (IDC_GLOBAL,  &StepInternetDlg::OnBnClickedGlobal)
  ON_EN_KILLFOCUS (IDC_COMMENT, &StepInternetDlg::OnEnChangeComment)
  ON_CBN_SELCHANGE(IDC_VERB,    &StepInternetDlg::OnCbnSelchangeVerb)
  ON_EN_KILLFOCUS (IDC_URL,     &StepInternetDlg::OnEnChangeUrl)
  ON_BN_CLICKED   (IDC_URL_PARM,&StepInternetDlg::OnBnClickedUrlParm)
  ON_BN_CLICKED   (IDC_GO,      &StepInternetDlg::OnBnClickedGO)
  ON_BN_CLICKED   (IDOK,        &StepInternetDlg::OnBnClickedOk)

  ON_COMMAND(ID_MENU_THEMELIME,       OnStyleLime)
  ON_COMMAND(ID_MENU_THEMESKYBLUE,    OnStyleSkyblue)
  ON_COMMAND(ID_MENU_THEMEPURPLE,     OnStylePurple)
  ON_COMMAND(ID_MENU_THEMEMODERATE,   OnStyleModerateGray)
  ON_COMMAND(ID_MENU_THEMEPURE,       OnStylePureGray)
  ON_COMMAND(ID_MENU_THEMEBLACK,      OnStyleBlackWhite)
  ON_COMMAND(ID_MENU_ABOUT,           OnAbout)
  ON_COMMAND(ID_MENU_EXIT,            OnExit)
END_MESSAGE_MAP()

BOOL
StepInternetDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();
  SetWindowText("StepEditor");
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
  InitVerbs();
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
  m_buttonUrlParm.SetIconImage(IDI_LIST);
  m_buttonGO.SetIconImage(IDI_RUN);
  m_buttonGlobal.SetIconImage(IDI_EARTH);

  if(theApp.GetGlobal())
  {
    m_editName.SetBkColor(GLOBAL_COLOR);
    m_editComment.SetBkColor(GLOBAL_COLOR);
  }

  EnableToolTips();
  RegisterTooltip(m_buttonUrlParm,"Choose global/test parameter(s) for the URL");
  RegisterTooltip(m_buttonGlobal, "Promote teststep to a global teststep");

  EffectiveParameters();

  SetCanResize();
  UpdateData(FALSE);
  return TRUE;
}

void
StepInternetDlg::SetupDynamicLayout()
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
  manager.AddItem(IDC_GRP_URL,    CMFCDynamicLayout::MoveVertical(20),                  CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_GO,         CMFCDynamicLayout::MoveVertical(20),                  CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_VERB,       CMFCDynamicLayout::MoveVertical(20),                  CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_URL,        CMFCDynamicLayout::MoveVertical(20),                  CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_URL_PARM,   CMFCDynamicLayout::MoveHorizontalAndVertical(100,20), CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_TAB_REQ,    CMFCDynamicLayout::MoveVertical(20),                  CMFCDynamicLayout::SizeHorizontalAndVertical(50,80));
  manager.AddItem(IDC_TAB_RES,    CMFCDynamicLayout::MoveHorizontalAndVertical( 50, 20),CMFCDynamicLayout::SizeHorizontalAndVertical(50,80));
  manager.AddItem(IDOK,           CMFCDynamicLayout::MoveHorizontalAndVertical(100,100),CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDCANCEL,       CMFCDynamicLayout::MoveHorizontalAndVertical(100,100),CMFCDynamicLayout::SizeNone());
}

void
StepInternetDlg::InitTabs()
{
  m_page1->Create(IDD_REQUEST,      &m_tabsRequest);
  m_page2->Create(IDD_HTTP_PARAM,   &m_tabsRequest);
  m_page3->Create(IDD_URL_HEADERS,  &m_tabsRequest);
  m_page4->Create(IDD_AUTHENTICATE, &m_tabsRequest);
  m_page5->Create(IDD_RAW_REQUEST,  &m_tabsRequest);

  m_tabsRequest.InsertItem(0, m_page1, "Request");
  m_tabsRequest.InsertItem(1, m_page2, "Parameters");
  m_tabsRequest.InsertItem(2, m_page3, "Headers");
  m_tabsRequest.InsertItem(3, m_page4, "Authenticate");
  m_tabsRequest.InsertItem(4, m_page5, "Raw request");

  m_tabsRequest.Init();

  m_page10->Create(IDD_RESPONSE,    &m_tabsResponse);
  m_page11->Create(IDD_RESP_HEADERS,&m_tabsResponse);
  m_page12->Create(IDD_RAW_RESPONSE,&m_tabsResponse);

  m_tabsResponse.InsertItem(0,m_page10,"Response");
  m_tabsResponse.InsertItem(1,m_page11,"Headers");
  m_tabsResponse.InsertItem(2,m_page12,"Raw response");

  m_tabsResponse.Init();
}

void
StepInternetDlg::InitVerbs()
{
  m_comboVerb.AddString("GET");
  m_comboVerb.AddString("POST");
  m_comboVerb.AddString("PATCH");
  m_comboVerb.AddString("UPDATE");
  m_comboVerb.AddString("PUT");
  m_comboVerb.AddString("DELETE");
  m_comboVerb.AddString("TRACE");
  m_comboVerb.AddString("OPTIONS");
  m_comboVerb.AddString("CONNECT");
  m_comboVerb.AddString("HEAD");
  m_comboVerb.AddString("PROPFIND");
  m_comboVerb.AddString("PROPPATCH");
  m_comboVerb.AddString("MKCOL");
  m_comboVerb.AddString("COPY");
  m_comboVerb.AddString("MOVE");
  m_comboVerb.AddString("LOCK");
  m_comboVerb.AddString("UNLOCK");
  m_comboVerb.AddString("VERSION-CONTROL");
  m_comboVerb.AddString("REPORT");
  m_comboVerb.AddString("CHECKOUT");
  m_comboVerb.AddString("CHECKIN");
  m_comboVerb.AddString("UNCHECKOUT");
  m_comboVerb.AddString("MKWORKSPACE");
  m_comboVerb.AddString("LABEL");
  m_comboVerb.AddString("MERGE");
  m_comboVerb.AddString("BASELINE-CONTROL");
  m_comboVerb.AddString("MKACTIVITY");
  m_comboVerb.AddString("ORDERPATCH");
  m_comboVerb.AddString("ACL");
  m_comboVerb.AddString("SEARCH");

  m_comboVerb.SetCurSel(0);
  m_comboVerb.SetMaxDroppedWidth(200);
  m_comboVerb.SetMinVisibleItems(40);
}

void
StepInternetDlg::InitStep()
{
  CString filename = theApp.GetEffectiveStep();

  // Read in the definition file for a test step
  try
  {
    m_testStep = new TestStepIN();
    m_testStep->ReadFromXML(filename);
  }
  catch (StdException& ex)
  {
    StyleMessageBox(this, ex.GetErrorMessage(), PRODUCT_NAME, MB_OK | MB_ICONERROR);
    theApp.GetMainWnd()->PostMessage(WM_QUIT, 0, 0);
  }
}

void
StepInternetDlg::InitGlobalParameters()
{
  CString filename = theApp.GetBaseDirectory() + "Parameters.xpar";

  ReadParameters(filename);
}

void
StepInternetDlg::InitParameters()
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
StepInternetDlg::ReadParameters(CString p_file, bool p_global /*= true*/)
{
  // read the definition of the parameters
  try
  {
    m_parameters.ReadFromXML(p_file, p_global);
  }
  catch(StdException& ex)
  {
    StyleMessageBox(this,ex.GetErrorMessage(),PRODUCT_NAME,MB_OK | MB_ICONERROR);
    theApp.GetMainWnd()->PostMessage(WM_QUIT, 0, 0);
  }
}

void
StepInternetDlg::EffectiveParameters()
{
  if(m_busy == false)
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

    // Set changes on tab 4 (Raw request)
    m_page5->InitTab();

    UpdateData(FALSE);
    m_busy = false;
  }
}

void
StepInternetDlg::LoadVariables()
{
  m_name    = m_testStep->GetName();
  m_comment = m_testStep->GetDocumentation();
  m_verb    = m_testStep->GetVerb();
  m_url     = m_testStep->GetURL();

  int ind = m_comboVerb.FindStringExact(0,m_verb);
  m_comboVerb.SetCurSel(ind);
}

void
StepInternetDlg::LoadVariablesTabs()
{
  // Initialize all the tabs
  m_page1->InitTab(m_testStep,&m_parameters);
  m_page2->InitTab(m_testStep,&m_parameters);
  m_page3->InitTab(m_testStep,&m_parameters);
  m_page4->InitTab(m_testStep,&m_parameters);
  m_page5->InitTab(m_testStep);

  ResetStepResult();
}

void
StepInternetDlg::ResetStepResult()
{
  m_page10->InitTab();
  m_page11->InitTab();
  m_page12->InitTab();
}

void 
StepInternetDlg::SetStepResult(StepResultIN* p_result)
{
  m_page4 ->SetResult(p_result->GetBearerToken());
  m_page10->SetResult(p_result);
  m_page11->SetResult(p_result);
  m_page12->SetResult(p_result);
}

bool
StepInternetDlg::SaveStep()
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
StepInternetDlg::StoreVariables()
{
  // General variables above the tabs
  m_testStep->SetName(m_name);
  m_testStep->SetDocumentation(m_comment);
  m_testStep->SetVerb(m_verb);
  m_testStep->SetURL(m_url);

  // Take all the changes from the tabs
  m_page1->StoreVariables();
  m_page2->StoreVariables();
  m_page3->StoreVariables();
  m_page4->StoreVariables();
  m_page5->StoreVariables();
}

// StepInternetDlg message handlers

void
StepInternetDlg::OnAbout()
{
  AboutDlg dlgAbout(this);
  dlgAbout.DoModal();
}

void 
StepInternetDlg::OnEnChangeStepname()
{
  UpdateData();
}

void
StepInternetDlg::OnBnClickedGlobal()
{
  if(StyleMessageBox(this,"Promote this test step to a global teststep?",PRODUCT_NAME,MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION) == IDYES)
  {
    theApp.PromoteTestStep(this);
  }
}

void 
StepInternetDlg::OnEnChangeComment()
{
  UpdateData();
}

void 
StepInternetDlg::OnCbnSelchangeVerb()
{
  UpdateData();
  EffectiveParameters();
}

void 
StepInternetDlg::OnEnChangeUrl()
{
  UpdateData();
  EffectiveParameters();
}

void 
StepInternetDlg::OnBnClickedUrlParm()
{
  SearchVarDlg dlg(&m_parameters,true,true,true);
  if(dlg.DoModal() == IDOK || dlg.GetSaved())
  {
    CString var = dlg.GetChosenVariable();
    CString variable;
    switch (dlg.GetResultType())
    {
      case ParType::PAR_GLOBAL: variable.Format("$%s$",var.GetString()); break;
      case ParType::PAR_RETURN: variable.Format("[%s]",var.GetString()); break;
      case ParType::PAR_BUFFER: variable.Format("<%s>",var.GetString()); break;
    }
    m_editUrl.InsertAtCurPos(variable,0);
    OnEnChangeUrl();
  }
}

void
StepInternetDlg::OnBnClickedGO()
{
  if(SaveStep())
  {
    theApp.StartTheInetRunner(this);
  }
}

void 
StepInternetDlg::OnBnClickedOk()
{
  if(SaveStep())
  {
    StyleDialog::OnOK();
  }
}

void
StepInternetDlg::OnExit()
{
  if(SaveStep())
  {
    StyleDialog::OnOK();
  }
}