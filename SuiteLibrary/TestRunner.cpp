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
// This File   : TestRunner.cpp
// What it does: Running one (1) test (General interface)
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TestRunner.h"
#include "TestStep.h"
#include "StepResult.h"
#include "TestMessages.h"
#include "ExtraMessages.h"
#include <LogAnalysis.h>
#include <QL_vm.h>
#include <QL_Interpreter.h>
#include <QL_Exception.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

TestRunner::TestRunner(CString    p_baseDirectory
                      ,CString    p_testDirectory
                      ,CString    p_testStepFilename
                      ,CString    p_parametersFilename
                      ,ValiSteps& p_localValidations
                      ,ValiSteps& p_globalValidations
                      ,HWND       p_consoleHWND
                      ,HWND       p_reportHWND
                      ,HWND       p_callingHWND
                      ,int        p_callingROW
                      ,bool       p_global
                      ,bool       p_loadtest)
           :m_baseDirectory     (p_baseDirectory)
           ,m_testDirectory     (p_testDirectory)
           ,m_testStepFilename  (p_testStepFilename)
           ,m_parametersFilename(p_parametersFilename)
           ,m_localValidations  (p_localValidations)
           ,m_globalValidations (p_globalValidations)
           ,m_consoleHNWD       (p_consoleHWND)
           ,m_reportHWND        (p_reportHWND)
           ,m_callingHWND       (p_callingHWND)
           ,m_callingROW        (p_callingROW)
           ,m_global            (p_global)
           ,m_loadtest          (p_loadtest)
{
}

TestRunner::~TestRunner()
{
  // Clean out all validations
  for (auto& vali : m_validations)
  {
    delete vali;
  }
  m_validations.clear();

  // Clean out the test and result
  if(m_testStep)
  {
    delete m_testStep;
    m_testStep = nullptr;
  }
  if(m_result)
  {
    delete m_result;
    m_result = nullptr;
  }
}

CString
TestRunner::GetEffectiveStepFilename()
{
  CString filename(m_baseDirectory);
  filename += m_global ? CString(_T("Steps\\")) : m_testDirectory;
  filename += m_testStepFilename;

  return filename;
}

void
TestRunner::ReadParameters()
{
  // read the global parameters
  CString filename = m_baseDirectory + _T("Parameters.xpar");
  m_parameters.ReadFromXML(filename);

  // read the definition of the test parameters
  filename = m_baseDirectory + m_testDirectory + m_parametersFilename;
  m_parameters.ReadFromXML(filename, false);
}

void
TestRunner::ReadCredentials()
{
  // read the global credentials
  CString filename = m_baseDirectory + _T("Credentials.cred");
  m_credentials.ReadFromXML(filename);
}

int
TestRunner::GetMaxRunningTime()
{
  return _ttoi(m_testStep->GetEffectiveMaxExecution());
}

// Perform the next step in the total progress
void
TestRunner::PerformStep(CString p_stepName)
{
  SetStep(p_stepName);
  m_progress += m_stepSize;
  SetProgress(m_progress);
}

// Waiting before we run a command
void
TestRunner::PreCommandWaiting()
{
  CString step(_T("Pre-command waiting."));
  PerformStep(step);

  int time = _ttoi(m_testStep->GetEffectiveWaitBeforeRun());
  if (time > 0)
  {
    WaitingForATimeout(step, time);
  }
}

// Waiting for cleanup after a command
void
TestRunner::PostCommandWaiting()
{
  CString step(_T("Post-command waiting."));
  PerformStep(step);

  int time = _ttoi(m_testStep->GetEffectiveWaitAfterRun());
  if (time >= 0)
  {
    WaitingForATimeout(step, time);
  }
}

void
TestRunner::WaitingForATimeout(CString p_stepname, int p_milliseconds)
{
  // Reset the progress step-name and progress-control bar
  p_stepname.AppendFormat(_T(" %.3f seconds"),((double)p_milliseconds / 1000.0));
  SetStep(p_stepname);
  SetProgress(0);

  // Preset the progress
  int progress = 1;
  int count    = 100;
  int interval = p_milliseconds / 100;

  // Progress intervals cannot be to short
  if (interval < MINIMUM_INTERVAL_TIME)
  {
    count    = (p_milliseconds / MINIMUM_INTERVAL_TIME) + 1;
    interval = p_milliseconds / count;
    progress = 100 / count;
  }

  // Perform the waiting
  for (int index = 0;index < count;++index)
  {
    SetProgress(index * progress);
    Sleep(interval);
  }

  // Progress complete
  SetProgress(100);
}

//////////////////////////////////////////////////////////////////////////
//
// TELLING IT THE OUTSIDE WORLD
//
//////////////////////////////////////////////////////////////////////////

void
TestRunner::SetTest(CString p_test)
{
  if(m_reportHWND)
  {
    ::SendMessage(m_reportHWND,WM_TEST_TESTNAME,(WPARAM)p_test.GetString(),0);
  }
}

void
TestRunner::SetStep(CString p_step)
{
  if(m_reportHWND)
  {
    ::SendMessage(m_reportHWND,WM_TEST_STEPNAME,(WPARAM)p_step.GetString(),0);
  }
}

void
TestRunner::SetProgress(int p_percent)
{
  if(m_reportHWND)
  {
    ::SendMessage(m_reportHWND,WM_TEST_PROGRESS,(WPARAM)p_percent,0);
  }
}

void
TestRunner::EndTesting(int p_result)
{
  if(m_reportHWND)
  {
    ::SendMessage(m_reportHWND,WM_TEST_ENDING,(WPARAM)p_result,0);
  }
  if(m_callingHWND && m_callingROW)
  {
    ::PostMessage(m_callingHWND,WM_READYTEST,(WPARAM)m_callingROW,p_result);
    ::PostMessage(m_callingHWND,WM_READYVALI,(WPARAM)m_callingROW,p_result);
  }
}

//////////////////////////////////////////////////////////////////////////
//
// Running a QL Script after the test step
//
//////////////////////////////////////////////////////////////////////////

__declspec(thread) LogAnalysis* runlog = nullptr;

void osputs_stdout(const TCHAR* str)
{
  static CString out;
  int len = (int)_tcslen(str);
  if (len > 0 && str[len - 1] == '\n')
  {
    out += str;

    if (runlog)
    {
      out = out.TrimRight(_T("\n"));
      out.Replace(_T("\r"), _T(""));
      runlog->BareStringLog(out);
    }
    else
    {
      OutputDebugString(out);
    }
    out.Empty();
  }
  else
  {
    out += str;
  }
}

void osputs_stderr(const TCHAR* str)
{
  osputs_stdout(str);
}

// Default implementation of a QL error messages
void
TestRunner::CreateQLErrorMessage(CString p_error)
{
  CString error(_T("ERROR: "));
  error += p_error;
  osputs_stderr(error.GetString());
}

// Perform a script
// Input p_result == 1 -> All validations where OK
// Input p_result == 0 -> Errors in validations 
int
TestRunner::PerformQLScript(int p_result)
{
  // Reset the running status
  m_running = false;

  // See if we must run the script
  ScriptStatus status = m_testStep->GetScriptStatus();
  if(status == ScriptStatus::NoScript)
  {
    // Pass back the original result
    return p_result;
  }

  int retCode = 0;
  try
  {
    // Getting the script to run
    // But first effectuate the output parameters of the last run command
    int unbound = m_testStep->EffectiveReplacements(&m_parameters,false);
    if (unbound)
    {
      throw QLException(_T("Parameters not replaced. ") + m_parameters.GetUnboundErrors(),unbound);
    }
    CString script = m_testStep->GetEffectiveScriptToRun();

    // Write to the logfile (if any)
    bool trace_run = false;
    bool trace_cmp = false;
    if(m_logfile)
    {
      runlog    = m_logfile;
      trace_run = m_logfile->GetLogLevel() >= HLL_TRACE;
      trace_cmp = m_logfile->GetLogLevel() >= HLL_TRACEDUMP;

      m_logfile->AnalysisLog(_T(__FUNCTION__),LogType::LOG_INFO,true, _T("Validation errors: %d"),p_result);
      m_logfile->AnalysisLog(_T(__FUNCTION__),LogType::LOG_INFO,false,_T("Running QL Script."));
      if(trace_run)
      {
        if(trace_cmp)
        {
          osputs_stdout(_T("Script before replacements:\n"));
          osputs_stdout(m_testStep->GetScriptToRun() + _T("\n"));
          osputs_stdout(_T("Script after replacements:\n"));
        }
        osputs_stdout(script);
        osputs_stdout(_T("\n"));
      }
    }

    // Compile our script first
    QLVirtualMachine vm;
    if(!vm.CompileBuffer(script,trace_cmp))
    {
      return 1;
    }
    // Set up an interpreter
    QLInterpreter inter(&vm,trace_run);
    CString entrypoint(_T("main"));

    // Set the latest test status
    inter.SetTestIterations(m_iterations);
    inter.SetTestResult(p_result);
    inter.SetTestRunning(0);

    // GO run our QL-script
    retCode = inter.Execute(entrypoint);

    // Should we continue running?
    m_running = inter.GetTestRunning();
  }
  catch(QLException& ex)
  {
    CreateQLErrorMessage(ex.GetErrorMessage());
    m_running = 0;
    return 1;
  }
  // Flush the log from the script
  osputs_stdout(_T("\n"));

  // Combine all the status codes
  switch(status)
  {
    case ScriptStatus::SuccessIsZero:     if(retCode != 0)
                                          {
                                            p_result = 0;
                                          }
                                          break;
    case ScriptStatus::SuccessIsNegative: if(retCode >= 0 || CheckStatusOK(retCode) == 0)
                                          {
                                            p_result = 0;
                                          }
                                          break;
    case ScriptStatus::SuccessIsPositive: if(retCode <= 0 || CheckStatusOK(retCode) == 0)
                                          {
                                            p_result = 0;
                                          }
                                          break;
  }
  if(m_logfile)
  {
    m_logfile->AnalysisLog(_T(__FUNCTION__),LogType::LOG_INFO,true,_T("QL Script. Return value: %d"),retCode);
    m_logfile->AnalysisLog(_T(__FUNCTION__),LogType::LOG_INFO,true,_T("QL Script. Extra iteration: %s"),m_running ? _T("Yes") : _T("No"));
  }
  return p_result;
}

