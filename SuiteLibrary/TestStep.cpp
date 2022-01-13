//////////////////////////////////////////////////////////////////////////
//
// TEST-STEP
//
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TestStep.h"
#include "TestStepCL.h"
#include "TestStepIN.h"
#include "EnsureFile.h"
#include "XMLMessage.h"
#include "StdException.h"
#include "ExtraExtensions.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

TestStep* ReadTestStep(CString p_filename)
{
  EnsureFile ensure;
  CString extension = ensure.ExtensionPart(p_filename);

  if(extension.CompareNoCase(EXTENSION_TESTSTEP_CL) == 0)
  {
    TestStepCL* result = new TestStepCL();
    result->ReadFromXML(p_filename);
    return result;
  }
  if(extension.CompareNoCase(EXTENSION_TESTSTEP_IN) == 0)
  {
    TestStepIN* result = new TestStepIN();
    result->ReadFromXML(p_filename);
    return result;
  }
  return nullptr;
}

// RE-Calculate the effective strings, returning the number of unbound parameters
int
TestStep::EffectiveReplacements(Parameters* /*p_parameters*/,bool /*p_forDisplay*/)
{
  // Nothing for now
  return 0;
}

//////////////////////////////////////////////////////////////////////////
//
// PROTECTED
//
//////////////////////////////////////////////////////////////////////////

void
TestStep::ReadFromXML(XMLMessage& msg,CString p_filename)
{
  // Check that we have a *.xrun extension
  CheckFilename(p_filename);

  // Load the file (if any)
  if(msg.LoadFile(p_filename) == false)
  {
    throw StdException("Could not load the XML file: " + p_filename);
  }

  // CHeck for XML error
  if(msg.GetInternalError() != XmlError::XE_NoError)
  {
    CString error;
    error.Format("Internal XML error in XRUN file [%d] %s",msg.GetInternalError(),msg.GetInternalErrorString().GetString());
    throw StdException(error);
  }

  // Check that it is our message type
  if(msg.GetRootNodeName().Compare("TestStep"))
  {
    throw StdException("XRUN file is not a TestStep definition: " + p_filename);
  }

  // Find names
  XMLElement* root = msg.GetRoot();
  m_name           = FindElementString(msg,root,"Name");
  m_documentation  = FindElementString(msg,root,"Documentation");

}

bool
TestStep::WriteToXML(XMLMessage& msg,CString p_filename)
{
  msg.SetRootNodeName("TestStep");

  XMLElement* root = msg.GetRoot();
  msg.AddElement(root, "Name",         XDT_String|XDT_CDATA,m_name);
  msg.AddElement(root, "Documentation",XDT_String|XDT_CDATA,m_documentation);

  return true;
}

CString
TestStep::FindElementString(XMLMessage& p_msg, XMLElement* p_start, CString p_name)
{
  XMLElement* elem = p_msg.FindElement(p_start, p_name);
  if (elem)
  {
    return elem->GetValue();
  }
  return "";
}

int
TestStep::FindElementInteger(XMLMessage& p_msg, XMLElement* p_start, CString p_name)
{
  XMLElement* elem = p_msg.FindElement(p_start, p_name);
  if (elem)
  {
    return atoi(elem->GetValue());
  }
  return 0;
}

bool
TestStep::FindElementBoolean(XMLMessage& p_msg, XMLElement* p_start, CString p_name)
{
  XMLElement* elem = p_msg.FindElement(p_start, p_name);
  if (elem)
  {
    if(elem->GetValue().CompareNoCase("true") == 0 ||
       elem->GetValue().CompareNoCase("yes")  == 0 ||
       elem->GetValue().Compare("1")          == 0)
    {
       return true;
    }
  }
  return false;
}

