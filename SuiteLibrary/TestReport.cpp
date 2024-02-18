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
// This program: SuiteLibrary
// This File   : TestReport.cpp
// What it does: Create a report of all tests and test steps
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TestReport.h"
#include "TestSuite.h"
#include "TestStep.h"
#include "TestStepCMD.h"
#include "TestStepNET.h"
#include "TestStepSQL.h"
#include "ExtraExtensions.h"
#include "StepResult.h"
#include "StepResultCMD.h"
#include "StepResultNET.h"
#include "StepResultSQL.h"
#include <time.h>
#include <sys\timeb.h>
#include <memory>

using std::shared_ptr;
using std::dynamic_pointer_cast;

TestReport::TestReport(CString      p_basepath
                      ,CString      p_filename
                      ,CString      p_name
                      ,ReportDepth  p_depth)
           :m_basepath(p_basepath)
           ,m_fileName(p_filename)
           ,m_name(p_name)
           ,m_depth(p_depth)
{
}

void 
TestReport::SetPageSize(int p_lines)
{
  if(REPORT_MIN_PAGE_LENGTH <= p_lines && p_lines <= REPORT_MAX_PAGE_LENGTH)
  {
    m_pageSize = p_lines;
  }
}

void 
TestReport::SetPageWidth(int p_width)
{
  if(REPORT_MIN_PAGE_WIDTH <= p_width && p_width <= REPORT_MAX_PAGE_WIDTH)
  {
    m_pageWidth = p_width;
  }
}

bool 
TestReport::StartReport()
{
  PrintTiming();
  if(!OpenReport())
  {
    return false;
  }
  PrintAllTestSuites();
  return CloseReport();
}

//////////////////////////////////////////////////////////////////////////
//
// PRIVATE
//
//////////////////////////////////////////////////////////////////////////

void
TestReport::Reset()
{
  m_fileName = _T("Report.txt");
  m_name     = _T("TESTREPORT");
  m_page      = 0;
  m_line      = 0;
  m_pos       = 0;
  m_pageSize  = REPORT_PAGE_LENGTH;
  m_pageWidth = REPORT_PAGE_WIDTH;
  // Close output file (if open)
  if(m_file.GetIsOpen())
  {
    m_file.Close();
  }
}

//////////////////////////////////////////////////////////////////////////
//
// Service Routines
//
void
TestReport::PrintTiming()
{
  __timeb64 now;
  struct tm today;

  _ftime64_s(&now);
  _localtime64_s(&today, &now.time);
  m_timing.Format(_T("%4.4d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d")
                 ,today.tm_year + 1900
                 ,today.tm_mon + 1
                 ,today.tm_mday
                 ,today.tm_hour
                 ,today.tm_min
                 ,today.tm_sec);
}

bool
TestReport::OpenReport()
{
  CString filename = m_basepath + m_fileName;
  m_file.SetFilename(filename);

  if(m_file.Open(winfile_write | open_trans_text,FAttributes::attrib_none,Encoding::UTF8))
  {
    PrintHeader();
  }
  return m_file.GetIsOpen();
}

bool
TestReport::CloseReport()
{
  if(!m_file.GetIsOpen())
  {
    return false;
  }
  if(m_page || m_line)
  {
    PrintLastLine();
  }
  PrintFooter();

  return m_file.Close(true);
}

// TESTREPORT                      2022-06-01 20:00:00
// ---------------------------------------------------
void
TestReport::PrintHeader()
{
  // Optional 'upside-down-holy-hand-grenade' (page feed)
  if(m_page)
  {
    m_file.Putch(_T('\f'));
  }
  m_file.Write(m_name);

  int numspaces = m_pageWidth - m_name.GetLength() - m_timing.GetLength();
  for(int index = 0;index < numspaces;++index)
  {
    m_file.Putch(_T(' '));
  }
  m_file.Write(m_timing);
  m_file.Putch(_T('\n'));
  PrintLine();
}

// --------------------------------------------------
//                                            Page: n
void
TestReport::PrintFooter()
{
  while(m_line++ < m_pageSize)
  {
    m_file.Putch(_T('\n'));
  }

  PrintLine();
  CString number;
  number.Format(_T("Page: %d"),++m_page);

  int numspaces = m_pageWidth - number.GetLength();
  for(int index = 0;index < numspaces;++index)
  {
    m_file.Putch(_T(' '));
  }
  m_file.Write(number);
  m_file.Putch(_T('\n'));
  m_line = 0;
}

// Print line of dashes across the report
void
TestReport::PrintLine()
{
  for(int index = 0;index < m_pageWidth;++index)
  {
    m_file.Putch(_T('-'));
  }
  m_file.Putch(_T('\n'));
}

void
TestReport::Indent()
{
  PrintLine(_T("      "),false);
}

void
TestReport::PrintLastLine()
{
  PrintLine(_T("**** End of report ****"));
}

void
TestReport::PrintLine(CString p_text,bool p_next /*= true*/)
{
  CString extra;
  bool next(false);

  // Newlines received?
  int newline = p_text.Find('\n');
  if(newline >= 0)
  {
    CString before = p_text.Left(newline);
    CString after  = p_text.Mid(newline + 1);
    PrintLine(before);
    PrintLine(after,p_next);
    return;
  }

  // Are we detecting a line overflow?
  if(m_pos + p_text.GetLength() > m_pageWidth)
  {
    extra  = p_text.Mid(m_pageWidth - m_pos);
    p_text = p_text.Left(m_pageWidth - m_pos);
    next   = true;
  }

  // Print our text
  m_file.Write(p_text);
  m_pos += p_text.GetLength();

  // Handle line overflow
  if(p_next || next)
  {
    m_file.Putch('\n');
    m_pos = 0;

    if(++m_line >= m_pageSize)
    {
      PrintFooter();
      PrintHeader();
      m_line = 0;
    }
  }

  // Recurse to print the rest of the line
  if(!extra.IsEmpty())
  {
    PrintLine(extra,p_next);
  }
}

// Print a text at the end of the line
void
TestReport::PrintAtEnd(CString p_text)
{
  int numSpaces = m_pageWidth - m_pos - p_text.GetLength();
  for(int index = 0;index < numSpaces;++index)
  {
    m_file.Putch(' ');
  }
  PrintLine(p_text,true);
}

void
TestReport::PrintLine(CString p_part1,const TCHAR* p_part2)
{
  Indent();
  PrintLine(p_part1,false);
  PrintLine(p_part2);
}

//////////////////////////////////////////////////////////////////////////
//
// Service Routines
//

void
TestReport::PrintAllTestSuites()
{
  CString pattern(m_basepath + _T("*") + EXTENSION_SUITE);

  // Read in all known files
  WIN32_FIND_DATA data;
  HANDLE find = nullptr;
  if((find = FindFirstFile(pattern,&data)) != INVALID_HANDLE_VALUE)
  {
    do
    {
      CString filename = CString(data.cFileName);
      TestSuite suite(m_basepath);
      suite.ReadFromXML(filename);
      PrintTestSuite(&suite);
    } 
    while(FindNextFile(find,&data) != 0);
    FindClose(find);
  }
}

void
TestReport::PrintTestSuite(TestSuite* p_suite)
{
  PrintLine(_T("Testsuite: ") + p_suite->GetName());
  PrintLine(_T("File name: ") + p_suite->GetFilename());
  PrintLine();

  bool ok(true);

  for(auto& test : p_suite->GetAllTests())
  {
    CString name   = test.second.m_name;
    CString result = test.second.m_lastResult;
    if(result.Compare(_T("OK")))
    {
      result = _T("ERROR");
    }
    PrintLine(_T("SET : "),false);
    PrintLine(name,false);
    PrintAtEnd(result);

    if(result.CompareNoCase(_T("OK")))
    {
      ok = false;
    }

    // See if we must do it with a greater depth
    if(m_depth > ReportDepth::REPORT_TESTS)
    {
      PrintTestSet(test.second.m_directory,test.second.m_filename);
    }
  }
  PrintLine(_T(""));
  PrintLine(_T("Total result: "),false);
  PrintLine(ok ? _T("OK") : _T("ERROR"));
  PrintLine(_T(""));
}

void
TestReport::PrintTestSet(CString p_directory,CString p_filename)
{
  TestSet set;
  set.ReadFromXML(m_basepath + p_directory + _T("\\") + p_filename);
  for(auto& run : set.GetTestRuns())
  {
    PrintLine(_T("Run : "),false);
    PrintLine(run.m_name,false);
    PrintAtEnd(run.m_lastResult);

    // See if we must do it with a greater depth
    if(m_depth >= ReportDepth::REPORT_STEPS)
    {
      try
      {
        PrintTestStep(p_directory,run.m_filename);
      }
      catch(StdException& ex)
      {
        PrintLine(_T("ERROR: ") + ex.GetErrorMessage());
      }
    }
  }
}

void
TestReport::PrintTestStep(CString p_directory,CString p_filename)
{
  shared_ptr<TestStep> step = shared_ptr<TestStep>(ReadTestStep(m_basepath + p_directory + _T("\\") + p_filename));
  if(!step)
  {
    throw StdException(_T("Teststep not found: ") + p_directory + p_filename);
  }
  shared_ptr<TestStepCMD> cmd = dynamic_pointer_cast<TestStepCMD>(step);
  if(cmd)
  {
    PrintTestStepCMD(cmd.get(), p_directory, p_filename);
  }
  shared_ptr<TestStepNET> net = dynamic_pointer_cast<TestStepNET>(step);
  if(net)
  {
    PrintTestStepNET(net.get(),p_directory,p_filename);
  }
  shared_ptr<TestStepSQL> sql = dynamic_pointer_cast<TestStepSQL>(step);
  if(sql)
  {
    PrintTestStepSQL(sql.get(),p_directory,p_filename);
  }
}

void
TestReport::PrintTestStepCMD(TestStepCMD* p_cmd,CString p_directory,CString p_filename)
{
  PrintLine(_T("Step: "),false);
  PrintLine(p_cmd->GetName(),false);

  if(m_depth == ReportDepth::REPORT_FULL)
  {
    // Print details of the step
    PrintLine(_T(""));
    PrintStepCMDDetails(p_cmd);
  }
  if(m_depth >= ReportDepth::REPORT_VALIDATE)
  {
    // Print all validations
  }
  
  // Print the result of the CMD step
  CString filename(p_filename);
  filename.Replace(EXTENSION_TESTSTEP_CMD,EXTENSION_RESULT_CMD);
  PrintStepResultCMD(p_directory,filename);
}

void  
TestReport::PrintTestStepNET(TestStepNET* p_net,CString p_directory,CString p_filename)
{
  PrintLine(_T("Step: "),false);
  PrintLine(p_net->GetName(),false);

  if(m_depth == ReportDepth::REPORT_FULL)
  {
    // Print details of the step
    PrintStepNETDetails(p_net);
  }
  if(m_depth >= ReportDepth::REPORT_VALIDATE)
  {
    // Print all validations
  }
  
  // Print the result of the CMD step
  CString filename(p_filename);
  filename.Replace(EXTENSION_TESTSTEP_NET,EXTENSION_RESULT_NET);
  PrintStepResultNET(p_directory,filename);
}

void
TestReport::PrintTestStepSQL(TestStepSQL* p_sql,CString p_directory,CString p_filename)
{
  PrintLine(_T("Step: "),false);
  PrintLine(p_sql->GetName(),false);

  if(m_depth == ReportDepth::REPORT_FULL)
  {
    // Print details of the step
    PrintStepSQLDetails(p_sql);
  }
  if(m_depth >= ReportDepth::REPORT_VALIDATE)
  {
    // Print all validations
  }
  
  // Print the result of the CMD step
  CString filename(p_filename);
  filename.Replace(EXTENSION_TESTSTEP_SQL,EXTENSION_RESULT_SQL);
  PrintStepResultSQL(p_directory,filename);
}

void
TestReport::PrintStepCMDDetails(TestStepCMD* p_cmd)
{
  // Directory path
  //        "----------------------- :"
  PrintLine(_T("Command   directory path: "),p_cmd->GetDirectoryPath());
  PrintLine(_T("Effective directory path: "),p_cmd->GetEffectiveDirectory());

  // Executable path
  PrintLine(_T("Command   executable    : "),p_cmd->GetRuntimer());
  PrintLine(_T("Effective executable    : "),p_cmd->GetEffectiveRuntimer());

  // Command line
  PrintLine(_T("Adding    command line  : "),p_cmd->GetCommandLine());
  PrintLine(_T("Effective command line  : "),p_cmd->GetEffectiveCommandLine());

  // Standard input after starting
  PrintLine(_T("Extra     standard input: "),p_cmd->GetStandardInput());
  PrintLine(_T("Effective standard input: "),p_cmd->GetEffectiveInput());

  // Options
  PrintLine(_T("Starting a window       : "),p_cmd->GetStartWindow()    ? _T("Yes") : _T("No"));
  PrintLine(_T("Wait for idle runtimer  : "),p_cmd->GetWaitForIdle()    ? _T("Yes") : _T("No"));
  PrintLine(_T("Using the return value  : "),p_cmd->GetUseReturnValue() ? _T("Yes") : _T("No"));
  PrintLine(_T("Using the output value  : "),p_cmd->GetUseOutputValue() ? _T("Yes") : _T("No"));
  PrintLine(_T("Using the error stream  : "),p_cmd->GetUseErrorValue()  ? _T("Yes") : _T("No"));
  // Variables
  PrintLine(_T("Using return variable   : "),p_cmd->GetReturnVariable());
  PrintLine(_T("Using output variable   : "),p_cmd->GetOutputVariable());
  PrintLine(_T("Using error  variable   : "),p_cmd->GetErrorVariable());
  // Timing
  PrintLine(_T("Waiting before running  : "),p_cmd->GetWaitBeforeRun());
  PrintLine(_T("Max execution time      : "),p_cmd->GetMaxExecution());
  PrintLine(_T("Waiting after  running  : "),p_cmd->GetWaitAfterRun());

  // Standard input
  ParMap& map = p_cmd->GetEnvironmentVars();
  if(p_cmd->GetHandleEnvironment() && !map.empty())
  {
    Indent();
    PrintLine(_T("Extra defined environment variables:"));
    for(auto& var : map)
    {
      Indent();
      PrintLine(var.first,false);
      PrintLine(_T(" : "),false);
      PrintLine(var.second);
    }
  }
  else
  {
    Indent();
    PrintLine(_T("Using the environment   : No"));
  }
}

void
TestReport::PrintStepNETDetails(TestStepNET* p_net)
{
  // URL
  //        "----------------------- :"
  PrintLine(_T("Internet call to URL    : "),p_net->GetEffectiveURL());
  PrintLine(_T("Internet call + params  : "),p_net->GetEffectiveCombinedURL());

  // URL Parts
  PrintLine(_T("URL parts : VERB        : "),p_net->GetVerb());
  PrintLine(_T("URL parts : URI         : "),p_net->GetURL());
  UrlParameters& params = p_net->GetParameters();
  if(!params.empty())
  {
    for(auto& parm : params)
    {
      PrintLine(_T("URL parts : parameter   : "),parm.m_name + _T("=") + parm.m_value);
    }
  }
  PrintLine(_T("URL parts : Anchor          : "), p_net->GetAnchor());

  UrlHeaders& allheaders = p_net->GetHeaders();
  if(allheaders.empty())
  {
    for(auto& head : allheaders)
    {
      PrintLine(_T("HTTP payload : header   : "),head.m_name + _T(": ") + head.m_value);
    }
  }
  if(p_net->GetBodyInputIsFile())
  {
    PrintLine(_T("HTTP payload : body file  : "),p_net->GetFilenameInput());
    PrintLine(_T("HTTP payload : effective  : "),p_net->GetEffectiveFileInput());
  }
  else
  {
    PrintLine(_T("HTTP payload : body       : "));
    PrintLine(p_net->GetBody());
  }
  if(p_net->GetBodyOutputIsFile())
  {
    PrintLine(_T("HTTP payload : output file: "),p_net->GetFilenameOutput());
    PrintLine(_T("HTTP payload : effective  : "),p_net->GetEffectiveFileOutput());
  }
  // Authentication
  PrintLine(_T("Authentication type     : "),p_net->GetAuthType());
  PrintLine(_T("Authentication user     : "),p_net->GetAuthUser());
  PrintLine(_T("Authentication password : "),p_net->GetAuthPassword());
  PrintLine(_T("Authentication grant    : "),p_net->GetAuthGrant());
  PrintLine(_T("OAuth2 token server     : "),p_net->GetAuthTokenServer());
  PrintLine(_T("OAuth2 client ID        : "),p_net->GetAuthClientID());
  PrintLine(_T("OAuth2 client key       : "),p_net->GetAuthClientKey());
  PrintLine(_T("OAuth2 client scope     : "),p_net->GetAuthClientScope());
  PrintLine(_T("OAuth2 bearer token     : "),p_net->GetAuthBearerToken());
  // Using parts
  PrintLine(_T("Using return status     : "),p_net->GetUseStatus()  ? _T("Yes") : _T("No"));
  PrintLine(_T("Using response headers  : "),p_net->GetUseHeaders() ? _T("Yes") : _T("No"));
  PrintLine(_T("Using response body     : "),p_net->GetUseBody()    ? _T("Yes") : _T("No"));
  // Effective
  Indent();
  PrintLine(_T("Effective total raw request:"));
  PrintLine(p_net->GetRawRequest());
}

void
TestReport::PrintStepSQLDetails(TestStepSQL* p_sql)
{
  PrintLine(_T("SQL datasource          : "),p_sql->GetDataSource());
  PrintLine(_T("SQL effective datasource: "),p_sql->GetEffectiveDatasource());
  PrintLine(_T("SQL username            : "),p_sql->GetUser());
  PrintLine(_T("SQL effective username  : "),p_sql->GetEffectiveUser());
  PrintLine(_T("SQL password            : "),p_sql->GetPassword());
  PrintLine(_T("SQL effective password  : "),_T("***********"));
  Indent();
  PrintLine(_T("SQL statement:"));
  PrintLine(p_sql->GetSQL());
  Indent();
  PrintLine(_T("SQL effective statement:"));
  PrintLine(p_sql->GetEffectiveSQL());
}

//////////////////////////////////////////////////////////////////////////
//
// RESULTS
//
//////////////////////////////////////////////////////////////////////////

void
TestReport::PrintStepResultCMD(CString p_directory,CString p_filename)
{
  CString filename = m_basepath + p_directory + _T("\\") + p_filename;
  shared_ptr<StepResult> result = shared_ptr<StepResult>(ReadStepResult(filename));
  if(!result)
  {
    throw StdException(_T("Step result not found: ") + filename);
  }
  shared_ptr<StepResultCMD> cmd = dynamic_pointer_cast<StepResultCMD>(result);
  if(!cmd)
  {
    throw StdException(_T("Expected a StepResult for a CMD: ") + filename);
  }
  // Print the result
  if(m_depth == ReportDepth::REPORT_FULL)
  {
    // Print details of the result
    PrintResultCMD(cmd.get());
  }
  if(m_depth >= ReportDepth::REPORT_VALIDATE)
  {
    // Print all validations
    PrintLine(_T(""));
    Indent();
    PrintLine(_T("Validations"));
    ValSteps& valis = cmd->GetValidations();
    for(auto& vali : valis)
    {
      CString validation;
      validation.Format(_T("%d: %s %s"),vali.m_number,vali.m_global ? _T("GLOB") : _T("LCAL"),vali.m_validation.GetString());
      Indent();
      PrintLine(validation,false);
      PrintAtEnd(vali.m_ok ? _T("OK") : _T("ERROR"));
    }
  }
  if(m_depth > ReportDepth::REPORT_STEPS)
  {
    Indent();
    PrintLine(_T("Total step result:"),false);
  }
  PrintAtEnd(cmd->GetTotalResult() ? _T("OK") : _T("ERROR"));
}


void
TestReport::PrintStepResultNET(CString p_directory, CString p_filename)
{
  CString filename = m_basepath + p_directory + _T("\\") + p_filename;
  shared_ptr<StepResult> result = shared_ptr<StepResult>(ReadStepResult(filename));
  if(!result)
  {
    throw StdException(_T("Step result not found: ") + filename);
  }
  shared_ptr<StepResultNET> net = dynamic_pointer_cast<StepResultNET>(result);
  if(!net)
  {
    throw StdException(_T("Expected a StepResult for a NET: ") + filename);
  }
  // Print the result
  if(m_depth == ReportDepth::REPORT_FULL)
  {
    // Print details of the result
    PrintResultNET(net.get());
  }
  if(m_depth >= ReportDepth::REPORT_VALIDATE)
  {
    // Print all validations
    PrintLine(_T(""));
    Indent();
    PrintLine(_T("Validations"));
    ValSteps& valis = net->GetValidations();
    for(auto& vali : valis)
    {
      CString validation;
      validation.Format(_T("%d: %s %s"),vali.m_number,vali.m_global ? _T("GLOB") : _T("LCAL"),vali.m_validation.GetString());
      Indent();
      PrintLine(validation,false);
      PrintAtEnd(vali.m_ok ? _T("OK") : _T("ERROR"));
    }
  }
  if(m_depth > ReportDepth::REPORT_STEPS)
  {
    Indent();
    PrintLine(_T("Total step result:"),false);
  }
  PrintAtEnd(net->GetTotalResult() ? _T("OK") : _T("ERROR"));

}

void
TestReport::PrintStepResultSQL(CString p_directory, CString p_filename)
{
  CString filename = m_basepath + p_directory + _T("\\") + p_filename;
  shared_ptr<StepResult> result = shared_ptr<StepResult>(ReadStepResult(filename));
  if(!result)
  {
    throw StdException(_T("Step result not found: ") + filename);
  }
  shared_ptr<StepResultSQL> sql = dynamic_pointer_cast<StepResultSQL>(result);
  if(!sql)
  {
    throw StdException(_T("Expected a StepResult for a SQL: ") + filename);
  }
  // Print the result
  if(m_depth == ReportDepth::REPORT_FULL)
  {
    // Print details of the result
    PrintResultSQL(sql.get());
  }
  if(m_depth >= ReportDepth::REPORT_VALIDATE)
  {
    // Print all validations
    PrintLine(_T(""));
    Indent();
    PrintLine(_T("Validations"));
    ValSteps& valis = sql->GetValidations();
    for(auto& vali : valis)
    {
      CString validation;
      validation.Format(_T("%d: %s %s"),vali.m_number,vali.m_global ? _T("GLOB") : _T("LCAL"),vali.m_validation.GetString());
      Indent();
      PrintLine(validation,false);
      PrintAtEnd(vali.m_ok ? _T("OK") : _T("ERROR"));
    }
  }
  if(m_depth > ReportDepth::REPORT_STEPS)
  {
    Indent();
    PrintLine(_T("Total step result:"),false);
  }
  PrintAtEnd(sql->GetTotalResult() ? _T("OK") : _T("ERROR"));
}

// Details of a result set
void
TestReport::PrintResultCMD(StepResultCMD* p_cmd)
{
  CString text;

  // Timing
  text.Format(_T("Test run for %.4f seconds"),p_cmd->GetTiming());
  Indent();
  PrintLine(text);
  // Return value
  text.Format(_T("Test command return value: %d"),p_cmd->GetReturnValue());
  Indent();
  PrintLine(text);
  // Standard output
  Indent();
  PrintLine(_T("Command standard output returned:"));
  Indent();
  PrintLine(p_cmd->GetStandardOutput());
  // Standard error
  if(!p_cmd->GetStandardError().IsEmpty())
  {
    Indent();
    PrintLine(_T("Command standard error returned:"));
    Indent();
    PrintLine(p_cmd->GetStandardError());
  }
}

void
TestReport::PrintResultNET(StepResultNET* p_net)
{
  CString text;

  // Timing
  text.Format(_T("Test run for %.4f seconds"),p_net->GetTiming());
  Indent();
  PrintLine(text);

  // HTTP Status
  text.Format(_T("HTTP return status: %d"),p_net->GetStatus());
  Indent();
  PrintLine(text);

  // 'invisible' OS error
  if(p_net->GetOSError())
  {
    Indent();
    PrintLine(_T("OS error status: "),false);
    PrintLine(p_net->GetOSErrorString());
  }
  CString token = p_net->GetBearerToken();
  if(!token.IsEmpty())
  {
    Indent();
    PrintLine(_T("Authorization bearer token: "),false);
    PrintLine(token);
  }
  Indent();
  PrintLine(_T("Total HTTP response + body:"));
  PrintLine(p_net->GetRawResponse());
}

void
TestReport::PrintResultSQL(StepResultSQL* p_sql)
{
  CString text;

  // Timing
  text.Format(_T("Test run for %.4f seconds"),p_sql->GetTiming());
  Indent();
  PrintLine(text);

  // Result
  text.Format(_T("SQL Succeeded: %d Rows: %d Columns: %d")
              ,p_sql->GetSucceeded()
              ,p_sql->GetResultRows()
              ,p_sql->GetResultCols());
  Indent();
  PrintLine(text);

  // SQLSTATE
  CString state = p_sql->GetSQLState();
  if(!state.IsEmpty())
  {
    Indent();
    PrintLine(_T("SQLSTATE: "));
    PrintLine(state);
  }

  // Native error
  CString error = p_sql->GetNativeStatus();
  if(!error.IsEmpty())
  {
    Indent();
    PrintLine(_T("Native error: "));
    PrintLine(error);
  }

  // Results;
  Indent();
  PrintLine(_T("Result data:"));
  Indent();
  PrintLine(_T("------------------------------ "),false);
  PrintLine(_T("------------------------------"));
  ResultMap& map = p_sql->GetResultMap();
  for(auto& row : map)
  {
    text.Format(_T("%32.32s %s"),row.first.GetString(),row.second.GetString());
    Indent();
    PrintLine(text);
  }

}
