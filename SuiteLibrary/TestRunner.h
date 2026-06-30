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
// This File   : Runner.h
// What it does: General virtual interface for all runners
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// TEST RUNNER for an EXECutable file making a POSIX program call
// including stdin, stdout and stderr
//
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "Parameters.h"
#include "Credentials.h"
#include "Validate.h"
#include <vector>

// 50 milliseconds is the smallest amount of waiting time
#define MINIMUM_INTERVAL_TIME  50 

using ValiSteps = std::vector<XString>;
class TestStep;
class StepResult;
class LogAnalysis;

class TestRunner
{
public:
  TestRunner(XString    p_baseDirectory
            ,XString    p_testDirectory
            ,XString    p_testStepFilename
            ,XString    p_parametersFilename
            ,ValiSteps& p_localValidations
            ,ValiSteps& p_globalValidations
            ,HWND       p_consoleHWND
            ,HWND       p_reportHWND
            ,HWND       p_callingHWND
            ,int        p_callingROW
            ,bool       p_global
            ,bool       p_loadtest);

  virtual ~TestRunner();

  // General interface
  virtual int  PerformTest() = 0;
  virtual void StopTestProgram() = 0;

  // GETTERS
  XString GetEffectiveStepFilename();
  int     GetMaxRunningTime();

protected:
  // General functions
  void ReadParameters();
  void ReadCredentials();
  void PerformStep(XString p_stepName);
  void PreCommandWaiting();
  void PostCommandWaiting();
  void WaitingForATimeout(XString p_stepname, int p_milliseconds);

  // Telling it the outside world
  void  SetTest(XString p_test);
  void  SetStep(XString p_step);
  void  SetProgress(int p_percent);
  void  EndTesting(int p_result);

  // QL Scripting
  virtual int   PerformQLScript(int p_result);
  virtual int   CheckStatusOK(int p_returnCode) = 0;
  virtual void  CreateQLErrorMessage(XString p_error);

  // General data for a test step
  XString     m_baseDirectory;
  XString     m_testDirectory;
  XString     m_testStepFilename;
  XString     m_parametersFilename;
  ValiSteps   m_localValidations;
  ValiSteps   m_globalValidations;

  HWND        m_consoleHNWD{  NULL };
  HWND        m_reportHWND {  NULL };
  HWND        m_callingHWND{  NULL };
  int         m_callingROW {     0 };
  HANDLE      m_thread     {  NULL };
  int         m_steps      {     0 };
  int         m_stepSize   {    12 };
  int         m_progress   {     0 };
  bool        m_global     { false };
  bool        m_loadtest   { false };
  Parameters  m_parameters;
  Credentials m_credentials;
  Validations m_validations;
  // The test and result of the test
  TestStep*   m_testStep { nullptr };
  StepResult* m_result   { nullptr };
  // Running a QL script
  LogAnalysis* m_logfile { nullptr };
  bool         m_running { true    };
  int          m_iterations {  0   };
};
