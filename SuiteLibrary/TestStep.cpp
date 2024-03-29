﻿///////////////////////////////////////////////////////////////////////////////////////////////////////
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
  unbound += p_parameters->Replace(m_scriptToRun,  m_effectiveScriptToRun,  p_forDisplay);
  unbound += p_parameters->Replace(m_statusOK,     m_effectiveStatusOK,     p_forDisplay);

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
    throw StdException(_T("Could not load the XML file: ") + p_filename);
  }

  // CHeck for XML error
  if(msg.GetInternalError() != XmlError::XE_NoError)
  {
    CString error;
    error.Format(_T("Internal XML error in XRUN file [%d] %s"),msg.GetInternalError(),msg.GetInternalErrorString().GetString());
    throw StdException(error);
  }

  // Check that it is our message type
  if(msg.GetRootNodeName().Compare(_T("TestStep")))
  {
    throw StdException(_T("XRUN file is not a TestStep definition: ") + p_filename);
  }

  // Find names
  XMLElement* root = msg.GetRoot();
  m_name           = FindElementString(msg,root,_T("Name"));
  m_documentation  = FindElementString(msg,root,_T("Documentation"));

  // Find Parameters
  XMLElement* param = msg.FindElement(root,_T("Parameters"),false);
  if(param)
  {
    m_killOnTimeout = FindElementBoolean(msg,param,_T("KillOnTimeout"));
    m_maxExecution  = FindElementString (msg,param,_T("MaxExecutionTime"));
    m_waitBeforeRun = FindElementString (msg,param,_T("WaitBeforeRun"));
    m_waitAfterRun  = FindElementString (msg,param,_T("WaitAfterRun"));
  }

  // Find our post-script
  XMLElement* script = msg.FindElement(root,_T("Script"),false);
  if(script)
  {
    m_scriptToRun  = FindElementString(msg,script,_T("QLScript"));
    m_statusOK     = FindElementString(msg,script,_T("StatusOK"));
    m_scriptStatus = (ScriptStatus) FindElementInteger(msg,script,_T("ScriptResult"));
  }
}

bool
TestStep::WriteToXML(XMLMessage& msg,CString p_filename)
{
  msg.SetRootNodeName(_T("TestStep"));

  XMLElement* root = msg.GetRoot();
  msg.AddElement(root, _T("Name"),         XDT_String|XDT_CDATA,m_name);
  msg.AddElement(root, _T("Documentation"),XDT_String|XDT_CDATA,m_documentation);

  XMLElement* parameters = msg.AddElement(root,_T("Parameters"),XDT_String,_T(""));
  msg.SetElement(parameters,_T("KillOnTimeout"),   m_killOnTimeout);
  msg.SetElement(parameters,_T("MaxExecutionTime"),m_maxExecution);
  msg.SetElement(parameters,_T("WaitBeforeRun"),   m_waitBeforeRun);
  msg.SetElement(parameters,_T("WaitAfterRun"),    m_waitAfterRun);

  if(!m_scriptToRun.IsEmpty())
  {
    XMLElement* script = msg.AddElement(root,_T("Script"),XDT_String,_T(""));
    msg.SetElement(script,_T("ScriptResult"),IntegerToString((int)m_scriptStatus));
    msg.SetElement(script,_T("QLScript"),m_scriptToRun);
    msg.SetElement(script,_T("StatusOK"),m_statusOK);
  }
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
  return _T("");
}

int
TestStep::FindElementInteger(XMLMessage& p_msg, XMLElement* p_start, CString p_name)
{
  XMLElement* elem = p_msg.FindElement(p_start, p_name);
  if (elem)
  {
    return _ttoi(elem->GetValue());
  }
  return 0;
}

bool
TestStep::FindElementBoolean(XMLMessage& p_msg, XMLElement* p_start, CString p_name)
{
  XMLElement* elem = p_msg.FindElement(p_start, p_name);
  if (elem)
  {
    if(elem->GetValue().CompareNoCase(_T("true")) == 0 ||
       elem->GetValue().CompareNoCase(_T("yes"))  == 0 ||
       elem->GetValue().Compare(_T("1"))          == 0)
    {
       return true;
    }
  }
  return false;
}
