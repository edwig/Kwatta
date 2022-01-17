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
#include <Analysis.h>
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
    // Perform the test (3 steps)
    PerformCommand();
    // Perform the validations (x * 1 steps)
    PerformAllValidations();
    // Write the results (1 step)
    SaveTestResults();
    // Save return parameters (if any)
    SaveResultParameters();
    // Return the conclusion (1 step)
    result = ReadTotalResult();
    // Send total result to callers
    EndTesting(result);
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
  // 5   Testing THE ACTUAL COMMAND
  // 6   Writing the results file
  // 7   Writing the return parameters
  // 8   Reaching a conclusion

  // +   1 for every validation step
  m_steps = 8 + (int)m_localValidations.size();
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
      if(m_client->GetLogging() == nullptr)
      {
        // Create logfile. Defaults are OK for this application
        LogAnalysis* log = new LogAnalysis("Kwatta Testrunner");
        log->SetLogLevel(loglevel);
        log->SetLogFilename(logfile);

        // Transfer logfile to the HTTP client
        m_client->SetLogging(log,true);
        m_client->SetLogLevel(loglevel);
      }
      else
      {
        m_client->GetLogging()->SetLogLevel(0);
        m_client->GetLogging()->SetLogLevel(loglevel);
      }
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

void
InetRunner::PerformCommand()
{
  PerformStep("RUN THE COMMAND...");

  // Take name and documentation
  m_result.SetName(m_testStep.GetName());
  m_result.SetDocumentation(m_testStep.GetDocumentation());

  // Prepare message
  m_message.SetVerb(m_testStep.GetVerb());
  m_message.SetBody(m_testStep.GetEffectiveBody());
  CString url = m_testStep.GetEffectiveCombinedURL();
  m_message.SetURL(url);

  // Adding all headers
  for(auto& header : m_testStep.GetEffectiveHeaders())
  {
    m_message.AddHeader(header.m_name,header.m_value);
  }

  // We want all return headers
  m_client->SetReadAllHeaders(true);

  // Test environments are normally lax with certificates
  // So be prepared to deal with not completely right ones!
  m_client->SetRelaxOptions(SECURITY_FLAG_IGNORE_CERT_CN_INVALID   |
                            SECURITY_FLAG_IGNORE_CERT_DATE_INVALID | 
                            SECURITY_FLAG_IGNORE_UNKNOWN_CA        | 
                            SECURITY_FLAG_IGNORE_CERT_WRONG_USAGE  );

  // RUN THE TEST !!
  // 
  // Start counter and send message
  HPFCounter counter;
  m_client->Send(&m_message);
  // Stop the high-performance timer!
  counter.Stop();
  m_result.SetTiming(counter.GetCounter());
  
  // Getting the OS status for the HTTP call
  CString message;
  int error = m_client->GetError(&message);

  m_result.SetOSError(error);
  m_result.SetOSErrorString(message);

  // Keep the return status
  m_result.SetStatus(m_message.GetStatus());

  // Keep result headers
  for(auto& header : *m_message.GetHeaderMap())
  {
    INPair pair;
    pair.m_name  = header.first;
    pair.m_value = header.second;
    m_result.SetHeader(pair);
  }

  // Keep result body
  m_result.SetBody(m_message.GetBody());

  // Keep last OAuth2 bearer token
  m_result.SetBearerToken(m_client->GetLastBearerToken());
}

void
InetRunner::PerformAllValidations()
{
  int step = 1;
  for(auto& vali : m_validations)
  {
    ValidateIN* validate = reinterpret_cast<ValidateIN*>(vali);
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

  CString filename = GetEffectiveStepFilename();
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
    Validate* validate = new ValidateIN();
    CString file = m_baseDirectory + m_testDirectory + filename;
    validate->ReadFromXML(file);
    m_validations.push_back(validate);
  }
  for(auto& filename : m_globalValidations)
  {
    Validate* validate = new ValidateIN();
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
}
