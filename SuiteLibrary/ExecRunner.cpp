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
// This File   : ExecRunner.cpp
// What it does: Running one (1) command line executable test
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ExecRunner.h"
#include "StdException.h"
#include "RunRedirect.h"
#include "TestMessages.h"
#include "ExtraMessages.h"
#include <HPFCounter.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

ExecRunner::ExecRunner(CString    p_baseDirectory
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
           :m_baseDirectory(p_baseDirectory)
           ,m_testDirectory(p_testDirectory)
           ,m_testStepFilename(p_testStepFilename)
           ,m_parametersFilename(p_parametersFilename)
           ,m_localValidations(p_localValidations)
           ,m_globalValidations(p_globalValidations)
           ,m_consoleHNWD(p_consoleHWND)
           ,m_reportHWND(p_reportHWND)
           ,m_callingHWND(p_callingHWND)
           ,m_callingROW(p_callingROW)
           ,m_global(p_global)
{
}

ExecRunner::~ExecRunner()
{
  // Clean out all validations
  for(auto& vali : m_validations)
  {
    delete vali;
  }
  m_validations.clear();
}

// General start of the test
// Public part, called by the main TestRunnerApp
int
ExecRunner::PerformTest()
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
    // Tell it to our callers
    EndTesting(result);
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

CString
ExecRunner::GetEffectiveStepFilename()
{
  CString filename(m_baseDirectory);
  filename += m_global ? CString("Steps\\") : m_testDirectory;
  filename += m_testStepFilename;

  return filename;
}

//////////////////////////////////////////////////////////////////////////
//
// PRIVATE
//
//////////////////////////////////////////////////////////////////////////

void
ExecRunner::InitRunner()
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
ExecRunner::ReadingAllFiles()
{
  SetTest("Reading all definition files");
  PerformStep("XML files...");

  ReadTestStep();
  ReadParameters();
  ReadValidations();

  // Now set the name proper to the given test step
  SetTest(m_testStep.GetName());
}

void
ExecRunner::ParameterProcessing()
{
  int unbound = 0;
  PerformStep("Parameter processing...");

  // Effectuate the parameters for the step
  unbound = m_testStep.EffectiveReplacements(&m_parameters,false);

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
ExecRunner::SetExtraEnvironmentStrings()
{
  PerformStep("Extra environment strings");

  int handle = m_testStep.GetHandleEnvironment();

  // Check that we must do something with them
  if (handle == 0)
  {
    return;
  }

  // Loop through all environment variables
  ParMap& environs = m_testStep.GetEnvironmentVars();
  for(auto& envi : environs)
  {
    CString name  = envi.first;
    CString value = envi.second;

    // Check if name already has a value
    if(handle == 1)
    {
      CString var;
      var.GetEnvironmentVariable(name);
      if (!var.IsEmpty())
      {
        continue;
      }
    }

    // handle == 1 || handle == 2 (add or replace)
    // Write or overwrite an environment variable
    ::SetEnvironmentVariable(name,value);
  }
}

// Waiting before we run a command
void
ExecRunner::PreCommandWaiting()
{
  CString step("Pre-command waiting.");
  PerformStep(step);

  int time = m_testStep.GetWaitBeforeRun();
  if (time > 0)
  {
    WaitingForATimeout(step,time);
  }
}

void
ExecRunner::PerformCommand()
{
  PerformStep("RUN THE COMMAND...");

  // Take name and documentation
  m_result.SetName(m_testStep.GetName());
  m_result.SetDocumentation(m_testStep.GetDocumentation());

  CString standardout;
  CString standarderr;
  HPFCounter counter;

  // ::PostMessage(theApp.GetConsoleHandle(),WM_SHOWWINDOW,TRUE,0);

  int retval = PosixCallProgram(m_testStep.GetEffectiveDirectory()
                               ,m_testStep.GetEffectiveRuntimer()
                               ,m_testStep.GetEffectiveCommandLine()
                               ,m_testStep.GetEffectiveInput()
                               ,standardout
                               ,standarderr
                               ,m_consoleHNWD
                               ,m_testStep.GetStartWindow()
                               ,m_testStep.GetWaitForIdle()
                               ,m_testStep.GetMaxExecution());

  // ::PostMessage(theApp.GetConsoleHandle(),WM_SHOWWINDOW,FALSE,0);

  // Stop the high-performance timer!
  counter.Stop();
  m_result.SetTiming(counter.GetCounter());

  // Keep the return value
  if(m_testStep.GetUseReturnValue())
  {
    m_result.SetReturnValue(retval);
    CString var = m_testStep.GetReturnVariable();
    if(!var.IsEmpty())
    {
      CString retvalstring;
      retvalstring.Format("%d",retval);
      m_parameters.OverwriteReturnParameter(var,retvalstring);
    }
  }

  // Keep standard output if desired
  if(m_testStep.GetUseOutputValue())
  {
    m_result.SetStandardOutput(standardout);
    CString var = m_testStep.GetOutputVariable();
    if(!var.IsEmpty())
    {
      m_parameters.OverwriteBufferParameter(var,standardout);
    }
  }

  // Keep error if desired, or program has gotten an error
  if(m_testStep.GetUseErrorValue() || !standarderr.IsEmpty())
  {
    m_result.SetStandardError(standarderr);
  }

  // Save errors to error variable if desired
  if(m_testStep.GetUseErrorValue())
  {
    CString var = m_testStep.GetErrorVariable();
    if(!var.IsEmpty())
    {
      m_parameters.OverwriteBufferParameter(var,standarderr);
    }
  }
}

// Waiting for cleanup after a command
void
ExecRunner::PostCommandWaiting()
{
  CString step("Post-command waiting.");
  PerformStep(step);

  int time = m_testStep.GetWaitAfterRun();
  if (time >= 0)
  {
    WaitingForATimeout(step,time);
  }
}

void
ExecRunner::PerformAllValidations()
{
  int step = 1;
  for(auto& vali : m_validations)
  {
    ValidateCL* validate = reinterpret_cast<ValidateCL*>(vali);
    PerformStep("Validation: " + validate->GetName());

    // Do the validations
    bool result = true;
    if(validate->ValidateReturnValue (m_result.GetReturnValue())    == false) result = false;
    if(validate->ValidateOutputBuffer(m_result.GetStandardOutput()) == false) result = false;
    if(validate->ValidateErrorBuffer (m_result.GetStandardError())  == false) result = false;

    // Add the validation to the result set
    m_result.AddValidation(step++,validate->GetName(),validate->GetFilename(),result,validate->GetGlobal());
  }
}

// Saving the test results to the disk
void
ExecRunner::SaveTestResults()
{
  PerformStep("Saving the test results");

  CString filename = GetEffectiveStepFilename();
  filename.MakeLower();
  filename.Replace(".xrun",".xres");

  if(m_result.WriteToXML(filename) == false)
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
ExecRunner::SaveResultParameters()
{
  PerformStep("Saving result parameters");
  bool saved(false);

  if(m_testStep.GetUseReturnValue() && !m_testStep.GetReturnVariable().IsEmpty())
  {
    CString value;
    value.Format("%d",m_result.GetReturnValue());
    m_parameters.OverwriteReturnParameter(m_testStep.GetReturnVariable(),value);
    saved = true;
  }
  if(m_testStep.GetUseOutputValue() && !m_testStep.GetOutputVariable().IsEmpty())
  {
    m_parameters.OverwriteBufferParameter(m_testStep.GetOutputVariable(),m_result.GetStandardOutput());
    saved = true;
  }
  if(m_testStep.GetUseErrorValue() && !m_testStep.GetErrorVariable().IsEmpty())
  {
    m_parameters.OverwriteBufferParameter(m_testStep.GetErrorVariable(),m_result.GetStandardError());
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
ExecRunner::ReadTotalResult()
{
  PerformStep("Getting total result...");
  return m_result.GetTotalResult();
}

//////////////////////////////////////////////////////////////////////////
//
// DETAILS

// Perform the next step in the total progress
void
ExecRunner::PerformStep(CString p_stepName)
{
  SetStep(p_stepName);
  m_progress += m_stepSize;
  SetProgress(m_progress);
}

void
ExecRunner::ReadTestStep()
{
  // Read in the definition file for a test step
  CString filename = GetEffectiveStepFilename();
  m_testStep.ReadFromXML(filename);
}

void
ExecRunner::ReadParameters()
{
  // read the global parameters
  CString filename = m_baseDirectory + "Parameters.xpar";
  m_parameters.ReadFromXML(filename);

  // read the definition of the test parameters
  filename = m_baseDirectory + m_testDirectory + m_parametersFilename;
  m_parameters.ReadFromXML(filename,false);
}

// Reads all validations from the command line
// Could be more than 1 file in all...
void
ExecRunner::ReadValidations()
{
  for(auto& filename : m_localValidations)
  {
    Validate* validate = new ValidateCL();
    CString file = m_baseDirectory + m_testDirectory + filename;
    validate->ReadFromXML(file);
    m_validations.push_back(validate);
  }
  for(auto& filename : m_globalValidations)
  {
    Validate* validate = new ValidateCL();
    CString file = m_baseDirectory + "Validations\\" + filename;
    validate->ReadFromXML(file);
    validate->SetGlobal(true);
    m_validations.push_back(validate);
  }
}

void
ExecRunner::WaitingForATimeout(CString p_stepname,int p_milliseconds)
{
  // Reset the progress step-name and progress-controlbar
  p_stepname.AppendFormat(" %.3f seconds",((double)p_milliseconds / 1000.0));
  SetStep(p_stepname);
  SetProgress(0);

  // Preset the progress
  int progress = 1;
  int count    = 100;
  int interval = p_milliseconds / 100;

  // Progress intervals cannot be to short
  if(interval < MINIMUM_INTERVAL_TIME)
  {
    count    = (p_milliseconds / MINIMUM_INTERVAL_TIME) + 1;
    interval = p_milliseconds / count;
    progress = 100 / count;
  }

  // Perform the waiting
  for(int index = 0; index < count; ++index)
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
ExecRunner::SetTest(CString p_test)
{
  if(m_reportHWND)
  {
    ::SendMessage(m_reportHWND,WM_TEST_TESTNAME,(WPARAM)p_test.GetString(),0);
  }
}

void
ExecRunner::SetStep(CString p_step)
{
  if(m_reportHWND)
  {
    ::SendMessage(m_reportHWND,WM_TEST_STEPNAME,(WPARAM)p_step.GetString(),0);
  }
}

void
ExecRunner::SetProgress(int p_percent)
{
  if(m_reportHWND)
  {
    ::SendMessage(m_reportHWND,WM_TEST_PROGRESS,(WPARAM)p_percent,0);
  }
}

void
ExecRunner::EndTesting(int p_result)
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
