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
// This File   : NETRunner.cpp
// What it does: Running one (1) HTTPMessage call to the internet
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NETRunner.h"
#include "TestMessages.h"
#include "ExtraMessages.h"
#include <LogAnalysis.h>
#include <StdException.h>
#include <RunRedirect.h>
#include <HPFCounter.h>
#include <HTTPClient.h>
#include <OAuth2Cache.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

NETRunner::NETRunner(CString      p_baseDirectory
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
                    ,bool         p_loadtest)
          :TestRunner(p_baseDirectory
                     ,p_testDirectory
                     ,p_testStepFilename
                     ,p_parametersFilename
                     ,p_localValidations
                     ,p_globalValidations
                     ,NULL
                     ,p_reportHWND
                     ,p_callingHWND
                     ,p_callingROW
                     ,p_global
                     ,p_loadtest)
          ,m_client(p_client)
          ,m_oauth(p_cache)
{
  m_testStep = new TestStepNET();
  m_result   = new StepResultNET();
}

NETRunner::~NETRunner()
{
  CleanUp();
}

int
NETRunner::PerformTest()
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
      // Send total result to callers
      EndTesting(result);
    }
  }
  catch(StdException& ex)
  {
    // WHOA: Something went wrong
    StyleMessageBox(NULL,ex.GetErrorMessage(),_T("Kwatta"),MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
    // Test did went WRONG!!
    result = 0;
  }
  return result;
}

void
NETRunner::StopTestProgram()
{
  m_client->Disconnect();
}

//////////////////////////////////////////////////////////////////////////
//
// PRIVATE
//
//////////////////////////////////////////////////////////////////////////

void
NETRunner::InitRunner()
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
NETRunner::ReadingAllFiles()
{
  SetTest(_T("Reading all definition files"));
  PerformStep(_T("XML files..."));

  ReadTestStep();
  ReadParameters();
  ReadCredentials();
  ReadValidations();

  // Now set the name proper to the given test step
  SetTest(m_testStep->GetName());
}

void
NETRunner::ParameterProcessing()
{
  int unbound = 0;
  PerformStep(_T("Parameter processing..."));

  TestStepNET* step = reinterpret_cast<TestStepNET*>(m_testStep);
  Credential* cred = m_credentials.FindCredential(step->GetCredential());
  if(cred)
  {
    step->SetAuthType       (cred->m_typeName);
    step->SetAuthUser       (cred->m_username);
    step->SetAuthPassword   (cred->m_password);
    step->SetAuthGrant      (cred->m_oauthGrant);
    step->SetAuthTokenServer(cred->m_tokenServer);
    step->SetAuthClientID   (cred->m_clientID);
    step->SetAuthClientKey  (cred->m_clientKey);
    step->SetAuthClientScope(cred->m_clientScope);
  }

  // Effectuate the parameters for the step
  unbound = step->EffectiveReplacements(&m_parameters,false);

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

  // Setting the timeouts
  int timeoutResolve = step->GetTimeoutResolve();
  if(timeoutResolve < MIN_TIMEOUT_RESOLVE)
  {
    timeoutResolve = MIN_TIMEOUT_RESOLVE;
  }
  m_client->SetTimeoutResolve(timeoutResolve);

  int timeooutConnect = step->GetTimeoutConnect();
  if(timeooutConnect < MIN_TIMEOUT_CONNECT)
  {
    timeooutConnect = MIN_TIMEOUT_CONNECT;
  }
  m_client->SetTimeoutConnect(timeooutConnect);

  int timeoutSend = step->GetTimeoutSend();
  if(timeoutSend < MIN_TIMEOUT_SEND)
  {
    timeoutSend = MIN_TIMEOUT_SEND;
  }
  m_client->SetTimeoutSend(timeoutSend);

  int timeoutReceive = step->GetTimeoutReceive();
  if(timeoutReceive < MIN_TIMEOUT_RECEIVE)
  {
    timeoutReceive = MIN_TIMEOUT_RECEIVE;
  }
  m_client->SetTimeoutReceive(timeoutReceive);
}

void
NETRunner::StartingLogfile()
{
  int loglevel = _ttoi(m_parameters.FindSystemParameter(_T("Loglevel")));
  CString logfile =    m_parameters.FindSystemParameter(_T("Logfile"));

  if(loglevel > 0 && !logfile.IsEmpty())
  {
    if(m_client)
    {
      // Create logfile. Defaults are OK for this application
      m_logfile = LogAnalysis::CreateLogfile(_T("Kwatta HTTP Test"));
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
NETRunner::PerformAuthentication()
{
  PerformStep(_T("Setting the authentication"));

  TestStepNET* step = reinterpret_cast<TestStepNET*>(m_testStep);
  CString auth = step->GetAuthType();
  if(auth.Find(_T("Basic")) >= 0)
  {
    SetBasicAuthentication();
  }
  else if(auth.Find(_T("NTLM Single-signon")) >= 0)
  {
    SetNTLMSSOAuthentication();
  }
  else if(auth.Find(_T("NTLM Logon")) >= 0)
  {
    SetNTLMAuthentication();
  }
  else if(auth.Find(_T("OAuth2")) >= 0)
  {
    SetOAuth2Authentication();
  }
}

void
NETRunner::PrepareMessage()
{
  // Create a message
  if(m_message)
  {
    delete m_message;
  }
  m_message = new HTTPMessage();

  TestStepNET* step = reinterpret_cast<TestStepNET*>(m_testStep);

  // VERB URL
  CString url = step->GetEffectiveCombinedURL();
  m_message->SetVerb(step->GetVerb());
  m_message->SetURL(url);

  // Adding all headers
  for(auto& header : step->GetEffectiveHeaders())
  {
    m_message->AddHeader(header.m_name,header.m_value);
  }

  // Getting the body of the message
  if(step->GetBodyInputIsFile())
  {
    CString file = m_baseDirectory + step->GetFilenameInput();
    m_message->GetFileBuffer()->SetFileName(file);
  }
  else
  {
    m_message->SetBody(step->GetEffectiveBody());
  }

  CString accept = m_message->GetHeader(_T("Accept"));
  if(accept.Find(_T("json")) >= 0)
  {
    m_isJson = true;
  }
}

void
NETRunner::PerformCommand()
{
  PerformStep(_T("RUN THE COMMAND..."));

  // Take name and documentation
  m_result->Reset();
  m_result->SetName(m_testStep->GetName());
  m_result->SetDocumentation(m_testStep->GetDocumentation());

  // Prepare message
  PrepareMessage();

  // Test environments are normally lax with certificates
  // So be prepared to deal with not completely right ones!
  m_client->SetRelaxOptions(SECURITY_FLAG_IGNORE_CERT_CN_INVALID   |
                            SECURITY_FLAG_IGNORE_CERT_DATE_INVALID | 
                            SECURITY_FLAG_IGNORE_UNKNOWN_CA        | 
                            SECURITY_FLAG_IGNORE_CERT_WRONG_USAGE  );

  // See if we must set a boobytrap
  if(m_testStep->GetKillOnTimeout())
  {
    int maxtime = _ttoi(m_testStep->GetEffectiveMaxExecution());
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
  m_result->SetTiming(counter.GetCounter());

  // Get everything from the test
  ExamineMessage();
}

void
NETRunner::ExamineMessage()
{
  TestStepNET*   step   = reinterpret_cast<TestStepNET*>(m_testStep);
  StepResultNET* result = reinterpret_cast<StepResultNET*>(m_result);

  // Getting the OS status for the HTTP call
  CString message;
  int error = m_client->GetError(&message);

  result->SetOSError(error);
  result->SetOSErrorString(message);

  // Keep the return status
  result->SetStatus(m_message->GetStatus());

  // Keep result headers
  for (auto& header : *m_message->GetHeaderMap())
  {
    INPair pair;
    pair.m_name = header.first;
    pair.m_value = header.second;
    result->SetHeader(pair);
  }

  // Keep the body of the message
  if(step->GetBodyOutputIsFile())
  {
    CString file = m_baseDirectory + step->GetFilenameOutput();
       m_message->GetFileBuffer()->SetFileName(file);
    if(m_message->GetFileBuffer()->WriteFile())
    {
      m_message->GetFileBuffer()->ResetFilename();
      m_message->SetBody(_T("<File written: OK>\r\nFile: ") + file);
    }
    else
    {
      m_message->GetFileBuffer()->ResetFilename();
      m_message->SetBody(_T("<FILE NOT WRITTEN>\r\nFile: ") + file);
    }
  }
  result->SetBody(m_message->GetBody());

  // Keep last OAuth2 bearer token
  result->SetBearerToken(m_client->GetLastBearerToken());
}

void
NETRunner::PerformAllValidations()
{
  StepResultNET* result = reinterpret_cast<StepResultNET*>(m_result);

  int step = 1;
  for(auto& vali : m_validations)
  {
    ValidateNET* validate = reinterpret_cast<ValidateNET*>(vali);
    PerformStep(_T("Validation: ") + validate->GetName());

    // Do the validations
    bool totalresult = true;
    if(!validate->ValidateStatusValue(&m_parameters,result->GetStatus()))
    {
      totalresult = false;
    }
    if(!validate->ValidateBodyValue(&m_parameters,result->GetBody()))
    {
      totalresult = false;
    }
    if(validate->GetCheckHeader())
    {
      CString header = result->GetHeader(validate->GetVerifyHeader());
      if(!validate->ValidateHeaderValue(&m_parameters,header))
      {
        totalresult = false;
      }
    }
    if(validate->GetCheckXML())
    {
      if(!validate->ValidateXMLValue(&m_parameters,result->GetBody()))
      {
        totalresult = false;
      }
    }
    if(validate->GetCheckJSON())
    {
      if(!validate->ValidateJSONValue(&m_parameters,result->GetBody()))
      {
        totalresult = false;
      }
    }

    // Add the validation to the result set
    result->AddValidation(step++,validate->GetName(),validate->GetFilename(),totalresult,validate->GetGlobal());
  }
}

void
NETRunner::SaveTestResults()
{
  if(m_loadtest)
  {
    return;
  }
  PerformStep(_T("Saving the test results"));
  StepResultNET* result = reinterpret_cast<StepResultNET*>(m_result);

  CString filename = m_baseDirectory + m_testDirectory + m_testStepFilename;
  filename.MakeLower();
  filename.Replace(_T(".irun"),_T(".ires"));

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
NETRunner::SaveResultParameters()
{
  if(m_loadtest)
  {
    return;
  }
  // Write back local parameters (return + stream)
  // Parameters already changed in "PerformAllValidations" !!
  m_parameters.WriteToXML();
}

int
NETRunner::ReadTotalResult()
{
  for(auto& vali : m_result->GetValidations())
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
NETRunner::ReadTestStep()
{
  TestStepNET* step = reinterpret_cast<TestStepNET*>(m_testStep);
  // Read in the definition file for a test step
  CString filename = GetEffectiveStepFilename();
  step->ReadFromXML(filename);
}

void
NETRunner::ReadValidations()
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
    CString file = m_baseDirectory + _T("Validations\\") + filename;
    validate->ReadFromXML(file);
    validate->SetGlobal(true);
    m_validations.push_back(validate);
  }
}

/*static*/ unsigned
__stdcall InetBoobytrap(void* p_data)
{
  NETRunner* runner = reinterpret_cast<NETRunner*>(p_data);
  int maxtime = runner->GetMaxRunningTime();
  // Wait the maximum time of the test
  Sleep(maxtime);
  // End the HTTPClient connection
  runner->StopTestProgram();

  return 0;
}

void
NETRunner::SetBoobytrap()
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
NETRunner::StopBoobytrap()
{
  TerminateThread(m_thread,0xFFFFFFFF);
}

int
NETRunner::CheckStatusOK(int p_returnCode)
{
  int statusOK = _ttoi(m_testStep->GetEffectiveStatusOK());
  if(statusOK != 0 && (p_returnCode != statusOK))
  {
    return 0;
  }
  // All OK
  return 1;
}

void  
NETRunner::CreateQLErrorMessage(CString p_error)
{
  CString error;
  if(m_isJson)
  {
    error = _T("{ \"error\" : \"QL Language script: ") + p_error + _T("\" }");
  }
  else
  {
    error = _T("<Error>\n")
            _T("  <Type>QL Language script</Type>\n")
            _T("  <Message>") + p_error + _T("</Message>\n")
            _T("</Error>\n");
  }
  StepResultNET* result = reinterpret_cast<StepResultNET*>(m_result);
  result->SetBody(error);
  SaveTestResults();
}

StepResultNET* 
NETRunner::GetStepResult()
{
  return reinterpret_cast<StepResultNET*>(m_result);
}

//////////////////////////////////////////////////////////////////////////
//
// AUTHENTICATION
//
//////////////////////////////////////////////////////////////////////////

void
NETRunner::SetBasicAuthentication()
{
  CString calluser;
  CString callpswd;

  TestStepNET* step = reinterpret_cast<TestStepNET*>(m_testStep);
  CString user = step->GetAuthUser();
  CString pswd = step->GetAuthPassword();

  // Allow for parameter replacements (but not the complete buffer!)
  m_parameters.Replace(user, calluser, false, ParType::PAR_BUFFER);
  m_parameters.Replace(pswd, callpswd, false, ParType::PAR_BUFFER);

  // Tell it the client
  m_client->SetUser(calluser);
  m_client->SetPassword(callpswd);
  m_client->SetPreEmptiveAuthorization(WINHTTP_AUTH_SCHEME_BASIC);
}

void  
NETRunner::SetNTLMSSOAuthentication()
{
  m_client->SetPreEmptiveAuthorization(WINHTTP_AUTH_SCHEME_NTLM);
  m_client->SetSingleSignOn(true);
}


void
NETRunner::SetNTLMAuthentication()
{
  SetBasicAuthentication();
  m_client->SetPreEmptiveAuthorization(WINHTTP_AUTH_SCHEME_NTLM);
}

void
NETRunner::SetOAuth2Authentication()
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
  TestStepNET* step = reinterpret_cast<TestStepNET*>(m_testStep);

  m_parameters.Replace(step->GetAuthTokenServer(),tokenServer,false,ParType::PAR_BUFFER);
  m_parameters.Replace(step->GetAuthClientID(),   clientID,   false,ParType::PAR_BUFFER);
  m_parameters.Replace(step->GetAuthClientKey(),  clientKey,  false,ParType::PAR_BUFFER);
  m_parameters.Replace(step->GetAuthClientScope(),clientScope,false,ParType::PAR_BUFFER);

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
NETRunner::CleanUp()
{
  // Clean up the message
  if(m_message)
  {
    delete m_message;
    m_message = nullptr;
  }
}
