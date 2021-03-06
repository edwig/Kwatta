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
#include "StepEditorDlg.h"
#include "StepInternetDlg.h"
#include "StepDatabaseDlg.h"
#include <LogAnalysis.h>
#include <ExtraMessages.h>
#include <ExtraExtensions.h>
#include <HTTPClient.h>
#include <OAuth2Cache.h>
#include <InetRunner.h>
#include <SQLRunner.h>
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
	SetRegistryKey("EDO\\Kwatta");

  InitContextMenuManager();
  InitKeyboardManager();
  InitTooltipManager();

  CMFCToolTipInfo ttParams;
  ttParams.m_bVislManagerTheme = TRUE;
  theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL, RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

  // Reading our command line for the XML files
  if(!ParseStartParameters())
  {
    return FALSE;
  }

  StepType type = DeduceStepType();

  if (type == StepType::Step_command)
  {
    StepEditorDlg dlg;
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
  else
  {
    StyleMessageBox(nullptr,"Step Editor started without a testfile with a known extension (XRUN, IRUN, QRUN)",KWATTA,MB_OK|MB_ICONERROR);
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
  filename += m_global ? CString("Steps\\") : m_testDirectory;
  filename += m_stepFilename;

  return filename;
}

void
StepEditorApp::PromoteTestStep(CWnd* p_wnd)
{
  CString stepsDirectory = m_baseDirectory + "Steps\\";
  CString globalStep = stepsDirectory + m_stepFilename;

  if(_access(globalStep,0) == 0)
  {
    if(StyleMessageBox(p_wnd,"This step already exists as a global step. Do you wish to overwrite it?",PRODUCT_NAME,MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION) == IDNO)
    {
      return;
    }
  }

  CWaitCursor sigh;
  CString testfile = m_baseDirectory + m_testDirectory + m_stepFilename;
  if(CopyFile(testfile, globalStep, FALSE) == FALSE)
  {
    CString error = GetLastErrorAsString(GetLastError());
    StyleMessageBox(p_wnd,"Could not create a global step: " + error,PRODUCT_NAME,MB_OK|MB_ICONERROR);
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
    InetRunner runner(GetBaseDirectory()
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
                     ,theApp.GetGlobal());

    // PERFORM OUR TEST
    result = runner.PerformTest();
  
    // Tel the main program what the test results are.
    p_caller->SetStepResult(runner.GetStepResult());

    if(m_client->GetLogging())
    {
      m_client->GetLogging()->Reset();
      ShellExecute(NULL,"open",m_client->GetLogging()->GetLogFileName().GetString(),"","",SW_SHOW);
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

//     if(m_client && m_client->GetLogging())
//     {
//       m_client->GetLogging()->Reset();
//       ShellExecute(NULL,"open",m_client->GetLogging()->GetLogFileName().GetString(),"","",SW_SHOW);
//     }
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
  return "";
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
    LPCSTR lpszParam = __argv[i];

    if (lpszParam[0] == '-' || lpszParam[0] == '/')
    {
           if (_strnicmp(&lpszParam[1], "STEP:", 5) == 0) m_stepFilename  = &lpszParam[6];
      else if (_strnicmp(&lpszParam[1], "BASE:", 5) == 0) m_baseDirectory = &lpszParam[6];
      else if (_strnicmp(&lpszParam[1], "TDIR:", 5) == 0) m_testDirectory = &lpszParam[6];
      else if (_strnicmp(&lpszParam[1], "PARAM:",6) == 0) m_paramFilename = &lpszParam[7];
      else if (_strnicmp(&lpszParam[1], "VALI:", 5) == 0) m_validations.push_back(CString(&lpszParam[6]));
      else if (_strnicmp(&lpszParam[1], "GLVAL:",6) == 0) m_globalValid.push_back(CString(&lpszParam[7]));
      else if (_strnicmp(&lpszParam[1], "HWND:", 5) == 0) m_callingHWND   = atoll(&lpszParam[6]);
      else if (_strnicmp(&lpszParam[1], "ROW:",  4) == 0) m_callingROW    = atol (&lpszParam[5]);
      else if (_strnicmp(&lpszParam[1], "GLOBAL",6) == 0) m_global        = true;
      else
      {
        Usage();
        return false;
      }
    }
  }
  if(m_stepFilename.IsEmpty())
  {
    StyleMessageBox(NULL,"Cannot start without a *.xrun or *.xnet file",PRODUCT_NAME,MB_OK|MB_ICONERROR);
    Usage();
    return false;
  }
  if(m_baseDirectory.IsEmpty())
  {
    StyleMessageBox(NULL,"Cannot start without a BASE directory",PRODUCT_NAME,MB_OK|MB_ICONERROR);
    Usage();
    return false;
  }
  // Check formatting of the base directory
  if(m_baseDirectory.Right(1) != '\\')
  {
    m_baseDirectory += '\\';
  }
  // Check formatting of the test directory
  if(m_testDirectory.Right(1) != '\\')
  {
    m_testDirectory += '\\';
  }
  return true;
}

void
StepEditorApp::Usage()
{
  CString usage = "USAGE of this program:\n"
                  "\n"
                  "StepEditor.exe <singleOptions> <multple-options>\n"
                  "\n"
                  "Single options:\n"
                  "/BASE:<base-directory>\tName of the Base directory of the test suite\n"
                  "/STEP:<filename>.xrun\tName of a command line test step\n"
                  "/STEP:<filename>.irun\tName of an internet test step\n"
                  "/STEP:<filename>.qrun\tName of an internet test step\n"
                  "/HWND:xxxxxx\t\tWindow handle of calling program\n"
                  "/ROW:nnnn\t\tRow number of the test step\n"
                  "\n"
                  "Multiple options:\n"
                  "/PARAM:<filename>.xpar     Parameter file";
  StyleMessageBox(NULL,usage.GetString(),PRODUCT_NAME,MB_OK|MB_ICONEXCLAMATION);
}
