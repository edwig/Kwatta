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

using FileList = std::vector<XString>;

class ResetAll
{
public:
  ResetAll(XString p_baseDirectory,XString p_testDirectory,XString p_step);

  bool      Reset(bool p_interface);
  XString   LastError() { return m_error; }

private:
  void      ResetSuite();
  void      ResetTestSet();
  void      ResetTestStep();
  void      DeleteList();

  XString   m_baseDirectory;
  XString   m_testDirectory;
  XString   m_testStep;
  bool      m_interface { false };
  XString   m_error;
  FileList  m_toBeDeleted;
};

