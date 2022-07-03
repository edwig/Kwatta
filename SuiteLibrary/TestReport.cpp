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
// What it does: Create a report of all tests and teststeps
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
  m_fileName = "Report.txt";
  m_name     = "TESTREPORT";
  if(m_file)
  {
    fclose(m_file);
    m_file = nullptr;
  }
  m_page      = 0;
  m_line      = 0;
  m_pos       = 0;
  m_pageSize  = REPORT_PAGE_LENGTH;
  m_pageWidth = REPORT_PAGE_WIDTH;
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
  m_timing.Format("%4.4d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d"
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
  fopen_s(&m_file, filename, "w");
  if(m_file)
  {
    PrintHeader();
  }
  return m_file != nullptr;
}

bool
TestReport::CloseReport()
{
  if(!m_file)
  {
    return false;
  }
  if(m_page || m_line)
  {
    PrintLastLine();
  }
  PrintFooter();

  fclose(m_file);
  m_file = nullptr;

  return true;
}

// TESTREPORT                      2022-06-01 20:00:00
// ---------------------------------------------------
void
TestReport::PrintHeader()
{
  // Optional 'upside-down-holy-hand-grenade' (page feed)
  if(m_page)
  {
    fputc('\f',m_file);
  }

  fprintf(m_file,m_name);
  int numspaces = m_pageWidth - m_name.GetLength() - m_timing.GetLength();
  for(int index = 0;index < numspaces;++index)
  {
    fputc(' ',m_file);
  }
  fprintf(m_file,m_timing);
  fputc('\n',m_file);
  PrintLine();
}

// --------------------------------------------------
//                                            Page: n
void
TestReport::PrintFooter()
{
  while(m_line++ < m_pageSize)
  {
    fputc('\n', m_file);
  }

  PrintLine();
  CString number;
  number.Format("Page: %d",++m_page);

  int numspaces = m_pageWidth - number.GetLength();
  for(int index = 0;index < numspaces;++index)
  {
    fputc(' ',m_file);
  }
  fprintf(m_file,number);
  fputc('\n',m_file);
  m_line = 0;
}

// Print line of dashes across the report
void
TestReport::PrintLine()
{
  for(int index = 0;index < m_pageWidth;++index)
  {
    fputc('-',m_file);
  }
  fputc('\n',m_file);
}

void
TestReport::Indent()
{
  PrintLine("      ",false);
}

void
TestReport::PrintLastLine()
{
  PrintLine("**** End of report ****");
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
  fprintf(m_file,p_text);
  m_pos += p_text.GetLength();

  // Handle line overflow
  if(p_next || next)
  {
    fputc('\n',m_file);
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
    fputc(' ',m_file);
  }
  PrintLine(p_text,true);
}

void
TestReport::PrintLine(CString p_part1,const char* p_part2)
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
  CString pattern(m_basepath + "*" + EXTENSION_SUITE);

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
  PrintLine("Testsuite: " + p_suite->GetName());
  PrintLine("File name: " + p_suite->GetFilename());
  PrintLine();

  bool ok(true);

  for(auto& test : p_suite->GetAllTests())
  {
    CString name   = test.second.m_name;
    CString result = test.second.m_lastResult;
    if(result.Compare("OK"))
    {
      result = "ERROR";
    }
    PrintLine("SET : ",false);
    PrintLine(name,false);
    PrintAtEnd(result);

    if(result.CompareNoCase("OK"))
    {
      ok = false;
    }

    // See if we must do it with a greater depth
    if(m_depth > ReportDepth::REPORT_TESTS)
    {
      PrintTestSet(test.second.m_directory,test.second.m_filename);
    }
  }
  PrintLine("");
  PrintLine("Total result: ",false);
  PrintLine(ok ? "OK" : "ERROR");
  PrintLine("");
}

void
TestReport::PrintTestSet(CString p_directory,CString p_filename)
{
  TestSet set;
  set.ReadFromXML(m_basepath + p_directory + "\\" + p_filename);
  for(auto& run : set.GetTestRuns())
  {
    PrintLine("Run : ",false);
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
        PrintLine("ERROR: " + ex.GetErrorMessage());
      }
    }
  }
}

void
TestReport::PrintTestStep(CString p_directory,CString p_filename)
{
  shared_ptr<TestStep> step = shared_ptr<TestStep>(ReadTestStep(m_basepath + p_directory + "\\" + p_filename));
  if(!step)
  {
    throw StdException("Teststep not found: " + p_directory + p_filename);
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
  PrintLine("Step: ",false);
  PrintLine(p_cmd->GetName(),false);

  if(m_depth == ReportDepth::REPORT_FULL)
  {
    // Print details of the step
    PrintLine("");
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
  PrintLine("Step: ",false);
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
  PrintLine("Step: ",false);
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
  PrintLine("Command   directory path: ",p_cmd->GetDirectoryPath());
  PrintLine("Effective directory path: ",p_cmd->GetEffectiveDirectory());

  // Executable path
  PrintLine("Command   executable    : ",p_cmd->GetRuntimer());
  PrintLine("Effective executable    : ",p_cmd->GetEffectiveRuntimer());

  // Command line
  PrintLine("Adding    command line  : ",p_cmd->GetCommandLine());
  PrintLine("Effective command line  : ",p_cmd->GetEffectiveCommandLine());

  // Standard input after starting
  PrintLine("Extra     standard input: ",p_cmd->GetStandardInput());
  PrintLine("Effective standard input: ",p_cmd->GetEffectiveInput());

  // Options
  PrintLine("Starting a window       : ",p_cmd->GetStartWindow()    ? "Yes" : "No");
  PrintLine("Wait for idle runtimer  : ",p_cmd->GetWaitForIdle()    ? "Yes" : "No");
  PrintLine("Using the return value  : ",p_cmd->GetUseReturnValue() ? "Yes" : "No");
  PrintLine("Using the output value  : ",p_cmd->GetUseOutputValue() ? "Yes" : "No");
  PrintLine("Using the error stream  : ",p_cmd->GetUseErrorValue()  ? "Yes" : "No");
  // Variables
  PrintLine("Using return variable   : ",p_cmd->GetReturnVariable());
  PrintLine("Using output variable   : ",p_cmd->GetOutputVariable());
  PrintLine("Using error  variable   : ",p_cmd->GetErrorVariable());
  // Timing
  PrintLine("Waiting before running  : ",p_cmd->GetWaitBeforeRun());
  PrintLine("Max execution time      : ",p_cmd->GetMaxExecution());
  PrintLine("Waiting after  running  : ",p_cmd->GetWaitAfterRun());

  // Standard input
  ParMap& map = p_cmd->GetEnvironmentVars();
  if(p_cmd->GetHandleEnvironment() && !map.empty())
  {
    Indent();
    PrintLine("Extra defined environment variables:");
    for(auto& var : map)
    {
      Indent();
      PrintLine(var.first,false);
      PrintLine(" : ",false);
      PrintLine(var.second);
    }
  }
  else
  {
    Indent();
    PrintLine("Using the environment   : No");
  }
}

void
TestReport::PrintStepNETDetails(TestStepNET* p_net)
{
  // URL
  //        "----------------------- :"
  PrintLine("Internet call to URL    : ",p_net->GetEffectiveURL());
  PrintLine("Internet call + params  : ",p_net->GetEffectiveCombinedURL());

  // URL Parts
  PrintLine("URL parts : VERB        : ",p_net->GetVerb());
  PrintLine("URL parts : URI         : ",p_net->GetURL());
  UrlParameters& params = p_net->GetParameters();
  if(!params.empty())
  {
    for(auto& parm : params)
    {
      PrintLine("URL parts : parameter   : ",parm.m_name + "=" + parm.m_value);
    }
  }
  PrintLine("URL parts : Anchor          : ", p_net->GetAnchor());

  UrlHeaders& headers = p_net->GetHeaders();
  if(!headers.empty())
  {
    for(auto& head : headers)
    {
      PrintLine("HTTP payload : header   : ",head.m_name + ": " + head.m_value);
    }
  }
  if(p_net->GetBodyInputIsFile())
  {
    PrintLine("HTTP payload : body file  : ",p_net->GetFilenameInput());
    PrintLine("HTTP payload : effective  : ",p_net->GetEffectiveFileInput());
  }
  else
  {
    PrintLine("HTTP payload : body       : ");
    PrintLine(p_net->GetBody());
  }
  if(p_net->GetBodyOutputIsFile())
  {
    PrintLine("HTTP payload : output file: ",p_net->GetFilenameOutput());
    PrintLine("HTTP payload : effective  : ",p_net->GetEffectiveFileOutput());
  }
  // Authentication
  PrintLine("Authentication type     : ",p_net->GetAuthType());
  PrintLine("Authentication user     : ",p_net->GetAuthUser());
  PrintLine("Authentication password : ",p_net->GetAuthPassword());
  PrintLine("Authentication grant    : ",p_net->GetAuthGrant());
  PrintLine("OAuth2 token server     : ",p_net->GetAuthTokenServer());
  PrintLine("OAuth2 client ID        : ",p_net->GetAuthClientID());
  PrintLine("OAuth2 client key       : ",p_net->GetAuthClientKey());
  PrintLine("OAuth2 client scope     : ",p_net->GetAuthClientScope());
  PrintLine("OAuth2 bearer token     : ",p_net->GetAuthBearerToken());
  // Using parts
  PrintLine("Using return status     : ",p_net->GetUseStatus()  ? "Yes" : "No");
  PrintLine("Using response headers  : ",p_net->GetUseHeaders() ? "Yes" : "No");
  PrintLine("Using response body     : ",p_net->GetUseBody()    ? "Yes" : "No");
  // Effective
  Indent();
  PrintLine("Effective total raw request:");
  PrintLine(p_net->GetRawRequest());
}

void
TestReport::PrintStepSQLDetails(TestStepSQL* p_sql)
{
  PrintLine("SQL datasource          : ",p_sql->GetDataSource());
  PrintLine("SQL effective datasource: ",p_sql->GetEffectiveDatasource());
  PrintLine("SQL username            : ",p_sql->GetUser());
  PrintLine("SQL effective username  : ",p_sql->GetEffectiveUser());
  PrintLine("SQL password            : ",p_sql->GetPassword());
  PrintLine("SQL effective password  : ","***********");
  Indent();
  PrintLine("SQL statement:");
  PrintLine(p_sql->GetSQL());
  Indent();
  PrintLine("SQL effective statement:");
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
  CString filename = m_basepath + p_directory + "\\" + p_filename;
  shared_ptr<StepResult> result = shared_ptr<StepResult>(ReadStepResult(filename));
  if(!result)
  {
    throw StdException("Step result not found: " + filename);
  }
  shared_ptr<StepResultCMD> cmd = dynamic_pointer_cast<StepResultCMD>(result);
  if(!cmd)
  {
    throw StdException("Expected a StepResult for a CMD: " + filename);
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
    PrintLine("");
    Indent();
    PrintLine("Validations");
    ValSteps& valis = cmd->GetValidations();
    for(auto& vali : valis)
    {
      CString validation;
      validation.Format("%d: %s %s",vali.m_number,vali.m_global ? "GLOB" : "LCAL",vali.m_validation.GetString());
      Indent();
      PrintLine(validation,false);
      PrintAtEnd(vali.m_ok ? "OK" : "ERROR");
    }
  }
  if(m_depth > ReportDepth::REPORT_STEPS)
  {
    Indent();
    PrintLine("Total step result:",false);
  }
  PrintAtEnd(cmd->GetTotalResult() ? "OK" : "ERROR");
}


void
TestReport::PrintStepResultNET(CString p_directory, CString p_filename)
{
  CString filename = m_basepath + p_directory + "\\" + p_filename;
  shared_ptr<StepResult> result = shared_ptr<StepResult>(ReadStepResult(filename));
  if(!result)
  {
    throw StdException("Step result not found: " + filename);
  }
  shared_ptr<StepResultNET> net = dynamic_pointer_cast<StepResultNET>(result);
  if(!net)
  {
    throw StdException("Expected a StepResult for a NET: " + filename);
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
    PrintLine("");
    Indent();
    PrintLine("Validations");
    ValSteps& valis = net->GetValidations();
    for(auto& vali : valis)
    {
      CString validation;
      validation.Format("%d: %s %s",vali.m_number,vali.m_global ? "GLOB" : "LCAL",vali.m_validation.GetString());
      Indent();
      PrintLine(validation,false);
      PrintAtEnd(vali.m_ok ? "OK" : "ERROR");
    }
  }
  if(m_depth > ReportDepth::REPORT_STEPS)
  {
    Indent();
    PrintLine("Total step result:",false);
  }
  PrintAtEnd(net->GetTotalResult() ? "OK" : "ERROR");

}

void
TestReport::PrintStepResultSQL(CString p_directory, CString p_filename)
{
  CString filename = m_basepath + p_directory + "\\" + p_filename;
  shared_ptr<StepResult> result = shared_ptr<StepResult>(ReadStepResult(filename));
  if(!result)
  {
    throw StdException("Step result not found: " + filename);
  }
  shared_ptr<StepResultSQL> sql = dynamic_pointer_cast<StepResultSQL>(result);
  if(!sql)
  {
    throw StdException("Expected a StepResult for a SQL: " + filename);
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
    PrintLine("");
    Indent();
    PrintLine("Validations");
    ValSteps& valis = sql->GetValidations();
    for(auto& vali : valis)
    {
      CString validation;
      validation.Format("%d: %s %s",vali.m_number,vali.m_global ? "GLOB" : "LCAL",vali.m_validation.GetString());
      Indent();
      PrintLine(validation,false);
      PrintAtEnd(vali.m_ok ? "OK" : "ERROR");
    }
  }
  if(m_depth > ReportDepth::REPORT_STEPS)
  {
    Indent();
    PrintLine("Total step result:",false);
  }
  PrintAtEnd(sql->GetTotalResult() ? "OK" : "ERROR");
}

// Details of a result set
void
TestReport::PrintResultCMD(StepResultCMD* p_cmd)
{
  CString text;

  // Timing
  text.Format("Test run for %.4f seconds",p_cmd->GetTiming());
  Indent();
  PrintLine(text);
  // Return value
  text.Format("Test command return value: %d",p_cmd->GetReturnValue());
  Indent();
  PrintLine(text);
  // Standard output
  Indent();
  PrintLine("Command standard output returned:");
  Indent();
  PrintLine(p_cmd->GetStandardOutput());
  // Standard error
  if(!p_cmd->GetStandardError().IsEmpty())
  {
    Indent();
    PrintLine("Command standard error returned:");
    Indent();
    PrintLine(p_cmd->GetStandardError());
  }
}

void
TestReport::PrintResultNET(StepResultNET* p_net)
{
  CString text;

  // Timing
  text.Format("Test run for %.4f seconds",p_net->GetTiming());
  Indent();
  PrintLine(text);

  // HTTP Status
  text.Format("HTTP return status: %d",p_net->GetStatus());
  Indent();
  PrintLine(text);

  // 'invisible' OS error
  if(p_net->GetOSError())
  {
    Indent();
    PrintLine("OS error status: ",false);
    PrintLine(p_net->GetOSErrorString());
  }
  CString token = p_net->GetBearerToken();
  if(!token.IsEmpty())
  {
    Indent();
    PrintLine("Authorization bearer token: ",false);
    PrintLine(token);
  }
  Indent();
  PrintLine("Total HTTP response + body:");
  PrintLine(p_net->GetRawResponse());
}

void
TestReport::PrintResultSQL(StepResultSQL* p_sql)
{
  CString text;

  // Timing
  text.Format("Test run for %.4f seconds",p_sql->GetTiming());
  Indent();
  PrintLine(text);

  // Result
  text.Format("SQL Succeeded: %d Rows: %d Columns: %d"
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
    PrintLine("SQLSTATE: ");
    PrintLine(state);
  }

  // Native error
  CString error = p_sql->GetNativeStatus();
  if(!error.IsEmpty())
  {
    Indent();
    PrintLine("Native error: ");
    PrintLine(error);
  }

  // Results;
  Indent();
  PrintLine("Result data:");
  Indent();
  PrintLine("------------------------------ ",false);
  PrintLine("------------------------------");
  ResultMap& map = p_sql->GetResultMap();
  for(auto& row : map)
  {
    text.Format("%32.32s %s",row.first.GetString(),row.second.GetString());
    Indent();
    PrintLine(text);
  }

}
