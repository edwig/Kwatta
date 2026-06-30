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
  XString m_name;
  XString m_filename;
}
GlobStep;

using GSteps = std::vector<GlobStep>;

typedef struct _globVali
{
  XString m_name;
  XString m_filename;
}
GlobValidation;

using GVali = std::vector<GlobValidation>;

typedef struct _test
{
  XString   m_directory;
  XString   m_name;
  XString   m_filename;
  bool      m_active { true };
  XString   m_lastResult;
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
  TestSuite(XString p_basedirectory);

  // Interface with the file system
  void      ReadFromXML(XString p_filename);  // Throws in case of an error
  bool      WriteToXML(bool p_force = false);

  // SETTERS
  void      SetDescription(XString p_description);
  void      SetActive(XString p_testname,bool p_active);
  void      SetFilename(XString p_filename);

  // GETTERS
  bool      GetIsChanged()         { return m_changed;       };
  XString   GetName()              { return m_name;          };
  XString   GetVersion()           { return m_version;       };
  XString   GetFilename()          { return m_filename;      };
  XString   GetBaseDirectory()     { return m_baseDirectory; };
  XString   GetDescription()       { return m_description;   }
  GSteps&   GetGlobalSteps()       { return m_globalSteps;   };
  GVali&    GetGlobalValidations() { return m_globalValidations; };
  Tests&    GetAllTests()          { return m_tests;         };

  void      AddGlobalStep(GlobStep& p_step);
  void      AddGlobalValidation(GlobValidation& p_validation);
  void      AddTest(Test& p_test);

  Test*     FindTest(XString p_testname);
  bool      RemoveTest(XString p_testname);

  bool      ChangeTestDirectory(XString p_testname, XString p_directory);
  bool      ChangeTestFilename (XString p_testname, XString p_filename);
  bool      ChangeTestTestname (XString p_testname, XString p_name);

private:
  void      CheckFilename(XString p_filename);
  XString   FindElementString (XMLMessage& p_msg,XMLElement* p_start,XString p_name);
  bool      FindElementBoolean(XMLMessage& p_msg,XMLElement* p_start,XString p_name);

  // Primary identity
  XString   m_name;
  XString   m_version;
  XString   m_filename;
  XString   m_baseDirectory;
  XString   m_description;

  // Global steps in "<basedirectory>\Steps"
  GSteps    m_globalSteps;
  // Global validations in "<basedirectory>\Validations"
  GVali     m_globalValidations;
  // All tests in "<basedirectory>
  Tests     m_tests;
  // Mutations
  bool      m_changed { false };
};
