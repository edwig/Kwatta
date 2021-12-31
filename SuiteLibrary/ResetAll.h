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
// This File   : ResetAll.h
// What it does: Reset all test results (suite, tests, steps)
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <vector>

using FileList = std::vector<CString>;

class ResetAll
{
public:
  ResetAll(CString p_baseDirectory,CString p_testDirectory,CString p_step);

  bool      Reset(bool p_interface);
  CString   LastError() { return m_error; }

private:
  void      ResetSuite();
  void      ResetTestSet();
  void      ResetTestStep();
  void      DeleteList();

  CString   m_baseDirectory;
  CString   m_testDirectory;
  CString   m_testStep;
  bool      m_interface { false };
  CString   m_error;
  FileList  m_toBeDeleted;
};

