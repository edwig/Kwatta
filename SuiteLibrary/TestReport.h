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
// This File   : TestReport.h
// What it does: Create a report of all tests and teststeps
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "TestSet.h"
#include <WinFile.h>

enum class ReportDepth
{
  REPORT_TESTS      // Only report status of each test directory
 ,REPORT_STEPS      // Report all steps
 ,REPORT_VALIDATE   // Report all steps + validations
 ,REPORT_FULL       // Report all steps + validations + output
};

#define REPORT_PAGE_LENGTH  66
#define REPORT_PAGE_WIDTH   80

#define REPORT_MIN_PAGE_LENGTH   40
#define REPORT_MAX_PAGE_LENGTH  256
#define REPORT_MIN_PAGE_WIDTH    40
#define REPORT_MAX_PAGE_WIDTH   200

class TestSuite;
class TestStepCMD;
class TestStepNET;
class TestStepSQL;
class StepResultCMD;
class StepResultNET;
class StepResultSQL;

class TestReport
{
public:
  TestReport(XString      p_basepath
            ,XString      p_filename
            ,XString      p_name
            ,ReportDepth  p_depth);

  bool StartReport();
  void SetPageSize(int p_lines);
  void SetPageWidth(int p_width);

private:
  void  Reset();

  // SERVICE ROUTINES
  void  PrintTiming();
  bool  OpenReport();
  bool  CloseReport();
  void  PrintHeader();
  void  PrintFooter();
  void  PrintLine(XString p_text,bool p_next = true);
  void  PrintLine(XString p_part1,const TCHAR* p_part2);
  void  PrintAtEnd(XString p_text);
  void  PrintLastLine();
  void  PrintLine();
  void  Indent();
  // PRINTING THE TESTS
  void  PrintAllTestSuites();
  void  PrintTestSuite(TestSuite* p_suite);
  void  PrintTestSet (XString p_directory,XString p_filename);
  void  PrintTestStep(XString p_directory,XString p_filename);

  void  PrintTestStepCMD(TestStepCMD* p_cmd,XString p_directory,XString p_filename);
  void  PrintTestStepNET(TestStepNET* p_net,XString p_directory,XString p_filename);
  void  PrintTestStepSQL(TestStepSQL* p_sql,XString p_directory,XString p_filename);

  void  PrintStepCMDDetails(TestStepCMD* p_cmd);
  void  PrintStepNETDetails(TestStepNET* p_cmd);
  void  PrintStepSQLDetails(TestStepSQL* p_cmd);

  void  PrintStepResultCMD(XString p_directory,XString p_filename);
  void  PrintStepResultNET(XString p_directory,XString p_filename);
  void  PrintStepResultSQL(XString p_directory,XString p_filename);
  // Details of a result set
  void  PrintResultCMD(StepResultCMD* p_cmd);
  void  PrintResultNET(StepResultNET* p_net);
  void  PrintResultSQL(StepResultSQL* p_sql);

  // Data
  ReportDepth m_depth;
  XString     m_basepath;
  XString     m_fileName;
  XString     m_name;
  XString     m_timing;
  WinFile     m_file;
  int         m_page      { 0 };
  int         m_line      { 0 };
  int         m_pos       { 0 };
  int         m_pageSize  { REPORT_PAGE_LENGTH };
  int         m_pageWidth { REPORT_PAGE_WIDTH  };
};
