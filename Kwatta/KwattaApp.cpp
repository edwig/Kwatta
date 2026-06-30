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
// This program: Kwatta
// This File   : KwattaApp.cpp
// What it does: main program file for the AX/TestSuite application
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "KwattaApp.h"
#include "KwattaAppDlg.h"
#include "SplashDlg.h"
#include <AboutDlg.h>
#include <ExtraExtensions.h>
#include <GetExePath.h>
#include "StartProgram.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// KwattaApp

BEGIN_MESSAGE_MAP(KwattaApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &KwattaApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CTestSuiteApp construction

KwattaApp::KwattaApp() noexcept
{
	m_bHiColorIcons = TRUE;

	// replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("EDO.Kwatta.TestSuite.Version"));
}

// The one and only CTestSuiteApp object

KwattaApp theApp;

// CTestSuiteApp initialization

BOOL KwattaApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

  AfxInitRichEdit2();
  AfxEnableControlContainer();
  
  CWinAppEx::InitInstance();

	EnableTaskbarInteraction(FALSE);

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
  SetRegistryKey(_T("EDO\\Kwatta"));
	LoadStdProfileSettings(10);  // Load standard INI file options (including MRU)

	InitContextMenuManager();
	InitKeyboardManager();
	InitTooltipManager();

	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

  if(!ParseCommandLine())
  {
    return FALSE;
  }

  SplashDlg splash;
  splash.DoModal();

  UINT_PTR result = IDOK;
  do 
  {
    KwattaAppDlg dlg(nullptr,m_suiteFilename);
    result = dlg.DoModal();
    m_suiteFilename.Empty();
  }
  while(result == IDOK);

	return FALSE;
}

// [/h] suite-filename.xtest
bool
KwattaApp::ParseCommandLine()
{
  for(int i = 1; i < __argc; i++)
  {
    LPCTSTR lpszParam = __targv[i];

    if(lpszParam[0] == _T('-') || lpszParam[0] == '/')
    {
      Usage();
      return false;
    }
    else
    {
      m_suiteFilename = lpszParam;
    }
  }

  // No file: Just use the starter
  if(m_suiteFilename.IsEmpty())
  {
    return true;
  }
  WinFile file(m_suiteFilename);
  XString extension = file.GetFilenamePartExtension();

  if(extension.CompareNoCase(EXTENSION_SUITE))
  {
    StyleMessageBox(NULL,_T("Parameter can only be an *.xtest test suite file"),_T(KWATTA),MB_OK|MB_ICONERROR);
    Usage();
    return false;
  }
  return true;
}

void
KwattaApp::Usage()
{
  XString usage = _T("USAGE of this program:\n")
                  _T("\n")
                  _T("Kwatta.exe [/h] [testsuite.xtest]\n")
                  _T("\n")
                  _T("/h\t\t\tShow this help page\n")
                  _T("<filename>.xtest\tName of a Kwatta test suite");
  StyleMessageBox(NULL,usage.GetString(),_T(KWATTA),MB_OK|MB_ICONEXCLAMATION);
}

// CTestSuiteApp message handlers

// App command to run the dialog
void KwattaApp::OnAppAbout()
{
  AutoFocus focus;
	AboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CTestSuiteApp customization load/save methods

void KwattaApp::PreLoadState()
{
	BOOL bNameValid;
	XString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void KwattaApp::LoadCustomState()
{
}

void KwattaApp::SaveCustomState()
{
}

// CTestSuiteApp message handlers

XString 
KwattaApp::GetBaseDirectoryClean()
{
  XString clean(m_baseDirectory);
  clean.TrimRight('\\');
  return clean;
}

XString
KwattaApp::GetParameterFilename(XString p_testname)
{
  XString parameters(p_testname);
  int pos = p_testname.ReverseFind('.');
  if(pos > 0)
  {
    parameters = p_testname.Left(pos) + _T(".xpar");
  }
  return parameters;
}

XString
KwattaApp::StandardParameters(HWND p_wnd,int p_row)
{
  XString param;
  param.Format(_T("/BASE:\"%s\" /HWND:%I64d /ROW:%d"),GetBaseDirectoryClean().GetString(),p_wnd,p_row);
  return param;
}

int
KwattaApp::StartTestEditor(Test& p_test,HWND p_hwnd,int p_row)
{
  // Construct program name
  XString program(_T("TestEditor.exe"));
  // Construct parameters
  XString parameters = StandardParameters(p_hwnd,p_row);
  parameters.AppendFormat(_T(" /TDIR:\"%s\""),p_test.m_directory.GetString());
  parameters.AppendFormat(_T(" /PARAM:\"%s\""),GetParameterFilename(p_test.m_filename).GetString());
  parameters.AppendFormat(_T(" /TEST:\"%s\""),p_test.m_filename.GetString());

  // Call program
  TRACE(_T("Call: %s %s\n"),program.GetString(), parameters.GetString());
  int result = StartProgram(program,parameters,true,false,false);

  // Logging what we just started, and what the result was

  return result;
}

int
KwattaApp::StartTestRunner(Test& p_test,HWND p_hwnd,int p_row)
{
  // Check if this is an active test
  if(p_test.m_active == false)
  {
    return -1;
  }
  // Construct program name
  XString program(_T("TestRunner.exe"));
  // Construct parameters
  XString parameters = StandardParameters(p_hwnd,p_row);
  parameters.AppendFormat(_T(" /TDIR:\"%s\""),p_test.m_directory.GetString());
  parameters.AppendFormat(_T(" /PARAM:\"%s\""),GetParameterFilename(p_test.m_filename).GetString());
  parameters.AppendFormat(_T(" /XSET:\"%s\""),p_test.m_filename.GetString());

  // Call program
  TRACE(_T("Call: %s %s\n"),program,parameters);
  int result = StartProgram(program,parameters,true,false,true);

  // Logging what we just started, and what the result was

  return result;
}
