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
#include "TestRunner.h"
#include "TestStepSQL.h"
#include "Parameters.h"
#include "ValidateSQL.h"
#include "StepResultSQL.h"
#include <SQLDatabase.h>
#include <SQLQuery.h>
#include <vector>

using namespace SQLComponents;

class SQLRunner : public TestRunner
{
public:
  SQLRunner(SQLDatabase*  p_database
           ,CString       p_baseDirectory
           ,CString       p_testDirectory
           ,CString       p_testStepFilename
           ,CString       p_parametersFilename
           ,ValiSteps&    p_localValidations
           ,ValiSteps&    p_globalValidations
           ,HWND          p_consoleHWND
           ,HWND          p_reportHWND
           ,HWND          p_callingHWND
           ,int           p_callingROW
           ,bool          p_global);
  virtual ~SQLRunner();

  virtual int     PerformTest();
  virtual void    StopTestProgram();

  StepResultSQL*  GetStepResult();

private:
  // Running the test
  void  InitRunner();
  void  ReadingAllFiles();
  void  ParameterProcessing();
  void  PerformCommand();
  void  PerformAllValidations();
  void  SaveTestResults();
  void  SaveResultParameters();
  int   ReadTotalResult();
  void  SetBoobytrap();
  void  StopBoobytrap();
  // Details
  void  ReadTestStep();
  void  ReadValidations();
  void  ReadResultSet();
  // For a QL test to JSON or XML translation
  virtual void  CreateQLErrorMessage(CString p_error) override;
  virtual int   CheckStatusOK(int p_returnCode);

  // Data specific to an SQL test step
  SQLDatabase*  m_database    { nullptr };
  SQLQuery*     m_query       { nullptr };
};
