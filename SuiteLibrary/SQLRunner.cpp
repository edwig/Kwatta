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
// This File   : SQLRunner.cpp
// What it does: Running one (1) database SQL test
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SQLRunner.h"
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

SQLRunner::SQLRunner(SQLDatabase* p_database
                    ,CString      p_baseDirectory
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
           ,m_database(p_database)
{
  m_testStep = new TestStepSQL();
  m_result   = new StepResultSQL();
}

SQLRunner::~SQLRunner()
{
  // Clean up the query object
  if(m_query)
  {
    delete m_query;
    m_query = nullptr;
  }
}

// General start of the test
// Public part, called by the main TestRunnerApp
int
SQLRunner::PerformTest()
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
SQLRunner::StopTestProgram()
{
  if(m_database && m_query)
  {
    try
    {
      m_query->DoCancelQuery();
    }
    catch(StdException& /*ex*/)
    {
      // What to do?
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
// PRIVATE
//
//////////////////////////////////////////////////////////////////////////

void
SQLRunner::InitRunner()
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
  // 4   Testing RUNNING THE SQL STATEMENT
  // 5   Testing post command waiting
  // 6   Writing the results file
  // 7   Writing result parameters
  // 8   Reaching a conclusion

  // +   1 for every validation step
  m_steps    = 8 + (int) m_localValidations.size();
  m_stepSize = 100 / m_steps;

  InitSQLComponents();
}

void
SQLRunner::ReadingAllFiles()
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
SQLRunner::ParameterProcessing()
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
SQLRunner::PerformCommand()
{
  PerformStep(_T("RUN THE SQL..."));
  TestStepSQL*   step   = dynamic_cast<TestStepSQL*>(m_testStep);
  StepResultSQL* result = dynamic_cast<StepResultSQL*>(m_result);

  // Take name and documentation
  m_result->SetName(m_testStep->GetName());
  m_result->SetDocumentation(m_testStep->GetDocumentation());
  result->SetSucceeded(false);

  CString     standardout;
  CString     standarderr;
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

  try
  {
    if(m_database->IsOpen() == false || 
       m_database->GetUserName().CompareNoCase(step->GetEffectiveUser()) ||
       m_database->GetDatasource().CompareNoCase(step->GetEffectiveDatasource()))
    { 
      if(m_database->IsOpen())
      {
        // Opened for a different user or datasource
        m_database->Close();
      }
      m_database->Open(step->GetEffectiveDatasource()
                      ,step->GetEffectiveUser()
                      ,step->GetEffectivePassword());
    }
    if(m_database->IsOpen())
    {
      m_query = new SQLQuery(m_database);
      SQLTransaction trans(m_database,_T("Test"));

      counter.Start();
      m_query->DoSQLStatement(step->GetEffectiveSQL());
      counter.Stop();
      if(m_query->GetRecord())
      {
        // Read the first record of the cursor
        ReadResultSet();
      }
      else
      {
        // Non-select statement, record the number of rows affected
        result->SetResultRows(m_query->GetNumberOfRows());
      }
      result->SetSucceeded(true);
      trans.Commit();
    }
  }
  catch(StdException& ex)
  {
    CString error = ex.GetErrorMessage();
    if(m_query)
    {
      result->SetNativeStatus(m_query->GetError() + error);
    }
    else if(m_database)
    {
      result->SetNativeStatus(m_database->GetErrorString() + error);
    }
  }
  result->SetSQLState(m_database->GetSQLState());

  // ::PostMessage(theApp.GetConsoleHandle(),WM_SHOWWINDOW,FALSE,0);

  // No more killing of the query by the boobytrap
  SQLQuery* temp = m_query;
  m_query = nullptr;
  delete temp;

  // Stop the high-performance timer!
  counter.Stop();
  m_result->SetTiming(counter.GetCounter());
}

void  
SQLRunner::ReadResultSet()
{
  StepResultSQL* result = dynamic_cast<StepResultSQL*>(m_result);

  // Record number of cols
  result->SetResultCols(m_query->GetNumberOfColumns());

  // Record the values of the first row;
  for(int ind = 1;ind <= m_query->GetNumberOfColumns();++ind)
  {
    CString name;
    CString value;
    m_query->GetColumnName(ind,name);
    m_query->GetColumn(ind)->GetAsString(value);

    result->AddResult(name,value);
  }

  // Read through all the other rows
  int rows = 1;
  while(m_query->GetRecord())
  {
    ++rows;
  }
  result->SetResultRows(rows);
}

void
SQLRunner::PerformAllValidations()
{
  StepResultSQL* result = dynamic_cast<StepResultSQL*>(m_result);

  int step = 1;
  for(auto& vali : m_validations)
  {
    ValidateSQL* validate = dynamic_cast<ValidateSQL*>(vali);
    PerformStep(_T("Validation: ") + validate->GetName());

    // Do the validations
    bool totalresult = true;
    if(validate->ValidateSucceeded   (&m_parameters,result->GetSucceeded())    == false) totalresult = false;
    if(validate->ValidateReturnedCols(&m_parameters,result->GetResultCols())   == false) totalresult = false;
    if(validate->ValidateReturnedRows(&m_parameters,result->GetResultRows())   == false) totalresult = false;
    if(validate->ValidateSQLState    (&m_parameters,result->GetSQLState())     == false) totalresult = false;
    if(validate->ValidateNativeStatus(&m_parameters,result->GetNativeStatus()) == false) totalresult = false;
    if(validate->ValidateFirstData   (&m_parameters,result->GetFirstData())    == false) totalresult = false;
    if(validate->ValidateColumnData  (result->GetResultMap())                  == false) totalresult = false;
    // Add the validation to the result set
    result->AddValidation(step++,validate->GetName(),validate->GetFilename(), totalresult,validate->GetGlobal());
  }
}

// Saving the test results to the disk
void
SQLRunner::SaveTestResults()
{
  PerformStep(_T("Saving the test results"));
  StepResultSQL* result = dynamic_cast<StepResultSQL*>(m_result);

  CString filename = m_baseDirectory + m_testDirectory + m_testStepFilename;
  filename.MakeLower();
  filename.Replace(EXTENSION_TESTSTEP_SQL,EXTENSION_RESULT_SQL);

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
SQLRunner::SaveResultParameters()
{
  PerformStep(_T("Saving result parameters"));
  // Write back local parameters (return + stream)
  m_parameters.WriteToXML();
}

// Conclusion of the test
int
SQLRunner::ReadTotalResult()
{
  PerformStep(_T("Getting total result..."));
  StepResultSQL* result = dynamic_cast<StepResultSQL*>(m_result);
  return result->GetTotalResult();
}

//////////////////////////////////////////////////////////////////////////
//
// DETAILS

StepResultSQL* 
SQLRunner::GetStepResult()
{
  return dynamic_cast<StepResultSQL*>(m_result);
}

void
SQLRunner::ReadTestStep()
{
  // Read in the definition file for a test step
  TestStepSQL* step = dynamic_cast<TestStepSQL*>(m_testStep);
  CString filename = GetEffectiveStepFilename();
  step->ReadFromXML(filename);
}

// Reads all validations from the command line
// Could be more than 1 file in all...
void
SQLRunner::ReadValidations()
{
  for(auto& filename : m_localValidations)
  {
    Validate* validate = new ValidateSQL();
    CString file = m_baseDirectory + m_testDirectory + filename;
    validate->ReadFromXML(file);
    m_validations.push_back(validate);
  }
  for(auto& filename : m_globalValidations)
  {
    Validate* validate = new ValidateSQL();
    CString file = m_baseDirectory + _T("Validations\\") + filename;
    validate->ReadFromXML(file);
    validate->SetGlobal(true);
    m_validations.push_back(validate);
  }
}

int
SQLRunner::CheckStatusOK(int p_returnCode)
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
SQLRunner::CreateQLErrorMessage(CString p_error)
{
  StepResultSQL* result = reinterpret_cast<StepResultSQL*>(m_result);

  result->AddResult(_T("QL-ERROR"),p_error);
}

/*static*/ unsigned
__stdcall ExecSQLBoobytrap(void* p_data)
{
  SQLRunner* runner = reinterpret_cast<SQLRunner*>(p_data);
  int maxtime = runner->GetMaxRunningTime();
  // Wait the maximum time of the test
  Sleep(maxtime);
  // End the test program
  runner->StopTestProgram();

  return 0;
}

void
SQLRunner::SetBoobytrap()
{
  // Start a new thread
  unsigned int threadID = 0L;
  if((m_thread = (HANDLE)_beginthreadex(NULL,0,ExecSQLBoobytrap,this,0,&threadID)) == INVALID_HANDLE_VALUE)
  {
    threadID = 0;
    m_thread = NULL;
  }
}

void
SQLRunner::StopBoobytrap()
{
  TerminateThread(m_thread,0xFFFFFFFF);
}
