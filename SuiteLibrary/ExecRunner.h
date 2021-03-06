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
// This File   : ExecRunner.h
// What it does: Running one (1) command line executable test
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
#include "TestStepCMD.h"
#include "Parameters.h"
#include "ValidateCMD.h"
#include "StepResultCMD.h"
#include <vector>

// 50 milliseconds is the smallest amount of waiting time
#define MINIMUM_INTERVAL_TIME  50 

using ValiSteps = std::vector<CString>;
class RunRedirect;

class ExecRunner
{
public:
  ExecRunner(CString    p_baseDirectory
            ,CString    p_testDirectory
            ,CString    p_testStepFilename
            ,CString    p_parametersFilename
            ,ValiSteps& p_localValidations
            ,ValiSteps& p_globalValidations
            ,HWND       p_consoleHWND
            ,HWND       p_reportHWND
            ,HWND       p_callingHWND
            ,int        p_callingROW
            ,bool       p_global);
 ~ExecRunner();

  int     PerformTest();
  CString GetEffectiveStepFilename();
  int     GetMaxRunningTime();
  void    StopTestProgram();

private:
  // Running the test
  void  InitRunner();
  void  ReadingAllFiles();
  void  ParameterProcessing();
  void  SetExtraEnvironmentStrings();
  void  PreCommandWaiting();
  void  PerformCommand();
  void  PostCommandWaiting();
  void  PerformAllValidations();
  void  SaveTestResults();
  void  SaveResultParameters();
  int   ReadTotalResult();
  void  SetBoobytrap();
  void  StopBoobytrap();
  // Details
  void  ReadTestStep();
  void  ReadParameters();
  void  ReadValidations();
  void  PerformStep(CString p_stepName);
  void  WaitingForATimeout(CString p_stepname,int p_miliseconds);

  // Telling it the outside world
  void  SetTest(CString p_test);
  void  SetStep(CString p_step);
  void  SetProgress(int p_percent);
  void  EndTesting(int p_result);

  CString       m_baseDirectory;
  CString       m_testDirectory;
  CString       m_testStepFilename;
  CString       m_parametersFilename;
  HWND          m_reportHWND { NULL };
  HWND          m_consoleHNWD{ NULL };
  int           m_steps      {  0 };
  int           m_stepSize   { 12 };
  int           m_progress   {  0 };
  TestStepCMD    m_testStep;
  Parameters    m_parameters;
  ValiSteps     m_localValidations;
  ValiSteps     m_globalValidations;
  Validations   m_validations;
  StepResultCMD  m_result;
  HWND          m_callingHWND { NULL    };
  int           m_callingROW  { 0       };
  bool          m_global      { false   };
  HANDLE        m_thread      { NULL    };
  RunRedirect*  m_redirect    { nullptr };
};
