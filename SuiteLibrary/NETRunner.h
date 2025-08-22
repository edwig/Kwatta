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
// This File   : NETRunner.h
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
#include "TestRunner.h"
#include "TestStepNET.h"
#include "Parameters.h"
#include "ValidateNET.h"
#include "StepResultNET.h"
#include <HTTPMessage.h>
#include <vector>

using ValiSteps = std::vector<CString>;
class HTTPClient;
class OAuth2Cache;

class NETRunner : public TestRunner
{
public:
  NETRunner(CString      p_baseDirectory
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
           ,bool         p_global
           ,bool         p_loadtest);
  virtual ~NETRunner();

  // General interface
  virtual int     PerformTest();
  virtual void    StopTestProgram();

  // GETTERS
  StepResultNET*  GetStepResult();

private:
  // Running the test
  void  InitRunner();
  void  CleanUp();
  void  ReadingAllFiles();
  void  ParameterProcessing();
  void  StartingLogfile();
  void  PerformAuthentication();
  void  PerformCommand();
  void  PerformAllValidations();
  void  SaveTestResults();
  void  SaveResultParameters();
  int   ReadTotalResult();
  void  SetBoobytrap();
  void  StopBoobytrap();
  // For a QL test to JSON or XML translation
  virtual void  CreateQLErrorMessage(CString p_error) override;
  virtual int   CheckStatusOK(int p_returnCode);

  // Setting authentication
  void  SetBasicAuthentication();
  void  SetNTLMAuthentication();
  void  SetNTLMSSOAuthentication();
  void  SetOAuth2Authentication();
  void  SetKeyHeaderAuthentication();

  // Details
  void  ReadTestStep();
  void  ReadValidations();
  void  PrepareMessage();
  void  ExamineMessage();

  // Data specific to a INET test step
  HTTPMessage*  m_message     { nullptr };
  HTTPClient*   m_client      { nullptr };
  OAuth2Cache*  m_oauth       { nullptr };
  bool          m_isJson      { false   };
};
