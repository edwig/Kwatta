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
// This File   : CMDRunner.h
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
#include "TestRunner.h"
#include "TestStepCMD.h"
#include "Parameters.h"
#include "ValidateCMD.h"
#include "StepResultCMD.h"
#include <vector>

class RunRedirect;

class CMDRunner : public TestRunner
{
public:
  CMDRunner(CString    p_baseDirectory
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
  virtual ~CMDRunner();

  // General interface
  virtual int     PerformTest();
  virtual void    StopTestProgram();

private:
  // Running the test
  void  InitRunner();
  void  ReadingAllFiles();
  void  ParameterProcessing();
  void  SetExtraEnvironmentStrings();
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
  // For a QL test to JSON or XML translation
  virtual void  CreateQLErrorMessage(CString p_error) override;
  virtual int   CheckStatusOK(int p_returnCode);

  // Data specific to a command-line test step
  RunRedirect*  m_redirect { nullptr };
};
