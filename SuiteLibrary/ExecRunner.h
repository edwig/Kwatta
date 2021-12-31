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
#include "TestStepCL.h"
#include "Parameters.h"
#include "ValidateCL.h"
#include "StepResultCL.h"
#include <vector>

// 50 milliseconds is the smallest amount of waiting time
#define MINIMUM_INTERVAL_TIME  50 

using ValiSteps = std::vector<CString>;

class ExecRunner
{
public:
  ExecRunner(CString    p_baseDirectory
            ,CString    p_testDirectory
            ,CString    p_testStepFilename
            ,CString    p_parametersFilename
            ,ValiSteps& p_validations
            ,HWND       p_consoleHWND
            ,HWND       p_reportHWND
            ,HWND       p_callingHWND
            ,int        p_callingROW);
 ~ExecRunner();

  int   PerformTest();

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
  TestStepCL    m_testStep;
  Parameters    m_parameters;
  ValiSteps     m_valiSteps;
  Validations   m_validations;
  StepResultCL  m_result;
  HWND          m_callingHWND { NULL };
  int           m_callingROW  { 0    };
};
