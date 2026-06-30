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
// This File   : TestSet.h
// What it does: One (1) step in a test set
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <vector>
#include "TestStep.h"

class XMLMessage;
class XMLElement;

// All validations of a test run
typedef struct  
{
  XString m_filename;
  bool    m_global;
  XString m_name;
}
TRValidation;

using TSValSet = std::vector<TRValidation>;

// Definition of a test run
typedef   struct
{
  XString   m_filename;
  bool      m_global;
  XString   m_name;
  TSValSet  m_validations;
  XString   m_lastResult;
}
TRun;

using TRunSet = std::vector<TRun>;

class TestSet
{
public:
  TestSet() = default;

  // Interface with the file system
  void      ReadFromXML(XString p_filename);  // Throws in case of an error
  bool      WriteToXML ();

  // GETTERS
  XString   GetName()          { return m_name;          }
  XString   GetDocumentation() { return m_documentation; }
  TRunSet&  GetTestRuns()      { return m_testruns;      }
  TRun&     GetTestRun(int p_run);
  bool      GetTotalResult();
  TRun*     GetRun(XString p_filename);
  TSValSet* GetValidationsByName(XString p_name);
  TSValSet* GetValidationsByFile(XString p_filename);

  // SETTERS
  void      SetName(XString p_name)         { m_name           = p_name; }
  void      SetDocumentation(XString p_doc) { m_documentation  = p_doc;  }
  void      SetFilename(XString p_file)     { m_filename       = p_file; }

  // FUNCTIONS
  TRun*     AddTestStep(XString p_stepname,XString p_filename,bool p_global);
  void      DeleteStep(int p_step);
  int       CopyStep(XString p_directory,XString p_filename,XString p_name);

protected:
  void      CheckFilename(XString p_filename);
  XString   FindElementString(XMLMessage& p_msg,XMLElement* p_start,XString p_name);

  XString   m_filename;
  XString   m_name;
  XString   m_documentation;
  TRunSet   m_testruns;
};
