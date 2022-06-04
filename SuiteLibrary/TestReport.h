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

enum class ReportDepth
{
  REPORT_TESTS      // Only report status of each test directory
 ,REPORT_STEPS      // Report all steps
 ,REPORT_VALIDATE   // Report all steps + validations
 ,REPORT_FULL       // Report all steps + validations + output
};

#define REPORT_PAGE_LENGTH 100
#define REPORT_PAGE_WIDTH   80

#define REPORT_MIN_PAGE_LENGTH   40
#define REPORT_MAX_PAGE_LENGTH  256
#define REPORT_MIN_PAGE_WIDTH    40
#define REPORT_MAX_PAGE_WIDTH   200

class TestSuite;

class TestReport
{
public:
  TestReport(CString      p_basepath
            ,CString      p_filename
            ,CString      p_name
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
  void  PrintLine(CString p_text,bool p_next = true);
  void  PrintAtEnd(CString p_text);
  void  PrintLastLine();
  void  PrintLine();
  // PRINTING THE TESTS
  void  PrintAllTestSuites();
  void  PrintTestSuite(TestSuite* p_suite);
  void  PrintTestSet (CString p_directory,CString p_filename);
  void  PrintTestStep(CString p_directory,CString p_filename);

  ReportDepth m_depth;
  CString     m_basepath;
  CString     m_fileName;
  CString     m_name;
  CString     m_timing;
  FILE*       m_file      { nullptr };
  int         m_page      { 0 };
  int         m_line      { 0 };
  int         m_pos       { 0 };
  int         m_pageSize  { REPORT_PAGE_LENGTH };
  int         m_pageWidth { REPORT_PAGE_WIDTH  };
};
