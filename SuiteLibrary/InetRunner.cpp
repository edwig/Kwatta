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
// This File   : InetRunner.cpp
// What it does: Running one (1) HTTPMessage call to the internet
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InetRunner.h"
#include "TestMessages.h"
#include "ExtraMessages.h"
#include <LogAnalysis.h>
#include <StdException.h>
#include <RunRedirect.h>
#include <HPFCounter.h>
#include <HTTPClient.h>
#include <OAuth2Cache.h>
#include <QL_vm.h>
#include <QL_Interpreter.h>
#include <QL_Exception.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

InetRunner::InetRunner(CString      p_baseDirectory
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
                      ,bool         p_global)
           :m_baseDirectory(p_baseDirectory)
           ,m_testDirectory(p_testDirectory)
           ,m_testStepFilename(p_testStepFilename)
           ,m_parametersFilename(p_parametersFilename)
           ,m_localValidations(p_localValidations)
           ,m_globalValidations(p_globalValidations)
           ,m_reportHWND(p_reportHWND)
           ,m_client(p_client)
           ,m_oauth(p_cache)
           ,m_callingHWND(p_callingHWND)
           ,m_callingROW(p_callingROW)
           ,m_global(p_global)
{
}

InetRunner::~InetRunner()
{
  CleanUp();
}

int
InetRunner::PerformTest()
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
    // Starting a logfile (if any)
    StartingLogfile();
    // SetAuthentication
    PerformAuthentication();

    while(m_running)
    {
      // One (extra) iteration
      ++m_interations;

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
      // Send total result to callers
      EndTesting(result);
    }
  }
  catch(StdException& ex)
  {
    // WHOA: Something went wrong
    StyleMessageBox(NULL,ex.GetErrorMessage(),"Kwatta",MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
    // Test did went WRONG!!
    result = 0;
  }
  return result;
}

CString
InetRunner::GetEffectiveStepFilename()
{
  CString filename(m_baseDirectory);
  filename += m_global ? CString("Steps\\") : m_testDirectory;
  filename += m_testStepFilename;

  return filename;
}

int
InetRunner::GetMaxRunningTime()
{
  return atoi(m_testStep.GetEffectiveMaxExecution());
}

void
InetRunner::DisconnectClient()
{
  m_client->Disconnect();
}

//////////////////////////////////////////////////////////////////////////
//
// PRIVATE
//
//////////////////////////////////////////////////////////////////////////

void
InetRunner::InitRunner()
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
  // 3   Starting logfile (if any)
  // 4   Set the authentication correct
  // 5   Pre-command waiting
  // 6   Testing THE ACTUAL COMMAND
  // 7   Post-command waiting
  // 8   Writing the results file
  // 9   Writing the return parameters
  // 10  Reaching a conclusion

  // +   1 for every validation step
  m_steps = 10 + (int)m_localValidations.size();
  m_stepSize = 100 / m_steps;
}

void
InetRunner::ReadingAllFiles()
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
InetRunner::ParameterProcessing()
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

  // Setting the timeouts
  int timeoutResolve = m_testStep.GetTimeoutResolve();
  if(timeoutResolve < MIN_TIMEOUT_RESOLVE)
  {
    timeoutResolve = MIN_TIMEOUT_RESOLVE;
  }
  m_client->SetTimeoutResolve(timeoutResolve);

  int timeooutConnect = m_testStep.GetTimeoutConnect();
  if(timeooutConnect < MIN_TIMEOUT_CONNECT)
  {
    timeooutConnect = MIN_TIMEOUT_CONNECT;
  }
  m_client->SetTimeoutConnect(timeooutConnect);

  int timeoutSend = m_testStep.GetTimeoutSend();
  if(timeoutSend < MIN_TIMEOUT_SEND)
  {
    timeoutSend = MIN_TIMEOUT_SEND;
  }
  m_client->SetTimeoutSend(timeoutSend);

  int timeoutReceive = m_testStep.GetTimeoutReceive();
  if(timeoutReceive < MIN_TIMEOUT_RECEIVE)
  {
    timeoutReceive = MIN_TIMEOUT_RECEIVE;
  }
  m_client->SetTimeoutReceive(timeoutReceive);
}

void
InetRunner::StartingLogfile()
{
  int loglevel = atoi(m_parameters.FindSystemParameter("Loglevel"));
  CString logfile =   m_parameters.FindSystemParameter("Logfile");

  if(loglevel > 0 && !logfile.IsEmpty())
  {
    if(m_client)
    {
      // Create logfile. Defaults are OK for this application
      m_logfile = LogAnalysis::CreateLogfile("Kwatta HTTP Test");
      m_logfile->SetLogLevel(loglevel);
      m_logfile->SetLogFilename(logfile);

      // Tell it to the client
      m_client->SetLogging(m_logfile,true);
      m_client->SetLogLevel(loglevel);
    }
  }
}

// SetAuthentication
void
InetRunner::PerformAuthentication()
{
  PerformStep("Setting the authentication");

  CString auth = m_testStep.GetAuthType();
  if(auth.Find("Basic") >= 0)
  {
    SetBasicAuthentication();
  }
  else if(auth.Find("NTLM Single") >= 0)
  {
    SetNTLMSSOAuthentication();
  }
  else if(auth.Find("NTLM") >= 0)
  {
    SetNTLMAuthentication();
  }
  else if(auth.Find("OAuth2") >= 0)
  {
    SetOAuth2Authentication();
  }
}

// Waiting before we run a command
void
InetRunner::PreCommandWaiting()
{
  CString step("Pre-command waiting.");
  PerformStep(step);

  int time = atoi(m_testStep.GetEffectiveWaitBeforeRun());
  if (time > 0)
  {
    WaitingForATimeout(step, time);
  }
}

void
InetRunner::PrepareMessage()
{
  // Create a message
  if(m_message)
  {
    delete m_message;
  }
  m_message = new HTTPMessage();

  // VERB URL
  CString url = m_testStep.GetEffectiveCombinedURL();
  m_message->SetVerb(m_testStep.GetVerb());
  m_message->SetURL(url);

  // Adding all headers
  for(auto& header : m_testStep.GetEffectiveHeaders())
  {
    m_message->AddHeader(header.m_name,header.m_value);
  }

  // Getting the body of the message
  if(m_testStep.GetBodyInputIsFile())
  {
    CString file = m_baseDirectory + m_testStep.GetFilenameInput();
    m_message->GetFileBuffer()->SetFileName(file);
  }
  else
  {
    m_message->SetBody(m_testStep.GetEffectiveBody());
  }

  CString accept = m_message->GetHeader("Accept");
  if(accept.Find("json") >= 0)
  {
    m_isJson = true;
  }
}

void
InetRunner::PerformCommand()
{
  PerformStep("RUN THE COMMAND...");

  // Take name and documentation
  m_result.Reset();
  m_result.SetName(m_testStep.GetName());
  m_result.SetDocumentation(m_testStep.GetDocumentation());

  // Prepare message
  PrepareMessage();

  // Test environments are normally lax with certificates
  // So be prepared to deal with not completely right ones!
  m_client->SetRelaxOptions(SECURITY_FLAG_IGNORE_CERT_CN_INVALID   |
                            SECURITY_FLAG_IGNORE_CERT_DATE_INVALID | 
                            SECURITY_FLAG_IGNORE_UNKNOWN_CA        | 
                            SECURITY_FLAG_IGNORE_CERT_WRONG_USAGE  );

  // See if we must set a boobytrap
  if(m_testStep.GetKillOnTimeout())
  {
    int maxtime = atoi(m_testStep.GetEffectiveMaxExecution());
    if(maxtime > 0)
    {
      SetBoobytrap();
    }
  }

  // Start counter and send message
  HPFCounter counter;

  //////////////////////////////////////////////////////////////////////////
  //
  // RUN THE TEST !!
  //
  m_client->Send(m_message);
  //
  //////////////////////////////////////////////////////////////////////////

  // Stop the high-performance timer!
  counter.Stop();
  m_result.SetTiming(counter.GetCounter());

  // Get everything from the test
  ExamineMessage();
}

void
InetRunner::ExamineMessage()
{
  // Getting the OS status for the HTTP call
  CString message;
  int error = m_client->GetError(&message);

  m_result.SetOSError(error);
  m_result.SetOSErrorString(message);

  // Keep the return status
  m_result.SetStatus(m_message->GetStatus());

  // Keep result headers
  for (auto& header : *m_message->GetHeaderMap())
  {
    INPair pair;
    pair.m_name = header.first;
    pair.m_value = header.second;
    m_result.SetHeader(pair);
  }

  // Keep the body of the message
  if(m_testStep.GetBodyOutputIsFile())
  {
    CString file = m_baseDirectory + m_testStep.GetFilenameOutput();
       m_message->GetFileBuffer()->SetFileName(file);
    if(m_message->GetFileBuffer()->WriteFile())
    {
      m_message->GetFileBuffer()->ResetFilename();
      m_message->SetBody("<File written: OK>\r\nFile: " + file);
    }
    else
    {
      m_message->GetFileBuffer()->ResetFilename();
      m_message->SetBody("<FILE NOT WRITTEN>\r\nFile: " + file);
    }
  }
  m_result.SetBody(m_message->GetBody());

  // Keep last OAuth2 bearer token
  m_result.SetBearerToken(m_client->GetLastBearerToken());
}

// Waiting for cleanup after a command
void
InetRunner::PostCommandWaiting()
{
  CString step("Post-command waiting.");
  PerformStep(step);

  int time = atoi(m_testStep.GetEffectiveWaitAfterRun());
  if (time >= 0)
  {
    WaitingForATimeout(step, time);
  }
}

void
InetRunner::PerformAllValidations()
{
  int step = 1;
  for(auto& vali : m_validations)
  {
    ValidateNET* validate = reinterpret_cast<ValidateNET*>(vali);
    PerformStep("Validation: " + validate->GetName());

    // Do the validations
    bool result = true;
    if(!validate->ValidateStatusValue(&m_parameters,m_result.GetStatus()))
    {
      result = false;
    }
    if(!validate->ValidateBodyValue(&m_parameters,m_result.GetBody()))
    {
      result = false;
    }
    if(validate->GetCheckHeader())
    {
      CString header = m_result.GetHeader(validate->GetVerifyHeader());
      if(!validate->ValidateHeaderValue(&m_parameters,header))
      {
        result = false;
      }
    }
    if(validate->GetCheckXML())
    {
      if(!validate->ValidateXMLValue(&m_parameters,m_result.GetBody()))
      {
        result = false;
      }
    }
    if(validate->GetCheckJSON())
    {
      if(!validate->ValidateJSONValue(&m_parameters,m_result.GetBody()))
      {
        result = false;
      }
    }

    // Add the validation to the result set
    m_result.AddValidation(step++,validate->GetName(),validate->GetFilename(),result,validate->GetGlobal());
  }
}

void
InetRunner::SaveTestResults()
{
  PerformStep("Saving the test results");

  CString filename = m_baseDirectory + m_testDirectory + m_testStepFilename;
  filename.MakeLower();
  filename.Replace(".irun",".ires");

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
InetRunner::SaveResultParameters()
{
  // Write back local parameters (return + stream)
  // Parameters already changed in "PerformAllValidations" !!
  m_parameters.WriteToXML();
}

int
InetRunner::ReadTotalResult()
{
  for(auto& vali : m_result.GetValidations())
  {
    if (vali.m_ok == false)
    {
      return 0;
    }
  }
  // Beware: no validations means: always OK
  return 1;
}

void
InetRunner::ReadTestStep()
{
  // Read in the definition file for a test step
  CString filename = GetEffectiveStepFilename();
  m_testStep.ReadFromXML(filename);
}

void
InetRunner::ReadParameters()
{
  // read the global parameters
  CString filename = m_baseDirectory + "Parameters.xpar";
  m_parameters.ReadFromXML(filename);

  // read the definition of the test parameters
  filename = m_baseDirectory + m_testDirectory + m_parametersFilename;
  m_parameters.ReadFromXML(filename,false);
}

void
InetRunner::ReadValidations()
{
  for(auto& filename : m_localValidations)
  {
    Validate* validate = new ValidateNET();
    CString file = m_baseDirectory + m_testDirectory + filename;
    validate->ReadFromXML(file);
    m_validations.push_back(validate);
  }
  for(auto& filename : m_globalValidations)
  {
    Validate* validate = new ValidateNET();
    CString file = m_baseDirectory + "Validations\\" + filename;
    validate->ReadFromXML(file);
    validate->SetGlobal(true);
    m_validations.push_back(validate);
  }
}

void
InetRunner::PerformStep(CString p_stepName)
{
  SetStep(p_stepName);
  m_progress += m_stepSize;
  SetProgress(m_progress);
}

void
InetRunner::WaitingForATimeout(CString p_stepname,int p_milliseconds)
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

/*static*/ unsigned
__stdcall InetBoobytrap(void* p_data)
{
  InetRunner* runner = reinterpret_cast<InetRunner*>(p_data);
  int maxtime = runner->GetMaxRunningTime();
  // Wait the maximum time of the test
  Sleep(maxtime);
  // End the HTTPClient connection
  runner->DisconnectClient();

  return 0;
}

void
InetRunner::SetBoobytrap()
{
  // Start a new thread
  unsigned int threadID = 0L;
  if ((m_thread = (HANDLE)_beginthreadex(NULL,0,InetBoobytrap,this,0,&threadID)) == INVALID_HANDLE_VALUE)
  {
    threadID = 0;
    m_thread = NULL;
  }
}

void
InetRunner::StopBoobytrap()
{
  TerminateThread(m_thread,0xFFFFFFFF);
}

//////////////////////////////////////////////////////////////////////////
//
// Running a QL Script after the test step
//
//////////////////////////////////////////////////////////////////////////

__declspec(thread) LogAnalysis* runlog = nullptr;

void osputs_stdout(const char* str)
{
  static CString out;
  int len = (int) strlen(str);
  if(len > 0 && str[len - 1] == '\n')
  {
    out += str;

    if(runlog)
    {
      out = out.TrimRight("\n");
      out.Replace("\r","");
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

void osputs_stderr(const char* str)
{
  osputs_stdout(str);
}

int
InetRunner::CheckStatusOK(int p_returnCode)
{
  int statusOK = atoi(m_testStep.GetEffectiveStatusOK());
  if(statusOK != 0 && (p_returnCode != statusOK))
  {
    return 1;
  }
  // All OK
  return 0;
}

void  
InetRunner::CreateQLErrorMessage(CString p_error)
{
  CString error;
  if(m_isJson)
  {
    error = "{ \"error\" : \"QL Language script: " + p_error + "\" }";
  }
  else
  {
    error = "<Error>\n"
            "  <Type>QL Language script</Type>\n"
            "  <Message>" + p_error + "</Message>\n"
            "</Error>\n";
  }
  m_result.SetBody(error);
  SaveTestResults();
}

int
InetRunner::PerformQLScript(int p_result)
{
  // Reset the running status
  m_running = false;

  // See if we must run the script
  ScriptStatus status = m_testStep.GetScriptStatus();
  if(status == ScriptStatus::NoScript)
  {
    return p_result;
  }

  int retCode = 0;
  try
  {
    // Getting the script to run
    // But first effectuate the output parameters of the last run command
    int unbound = m_testStep.EffectiveReplacements(&m_parameters,false);
    if (unbound)
    {
      throw QLException("Parameters not replaced. " + m_parameters.GetUnboundErrors(),unbound);
    }
    CString script = m_testStep.GetEffectiveScriptToRun();

    // Write to the logfile (if any)
    bool trace_run = false;
    bool trace_cmp = false;
    if(m_logfile)
    {
      runlog    = m_logfile;
      trace_run = m_logfile->GetLogLevel() >= HLL_LOGGING;
      trace_cmp = m_logfile->GetLogLevel() >= HLL_TRACEDUMP;

      m_logfile->AnalysisLog(__FUNCTION__,LogType::LOG_INFO,true, "Validation errors: %d",p_result);
      m_logfile->AnalysisLog(__FUNCTION__,LogType::LOG_INFO,false,"Running QL Script.");
      osputs_stdout(script);
      osputs_stdout("\n");
    }

    // Compile our script first
    QLVirtualMachine vm;
    if(!vm.CompileBuffer(script,trace_cmp))
    {
      return 1;
    }
    // Set up an interpreter
    QLInterpreter inter(&vm,trace_run);
    CString entrypoint("main");

    // Set the latest test status
    inter.SetTestIterations(m_interations);
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
  osputs_stdout("\n");

  // Combine all the status codes
  switch(status)
  {
    case ScriptStatus::SuccessIsZero:     if(retCode != 0)
                                          {
                                            p_result = 1;
                                          }
                                          break;
    case ScriptStatus::SuccessIsNegative: if(retCode >= 0 || CheckStatusOK(retCode) != 0)
                                          {
                                            p_result = 1;
                                          }
                                          break;
    case ScriptStatus::SuccessIsPositive: if(retCode <= 0 || CheckStatusOK(retCode) != 0)
                                          {
                                            p_result = 1;
                                          }
                                          break;
  }
  if(m_logfile)
  {
    m_logfile->AnalysisLog(__FUNCTION__,LogType::LOG_INFO,true,"QL Script. Return value: %d",retCode);
    m_logfile->AnalysisLog(__FUNCTION__,LogType::LOG_INFO,true,"QL Script. Extra iteration: %s",m_running ? "Yes" : "No");
  }
  return p_result;
}

//////////////////////////////////////////////////////////////////////////
//
// TELLING IT THE OUTSIDE WORLD
//
//////////////////////////////////////////////////////////////////////////

void
InetRunner::SetTest(CString p_test)
{
  if(m_reportHWND)
  {
    ::SendMessage(m_reportHWND,WM_TEST_TESTNAME,(WPARAM)p_test.GetString(),0);
  }
}

void
InetRunner::SetStep(CString p_step)
{
  if(m_reportHWND)
  {
    ::SendMessage(m_reportHWND,WM_TEST_STEPNAME,(WPARAM)p_step.GetString(),0);
  }
}

void
InetRunner::SetProgress(int p_percent)
{
  if(m_reportHWND)
  {
    ::SendMessage(m_reportHWND,WM_TEST_PROGRESS,(WPARAM)p_percent,0);
  }
}

void
InetRunner::EndTesting(int p_result)
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
// AUTHENTICATION
//
//////////////////////////////////////////////////////////////////////////

void
InetRunner::SetBasicAuthentication()
{
  CString calluser;
  CString callpswd;
  CString user = m_testStep.GetAuthUser();
  CString pswd = m_testStep.GetAuthPassword();

  // Allow for parameter replacements (but not the complete buffer!)
  m_parameters.Replace(user, calluser, false, ParType::PAR_BUFFER);
  m_parameters.Replace(pswd, callpswd, false, ParType::PAR_BUFFER);

  // Tell it the client
  m_client->SetUser(calluser);
  m_client->SetPassword(callpswd);
  m_client->SetPreEmptiveAuthorization(WINHTTP_AUTH_SCHEME_BASIC);
}

void  
InetRunner::SetNTLMSSOAuthentication()
{
  m_client->SetPreEmptiveAuthorization(WINHTTP_AUTH_SCHEME_NTLM);
  m_client->SetSingleSignOn(true);
}


void
InetRunner::SetNTLMAuthentication()
{
  SetBasicAuthentication();
  m_client->SetPreEmptiveAuthorization(WINHTTP_AUTH_SCHEME_NTLM);
}

void
InetRunner::SetOAuth2Authentication()
{
  if(!m_oauth)
  {
    return;
  }
  // Getting our parameters
  CString tokenServer;
  CString clientID;
  CString clientKey;
  CString clientScope;
  m_parameters.Replace(m_testStep.GetAuthTokenServer(),tokenServer,false,ParType::PAR_BUFFER);
  m_parameters.Replace(m_testStep.GetAuthClientID(),   clientID,   false,ParType::PAR_BUFFER);
  m_parameters.Replace(m_testStep.GetAuthClientKey(),  clientKey,  false,ParType::PAR_BUFFER);
  m_parameters.Replace(m_testStep.GetAuthClientScope(),clientScope,false,ParType::PAR_BUFFER);

  // Checking for a session
  int session = m_oauth->GetHasSession(clientID,clientKey);
  if(!session)
  {
    session = m_oauth->CreateClientCredentialsGrant(tokenServer,clientID,clientKey,clientScope);
    m_oauth->SetDevelopment();
  }
  m_client->SetOAuth2Cache(m_oauth);
  m_client->SetOAuth2Session(session);
}

//////////////////////////////////////////////////////////////////////////
//
// CLEAN UP
//
//////////////////////////////////////////////////////////////////////////

void
InetRunner::CleanUp()
{
  // Clean up all validations
  for(auto& val : m_validations)
  {
    delete val;
  }
  m_validations.clear();

  if(m_message)
  {
    delete m_message;
    m_message = nullptr;
  }
}
