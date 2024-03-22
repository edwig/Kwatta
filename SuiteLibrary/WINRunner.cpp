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
// This File   : WINRunner.cpp
// What it does: Running one (1) Windows UI test
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WINRunner.h"
#include "StepResultWIN.h"
#include "ValidateWIN.h"
#include "StdException.h"
#include "TestMessages.h"
#include "ExtraMessages.h"
#include "ExtraExtensions.h"
#include <HPFCounter.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// 50 milliseconds is the smallest amount of waiting time
#define MINIMUM_INTERVAL_TIME  50 

WINRunner::WINRunner(CString      p_baseDirectory
                    ,CString      p_testDirectory
                    ,CString      p_testStepFilename
                    ,CString      p_parametersFilename
                    ,ValiSteps&   p_localValidations
                    ,ValiSteps&   p_globalValidations
                    ,HWND         p_consoleHWND
                    ,HWND         p_reportHWND
                    ,HWND         p_callingHWND
                    ,int          p_callingROW
                    ,bool         p_global)
           :TestRunner(p_baseDirectory
                      ,p_testDirectory
                      ,p_testStepFilename
                      ,p_parametersFilename
                      ,p_localValidations
                      ,p_globalValidations
                      ,p_consoleHWND
                      ,p_reportHWND
                      ,p_callingHWND
                      ,p_callingROW
                      ,p_global
                      ,false)
{
  m_testStep = new TestStepWIN();
  m_result   = new StepResultWIN();
}

WINRunner::~WINRunner()
{
}

// General start of the test
// Public part, called by the main TestRunnerApp
int
WINRunner::PerformTest()
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  // Predefined result will be 0 (failed)
  int result = 0;

  try
  {
    // Init the runner
    InitRunner();
    // Reading the test files (1 step)
    ReadingAllFiles();
    // Initial parameter processing (1 step)
    ParameterProcessing();

    while(m_running)
    {
      // One (extra) iteration
      ++m_iterations;

      // Perform the test (3 steps)
      PreCommandWaiting();
      PerformCommand();
      PostCommandWaiting();
      // Perform the validations (x * 1 steps)
      PerformAllValidations();
      // Write the results (1 step)
      SaveTestResults();
      // Save return parameters (if any)
      SaveResultParameters();
      // Return the conclusion (1 step)
      result = ReadTotalResult();
      // Possibly run our script, controlling m_running
      result = PerformQLScript(result);
      // Tell it to our callers
      EndTesting(result);
    }
  }
  catch(StdException& ex)
  {
    // WHOA: Something went wrong
    StyleMessageBox(NULL,ex.GetErrorMessage(),_T("Kwatta"),MB_OK|MB_ICONERROR|MB_SETFOREGROUND);
    // Test did went WRONG!!
    result = 0;
  }
  return result;
}

void
WINRunner::StopTestProgram()
{
  // Do Nothing! no interaction yet
}

StepResultWIN* 
WINRunner::GetStepResult()
{
  return dynamic_cast<StepResultWIN*>(m_result);
}

//////////////////////////////////////////////////////////////////////////
//
// PRIVATE
//
//////////////////////////////////////////////////////////////////////////

void
WINRunner::InitRunner()
{
  // Reset the dialog
  SetTest(_T("Initializing"));
  SetStep(_T(""));
  SetProgress(0);

  // Calculate the number of steps
  // 8 steps are from: 
  // ---------------------------------
  // 1   Reading the files
  // 2   Initial parameter processing
  // 3   Testing pre command waiting
  // 4   Testing RUNNING THE WINDOWS UI TEST
  // 5   Testing post command waiting
  // 6   Writing the results file
  // 7   Writing result parameters
  // 8   Reaching a conclusion

  // +   1 for every validation step
  m_steps    = 8 + (int) m_localValidations.size();
  m_stepSize = 100 / m_steps;
}

void
WINRunner::ReadingAllFiles()
{
  SetTest(_T("Reading all definition files"));
  PerformStep(_T("XML files..."));

  ReadTestStep();
  ReadParameters();
  ReadValidations();

  // Now set the name proper to the given test step
  SetTest(m_testStep->GetName());
}

void
WINRunner::ParameterProcessing()
{
  int unbound = 0;
  PerformStep(_T("Parameter processing..."));

  // Effectuate the parameters for the step
  unbound = m_testStep->EffectiveReplacements(&m_parameters,false);

  // Effectuate the parameters for the validation steps
  for(auto& validate : m_validations)
  {
    unbound += validate->EffectiveReplacements(&m_parameters,false);
  }

  // Cannot perform a test step if still unbound parameters exists
  if(unbound)
  {
    CString error;
    error.Format(_T("Cannot perform test. Existing unbound parameters: %d"),unbound);
    throw StdException(error);
  }
}

void
WINRunner::PerformCommand()
{
  PerformStep(_T("WINDOWS UI..."));
  TestStepWIN*   step   = dynamic_cast<TestStepWIN*>(m_testStep);
  StepResultWIN* result = dynamic_cast<StepResultWIN*>(m_result);

  // Take name and documentation
  m_result->SetName(m_testStep->GetName());
  m_result->SetDocumentation(m_testStep->GetDocumentation());
  result->SetLastOSError(0);
  result->SetErrorString(_T(""));

  HPFCounter  counter;

  // See if we must set a boobytrap
  if(m_testStep->GetKillOnTimeout())
  {
    int maxtime = _ttoi(m_testStep->GetEffectiveMaxExecution());
    if (maxtime > 0)
    {
      SetBoobytrap();
    }
  }

  CString log;
  CString errors;
  UINT    error(0);
  try
  {

    // HERE COMES THE UI TESTING
    int index = 0;
    WinActionList& list = step->GetActions();
    for(auto& action : list)
    {
      log.AppendFormat(_T("Run action: %d\n"),++index);
      if(action->PerformAction(log,errors,error) != 0)
      {
        break;
      }
      if(!action->m_effectiveWait.IsEmpty())
      {
        log.AppendFormat(_T("Sleeping [%s] ms\n"),action->m_effectiveWait.GetString());
        int wait = _ttoi(action->m_effectiveWait);
        Sleep(wait);
      }
    }
  }
  catch(StdException& ex)
  {
    errors += ex.GetErrorMessage();
    if(error == 0)
    {
      error = ::GetLastError();
    }
    error = max(1,error);
  }
  result->SetLastOSError(error);
  result->SetErrorString(errors);
  result->AddResultString(log);

  // ::PostMessage(theApp.GetConsoleHandle(),WM_SHOWWINDOW,FALSE,0);


  // Stop the high-performance timer!
  counter.Stop();
  m_result->SetTiming(counter.GetCounter());
}

void
WINRunner::PerformAllValidations()
{
  int step = 1;
  StepResultWIN* result = dynamic_cast<StepResultWIN*>(m_result);

  for (auto& vali : m_validations)
  {
    ValidateWIN* validate = dynamic_cast<ValidateWIN*>(vali);
    PerformStep(_T("Validation: ") + validate->GetName());

    // Do the validations
    bool totalresult = true;
    if (validate->ValidateReturnValue(result->GetLastOSError())  == false) totalresult = false;
    if (validate->ValidateLogging    (result->GetResultString()) == false) totalresult = false;
    if (validate->ValidateErrors     (result->GetErrorString())  == false) totalresult = false;

    // Add the validation to the result set
    result->AddValidation(step++,validate->GetName(),validate->GetFilename(),totalresult,validate->GetGlobal());
  }
}

// Saving the test results to the disk
void
WINRunner::SaveTestResults()
{
  PerformStep(_T("Saving the test results"));
  StepResultWIN* result = dynamic_cast<StepResultWIN*>(m_result);

  CString filename = m_baseDirectory + m_testDirectory + m_testStepFilename;
  filename.MakeLower();
  filename.Replace(EXTENSION_TESTSTEP_WIN,EXTENSION_RESULT_WIN);

  if(result->WriteToXML(filename) == false)
  {
    CString error;
    error.Format(_T("Cannot save results file: %s"),filename.GetString());
    throw StdException(error);
  }

  // Eventually changed parameters need to be written
  m_parameters.WriteToXML();
}

// Save return parameters (if any)
void
WINRunner::SaveResultParameters()
{
  PerformStep(_T("Saving result parameters"));
  // Write back local parameters (return + stream)
  m_parameters.WriteToXML();
}

// Conclusion of the test
int
WINRunner::ReadTotalResult()
{
  PerformStep(_T("Getting total result..."));
  StepResultWIN* result = dynamic_cast<StepResultWIN*>(m_result);
  return result->GetTotalResult();
}

//////////////////////////////////////////////////////////////////////////
//
// DETAILS

void
WINRunner::ReadTestStep()
{
  // Read in the definition file for a test step
  TestStepWIN* step = dynamic_cast<TestStepWIN*>(m_testStep);
  CString filename = GetEffectiveStepFilename();
  step->ReadFromXML(filename);
}

// Reads all validations from the command line
// Could be more than 1 file in all...
void
WINRunner::ReadValidations()
{
  for(auto& filename : m_localValidations)
  {
    Validate* validate = new ValidateWIN();
    CString file = m_baseDirectory + m_testDirectory + filename;
    validate->ReadFromXML(file);
    m_validations.push_back(validate);
  }
  for(auto& filename : m_globalValidations)
  {
    Validate* validate = new ValidateWIN();
    CString file = m_baseDirectory + _T("Validations\\") + filename;
    validate->ReadFromXML(file);
    validate->SetGlobal(true);
    m_validations.push_back(validate);
  }
}

int
WINRunner::CheckStatusOK(int p_returnCode)
{
  int statusOK = _ttoi(m_testStep->GetEffectiveStatusOK());
  if (statusOK != 0 && (p_returnCode != statusOK))
  {
    return 0;
  }
  // All OK
  return 1;
}

void
WINRunner::CreateQLErrorMessage(CString p_error)
{
  StepResultWIN* result = reinterpret_cast<StepResultWIN*>(m_result);

  CString error = result->GetErrorString();
  error += p_error;
  result->SetErrorString(error);
  result->SetLastOSError(1);
}

/*static*/ unsigned
__stdcall ExecWINBoobytrap(void* p_data)
{
  WINRunner* runner = reinterpret_cast<WINRunner*>(p_data);
  int maxtime = runner->GetMaxRunningTime();
  // Wait the maximum time of the test
  Sleep(maxtime);
  // End the test program
  runner->StopTestProgram();

  return 0;
}

void
WINRunner::SetBoobytrap()
{
  // Start a new thread
  unsigned int threadID = 0L;
  if((m_thread = (HANDLE)_beginthreadex(NULL,0,ExecWINBoobytrap,this,0,&threadID)) == INVALID_HANDLE_VALUE)
  {
    threadID = 0;
    m_thread = NULL;
  }
}

void
WINRunner::StopBoobytrap()
{
  TerminateThread(m_thread,0xFFFFFFFF);
}
