#include "stdafx.h"
#include "TestSuite.h"
#include "EnsureFile.h"
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
  EnsureFile ensure;
  m_filename = m_baseDirectory + ensure.FilenamePart(p_filename);

  XMLMessage msg;


  // Load the file (if any)
  if(msg.LoadFile(m_filename) == false)
  {
    throw StdException("Could not load the XML file: " + m_filename);
  }

  // CHeck for XML error
  if(msg.GetInternalError() != XmlError::XE_NoError)
  {
    CString error;
    error.Format("Internal XML error in XTEST file [%d] %s",msg.GetInternalError(),msg.GetInternalErrorString().GetString());
    throw StdException(error);
  }

  // Check that it is our message type
  if(msg.GetRootNodeName().Compare("TestSuite"))
  {
    throw StdException("XTEST file is not a test-suite definition: " + m_filename);
  }

  // Load identity
  XMLElement* root = msg.GetRoot();
  m_version     = FindElementString(msg,root,"Version");
  m_name        = FindElementString(msg,root,"Name");
  m_description = FindElementString(msg,root,"Description");

  // Load all global steps
  XMLElement* steps = msg.FindElement(root,"Steps",false);
  if(steps)
  {
    XMLElement* step = msg.FindElement(steps,"Step",false);

    while(step)
    {
      GlobStep gstep;
      gstep.m_name     = FindElementString(msg,step,"Name");
      gstep.m_filename = FindElementString(msg,step,"File");
      m_globalSteps.push_back(gstep);

      step = msg.GetElementSibling(step);
    }
  }

  // Load all global validations
  XMLElement* validations = msg.FindElement(root,"Validations",false);
  if(validations)
  {
    XMLElement* validation = msg.FindElement(validations,"Validation",false);
    while(validation)
    {
      GlobValidation vali;
      vali.m_name     = FindElementString(msg,validation,"Name");
      vali.m_filename = FindElementString(msg,validation,"File");
      m_globalValidations.push_back(vali);

      validation = msg.GetElementSibling(validation);
    }
  }

  // Load all tests
  int num = 1;
  XMLElement* tests = msg.FindElement(root,"Tests",false);
  if(tests)
  {
    XMLElement* test = msg.FindElement(tests,"Test",false);
    while(test)
    {
      Test tst;
      tst.m_name       = FindElementString (msg,test,"Name");
      tst.m_directory  = FindElementString (msg,test,"Dir");
      tst.m_filename   = FindElementString (msg,test,"File");
      tst.m_active     = FindElementBoolean(msg,test,"Run");
      tst.m_lastResult = FindElementString (msg,test,"Result");
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

  msg.SetRootNodeName("TestSuite");

  XMLElement* root = msg.GetRoot();

  // Global elements
  msg.SetElement(root,"Version",m_version);
  msg.SetElement(root,"Name",   m_name);
  msg.SetElement(root,"Description",XDT_String|XDT_CDATA,m_description);

  // Add all global steps
  XMLElement* steps = msg.AddElement(root,"Steps",XDT_String,"");
  for(auto& step : m_globalSteps)
  {
    XMLElement* xstep = msg.AddElement(steps,"Step",XDT_String,"");
    msg.SetElement(xstep,"Name",step.m_name);
    msg.SetElement(xstep,"File",step.m_filename);
  }

  // Add all global validations
  XMLElement* validations = msg.AddElement(root,"Validations",XDT_String,"");
  for(auto& vali : m_globalValidations)
  {
    XMLElement* xvali = msg.AddElement(validations,"Validation",XDT_String,"");
    msg.SetElement(xvali,"Name",vali.m_name);
    msg.SetElement(xvali,"File",vali.m_filename);
  }

  // Add all TESTS
  XMLElement* tests = msg.AddElement(root,"Tests",XDT_String,"");
  for(auto& test : m_tests)
  {
    XMLElement* xtest = msg.AddElement(tests,"Test",XDT_String,"");
    msg.SetElement(xtest,"Dir",   test.second.m_directory);
    msg.SetElement(xtest,"File",  test.second.m_filename);
    msg.SetElement(xtest,"Name",  test.second.m_name);
    msg.SetElement(xtest,"Run",   test.second.m_active);
    msg.SetElement(xtest,"Result",test.second.m_lastResult);
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

//////////////////////////////////////////////////////////////////////////
//
// PRIVATE
//
//////////////////////////////////////////////////////////////////////////

void
TestSuite::CheckFilename(CString p_filename)
{
  // Split of only the extension
  EnsureFile ensure;
  CString extension = ensure.ExtensionPart(p_filename);
  if(extension.CompareNoCase(".xtest") != 0)
  {
    throw StdException("A test suite XML definition file must be saved as a *.XTEST");
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
  return "";
}

bool
TestSuite::FindElementBoolean(XMLMessage& p_msg,XMLElement* p_start,CString p_name)
{
  XMLElement* elem = p_msg.FindElement(p_start,p_name,false);
  if (elem)
  {
    if(elem->GetValue().CompareNoCase("true") == 0 ||
       elem->GetValue().CompareNoCase("yes") == 0 ||
       elem->GetValue().Compare("1") == 0)
    {
      return true;
    }
  }
  return false;
}

