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
// This File   : SQLRunner.h
// What it does: Running one (1) database SQL test
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "TestStepSQL.h"
#include "Parameters.h"
#include "ValidateSQL.h"
#include "StepResultSQL.h"
#include <SQLDatabase.h>
#include <SQLQuery.h>
#include <vector>

using namespace SQLComponents;
using ValiSteps = std::vector<CString>;

class SQLRunner
{
public:
  SQLRunner(CString    p_baseDirectory
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
 ~SQLRunner();

  int     PerformTest();
  CString GetEffectiveStepFilename();
  int     GetMaxRunningTime();
  void    StopTestProgram();
  StepResultSQL* GetStepResult();

private:
  // Running the test
  void  InitRunner();
  void  ReadingAllFiles();
  void  ParameterProcessing();
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
  void  ReadResultSet();

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
  TestStepSQL   m_testStep;
  Parameters    m_parameters;
  ValiSteps     m_localValidations;
  ValiSteps     m_globalValidations;
  Validations   m_validations;
  StepResultSQL m_result;
  HWND          m_callingHWND { NULL    };
  int           m_callingROW  { 0       };
  bool          m_global      { false   };
  HANDLE        m_thread      { NULL    };
  // Database objects
  SQLDatabase*  m_database    { nullptr };
  SQLQuery*     m_query       { nullptr };
};
