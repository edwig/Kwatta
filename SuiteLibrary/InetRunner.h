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
// This File   : InetRunner.h
// What it does: Running one (1) HTTPMessage call to the internet
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// TEST RUNNER for a HTTPMesssage making a call to the internet
// including stdin, stdout and stderr
//
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "TestStepNET.h"
#include "Parameters.h"
#include "ValidateNET.h"
#include "StepResultNET.h"
#include <HTTPMessage.h>
#include <vector>

// 50 milliseconds is the smallest amount of waiting time
#define MINIMUM_INTERVAL_TIME  50 

using ValiSteps = std::vector<CString>;
class HTTPClient;
class OAuth2Cache;

class InetRunner
{
public:
  InetRunner(CString      p_baseDirectory
            ,CString      p_testDirectory
            ,CString      p_testStepFilename
            ,CString      p_parametersFilename
            ,ValiSteps&   p_localValidations
            ,ValiSteps&   p_globalValidations
            ,HWND         p_reportHWND
            ,HTTPClient*  p_client
            ,OAuth2Cache* p_cache
            ,HWND         p_callingHWND
            ,int          p_callingROW
            ,bool         p_global);
  ~InetRunner();

  int   PerformTest();

  // GETTERS
  StepResultNET* GetStepResult() { return &m_result; }
  CString        GetEffectiveStepFilename();
  int            GetMaxRunningTime();
  void           DisconnectClient();

private:
  // Running the test
  void  InitRunner();
  void  CleanUp();
  void  ReadingAllFiles();
  void  ParameterProcessing();
  void  StartingLogfile();
  void  PerformAuthentication();
  void  PreCommandWaiting();
  void  PerformCommand();
  void  PostCommandWaiting();
  void  PerformAllValidations();
  void  SaveTestResults();
  void  SaveResultParameters();
  int   ReadTotalResult();
  int   PerformQLScript(int p_result);
  int   CheckStatusOK(int p_returnCode);
  void  CreateQLErrorMessage(CString p_error);
  void  SetBoobytrap();
  void  StopBoobytrap();

  // Setting authentication
  void  SetBasicAuthentication();
  void  SetNTLMAuthentication();
  void  SetNTLMSSOAuthentication();
  void  SetOAuth2Authentication();

  // Details
  void  ReadTestStep();
  void  ReadParameters();
  void  ReadValidations();
  void  PerformStep(CString p_stepName);
  void  WaitingForATimeout(CString p_stepname,int p_milliseconds);
  void  PrepareMessage();
  void  ExamineMessage();

  // Telling it the outside world
  void  SetTest(CString p_test);
  void  SetStep(CString p_step);
  void  SetProgress(int p_percent);
  void  EndTesting(int p_result);

  CString       m_baseDirectory;
  CString       m_testDirectory;
  CString       m_testStepFilename;
  CString       m_parametersFilename;
  HWND          m_reportHWND  { NULL };
  int           m_steps       { 0    };
  int           m_stepSize    { 12   };
  int           m_progress    { 0    };
  TestStepNET   m_testStep;
  Parameters    m_parameters;
  ValiSteps     m_localValidations;
  ValiSteps     m_globalValidations;
  Validations   m_validations;
  StepResultNET m_result;
  HTTPMessage*  m_message     { nullptr };
  HTTPClient*   m_client      { nullptr };
  OAuth2Cache*  m_oauth       { nullptr };
  LogAnalysis*  m_logfile     { nullptr };
  HWND          m_callingHWND { NULL    };
  int           m_callingROW  { 0       };
  bool          m_global      { false   };
  HANDLE        m_thread      { NULL    };
  bool          m_running     { true    };
  int           m_interations { 0       };
  bool          m_isJson      { false   };
};
