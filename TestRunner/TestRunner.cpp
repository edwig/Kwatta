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
// This program: TestRunner
// This File   : TestRunner.cpp
// What it does: Running a test-set or single test-step
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TestRunner.h"
#include "TestRunnerDlg.h"
#include <TestSet.h>
#include <TestMessages.h>
#include <ExtraExtensions.h>
#include <ExecRunner.h>
#include <InetRunner.h>
#include <SQLRunner.h>
#include <HTTPClient.h>
#include <OAuth2Cache.h>
#include <LogAnalysis.h>
#include <StdException.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// CTestRunnerApp

BEGIN_MESSAGE_MAP(TestRunnerApp,CWinApp)
	ON_COMMAND(ID_HELP,&CWinApp::OnHelp)
END_MESSAGE_MAP()

// CTestRunnerApp construction

TestRunnerApp::TestRunnerApp()
{
	// support Restart Manager
	// m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}

TestRunnerApp::~TestRunnerApp()
{
  // Delete the console again
  if(m_console)
  {
    delete m_console;
    m_console = nullptr;
  }
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

// The one and only CTestRunnerApp object

TestRunnerApp theApp;


// CTestRunnerApp initialization

BOOL 
TestRunnerApp::InitInstance()
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

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
  SetRegistryKey("EDO\\Kwatta");

  // Get our work
  if(!ParseStartParameters())
  {
    return FALSE;
  }

  // Create extra objects (console/http-client)
  if(!CreateTestBed())
  {
    return FALSE;
  }

  // Go run the test
	TestRunnerDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int
TestRunnerApp::ExitInstance()
{
  int quitval = CWinApp::ExitInstance();

  if(m_client && m_client->GetLogging())
  {
    m_client->GetLogging()->Reset();
    ShellExecute(NULL,"open",m_client->GetLogging()->GetLogFileName().GetString(),"","",SW_SHOW);
  }

  // In POSIX applications a return value != 0 is an indication of an error
  // Typically values like -3 or 3 or 5 are used for error situations
  // and a zero for "No-error"

  if (quitval == 0 || m_totalResult == 0)
  {
    return 0;  // ERROR
  }
  // Test was a success!
  return m_totalResult;
}

RunTestType
TestRunnerApp::DeduceTestType()
{
  m_type = RunTestType::RunTest_Unknown;

  int pos = m_stepFilename.ReverseFind('.');
  if(pos > 0)
  {
    CString extension = m_stepFilename.Mid(pos);
    if(extension.CompareNoCase(EXTENSION_TESTSTEP_CMD) == 0)
    {
      return (m_type = RunTestType::RunTest_TestStep_Execute);
    }
    else if(extension.CompareNoCase(EXTENSION_TESTSTEP_NET) == 0)
    {
      return (m_type = RunTestType::RunTest_TestStep_Internet);
    }
    else if(extension.CompareNoCase(EXTENSION_TESTSTEP_SQL) == 0)
    {
      return (m_type = RunTestType::RunTest_TestStep_Database);
    }
  }

  pos = m_xsetFilename.ReverseFind('.');
  if(pos > 0)
  {
    CString extension = m_xsetFilename.Mid(pos);
    if(extension.CompareNoCase(EXTENSION_SET) == 0)
    {
      return (m_type = RunTestType::RunTest_TestSet);
    }
  }
  return m_type;
}

bool
TestRunnerApp::CreateTestBed()
{
  // Find the type of test to run
  if(DeduceTestType() == RunTestType::RunTest_Unknown)
  {
    return false;
  }

  switch(m_type)
  {
    case RunTestType::RunTest_TestStep_Execute:  CreateConsole();
                                                 break;
    case RunTestType::RunTest_TestStep_Internet: CreateHTTPClient();
                                                 break;
    case RunTestType::RunTest_TestStep_Database: CreateDatabase();
                                                 break;
    case RunTestType::RunTest_TestSet:           CreateConsole();
                                                 CreateHTTPClient();
                                                 CreateDatabase();
                                                 break;
  }
  return true;
}

void
TestRunnerApp::CreateConsole()
{
  if(m_console)
  {
    delete m_console;
  }

  // Creating the console
  m_console = new ConsoleDlg();
  m_console->Create(MAKEINTRESOURCE(IDD_CONSOLE));
  m_console->OnInitDialog();
  m_console->Move();
  m_console->ShowWindow(SW_SHOW);
}

void
TestRunnerApp::CreateHTTPClient()
{
  if(m_client)
  {
    return;
  }
  m_client = new HTTPClient();
  m_cache  = new OAuth2Cache();
}

void
TestRunnerApp::CreateDatabase()
{
  if(m_database)
  {
    return;
  }
  m_database = new SQLDatabase();
}

/*static*/ unsigned
__stdcall StartTheExecRunner(void* p_data)
{
  ExecRunner runner(theApp.GetBaseDirectory()
                   ,theApp.GetTestDirectory()
                   ,theApp.GetTeststepFilename()
                   ,theApp.GetParametersFilename()
                   ,theApp.GetValidations()
                   ,theApp.GetGlobalValidations()
                   ,theApp.GetConsoleHandle()
                   ,theApp.m_pMainWnd->GetSafeHwnd()
                   ,(HWND)theApp.GetCallingHandle()
                   ,theApp.GetCallingRow()
                   ,theApp.GetGlobal());

  int result = runner.PerformTest();
  theApp.EndApplication(result);
  return (unsigned) result;
}

/*static*/ unsigned
__stdcall StartTheInetRunner(void* p_data)
{
  InetRunner runner(theApp.GetBaseDirectory()
                   ,theApp.GetTestDirectory()
                   ,theApp.GetTeststepFilename()
                   ,theApp.GetParametersFilename()
                   ,theApp.GetValidations()
                   ,theApp.GetGlobalValidations()
                   ,theApp.m_pMainWnd->GetSafeHwnd()
                   ,theApp.GetHTTPClient()
                   ,theApp.GetOauth2Cache()
                   ,(HWND)theApp.GetCallingHandle()
                   ,theApp.GetCallingRow()
                   ,theApp.GetGlobal());

  int result = runner.PerformTest();
  theApp.EndApplication(result);
  return (unsigned) result;
}

/*static*/ unsigned
__stdcall StartTheSQLRunner(void* p_data)
{
  int result = 0;
  {
    SQLRunner runner(theApp.GetDatabase()
                    ,theApp.GetBaseDirectory()
                    ,theApp.GetTestDirectory()
                    ,theApp.GetTeststepFilename()
                    ,theApp.GetParametersFilename()
                    ,theApp.GetValidations()
                    ,theApp.GetGlobalValidations()
                    ,theApp.m_pMainWnd->GetSafeHwnd()
                    ,theApp.m_pMainWnd->GetSafeHwnd()
                    ,(HWND)theApp.GetCallingHandle()
                    ,theApp.GetCallingRow()
                    ,theApp.GetGlobal());

    result = runner.PerformTest();
  }
  theApp.EndApplication(result);
  return (unsigned)result;
}

/*static*/ unsigned
__stdcall StartTheSetRunner(void* p_data)
{
  unsigned totalresult = 1;
  theApp.SetLastTest(false);
  CWnd* report = theApp.m_pMainWnd;

  try
  {
    CString path = theApp.GetBaseDirectory() +
                   theApp.GetTestDirectory() +
                   theApp.GetTestSetFilename();
    TestSet set;
    set.ReadFromXML(path);

    unsigned increment = 100 / (int) set.GetTestRuns().size();
    unsigned total = 0;

    for(auto& test : set.GetTestRuns())
    {
      // Set the filename
      theApp.SetTeststepFilename(test.m_filename);

      // Set total progress
      report->SendMessage(WM_TEST_TOTAL,total,0);
      total += increment;

      // Copy the validation steps
      ValiSteps& steps = theApp.GetValidations();
      steps.clear();
      for(auto& val : test.m_validations)
      {
        steps.push_back(val.m_filename);
      }

      // Start the test
      unsigned result = 0;
      RunTestType type = theApp.DeduceTestType();
      switch(type)
      {
        case RunTestType::RunTest_TestStep_Execute:  result = StartTheExecRunner(nullptr); break;
        case RunTestType::RunTest_TestStep_Internet: result = StartTheInetRunner(nullptr); break;
        case RunTestType::RunTest_TestStep_Database: result = StartTheSQLRunner (nullptr); break;
      }
      totalresult *= result;

      test.m_lastResult = result ? "OK" : "ERROR";
    }
    // Save the results;
    set.WriteToXML();
  }
  catch(StdException& ex)
  {
    StyleMessageBox(nullptr,ex.GetErrorMessage(),"ERROR",MB_OK|MB_ICONERROR);
  }

  theApp.SetLastTest(true);
  theApp.EndApplication(totalresult);
  return totalresult;
}

void
TestRunnerApp::StartRunner()
{
  _beginthreadex_proc_type procedure = nullptr;

  if(m_type == RunTestType::RunTest_TestStep_Execute)
  {
    procedure = &StartTheExecRunner;
  }
  else if(m_type == RunTestType::RunTest_TestStep_Internet)
  {
    procedure = &StartTheInetRunner;
  }
  else if(m_type == RunTestType::RunTest_TestStep_Database)
  {
    procedure = &StartTheSQLRunner;
  }
  else if(m_type == RunTestType::RunTest_TestSet)
  {
    procedure = &StartTheSetRunner;
  }
  else
  {
    StyleMessageBox(nullptr,"Testfile extension not recognized!","Kwatta",MB_OK|MB_ICONERROR);
    return;
  }

  // Start a new thread
  unsigned int threadID = 0L;
  if ((m_thread = (HANDLE)_beginthreadex(NULL,0,procedure,nullptr,0,&threadID)) == INVALID_HANDLE_VALUE)
  {
    threadID = 0;
    m_thread = NULL;
    StyleMessageBox(NULL,"Cannot start a thread for the TestRunner.",PRODUCT_NAME,MB_OK|MB_ICONERROR);
  }
}

void
TestRunnerApp::EndApplication(int p_result)
{
  // Safe for ExitInstance
  m_totalResult = p_result;

  if(m_last && m_pMainWnd)
  {
    m_pMainWnd->PostMessage(WM_TEST_ENDING,(WPARAM)p_result,0);
  }
}

bool
TestRunnerApp::ParseStartParameters()
{
  for (int i = 1; i < __argc; i++)
  {
    LPCSTR lpszParam = __argv[i];

    if (lpszParam[0] == '-' || lpszParam[0] == '/')
    {
           if (_strnicmp(&lpszParam[1], "STEP:",  5) == 0) m_stepFilename  = &lpszParam[6];
      else if (_strnicmp(&lpszParam[1], "XSET:",  5) == 0) m_xsetFilename  = &lpszParam[6];
      else if (_strnicmp(&lpszParam[1], "BASE:",  5) == 0) m_baseDirectory = &lpszParam[6];
      else if (_strnicmp(&lpszParam[1], "TDIR:",  5) == 0) m_testDirectory = &lpszParam[6];
      else if (_strnicmp(&lpszParam[1], "PARAM:", 6) == 0) m_paramFilename = &lpszParam[7];
      else if (_strnicmp(&lpszParam[1], "HWND:",  5) == 0) m_callingHWND   = atoll(&lpszParam[6]);
      else if (_strnicmp(&lpszParam[1], "ROW:",   4) == 0) m_callingROW    = atol (&lpszParam[5]);
      else if (_strnicmp(&lpszParam[1], "VALI:",  5) == 0) m_validations.push_back(CString(&lpszParam[6]));
      else if (_strnicmp(&lpszParam[1], "GLVAL:", 6) == 0) m_globalValid.push_back(CString(&lpszParam[7]));
      else if (_strnicmp(&lpszParam[1], "GLOBAL", 6) == 0) m_global        = true;
      else
      {
        Usage();
        return false;
      }
    }
  }
  if(m_stepFilename.IsEmpty() && m_xsetFilename.IsEmpty())
  {
    StyleMessageBox(NULL, "Cannot start without a <stepname>.xrun or a <setname>.xset file", PRODUCT_NAME, MB_OK | MB_ICONERROR);
    Usage();
    return false;
  }
  if(m_paramFilename.IsEmpty())
  {
    StyleMessageBox(NULL, "Cannot start without a <parameters>.xpar file", PRODUCT_NAME, MB_OK | MB_ICONERROR);
    Usage();
    return false;
  }
  if(m_baseDirectory.IsEmpty())
  {
    StyleMessageBox(NULL,"Cannot start without a BASE directory",PRODUCT_NAME,MB_OK|MB_ICONERROR);
    Usage();
    return false;
  }
  if(m_testDirectory.IsEmpty())
  {
    StyleMessageBox(NULL,"Cannot start without a TEST directory",PRODUCT_NAME,MB_OK | MB_ICONERROR);
    Usage();
    return false;
  }
  // Check formatting of the base directory
  if(m_baseDirectory.Right(1) != '\\')
  {
    m_baseDirectory += '\\';
  }
  if(m_testDirectory.Right(1) != '\\')
  {
    m_testDirectory += '\\';
  }
  return true;
}

void
TestRunnerApp::Usage()
{
  CString usage = "USAGE of this program:\n"
                  "\n"
                  "Testrunner.exe <directory-options> <file-options> [<return-options>]\n"
                  "\n"
                  "Directory options:\n"
                  "/BASE:base-directory\t\tBase-directory of the total test suite\n"
                  "/TDIR:test-directory\t\tTest-directory of the current test set\n"
                  "\n"
                  "File options:\n"
                  "/STEP:<filename>.xrun\t\tCurrent test step to run\n"
                  "/PARAM:<filename>.xpar\t\tLocal parameter file for the test set\n"
                  "/VALI:<filename>.xval\t\tLocal validation steps after running the test\n"
                  "/GLVAL:<filename>.xval\t\tGlobal validation steps after running the test\n"
                  "\t\t\t\tValidations can be added multiple times\n"
                  "\n"
                  "Return options:\n"
                  "/ROW:n\t\t\t\tCurrent row in the calling program, can be 0\n"
                  "/HWND:xx\t\t\tWindow handle to report readiness\n";
  StyleMessageBox(NULL, usage.GetString(), PRODUCT_NAME, MB_OK | MB_ICONEXCLAMATION);
}
