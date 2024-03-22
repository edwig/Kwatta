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
// This File   : TestRunner.h
// What it does: Running a test-set or single test-step
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "ConsoleDlg.h"
#include <SQLDatabase.h>
#include <vector>

using SQLComponents::SQLDatabase;

#ifndef __AFXWIN_H__
	#error _T("include 'stdafx.h' before including this file for PCH")
#endif

#include "resource.h"		// main symbols

// Database user and password
extern CString db_database;
extern CString db_user;
extern CString db_password;

// Start parameters, starting at the default
extern int    qlargc;
extern TCHAR** qlargv;


#define PRODUCT_NAME _T("Testrunner")
using ValiSteps = std::vector<CString>;

class HTTPClient;
class OAuth2Cache;

enum class RunTestType
{
  RunTest_Unknown
 ,RunTest_TestStep_Execute
 ,RunTest_TestStep_Internet
 ,RunTest_TestStep_Database
 ,RunTest_TestStep_Windows
 ,RunTest_TestSet
};

class TestRunnerApp : public CWinApp
{
public:
	TestRunnerApp();
  virtual ~TestRunnerApp();

  // Start and stop
  void          StartRunner();
  void          EndApplication(int p_result);
  // Interface from the command line
  CString       GetTeststepFilename()   { return m_stepFilename;  }
  CString       GetTestSetFilename()    { return m_xsetFilename;  }
  CString       GetParametersFilename() { return m_paramFilename; }
  CString       GetBaseDirectory()      { return m_baseDirectory; }
  CString       GetTestDirectory()      { return m_testDirectory; }
  ValiSteps&    GetValidations()        { return m_validations;   }
  ValiSteps&    GetGlobalValidations()  { return m_globalValid;   }
  HWND          GetConsoleHandle()      { return m_console->GetSafeHwnd(); }
  HTTPClient*   GetHTTPClient()         { return m_client;        }
  OAuth2Cache*  GetOauth2Cache()        { return m_cache;         }
  SQLDatabase*  GetDatabase()           { return m_database;      }
  UINT64        GetCallingHandle()      { return m_callingHWND;   }
  int           GetCallingRow()         { return m_callingROW;    }
  bool          GetLastTest()           { return m_last;          }
  bool          GetGlobal()             { return m_global;        }
  bool          GetLoadtest()           { return m_loadtest;      }

  // SETTERS

  RunTestType   DeduceTestType();
  void          SetTeststepFilename(CString p_file) { m_stepFilename = p_file; }
  void          SetLastTest(bool p_last)            { m_last         = p_last; }

  // FUNCTIONS
  void          ResetHTTPClient();

private:
  bool          ParseStartParameters();
  bool          CreateTestBed();
  void          CreateConsole();
  void          CreateHTTPClient();
  void          CreateDatabase();
  void          Usage();

  // Data to remember
  CString       m_stepFilename;
  CString       m_xsetFilename;
  CString       m_paramFilename;
  CString       m_baseDirectory;
  CString       m_testDirectory;
  ValiSteps     m_validations;
  ValiSteps     m_globalValid;
  RunTestType   m_type        { RunTestType::RunTest_Unknown };
  // The test objects
  HANDLE        m_thread      { NULL    };
  ConsoleDlg*   m_console     { nullptr };
  HTTPClient*   m_client      { nullptr };
  OAuth2Cache*  m_cache       { nullptr };
  SQLDatabase*  m_database    { nullptr };
  bool          m_last        { true    };
  bool          m_loadtest    { false   };
  // Results
  int           m_totalResult { 0     };
  UINT64        m_callingHWND { NULL  };
  int           m_callingROW  { 0     };
  bool          m_global      { false };

public:
  // Overrides
  virtual BOOL  InitInstance();
  virtual int   ExitInstance();

  // Implementation
	DECLARE_MESSAGE_MAP()
};

extern TestRunnerApp theApp;
