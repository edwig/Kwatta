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
// This File   : StepEditor.cpp
// What it does: Main program for editing a test step
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StepEditor.h"
#include "StepCommandDlg.h"
#include "StepInternetDlg.h"
#include "StepDatabaseDlg.h"
#include "StepWindowsDlg.h"
#include <LogAnalysis.h>
#include <ExtraMessages.h>
#include <ExtraExtensions.h>
#include <HTTPClient.h>
#include <OAuth2Cache.h>
#include <NETRunner.h>
#include <SQLRunner.h>
#include <WINRunner.h>
#include <GetLastErrorAsString.h>
#include <io.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// CStepEditorApp

// The one and only CStepEditorApp object
StepEditorApp theApp;

BEGIN_MESSAGE_MAP(StepEditorApp,CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinAppEx::OnHelp)
END_MESSAGE_MAP()


// CStepEditorApp construction

StepEditorApp::StepEditorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

StepEditorApp::~StepEditorApp()
{
  if(m_client)
  {
    delete m_client;
    m_client = nullptr;
  }
  if(m_cache)
  {
    delete m_cache;
    m_cache = nullptr;
  }
  if(m_database)
  {
    delete m_database;
    m_database = nullptr;
  }
}

// CStepEditorApp initialization
BOOL 
StepEditorApp::InitInstance()
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


	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	SetRegistryKey(_T("EDO\\Kwatta"));

  InitContextMenuManager();
  InitKeyboardManager();
  InitTooltipManager();

  CMFCToolTipInfo ttParams;
  ttParams.m_bVislManagerTheme = TRUE;
  theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL, RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

  // Start parameters, starting at the default
  qlargc = __argc;
  qlargv = __targv;

  // Reading our command line for the XML files
  if(!ParseStartParameters())
  {
    return FALSE;
  }

  StepType type = DeduceStepType();

  if (type == StepType::Step_command)
  {
    StepCommandDlg dlg;
    dlg.DoModal();
  }
  else if(type == StepType::Step_http)
  {
    StepInternetDlg dlg;
    dlg.DoModal();
  }
  else if(type == StepType::Step_sql)
  {
    StepDatabaseDlg dlg;
    dlg.DoModal();
  }
  else if(type == StepType::Step_win)
  {
    StepWindowsDlg dlg;
    dlg.DoModal();
  }
  else
  {
    StyleMessageBox(nullptr,_T("Step Editor started without a testfile with a known extension (XRUN, IRUN, QRUN)"),_T(KWATTA),MB_OK|MB_ICONERROR);
  }
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int
StepEditorApp::ExitInstance()
{
  if(m_callingHWND && m_callingROW)
  {
    ::PostMessage((HWND)m_callingHWND,WM_READYTEST,(WPARAM)m_callingROW,(LPARAM)-1);
  }
  return CWinAppEx::ExitInstance();
}

SQLDatabase*
StepEditorApp::GetDatabase()
{
  if(!m_database)
  {
    m_database = new SQLDatabase();
  }
  return m_database;
}

CString
StepEditorApp::GetEffectiveStep()
{
  CString filename(m_baseDirectory);
  filename += m_global ? CString(_T("Steps\\")) : m_testDirectory;
  filename += m_stepFilename;

  return filename;
}

void
StepEditorApp::PromoteTestStep(CWnd* p_wnd)
{
  CString stepsDirectory = m_baseDirectory + _T("Steps\\");
  CString globalStep = stepsDirectory + m_stepFilename;

  if(_taccess(globalStep,0) == 0)
  {
    if(StyleMessageBox(p_wnd,_T("This step already exists as a global step. Do you wish to overwrite it?"),PRODUCT_NAME,MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION) == IDNO)
    {
      return;
    }
  }

  CWaitCursor sigh;
  CString testfile = m_baseDirectory + m_testDirectory + m_stepFilename;
  if(CopyFile(testfile, globalStep, FALSE) == FALSE)
  {
    CString error = GetLastErrorAsString(GetLastError());
    StyleMessageBox(p_wnd,_T("Could not create a global step: ") + error,PRODUCT_NAME,MB_OK|MB_ICONERROR);
  }
}

void
StepEditorApp::CreateHTTPClient()
{
  if(m_client)
  {
    return;
  }
  m_client = new HTTPClient();
  m_cache  = new OAuth2Cache();
}

int
StepEditorApp::StartTheInetRunner(StepInternetDlg* p_caller)
{
  int result = 0;
  if(p_caller)
  {
    p_caller->ResetStepResult();

    // Start a new test
    CreateHTTPClient();
    NETRunner runner(GetBaseDirectory()
                     ,GetTestDirectory()
                     ,GetTestStepFilename()
                     ,GetParametersFilename()
                     ,GetValidations()
                     ,GetGlobalValidations()
                     ,m_pMainWnd->GetSafeHwnd()
                     ,GetHTTPClient()
                     ,GetOAuth2Cache()
                     ,0
                     ,0
                     ,theApp.GetGlobal()
                     ,false);

    // PERFORM OUR TEST
    result = runner.PerformTest();
  
    // Tel the main program what the test results are.
    p_caller->SetStepResult(runner.GetStepResult());

    if(m_client->GetLogging())
    {
      m_client->GetLogging()->Reset();
      ShellExecute(NULL,_T("open"),m_client->GetLogging()->GetLogFileName().GetString(),_T(""),_T(""),SW_SHOW);
      m_client->SetLogging(nullptr);
      m_client->SetLogLevel(HLL_NOLOG);
    }
  }
  return result;
}

int
StepEditorApp::StartTheSQLRunner(StepDatabaseDlg* p_caller)
{
  int result = 0;
  if(p_caller)
  {
    p_caller->ResetStepResult();

    // Start a new test
    SQLRunner runner(GetDatabase()
                    ,GetBaseDirectory()
                    ,GetTestDirectory()
                    ,GetTestStepFilename()
                    ,GetParametersFilename()
                    ,GetValidations()
                    ,GetGlobalValidations()
                    ,m_pMainWnd->GetSafeHwnd()
                    ,0
                    ,0
                    ,0
                    ,theApp.GetGlobal());

    // PERFORM OUR TEST
    result = runner.PerformTest();
  
    // Tel the main program what the test results are.
    p_caller->SetStepResult(runner.GetStepResult());

    if(m_client && m_client->GetLogging())
    {
      m_client->GetLogging()->Reset();
      ShellExecute(NULL,_T("open"),m_client->GetLogging()->GetLogFileName().GetString(),_T(""),_T(""),SW_SHOW);
    }
  }
  return result;
}

int
StepEditorApp::StartTheWINRunner(StepWindowsDlg* p_caller)
{
  int result = 0;
  if(p_caller)
  {
    p_caller->ResetStepResult();

    // Start a new test
    WINRunner runner(GetBaseDirectory()
                    ,GetTestDirectory()
                    ,GetTestStepFilename()
                    ,GetParametersFilename()
                    ,GetValidations()
                    ,GetGlobalValidations()
                    ,m_pMainWnd->GetSafeHwnd()
                    ,0
                    ,0
                    ,0
                    ,theApp.GetGlobal());

    // PERFORM OUR TEST
    result = runner.PerformTest();
  
    // Tel the main program what the test results are.
    p_caller->SetStepResult(runner.GetStepResult());
  }
  return result;
}


StepType
StepEditorApp::DeduceStepType()
{
  int pos = m_stepFilename.ReverseFind('.');
  if (pos > 0)
  {
    CString extension = m_stepFilename.Mid(pos);
    if (extension.CompareNoCase(EXTENSION_TESTSTEP_CMD) == 0)
    {
      return StepType::Step_command;
    }
    else if(extension.CompareNoCase(EXTENSION_TESTSTEP_NET) == 0)
    {
      return StepType::Step_http;
    }
    else if(extension.CompareNoCase(EXTENSION_TESTSTEP_SQL) == 0)
    {
      return StepType::Step_sql;
    }
    else if(extension.CompareNoCase(EXTENSION_TESTSTEP_WIN) == 0)
    {
      return StepType::Step_win;
    }
  }
  // DEFAULT !!
  return StepType::Step_command;
}

CString 
StepEditorApp::RefreshBearerToken()
{
  if(m_cache && m_client)
  {
    return m_cache->GetBearerToken(m_client->GetOAuth2Session(),true);
  }
  return _T("");
}

bool
StepEditorApp::GetBearerTokenIsExpired()
{
  if(m_cache && m_client)
  {
    return m_cache->GetIsExpired(m_client->GetOAuth2Session());
  }
  return false;
}

// /BASE:Base-directory
// /STEP:filename.xrun
// /PARAM:param.xpar
bool
StepEditorApp::ParseStartParameters()
{
  for (int i = 1; i < __argc; i++)
  {
    LPCTSTR lpszParam = __targv[i];

    if (lpszParam[0] == _T('-') || lpszParam[0] == '/')
    {
           if (_tcsnicmp(&lpszParam[1], _T("STEP:"), 5) == 0) m_stepFilename  = &lpszParam[6];
      else if (_tcsnicmp(&lpszParam[1], _T("BASE:"), 5) == 0) m_baseDirectory = &lpszParam[6];
      else if (_tcsnicmp(&lpszParam[1], _T("TDIR:"), 5) == 0) m_testDirectory = &lpszParam[6];
      else if (_tcsnicmp(&lpszParam[1], _T("PARAM:"),6) == 0) m_paramFilename = &lpszParam[7];
      else if (_tcsnicmp(&lpszParam[1], _T("VALI:"), 5) == 0) m_validations.push_back(CString(&lpszParam[6]));
      else if (_tcsnicmp(&lpszParam[1], _T("GLVAL:"),6) == 0) m_globalValid.push_back(CString(&lpszParam[7]));
      else if (_tcsnicmp(&lpszParam[1], _T("HWND:"), 5) == 0) m_callingHWND   = _ttoll(&lpszParam[6]);
      else if (_tcsnicmp(&lpszParam[1], _T("ROW:"),  4) == 0) m_callingROW    = _ttol (&lpszParam[5]);
      else if (_tcsnicmp(&lpszParam[1], _T("GLOBAL"),6) == 0) m_global        = true;
      else
      {
        Usage();
        return false;
      }
    }
  }
  if(m_stepFilename.IsEmpty())
  {
    StyleMessageBox(NULL,_T("Cannot start without a *.xrun or *.xnet file"),PRODUCT_NAME,MB_OK|MB_ICONERROR);
    Usage();
    return false;
  }
  if(m_baseDirectory.IsEmpty())
  {
    StyleMessageBox(NULL,_T("Cannot start without a BASE directory"),PRODUCT_NAME,MB_OK|MB_ICONERROR);
    Usage();
    return false;
  }
  // Check formatting of the base directory
  if(m_baseDirectory.Right(1) != '\\')
  {
    m_baseDirectory += _T('\\');
  }
  // Check formatting of the test directory
  if(m_testDirectory.Right(1) != '\\')
  {
    m_testDirectory += _T('\\');
  }
  return true;
}

void
StepEditorApp::Usage()
{
  CString usage = _T("USAGE of this program:\n")
                  _T("\n")
                  _T("StepEditor.exe <singleOptions> <multple-options>\n")
                  _T("\n")
                  _T("Single options:\n")
                  _T("/BASE:<base-directory>\tName of the Base directory of the test suite\n")
                  _T("/STEP:<filename>.xrun\tName of a command line test step\n")
                  _T("/STEP:<filename>.irun\tName of an internet test step\n")
                  _T("/STEP:<filename>.qrun\tName of an internet test step\n")
                  _T("/STEP:<filename>.wrun\tName of a Windows-UI test step\n")
                  _T("/HWND:xxxxxx\t\tWindow handle of calling program\n")
                  _T("/ROW:nnnn\t\tRow number of the test step\n")
                  _T("\n")
                  _T("Multiple options:\n")
                  _T("/PARAM:<filename>.xpar     Parameter file");
  StyleMessageBox(NULL,usage.GetString(),PRODUCT_NAME,MB_OK|MB_ICONEXCLAMATION);
}
