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
// This File   : StepCommandDlg.cpp
// What it does: Main dialog for editing a command-line test step
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StepEditor.h"
#include "StepCommandDlg.h"
#include <AboutDlg.h>
#include <SearchVarDlg.h>
#include <StdException.h>
#include <afxdialogex.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// CStepEditorDlg dialog

StepCommandDlg::StepCommandDlg(CWnd* pParent /*=nullptr*/)
	             :StyleDialog(IDD_STEPEDITOR_CMD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

  m_tab1 = new InputDlg(this);
  m_tab2 = new OutputDlg(this);
  m_tab3 = new ParametersDlg(this);
  m_tab4 = new EnvironmentDlg(this);
  m_tab5 = new ScriptDlg(this);
}

StepCommandDlg::~StepCommandDlg()
{
  delete m_tab1;
  delete m_tab2;
  delete m_tab3;
  delete m_tab4;
  delete m_tab5;

  if(m_testStep)
  {
    delete m_testStep;
    m_testStep = nullptr;
  }
}

void StepCommandDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);

  DDX_Control(pDX, IDC_STEPNAME,     m_editName,          m_name);
  DDX_Control(pDX, IDC_GLOBAL,       m_buttonGlobal);
  DDX_Control(pDX, IDC_BOUND,        m_editBound,         m_bound);
  DDX_Control(pDX, IDC_COMMENT,      m_editDocumentation, m_documentation);
  DDX_Control(pDX, IDC_DIRECTORY,    m_editDirectory,     m_directory);
  DDX_Control(pDX, IDC_RUNTIMER,     m_editRuntimer,      m_runtimer);
  DDX_Control(pDX, IDC_COMMANDLINE,  m_editCommandLine,   m_commandLine);
  DDX_Control(pDX, IDC_DIR_PARM,     m_buttonDirParm);
  DDX_Control(pDX, IDC_RUN_PARM,     m_buttonRunParm);
  DDX_Control(pDX, IDC_COM_PARM,     m_buttonComParm);
  DDX_Control(pDX, IDC_TABS,         m_tab);
  DDX_Control(pDX, IDOK,             m_buttonOK);
  DDX_Control(pDX, IDCANCEL,         m_buttonCancel);

  if(pDX->m_bSaveAndValidate == FALSE)
  {
    m_buttonGlobal.EnableWindow(!theApp.GetGlobal());
  }
}

BEGIN_MESSAGE_MAP(StepCommandDlg, StyleDialog)
  ON_WM_SYSCOMMAND()
  ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
  ON_EN_KILLFOCUS(IDC_STEPNAME,        &StepCommandDlg::OnEnChangeStepname)
  ON_BN_CLICKED  (IDC_GLOBAL,          &StepCommandDlg::OnBnClickedGlobal)
  ON_EN_KILLFOCUS(IDC_COMMENT,         &StepCommandDlg::OnEnChangeComment)
  ON_EN_KILLFOCUS(IDC_DIRECTORY,       &StepCommandDlg::OnEnChangeDirectory)
  ON_EN_KILLFOCUS(IDC_RUNTIMER,        &StepCommandDlg::OnEnChangeRuntimer)
  ON_EN_KILLFOCUS(IDC_COMMANDLINE,     &StepCommandDlg::OnEnChangeCommandline)
  ON_BN_CLICKED  (IDC_DIR_PARM,        &StepCommandDlg::OnBnClickedDirParm)
  ON_BN_CLICKED  (IDC_RUN_PARM,        &StepCommandDlg::OnBnClickedRunParm)
  ON_BN_CLICKED  (IDC_COM_PARM,        &StepCommandDlg::OnBnClickedComParm)
  ON_BN_CLICKED(IDOK,                  &StepCommandDlg::OnBnClickedOk)

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

// CStepEditorDlg message handlers

BOOL StepCommandDlg::OnInitDialog()
{
	StyleDialog::OnInitDialog();
  SetWindowText(_T("StepEditor"));
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
		XString strAboutMenu;
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

  // Do our initialization: in this order!!
  // Read all data
  InitButtons();
  InitTabs();
  InitStep();
  InitGlobalParameters();
  InitParameters();
  InitControls();

  if(theApp.GetGlobal())
  {
    m_editName.SetBkColor(GLOBAL_COLOR);
    m_editDocumentation.SetBkColor(GLOBAL_COLOR);
  }

  // Effectuate the parameters
  EffectiveParameters();

  // Fill in the variables: After the loading of the XML files
  LoadVariables();
  LoadVariablesTabs();

  SetCanResize();
  UpdateData(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void
StepCommandDlg::SetupDynamicLayout()
{
  StyleDialog::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif

  manager.AddItem(IDC_GRP_STEP,   CMFCDynamicLayout::MoveNone(),          CMFCDynamicLayout::SizeHorizontalAndVertical(100, 20));
  manager.AddItem(IDC_STEPNAME,   CMFCDynamicLayout::MoveNone(),          CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_GLOBAL,     CMFCDynamicLayout::MoveHorizontal(100), CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_BOUND,      CMFCDynamicLayout::MoveHorizontal(100), CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_COMMENT,    CMFCDynamicLayout::MoveNone(),          CMFCDynamicLayout::SizeHorizontalAndVertical(100,20));
  manager.AddItem(IDC_GRP_COMMAND,CMFCDynamicLayout::MoveVertical(20),    CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_DIRECTORY,  CMFCDynamicLayout::MoveVertical(20),    CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_RUNTIMER,   CMFCDynamicLayout::MoveVertical(20),    CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_COMMANDLINE,CMFCDynamicLayout::MoveVertical(20),    CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_ST_BASE,    CMFCDynamicLayout::MoveVertical(20),    CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_ST_EXEC,    CMFCDynamicLayout::MoveVertical(20),    CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_ST_COMMAND, CMFCDynamicLayout::MoveVertical(20),    CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_DIR_PARM,   CMFCDynamicLayout::MoveHorizontalAndVertical(100,20), CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_RUN_PARM,   CMFCDynamicLayout::MoveHorizontalAndVertical(100,20), CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_COM_PARM,   CMFCDynamicLayout::MoveHorizontalAndVertical(100,20), CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_TABS,       CMFCDynamicLayout::MoveVertical(20),                  CMFCDynamicLayout::SizeHorizontalAndVertical(100,80));
  manager.AddItem(IDOK,           CMFCDynamicLayout::MoveHorizontalAndVertical(100,100),CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDCANCEL,       CMFCDynamicLayout::MoveHorizontalAndVertical(100,100),CMFCDynamicLayout::SizeNone());
}

void
StepCommandDlg::EffectiveParameters()
{
  if(m_busy == false && m_testStep)
  {
    m_busy = true;

    XString bound(_T("Parameters: OK"));
    // Effectuate the parameters
    m_unbound = m_testStep->EffectiveReplacements(&m_parameters,true);

    if(m_unbound > 0)
    {
      bound.Format(_T("Unbound parameters: %d"), m_unbound);
    }
    m_bound = bound;

    m_editBound.SetTextColor(m_unbound > 0 ? RGB(255,0,0) : RGB(0,255,0));

    // Set changes on tab 3 (output parameters)
    m_tab1->InitTab(m_testStep,&m_parameters);
    m_tab3->InitTab(m_testStep,&m_parameters);

    UpdateData(FALSE);
  }
  m_busy = false;
}

void
StepCommandDlg::InitButtons()
{
  EnableToolTips();

  m_buttonGlobal. SetIconImage(IDI_EARTH);
  m_buttonDirParm.SetIconImage(IDI_LIST);
  m_buttonRunParm.SetIconImage(IDI_LIST);
  m_buttonComParm.SetIconImage(IDI_LIST);

  RegisterTooltip(m_buttonGlobal, _T("Promote teststep to a global teststep"));
  RegisterTooltip(m_buttonDirParm,_T("Choose global/test parameter(s) for the directory."));
  RegisterTooltip(m_buttonRunParm,_T("Choose global/test parameter(s) for the runtimer."));
  RegisterTooltip(m_buttonComParm,_T("Choose global/test parameter(s) for the commandline."));
}

void
StepCommandDlg::InitTabs()
{
  m_tab1->Create(IDD_INPUT,     &m_tab);
  m_tab2->Create(IDD_OUTPUT,    &m_tab);
  m_tab3->Create(IDD_PARAMETERS,&m_tab);
  m_tab4->Create(IDD_ENVIRON,   &m_tab);
  m_tab5->Create(IDD_SCRIPT,    &m_tab);

  m_tab.InsertItem(0, m_tab1,_T("Input"));
  m_tab.InsertItem(1, m_tab2,_T("Output"));
  m_tab.InsertItem(2, m_tab3,_T("Parameters"));
  m_tab.InsertItem(3, m_tab4,_T("Environment"));
  m_tab.InsertItem(4, m_tab5,_T("Script"));

  m_tab.Init();
}

void
StepCommandDlg::InitControls()
{
  // Set icons on buttons
  m_buttonOK    .SetStyle(_T("ok"));
  m_buttonCancel.SetStyle(_T("can"));

  // Set default font to bold for parameters message
  m_editBound.SetMutable(false);
  m_editBound.SetBorderColor(RGB(0, 255, 0));
  m_editBound.SetFontStyle(true);
}

void
StepCommandDlg::InitStep()
{
  XString filename = theApp.GetEffectiveStep();

  // Read in the definition file for a test step
  try
  {
    m_testStep = new TestStepCMD();
    m_testStep->ReadFromXML(filename);
  }
  catch(StdException& ex)
  {
    StyleMessageBox(this,ex.GetErrorMessage(),PRODUCT_NAME,MB_OK|MB_ICONERROR);
    theApp.GetMainWnd()->PostMessage(WM_QUIT,0,0);
  }
}

void
StepCommandDlg::InitGlobalParameters()
{
  XString filename = theApp.GetBaseDirectory() + _T("Parameters.xpar");

  ReadParameters(filename);
}

void
StepCommandDlg::InitParameters()
{
  XString filename = theApp.GetBaseDirectory() + theApp.GetTestDirectory() + theApp.GetParametersFilename();

  // Possibly no parameter file bound to the step!
  if(filename.IsEmpty())
  {
    return;
  }
  ReadParameters(filename,false);
}

void
StepCommandDlg::ReadParameters(XString p_file,bool p_global /*= true*/)
{
  // read the definition of the parameters
  try
  {
    m_parameters.ReadFromXML(p_file,p_global);
  }
  catch(StdException& ex)
  {
    StyleMessageBox(this,ex.GetErrorMessage(),PRODUCT_NAME,MB_OK|MB_ICONERROR);
    theApp.GetMainWnd()->PostMessage(WM_QUIT, 0, 0);
  }
}

void
StepCommandDlg::LoadVariables()
{
  m_name          = m_testStep->GetName();
  m_documentation = m_testStep->GetDocumentation();
  m_directory     = m_testStep->GetDirectoryPath();
  m_runtimer      = m_testStep->GetRuntimer();
  m_commandLine   = m_testStep->GetCommandLine();
}

void
StepCommandDlg::LoadVariablesTabs()
{
  // Initialize all the tabs
  m_tab1->InitTab(m_testStep,&m_parameters);
  m_tab2->InitTab(m_testStep,&m_parameters);
  m_tab3->InitTab(m_testStep,&m_parameters);
  m_tab4->InitTab(m_testStep);
  m_tab5->InitTab(m_testStep,&m_parameters);

  // Effectuate again if temporary environment
  if(!m_testStep->GetEnvironmentVars().empty())
  {
    EffectiveParameters();
  }
}

bool
StepCommandDlg::SaveStep()
{
  StoreVariables();

  XString filenameStep = theApp.GetEffectiveStep();
  XString filenameParm = theApp.GetBaseDirectory() + theApp.GetTestDirectory() + theApp.GetParametersFilename();

  try
  {
    bool res1 = m_testStep->WriteToXML(filenameStep);
    bool res2 = m_parameters.WriteToXML();

    return (res1 && res2);
  }
  catch(StdException& ex)
  {
    StyleMessageBox(this,ex.GetErrorMessage(),PRODUCT_NAME,MB_OK|MB_ICONERROR);
  }
  return false;
}

void
StepCommandDlg::StoreVariables()
{
  // General variables above the tabs
  m_testStep->SetName(m_name);
  m_testStep->SetDocumentation(m_documentation);
  m_testStep->SetDirectoryPath(m_directory);
  m_testStep->SetRuntimer(m_runtimer);
  m_testStep->SetCommandLine(m_commandLine);

  // Take all the changes from the tabs
  m_tab1->StoreVariables();
  m_tab2->StoreVariables();
  m_tab3->StoreVariables();
  m_tab4->StoreVariables();
  m_tab5->StoreVariables();
}

void
StepCommandDlg::ChooseVariable(StyleEdit& p_edit)
{
  AutoFocus focus;
  SearchVarDlg dlg(this,&m_parameters,true,true,true,true);
  if(dlg.DoModal() == IDOK || dlg.GetSaved())
  {
    XString variable = dlg.GetVariable();
    p_edit.InsertAtCurPos(variable, 0);
    UpdateData();
  }
}

void
StepCommandDlg::OnAbout()
{
  AutoFocus focus;
  AboutDlg dlgAbout(this);
  dlgAbout.DoModal();
}

void 
StepCommandDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if((nID & 0xFFF0) == IDM_ABOUTBOX)
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

void StepCommandDlg::OnPaint()
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
HCURSOR StepCommandDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void 
StepCommandDlg::OnEnChangeStepname()
{
  UpdateData();
}

void
StepCommandDlg::OnBnClickedGlobal()
{
  if(StyleMessageBox(this,_T("Promote this test step to a global teststep?"),PRODUCT_NAME,MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION) == IDYES)
  {
    theApp.PromoteTestStep(this);
  }
}

void
StepCommandDlg::OnEnChangeComment()
{
  UpdateData();
}

void 
StepCommandDlg::OnEnChangeDirectory()
{
  UpdateData();
  m_testStep->SetDirectoryPath(m_directory);
  EffectiveParameters();
  m_tab3->InitTab(m_testStep,&m_parameters);
}

void 
StepCommandDlg::OnEnChangeRuntimer()
{
  UpdateData();
  m_testStep->SetRuntimer(m_runtimer);
  EffectiveParameters();
  m_tab3->InitTab(m_testStep,&m_parameters);
}

void 
StepCommandDlg::OnEnChangeCommandline()
{
  UpdateData();
  m_testStep->SetCommandLine(m_commandLine);
  EffectiveParameters();
  m_tab3->InitTab(m_testStep,&m_parameters);
}

void 
StepCommandDlg::OnBnClickedDirParm()
{
  ChooseVariable(m_editDirectory);
}

void 
StepCommandDlg::OnBnClickedRunParm()
{
  ChooseVariable(m_editRuntimer);
}

void 
StepCommandDlg::OnBnClickedComParm()
{
  ChooseVariable(m_editCommandLine);
}

void 
StepCommandDlg::OnBnClickedOk()
{
  if(SaveStep())
  {
    StyleDialog::OnOK();
  }
}

void
StepCommandDlg::OnExit()
{
  OnBnClickedOk();
}
