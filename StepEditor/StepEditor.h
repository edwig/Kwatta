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
// This File   : StepEditor.h
// What it does: Main program for editing a test step
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __AFXWIN_H__
	#error _T("include 'stdafx.h' before including this file for PCH")
#endif

#include "TestStep.h"
#include "resource.h"		// main symbols
#include <SQLDatabase.h>

#define PRODUCT_NAME _T("StepEditor")

class HTTPClient;
class OAuth2Cache;
class StepInternetDlg;
class StepDatabaseDlg;
class StepWindowsDlg;
using ValiSteps = std::vector<XString>;

using SQLComponents::SQLDatabase;

// StepEditorApp:
// See StepEditor.cpp for the implementation of this class
//

class StepEditorApp : public CWinAppEx
{
public:
	StepEditorApp();
 ~StepEditorApp();

  XString       GetEffectiveStep();
  XString       GetTestStepFilename()       { return m_stepFilename;  }
  XString       GetParametersFilename()     { return m_paramFilename; }
  XString       GetBaseDirectory()          { return m_baseDirectory; }
  XString       GetTestDirectory()          { return m_testDirectory; }
  HTTPClient*   GetHTTPClient()             { return m_client;        }
  OAuth2Cache*  GetOAuth2Cache()            { return m_cache;         }
  ValiSteps&    GetValidations()            { return m_validations;   }
  ValiSteps&    GetGlobalValidations()      { return m_globalValid;   }
  bool          GetGlobal()                 { return m_global;        }
  int           StartTheInetRunner(StepInternetDlg* p_caller);
  int           StartTheSQLRunner (StepDatabaseDlg* p_caller);
  int           StartTheWINRunner (StepWindowsDlg*  p_caller);
  XString       RefreshBearerToken();
  bool          GetBearerTokenIsExpired();
  void          PromoteTestStep(CWnd* p_wnd);
  SQLDatabase*  GetDatabase();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int  ExitInstance();
  void CreateHTTPClient();

  // Implementation

	DECLARE_MESSAGE_MAP()

private:
  bool      ParseStartParameters();
  StepType  DeduceStepType();
  void      Usage();

  XString m_stepFilename;
  XString m_paramFilename;
  XString m_baseDirectory;
  XString m_testDirectory;
  UINT64  m_callingHWND;
  int     m_callingROW;
  int     m_global        { false   };
  HTTPClient*  m_client   { nullptr };
  OAuth2Cache* m_cache    { nullptr };
  SQLDatabase* m_database { nullptr };
  ValiSteps    m_validations;
  ValiSteps    m_globalValid;
};

extern StepEditorApp theApp;
