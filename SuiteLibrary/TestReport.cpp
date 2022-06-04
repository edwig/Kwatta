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
#include <time.h>
#include <sys\timeb.h>

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

void
TestReport::PrintFooter()
{
  PrintLine();
  CString number;
  number.Format("%d",++m_page);

  int numspaces = m_pageWidth - number.GetLength();
  for(int index = 0;index < numspaces;++index)
  {
    fputc(' ',m_file);
  }
  fprintf(m_file,number);
  fputc('\n',m_file);
}

// Print line of spaces across the report
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
TestReport::PrintLastLine()
{
  PrintLine("**** End of report ****");
}

void
TestReport::PrintLine(CString p_text,bool p_next /*= true*/)
{
  CString extra;
  bool next(false);

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

//////////////////////////////////////////////////////////////////////////
//
// Service Routines
//

void
TestReport::PrintAllTestSuites()
{
  CString pattern(m_basepath + "*.xtest");

  // Read in all known files
  WIN32_FIND_DATA data;
  HANDLE find = nullptr;
  if((find = FindFirstFile(pattern,&data)) != INVALID_HANDLE_VALUE)
  {
    do
    {
      CString filename = CString(data.cFileName);
      try
      {
        TestSuite suite(m_basepath);
        suite.ReadFromXML(filename);
        PrintTestSuite(&suite);
      }
      catch (StdException& ex)
      {
        PrintLine("ERROR: "+ ex.GetErrorMessage());
      }
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
    PrintLine("SET: ",false);
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
  set.ReadFromXML(p_directory + p_filename);
  for(auto& run : set.GetTestRuns())
  {
    PrintLine("Run: ",false);
    PrintLine(run.m_name);
    PrintAtEnd(run.m_lastResult);

    // See if we must do it with a greater depth
    if(m_depth > ReportDepth::REPORT_STEPS)
    {
      PrintTestStep(p_directory,run.m_filename);
    }
  }
}

void
TestReport::PrintTestStep(CString p_directory,CString p_filename)
{
  TestStep* step = ReadTestStep(m_basepath + p_directory + p_filename);
  if(!step)
  {
    throw StdException("Teststep not found: " + p_directory + p_filename);
  }
}