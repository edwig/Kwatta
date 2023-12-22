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
// This File   : ValidateWindowsDlg.cpp
// What it does: Main dialog for the validation of a Windows UI test
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "afxdialogex.h"
#include "ValidateEditor.h"
#include "ValidateWindowsDlg.h"
#include <AboutDlg.h>
#include <StdException.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// CValidateEditorDlg dialog

ValidateWindowsDlg::ValidateWindowsDlg(CWnd* pParent /*=nullptr*/)
                   :StyleDialog(IDD_VALIDATEEDITOR, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

  m_tab1 = new WINLoggingDlg(this);
  m_tab2 = new WINReturnDlg (this);
  m_tab3 = new WINErrorsDlg (this);
}

ValidateWindowsDlg::~ValidateWindowsDlg()
{
  delete m_tab1;
  delete m_tab2;
  delete m_tab3;
}

void ValidateWindowsDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_NAME,          m_editName,m_name);
  DDX_Control(pDX, IDC_GLOBAL,        m_buttonGlobal);
  DDX_Control(pDX, IDC_BOUND,         m_editBound,m_bound);
  DDX_Control(pDX, IDC_DOCUMENTATION, m_editDocumentation,m_documentation);
  DDX_Control(pDX, IDC_TABS,          m_tabs);
  DDX_Control(pDX, IDOK,              m_buttonOK);
  DDX_Control(pDX, IDCANCEL,          m_buttonCancel);

  if(pDX->m_bSaveAndValidate == FALSE)
  {
    m_buttonGlobal.EnableWindow(!theApp.GetGlobal());
  }
}

BEGIN_MESSAGE_MAP(ValidateWindowsDlg, StyleDialog)
  ON_WM_SYSCOMMAND()
  ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
  ON_EN_CHANGE (IDC_NAME,             &ValidateWindowsDlg::OnEnChangeName)
  ON_BN_CLICKED(IDC_GLOBAL,           &ValidateWindowsDlg::OnBnClickedGlobal)
  ON_EN_CHANGE (IDC_DOCUMENTATION,    &ValidateWindowsDlg::OnEnChangeDocumentation)
  ON_BN_CLICKED(IDOK,                 &ValidateWindowsDlg::OnBnClickedOk)
  ON_BN_CLICKED(IDCANCEL,             &ValidateWindowsDlg::OnBnClickedCancel)

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

// CValidateEditorDlg message handlers

BOOL ValidateWindowsDlg::OnInitDialog()
{
	StyleDialog::OnInitDialog();
  SetWindowText("Validate Editor (Command line)");
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

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void
ValidateWindowsDlg::SetupDynamicLayout()
{
  StyleDialog::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif

  manager.AddItem(IDC_GRP_VALID,    CMFCDynamicLayout::MoveNone(),          CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_BOUND,        CMFCDynamicLayout::MoveHorizontal(100), CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_DOCUMENTATION,CMFCDynamicLayout::MoveNone(),          CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_TABS,         CMFCDynamicLayout::MoveNone(),          CMFCDynamicLayout::SizeHorizontalAndVertical(100,100));
  manager.AddItem(IDOK,             CMFCDynamicLayout::MoveHorizontalAndVertical(100, 100), CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDCANCEL,         CMFCDynamicLayout::MoveHorizontalAndVertical(100, 100), CMFCDynamicLayout::SizeNone());
}

void
ValidateWindowsDlg::InitTabs()
{
  m_tab1->Create(IDD_WINLOGGING,&m_tabs);
  m_tab2->Create(IDD_WINRETURN, &m_tabs);
  m_tab3->Create(IDD_WINERRORS, &m_tabs);

  m_tabs.InsertItem(0,m_tab1,"Logging");
  m_tabs.InsertItem(1,m_tab2,"Return value");
  m_tabs.InsertItem(2,m_tab3,"Error list");

  m_tabs.Init();
}

void
ValidateWindowsDlg::InitButtons()
{
  m_buttonGlobal.SetIconImage(IDI_EARTH);
  m_buttonOK.SetStyle("ok");
  m_buttonCancel.SetStyle("can");

  EnableToolTips();
  RegisterTooltip(m_buttonGlobal,"Promote validation to a global validation");
}

void
ValidateWindowsDlg::InitValidation()
{
  CString filename = theApp.GetEffectiveValidation();

  // Read in the definition file for a test step
  try
  {
    m_validate.ReadFromXML(filename);
  }
  catch (StdException& ex)
  {
    StyleMessageBox(this,ex.GetErrorMessage(),PRODUCT_NAME,MB_OK|MB_ICONERROR);
    theApp.GetMainWnd()->PostMessage(WM_QUIT, 0, 0);
  }
}

void
ValidateWindowsDlg::InitParameters()
{
  CString global = theApp.GetBaseDirectory() + "Parameters.xpar";
  CString local  = theApp.GetBaseDirectory() + theApp.GetTestDirectory() + theApp.GetParameterFilename();

  // read the definition of the parameters
  try
  {
    m_parameters.ReadFromXML(global);
    m_parameters.ReadFromXML(local,false);
  }
  catch(StdException& ex)
  {
    StyleMessageBox(this,ex.GetErrorMessage(),PRODUCT_NAME,MB_OK|MB_ICONERROR);
    theApp.GetMainWnd()->PostMessage(WM_QUIT,0,0);
  }
}

void
ValidateWindowsDlg::EffectiveParameters()
{
  if(m_busy == false)
  {
    m_busy = true;

    m_bound = "Parameters: OK";
    // Effectuate the parameters
    m_unbound = m_validate.EffectiveReplacements(&m_parameters,true);

    if (m_unbound > 0)
    {
      m_bound.Format("Unbound parameters: %d", m_unbound);
    }

    m_editBound.SetTextColor  (m_unbound > 0 ? RGB(255, 0, 0) : RGB(0, 255, 0));
    m_editBound.SetBorderColor(m_unbound > 0 ? RGB(255, 0, 0) : RGB(0, 255, 0));

    UpdateData(FALSE);
  }
  m_busy = false;

}

void
ValidateWindowsDlg::LoadVariables()
{
  m_name          = m_validate.GetName();
  m_documentation = m_validate.GetDocumentation();
}

void
ValidateWindowsDlg::LoadVariablesTabs()
{
  m_tab1->InitTab(&m_validate,&m_parameters);
  m_tab2->InitTab(&m_validate,&m_parameters);
  m_tab3->InitTab(&m_validate,&m_parameters);
}

void
ValidateWindowsDlg::StoreVariables()
{
  // General variables
  m_validate.SetName(m_name);
  m_validate.SetDocumentation(m_documentation);

  // Take all the changes from the tabs
  m_tab1->StoreVariables();
  m_tab2->StoreVariables();
  m_tab3->StoreVariables();
}

bool
ValidateWindowsDlg::SaveValidate()
{
  StoreVariables();

  try
  {
    bool res1 = m_validate  .WriteToXML(m_validate.GetFilename());
    bool res2 = m_parameters.WriteToXML();

    return (res1 && res2);
  }
  catch (StdException& ex)
  {
    StyleMessageBox(this, ex.GetErrorMessage(), PRODUCT_NAME, MB_OK | MB_ICONERROR);
  }
  return false;
}

//////////////////////////////////////////////////////////////////////////
//
// Message handlers

void
ValidateWindowsDlg::OnAbout()
{
  AboutDlg dlg(this);
  dlg.DoModal();
}

void 
ValidateWindowsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
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

void ValidateWindowsDlg::OnPaint()
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
HCURSOR 
ValidateWindowsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void 
ValidateWindowsDlg::OnEnChangeName()
{
  UpdateData();
}

void
ValidateWindowsDlg::OnBnClickedGlobal()
{
  if(StyleMessageBox(this,"Promote this validation to a global validation?",PRODUCT_NAME,MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION) == IDYES)
  {
    theApp.PromoteValidation(this);
  }
}

void
ValidateWindowsDlg::OnEnChangeDocumentation()
{
  UpdateData();
}

void 
ValidateWindowsDlg::OnBnClickedOk()
{
  if(SaveValidate())
  {
    StyleDialog::OnOK();
  }
}

void 
ValidateWindowsDlg::OnBnClickedCancel()
{
  StyleDialog::OnCancel();
}

void
ValidateWindowsDlg::OnExit()
{
  if (SaveValidate())
  {
    StyleDialog::OnOK();
  }
}
