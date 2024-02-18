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
// This File   : TestSuite.cpp
// What it does: The complete test suite. All tests and teststeps. The main suite level!
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TestSuite.h"
#include "StdException.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

TestSuite::TestSuite(CString p_basedirectory)
          :m_baseDirectory(p_basedirectory)
{
}

void
TestSuite::ReadFromXML(CString p_filename)
{
  // Check for correct extension
  CheckFilename(p_filename);

  // Getting our correct relative pathname
  WinFile file(p_filename);
  m_filename = m_baseDirectory + file.GetFilenamePartFilename();

  XMLMessage msg;


  // Load the file (if any)
  if(msg.LoadFile(m_filename) == false)
  {
    throw StdException(_T("Could not load the XML file: ") + m_filename);
  }

  // CHeck for XML error
  if(msg.GetInternalError() != XmlError::XE_NoError)
  {
    CString error;
    error.Format(_T("Internal XML error in XTEST file [%d] %s"),msg.GetInternalError(),msg.GetInternalErrorString().GetString());
    throw StdException(error);
  }

  // Check that it is our message type
  if(msg.GetRootNodeName().Compare(_T("TestSuite")))
  {
    throw StdException(_T("XTEST file is not a test-suite definition: ") + m_filename);
  }

  // Load identity
  XMLElement* root = msg.GetRoot();
  m_version     = FindElementString(msg,root,_T("Version"));
  m_name        = FindElementString(msg,root,_T("Name"));
  m_description = FindElementString(msg,root,_T("Description"));

  // Load all global steps
  XMLElement* steps = msg.FindElement(root,_T("Steps"),false);
  if(steps)
  {
    XMLElement* step = msg.FindElement(steps,_T("Step"),false);

    while(step)
    {
      GlobStep gstep;
      gstep.m_name     = FindElementString(msg,step,_T("Name"));
      gstep.m_filename = FindElementString(msg,step,_T("File"));
      m_globalSteps.push_back(gstep);

      step = msg.GetElementSibling(step);
    }
  }

  // Load all global validations
  XMLElement* validations = msg.FindElement(root,_T("Validations"),false);
  if(validations)
  {
    XMLElement* validation = msg.FindElement(validations,_T("Validation"),false);
    while(validation)
    {
      GlobValidation vali;
      vali.m_name     = FindElementString(msg,validation,_T("Name"));
      vali.m_filename = FindElementString(msg,validation,_T("File"));
      m_globalValidations.push_back(vali);

      validation = msg.GetElementSibling(validation);
    }
  }

  // Load all tests
  int num = 1;
  XMLElement* tests = msg.FindElement(root,_T("Tests"),false);
  if(tests)
  {
    XMLElement* test = msg.FindElement(tests,_T("Test"),false);
    while(test)
    {
      Test tst;
      tst.m_name       = FindElementString (msg,test,_T("Name"));
      tst.m_directory  = FindElementString (msg,test,_T("Dir"));
      tst.m_filename   = FindElementString (msg,test,_T("File"));
      tst.m_active     = FindElementBoolean(msg,test,_T("Run"));
      tst.m_lastResult = FindElementString (msg,test,_T("Result"));
      m_tests.insert(std::make_pair(num++,tst));

      test = msg.GetElementSibling(test);
    }
  }

  m_changed = false;
}

bool
TestSuite::WriteToXML(bool p_force /*=false*/)
{
  if(!p_force && m_changed == false)
  {
    return true;
  }

  XMLMessage msg;

  msg.SetRootNodeName(_T("TestSuite"));

  XMLElement* root = msg.GetRoot();

  // Global elements
  msg.SetElement(root,_T("Version"),m_version);
  msg.SetElement(root,_T("Name"),   m_name);
  msg.SetElement(root,_T("Description"),XDT_String|XDT_CDATA,m_description);

  // Add all global steps
  XMLElement* steps = msg.AddElement(root,_T("Steps"),XDT_String,_T(""));
  for(auto& step : m_globalSteps)
  {
    XMLElement* xstep = msg.AddElement(steps,_T("Step"),XDT_String,_T(""));
    msg.SetElement(xstep,_T("Name"),step.m_name);
    msg.SetElement(xstep,_T("File"),step.m_filename);
  }

  // Add all global validations
  XMLElement* validations = msg.AddElement(root,_T("Validations"),XDT_String,_T(""));
  for(auto& vali : m_globalValidations)
  {
    XMLElement* xvali = msg.AddElement(validations,_T("Validation"),XDT_String,_T(""));
    msg.SetElement(xvali,_T("Name"),vali.m_name);
    msg.SetElement(xvali,_T("File"),vali.m_filename);
  }

  // Add all TESTS
  XMLElement* tests = msg.AddElement(root,_T("Tests"),XDT_String,_T(""));
  for(auto& test : m_tests)
  {
    XMLElement* xtest = msg.AddElement(tests,_T("Test"),XDT_String,_T(""));
    msg.SetElement(xtest,_T("Dir"),   test.second.m_directory);
    msg.SetElement(xtest,_T("File"),  test.second.m_filename);
    msg.SetElement(xtest,_T("Name"),  test.second.m_name);
    msg.SetElement(xtest,_T("Run"),   test.second.m_active);
    msg.SetElement(xtest,_T("Result"),test.second.m_lastResult);
  }

  // Now save it
  if (msg.SaveFile(m_filename))
  {
    m_changed = false;
    return true;
  }
  return false;
}

void
TestSuite::AddGlobalStep(GlobStep& p_step)
{
  m_globalSteps.push_back(p_step);
  m_changed = true;
}

void
TestSuite::AddGlobalValidation(GlobValidation& p_validation)
{
  m_globalValidations.push_back(p_validation);
  m_changed = true;
}

void
TestSuite::AddTest(Test& p_test)
{
  int num = 1 + (int)m_tests.size();
  m_tests.insert(std::make_pair(num,p_test));
  m_changed = true;
}

void
TestSuite::SetDescription(CString p_description)
{ 
  m_description = p_description; 
  m_changed = true;
}

Test* 
TestSuite::FindTest(CString p_testname)
{
  for(Tests::iterator it = m_tests.begin();it != m_tests.end();++it)
  {
    if(it->second.m_name.CompareNoCase(p_testname) == 0)
    {
      return &it->second;
    }
  }
  return nullptr;
}

bool
TestSuite::RemoveTest(CString p_testname)
{
  for(Tests::iterator it = m_tests.begin();it != m_tests.end();++it)
  {
    if(it->second.m_name.CompareNoCase(p_testname) == 0)
    {
      m_tests.erase(it);
      m_changed = true;
      return true;
    }
  }
  return false;
}

void
TestSuite::SetActive(CString p_testname,bool p_active)
{
  Test* test = FindTest(p_testname);
  if(test)
  {
    test->m_active = p_active;
  }
}

void
TestSuite::SetFilename(CString p_filename)
{
  m_filename = p_filename;
}

bool
TestSuite::ChangeTestDirectory(CString p_testname,CString p_directory)
{
  Test* test = FindTest(p_testname);
  if(test)
  {
    test->m_directory = p_directory;
    m_changed = true;
    return true;
  }
  return false;
}

bool
TestSuite::ChangeTestFilename(CString p_testname,CString p_filename)
{
  Test* test = FindTest(p_testname);
  if(test)
  {
    test->m_filename = p_filename;
    m_changed = true;
    return true;
  }
  return false;
}

bool
TestSuite::ChangeTestTestname(CString p_testname,CString p_name)
{
  Test* test = FindTest(p_testname);
  if(test)
  {
    test->m_name = p_name;
    m_changed = true;
    return true;
  }
  return false;
}

//////////////////////////////////////////////////////////////////////////
//
// PRIVATE
//
//////////////////////////////////////////////////////////////////////////

void
TestSuite::CheckFilename(CString p_filename)
{
  // Split of only the extension
  WinFile file(p_filename);
  CString extension = file.GetFilenamePartExtension();
  if(extension.CompareNoCase(_T(".xtest")) != 0)
  {
    throw StdException(_T("A test suite XML definition file must be saved as a *.XTEST"));
  }
}

CString
TestSuite::FindElementString(XMLMessage& p_msg,XMLElement* p_start,CString p_name)
{
  XMLElement* elem = p_msg.FindElement(p_start,p_name,false);
  if (elem)
  {
    return elem->GetValue();
  }
  return _T("");
}

bool
TestSuite::FindElementBoolean(XMLMessage& p_msg,XMLElement* p_start,CString p_name)
{
  XMLElement* elem = p_msg.FindElement(p_start,p_name,false);
  if (elem)
  {
    if(elem->GetValue().CompareNoCase(_T("true")) == 0 ||
       elem->GetValue().CompareNoCase(_T("yes")) == 0 ||
       elem->GetValue().Compare(_T("1")) == 0)
    {
      return true;
    }
  }
  return false;
}

