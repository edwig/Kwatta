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
// This File   : TestSet.cpp
// What it does: One (1) step in a test set
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TestSet.h"
#include <StdException.h>
#include <XMLMessage.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

void
TestSet::ReadFromXML(CString p_filename)
{
  XMLMessage msg;

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
    error.Format(_T("Internal XML error in XSET file [%d] %s"),msg.GetInternalError(),msg.GetInternalErrorString().GetString());
    throw StdException(error);
  }

  // Check that it is our message type
  if(msg.GetRootNodeName().Compare(_T("Test")))
  {
    throw StdException(_T("XSET file is not a TestSet definition: ") + p_filename);
  }

  // Find names
  XMLElement* root = msg.GetRoot();
  m_name           = FindElementString(msg,root,_T("Name"));
  m_documentation  = FindElementString(msg,root,_T("Documentation"));

  XMLElement* steps = msg.FindElement(_T("Steps"));
  if(steps)
  {
    XMLElement* step = msg.FindElement(steps,_T("Step"),false);
    while(step)
    {
      TRun run;
      XMLElement* xrun = msg.FindElement(step,_T("xrun"),false);
      if(xrun)
      {
        run.m_filename = xrun->GetValue();
        run.m_global   = msg.GetAttributeBoolean(xrun,_T("global"));
        run.m_name     = msg.GetAttribute(xrun,_T("name"));
      }
      run.m_lastResult = FindElementString(msg,step,_T("LastResult"));

      XMLElement* validations = msg.FindElement(step,_T("Validations"),false);
      if(validations)
      {
        XMLElement* validation = msg.FindElement(validations,_T("Validation"),false);
        while(validation)
        {
          TRValidation vali;
          vali.m_filename = validation->GetValue();
          vali.m_global   = msg.GetAttributeBoolean(validation,_T("global"));
          vali.m_name     = msg.GetAttribute(validation,_T("name"));
          // Store validation and find next
          run.m_validations.push_back(vali);
          validation = msg.GetElementSibling(validation);
        }
      }
      // Store and find next step
      m_testruns.push_back(run);
      step = msg.GetElementSibling(step);
    }
  }
  m_filename = p_filename;
}

bool
TestSet::WriteToXML()
{
  XMLMessage msg;

  msg.SetRootNodeName(_T("Test"));

  XMLElement* root = msg.GetRoot();
  msg.AddElement(root, _T("Name"),          XDT_String, m_name);
  msg.AddElement(root, _T("Documentation"), XDT_String, m_documentation);

  XMLElement* steps = msg.AddElement(root,_T("Steps"),XDT_String,_T(""));
  for(auto& run : m_testruns)
  {
    // Add the step
    XMLElement* step = msg.AddElement(steps,_T("Step"),XDT_String,_T(""));
    XMLElement* xrun = msg.AddElement(step, _T("xrun"),XDT_String,run.m_filename);
    if(run.m_global)
    {
      msg.SetAttribute(xrun,_T("global"),_T("true"));
    }
    if(!run.m_name.IsEmpty())
    {
      msg.SetAttribute(xrun,_T("name"),run.m_name);
    }

    // Add all validation steps of the run
    XMLElement* validations = msg.AddElement(step,_T("Validations"),XDT_String,_T(""));
    for(auto& vali : run.m_validations)
    {
      XMLElement* validation = msg.AddElement(validations,_T("Validation"),XDT_String,vali.m_filename);
      if(vali.m_global)
      {
        msg.SetAttribute(validation,_T("global"),_T("true"));
      }
      if(!vali.m_name.IsEmpty())
      {
        msg.SetAttribute(validation,_T("name"),vali.m_name);
      }
    }

    // Add any last result of the run
    if(!run.m_lastResult.IsEmpty())
    {
      msg.AddElement(step,_T("LastResult"),XDT_String,run.m_lastResult);
    }
  }

  // Now save it
  return msg.SaveFile(m_filename);
}

bool
TestSet::GetTotalResult()
{
  if(m_testruns.empty())
  {
    return false;
  }

  bool result = true;
  for(auto& run : m_testruns)
  {
    if (run.m_lastResult.CompareNoCase(_T("OK")))
    {
      result = false;
      break;
    }
  }
  return result;
}

TRun* 
TestSet::GetRun(CString p_filename)
{
  for(auto& trun : m_testruns)
  {
    if(trun.m_filename.CompareNoCase(p_filename) == 0)
    {
      return &trun;
    }
  }
  return nullptr;
}

TSValSet*
TestSet::GetValidationsByName(CString p_name)
{
  for(auto& trun : m_testruns)
  {
    if(trun.m_name.CompareNoCase(p_name) == 0)
    {
      return &trun.m_validations;
    }
  }
  return nullptr;
}

TSValSet*
TestSet::GetValidationsByFile(CString p_filename)
{
  for(auto& trun : m_testruns)
  {
    if(trun.m_filename.CompareNoCase(p_filename) == 0)
    {
      return &trun.m_validations;
    }
  }
  return nullptr;
}

TRun*
TestSet::AddTestStep(CString p_name,CString p_filename,bool p_global)
{
  TRun run;
  run.m_name     = p_name;
  run.m_filename = p_filename;
  run.m_global   = p_global;
  m_testruns.push_back(run);
  return &(m_testruns.back());
}

void
TestSet::DeleteStep(int p_step)
{
  if(p_step < 0 || p_step >= (int)m_testruns.size())
  {
    return;
  }
  TRunSet::iterator it = m_testruns.begin();
  while (p_step--)
  {
    ++it;
  }
  m_testruns.erase(it);
}

TRun& 
TestSet::GetTestRun(int p_run)
{
  if(p_run >= 0 && p_run < (int)m_testruns.size())
  {
    return m_testruns[p_run];
  }
  throw StdException(_T("Internal error"));
}

//////////////////////////////////////////////////////////////////////////
//
// PRIVATE
//
//////////////////////////////////////////////////////////////////////////

void
TestSet::CheckFilename(CString p_filename)
{
  // Split of only the extension
  TCHAR extension[_MAX_EXT];
  _tsplitpath_s(p_filename, NULL, 0, NULL, 0, NULL, 0, extension, _MAX_EXT);

  // Check that we have the right one
  if (_tcsncicmp(extension, _T(".xset"), 5))
  {
    throw StdException(_T("A TestSet XML definition file must be saved as a *.XSET"));
  }
}

CString
TestSet::FindElementString(XMLMessage& p_msg, XMLElement* p_start, CString p_name)
{
  XMLElement* elem = p_msg.FindElement(p_start, p_name);
  if (elem)
  {
    return elem->GetValue();
  }
  return _T("");
}

