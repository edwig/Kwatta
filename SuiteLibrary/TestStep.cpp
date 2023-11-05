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
// This File   : TestStep.cpp
// What it does: One (1) test step (main class)
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TestStep.h"
#include "TestStepCMD.h"
#include "TestStepNET.h"
#include "TestStepSQL.h"
#include "XMLMessage.h"
#include "StdException.h"
#include "ExtraExtensions.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

TestStep::TestStep()
{
}

TestStep::~TestStep()
{
}

TestStep* ReadTestStep(CString p_filename)
{
  WinFile file(p_filename);
  CString extension = file.GetFilenamePartExtension();

  if(extension.CompareNoCase(EXTENSION_TESTSTEP_CMD) == 0)
  {
    TestStepCMD* result = new TestStepCMD();
    try
    {
      result->ReadFromXML(p_filename);
    }
    catch(StdException& ex)
    {
      delete result;
      throw ex;
    }
    return result;
  }
  if(extension.CompareNoCase(EXTENSION_TESTSTEP_NET) == 0)
  {
    TestStepNET* result = new TestStepNET();
    try
    {
      result->ReadFromXML(p_filename);
    }
    catch(StdException& ex)
    {
      delete result;
      throw ex;
    }
    return result;
  }
  if(extension.CompareNoCase(EXTENSION_TESTSTEP_SQL) == 0)
  {
    TestStepSQL* result = new TestStepSQL();
    try
    {
      result->ReadFromXML(p_filename);
    }
    catch(StdException& ex)
    {
      delete result;
      throw ex;
    }
    return result;
  }
  return nullptr;
}

// RE-Calculate the effective strings, returning the number of unbound parameters
int
TestStep::EffectiveReplacements(Parameters* p_parameters,bool p_forDisplay)
{
  int unbound = 0;

  unbound += p_parameters->Replace(m_maxExecution, m_effectiveMaxEcecution, p_forDisplay);
  unbound += p_parameters->Replace(m_waitBeforeRun,m_effectiveWaitBeforeRun,p_forDisplay);
  unbound += p_parameters->Replace(m_waitAfterRun, m_effectiveWaitAfterRun, p_forDisplay);

  return unbound;
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

    // Find Parameters
  XMLElement* param = msg.FindElement(root,"Parameters",false);
  if(param)
  {
    m_killOnTimeout = FindElementBoolean(msg,param,"KillOnTimeout");
    m_maxExecution  = FindElementString (msg,param,"MaxExecutionTime");
    m_waitBeforeRun = FindElementString (msg,param,"WaitBeforeRun");
    m_waitAfterRun  = FindElementString (msg,param,"WaitAfterRun");
  }
}

bool
TestStep::WriteToXML(XMLMessage& msg,CString p_filename)
{
  msg.SetRootNodeName("TestStep");

  XMLElement* root = msg.GetRoot();
  msg.AddElement(root, "Name",         XDT_String|XDT_CDATA,m_name);
  msg.AddElement(root, "Documentation",XDT_String|XDT_CDATA,m_documentation);

  XMLElement* parameters = msg.AddElement(root,"Parameters",XDT_String,"");
  msg.SetElement(parameters,"KillOnTimeout",   m_killOnTimeout);
  msg.SetElement(parameters,"MaxExecutionTime",m_maxExecution);
  msg.SetElement(parameters,"WaitBeforeRun",   m_waitBeforeRun);
  msg.SetElement(parameters,"WaitAfterRun",    m_waitAfterRun);

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
