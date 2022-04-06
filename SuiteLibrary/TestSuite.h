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
// This File   : TestSuite.h
// What it does: The complete test suite. All tests and teststeps. The main suite level!
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <XMLMessage.h>
#include <vector>
#include <map>

typedef struct _globStep
{
  CString m_name;
  CString m_filename;
}
GlobStep;

using GSteps = std::vector<GlobStep>;

typedef struct _globVali
{
  CString m_name;
  CString m_filename;
}
GlobValidation;

using GVali = std::vector<GlobValidation>;

typedef struct _test
{
  CString   m_directory;
  CString   m_name;
  CString   m_filename;
  bool      m_active { true };
  CString   m_lastResult;
}
Test;

using Tests = std::map<int,Test>;


//////////////////////////////////////////////////////////////////////////
//
// The testsuite class
//
//////////////////////////////////////////////////////////////////////////

class TestSuite
{
public:
  TestSuite(CString p_basedirectory);

  // Interface with the file system
  void      ReadFromXML(CString p_filename);  // Throws in case of an error
  bool      WriteToXML(bool p_force = false);

  // SETTERS
  void      SetDescription(CString p_description);
  void      SetActive(CString p_testname,bool p_active);
  void      SetFilename(CString p_filename);

  // GETTERS
  bool      GetIsChanged()         { return m_changed;       };
  CString   GetName()              { return m_name;          };
  CString   GetVersion()           { return m_version;       };
  CString   GetFilename()          { return m_filename;      };
  CString   GetBaseDirectory()     { return m_baseDirectory; };
  CString   GetDescription()       { return m_description;   }
  GSteps&   GetGlobalSteps()       { return m_globalSteps;   };
  GVali&    GetGlobalValidations() { return m_globalValidations; };
  Tests&    GetAllTests()          { return m_tests;         };

  void      AddGlobalStep(GlobStep& p_step);
  void      AddGlobalValidation(GlobValidation& p_validation);
  void      AddTest(Test& p_test);

  Test*     FindTest(CString p_testname);
  bool      RemoveTest(CString p_testname);

  bool      ChangeTestDirectory(CString p_testname, CString p_directory);
  bool      ChangeTestFilename (CString p_testname, CString p_filename);
  bool      ChangeTestTestname (CString p_testname, CString p_name);

private:
  void      CheckFilename(CString p_filename);
  CString   FindElementString (XMLMessage& p_msg,XMLElement* p_start,CString p_name);
  bool      FindElementBoolean(XMLMessage& p_msg,XMLElement* p_start,CString p_name);

  // Primary identity
  CString   m_name;
  CString   m_version;
  CString   m_filename;
  CString   m_baseDirectory;
  CString   m_description;

  // Global steps in "<basedirectory>\Steps"
  GSteps    m_globalSteps;
  // Global validations in "<basedirectory>\Validations"
  GVali     m_globalValidations;
  // All tests in "<basedirectory>
  Tests     m_tests;
  // Mutations
  bool      m_changed { false };
};
