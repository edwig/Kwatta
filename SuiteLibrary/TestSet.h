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
  CString m_filename;
  bool    m_global;
  CString m_name;
}
TRValidation;

using TSValSet = std::vector<TRValidation>;

// Definition of a test run
typedef   struct
{
  CString   m_filename;
  bool      m_global;
  CString   m_name;
  TSValSet  m_validations;
  CString   m_lastResult;
}
TRun;

using TRunSet = std::vector<TRun>;

class TestSet
{
public:
  TestSet() = default;

  // Interface with the file system
  void      ReadFromXML(CString p_filename);  // Throws in case of an error
  bool      WriteToXML ();

  // GETTERS
  CString   GetName()          { return m_name;          }
  CString   GetDocumentation() { return m_documentation; }
  TRunSet&  GetTestRuns()      { return m_testruns;      }
  TRun&     GetTestRun(int p_run);
  bool      GetTotalResult();
  TRun*     GetRun(CString p_filename);
  TSValSet* GetValidations(CString p_filename);

  // SETTERS
  void      SetName(CString p_name)         { m_name           = p_name; }
  void      SetDocumentation(CString p_doc) { m_documentation  = p_doc;  }
  void      SetFilename(CString p_file)     { m_filename       = p_file; }

  // FUNCTIONS
  TRun*     AddTestStep(CString p_stepname,CString p_filename);
  void      DeleteStep(int p_step);
  int       CopyStep(CString p_directory,CString p_filename,CString p_name);

protected:
  void      CheckFilename(CString p_filename);
  CString   FindElementString(XMLMessage& p_msg,XMLElement* p_start,CString p_name);

  CString   m_filename;
  CString   m_name;
  CString   m_documentation;
  TRunSet   m_testruns;
};
