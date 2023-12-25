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
// This File   : CMDRunner.cpp
// What it does: Running one (1) command line executable test
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CMDRunner.h"
#include "StdException.h"
#include "RunRedirect.h"
#include "TestMessages.h"
#include "ExtraMessages.h"
#include <RegistryManager.h>
#include <HPFCounter.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CMDRunner::CMDRunner(CString    p_baseDirectory
                    ,CString    p_testDirectory
                    ,CString    p_testStepFilename
                    ,CString    p_parametersFilename
                    ,ValiSteps& p_localValidations
                    ,ValiSteps& p_globalValidations
                    ,HWND       p_consoleHWND
                    ,HWND       p_reportHWND
                    ,HWND       p_callingHWND
                    ,int        p_callingROW
                    ,bool       p_global)
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
                     ,p_global)
{
  m_testStep = new TestStepCMD();
  m_result   = new StepResultCMD();
}

CMDRunner::~CMDRunner()
{
}

// General start of the test
// Public part, called by the main TestRunnerApp
int
CMDRunner::PerformTest()
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
    // Environment strings processing  (1 step)
    SetExtraEnvironmentStrings();
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
    StyleMessageBox(NULL,ex.GetErrorMessage(),"Kwatta",MB_OK|MB_ICONERROR|MB_SETFOREGROUND);
    // Test did went WRONG!!
    result = 0;
  }
  return result;
}

void
CMDRunner::StopTestProgram()
{
  if(m_redirect)
  {
    m_redirect->TerminateChildProcess();
  }
}

//////////////////////////////////////////////////////////////////////////
//
// PRIVATE
//
//////////////////////////////////////////////////////////////////////////

void
CMDRunner::InitRunner()
{
  // Reset the dialog
  SetTest("Initializing");
  SetStep("");
  SetProgress(0);

  // Calculate the number of steps
  // 8 steps are from: 
  // ---------------------------------
  // 1   Reading the files
  // 2   Initial parameter processing
  // 3   Processing environment strings
  // 4   Testing pre command waiting
  // 5   Testing THE ACTUAL COMMAND
  // 6   Testing post command waiting
  // 7   Writing the results file
  // 8   Writing result parameters
  // 9   Reaching a conclusion

  // +   1 for every validation step
  m_steps    = 9 + (int) m_localValidations.size();
  m_stepSize = 100 / m_steps;
}

void
CMDRunner::ReadingAllFiles()
{
  SetTest("Reading all definition files");
  PerformStep("XML files...");

  ReadTestStep();
  ReadParameters();
  ReadValidations();

  // Now set the name proper to the given test step
  SetTest(m_testStep->GetName());
}

void
CMDRunner::ParameterProcessing()
{
  int unbound = 0;
  PerformStep("Parameter processing...");

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
    error.Format("Cannot perform test. Existing unbound parameters: %d",unbound);
    throw StdException(error);
  }
}

// Environment strings processing
void
CMDRunner::SetExtraEnvironmentStrings()
{
  PerformStep("Extra environment strings");

  TestStepCMD* step = reinterpret_cast<TestStepCMD*>(m_testStep);
  int handle = step->GetHandleEnvironment();

  // Check that we must do something with them
  if (handle == 0)
  {
    return;
  }

  // Loop through all environment variables
  ParMap& environs = step->GetEnvironmentVars();
  for(auto& envi : environs)
  {
    CString name  = envi.first;
    CString value = envi.second;

    // Check if name already has a value
    if(handle == 1)
    {
      CString var;
      if(!var.GetEnvironmentVariable(name))
      {
        var = GetGlobalEnvironmentVariable(name);
      }
      if(!var.IsEmpty())
      {
        continue;
      }
    }

    // handle == 1 || handle == 2 (add or replace)
    // Write or overwrite an environment variable
    ::SetEnvironmentVariable(name,value);
  }
}

void
CMDRunner::PerformCommand()
{
  PerformStep("RUN THE COMMAND...");

  TestStepCMD*   step   = reinterpret_cast<TestStepCMD*>(m_testStep);
  StepResultCMD* result = reinterpret_cast<StepResultCMD*>(m_result);

  // Take name and documentation
  m_result->SetName(m_testStep->GetName());
  m_result->SetDocumentation(m_testStep->GetDocumentation());

  CString     standardout;
  CString     standarderr;
  HPFCounter  counter;

  // See if we must set a boobytrap
  if(m_testStep->GetKillOnTimeout())
  {
    int maxtime = atoi(m_testStep->GetEffectiveMaxExecution());
    if (maxtime > 0)
    {
      SetBoobytrap();
    }
  }

  // ::PostMessage(theApp.GetConsoleHandle(),WM_SHOWWINDOW,TRUE,0);
  int retval = PosixCallProgram(step->GetEffectiveDirectory()
                               ,step->GetEffectiveRuntimer()
                               ,step->GetEffectiveCommandLine()
                               ,step->GetEffectiveInput()
                               ,standardout
                               ,standarderr
                               ,m_consoleHNWD
                               ,step->GetStartWindow()
                               ,step->GetWaitForIdle()
                               ,atoi(step->GetMaxExecution())
                               ,&m_redirect);
  // ::PostMessage(theApp.GetConsoleHandle(),WM_SHOWWINDOW,FALSE,0);

  // No more killing of the child program by the boobytrap
  m_redirect = nullptr;

  // Stop the high-performance timer!
  counter.Stop();
  m_result->SetTiming(counter.GetCounter());

  // Keep the return value
  if(step->GetUseReturnValue())
  {
    result->SetReturnValue(retval);
    CString var = step->GetReturnVariable();
    if(!var.IsEmpty())
    {
      CString retvalstring;
      retvalstring.Format("%d",retval);
      m_parameters.OverwriteReturnParameter(var,retvalstring);
    }
  }

  // Keep standard output if desired
  if(step->GetUseOutputValue())
  {
    result->SetStandardOutput(standardout);
    CString var = step->GetOutputVariable();
    if(!var.IsEmpty())
    {
      m_parameters.OverwriteBufferParameter(var,standardout);
    }
  }

  // Keep error if desired, or program has gotten an error
  if(step->GetUseErrorValue() || !standarderr.IsEmpty())
  {
    result->SetStandardError(standarderr);
  }

  // Save errors to error variable if desired
  if(step->GetUseErrorValue())
  {
    CString var = step->GetErrorVariable();
    if(!var.IsEmpty())
    {
      m_parameters.OverwriteBufferParameter(var,standarderr);
    }
  }
}

void
CMDRunner::PerformAllValidations()
{
  int step = 1;
  StepResultCMD* result = reinterpret_cast<StepResultCMD*>(m_result);

  for(auto& vali : m_validations)
  {
    ValidateCMD* validate = reinterpret_cast<ValidateCMD*>(vali);
    PerformStep("Validation: " + validate->GetName());

    // Do the validations
    bool totalresult = true;
    if(validate->ValidateReturnValue (result->GetReturnValue())    == false) totalresult = false;
    if(validate->ValidateOutputBuffer(result->GetStandardOutput()) == false) totalresult = false;
    if(validate->ValidateErrorBuffer (result->GetStandardError())  == false) totalresult = false;

    // Add the validation to the result set
    result->AddValidation(step++,validate->GetName(),validate->GetFilename(),totalresult,validate->GetGlobal());
  }
}

// Saving the test results to the disk
void
CMDRunner::SaveTestResults()
{
  PerformStep("Saving the test results");

  CString filename = m_baseDirectory + m_testDirectory + m_testStepFilename;
  filename.MakeLower();
  filename.Replace(".xrun",".xres");

  StepResultCMD* result = reinterpret_cast<StepResultCMD*>(m_result);
  if(result->WriteToXML(filename) == false)
  {
    CString error;
    error.Format("Cannot save results file: %s",filename.GetString());
    throw StdException(error);
  }

  // Eventually changed parameters need to be written
  m_parameters.WriteToXML();
}

// Save return parameters (if any)
void
CMDRunner::SaveResultParameters()
{
  PerformStep("Saving result parameters");
  bool saved(false);

  TestStepCMD*   step   = reinterpret_cast<TestStepCMD*>(m_testStep);
  StepResultCMD* result = reinterpret_cast<StepResultCMD*>(m_result);

  if(step->GetUseReturnValue() && !step->GetReturnVariable().IsEmpty())
  {
    CString value;
    value.Format("%d",result->GetReturnValue());
    m_parameters.OverwriteReturnParameter(step->GetReturnVariable(),value);
    saved = true;
  }
  if(step->GetUseOutputValue() && !step->GetOutputVariable().IsEmpty())
  {
    m_parameters.OverwriteBufferParameter(step->GetOutputVariable(),result->GetStandardOutput());
    saved = true;
  }
  if(step->GetUseErrorValue() && !step->GetErrorVariable().IsEmpty())
  {
    m_parameters.OverwriteBufferParameter(step->GetErrorVariable(),result->GetStandardError());
    saved = true;
  }

  if(saved)
  {
    // Write back local parameters (return + stream)
    m_parameters.WriteToXML();
  }
}

// Conclusion of the test
int
CMDRunner::ReadTotalResult()
{
  PerformStep("Getting total result...");
  return m_result->GetTotalResult();
}

//////////////////////////////////////////////////////////////////////////
//
// DETAILS

void
CMDRunner::ReadTestStep()
{
  TestStepCMD* step = reinterpret_cast<TestStepCMD*>(m_testStep);

  // Read in the definition file for a test step
  CString filename = GetEffectiveStepFilename();
  step->ReadFromXML(filename);
}

// Reads all validations from the command line
// Could be more than 1 file in all...
void
CMDRunner::ReadValidations()
{
  for(auto& filename : m_localValidations)
  {
    Validate* validate = new ValidateCMD();
    CString file = m_baseDirectory + m_testDirectory + filename;
    validate->ReadFromXML(file);
    m_validations.push_back(validate);
  }
  for(auto& filename : m_globalValidations)
  {
    Validate* validate = new ValidateCMD();
    CString file = m_baseDirectory + "Validations\\" + filename;
    validate->ReadFromXML(file);
    validate->SetGlobal(true);
    m_validations.push_back(validate);
  }
}

void
CMDRunner::CreateQLErrorMessage(CString p_error) 
{
  StepResultCMD* result = reinterpret_cast<StepResultCMD*>(m_result);

  CString error = result->GetStandardError();
  error += p_error;
  result->SetStandardError(error);
}

int
CMDRunner::CheckStatusOK(int p_returnCode)
{
  int statusOK = atoi(m_testStep->GetEffectiveStatusOK());
  if (statusOK != 0 && (p_returnCode != statusOK))
  {
    return 0;
  }
  // All OK
  return 1;
}

/*static*/ unsigned
__stdcall ExecBoobytrap(void* p_data)
{
  CMDRunner* runner = reinterpret_cast<CMDRunner*>(p_data);
  int maxtime = runner->GetMaxRunningTime();
  // Wait the maximum time of the test
  Sleep(maxtime);
  // End the test program
  runner->StopTestProgram();

  return 0;
}

void
CMDRunner::SetBoobytrap()
{
  // Start a new thread
  unsigned int threadID = 0L;
  if((m_thread = (HANDLE)_beginthreadex(NULL,0,ExecBoobytrap,this,0,&threadID)) == INVALID_HANDLE_VALUE)
  {
    threadID = 0;
    m_thread = NULL;
  }
}

void
CMDRunner::StopBoobytrap()
{
  ::TerminateThread(m_thread,0xFFFFFFFF);
}
