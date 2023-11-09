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
// This File   : StepResult.cpp
// What it does: Result of one (1) step in a test set
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "StepResult.h"
#include "StdException.h"
#include "ExtraExtensions.h"
#include "StepResultCMD.h"
#include "StepResultNET.h"
#include "StepResultSQL.h"
#include <memory>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

using std::shared_ptr;

void 
StepResult::Reset()
{
  m_name.Empty();
  m_documentation.Empty();
  m_seconds = 0.0;
  // Validation steps
  m_validations.clear();
}

StepResult*
ReadStepResult(CString p_filename)
{
  WinFile file(p_filename);
  CString extension = file.GetFilenamePartExtension(); 

  if(extension.CompareNoCase(EXTENSION_RESULT_CMD) == 0)
  {
    StepResultCMD* result = new StepResultCMD();
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
  if(extension.CompareNoCase(EXTENSION_RESULT_NET) == 0)
  {
    StepResultNET* result = new StepResultNET();
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
  if(extension.CompareNoCase(EXTENSION_RESULT_SQL) == 0)
  {
    StepResultSQL* result = new StepResultSQL();
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

//////////////////////////////////////////////////////////////////////////
//
// PROTECTED
//
//////////////////////////////////////////////////////////////////////////

void    
StepResult::ReadFromXML(XMLMessage& p_msg,CString p_filename)
{
  // Check that we have a *.xrun extension
  CheckFilename(p_filename);

  // Load the file (if any)
  if(p_msg.LoadFile(p_filename) == false)
  {
    throw StdException("Could not load the XML file: " + p_filename);
  }

  // CHeck for XML error
  if(p_msg.GetInternalError() != XmlError::XE_NoError)
  {
    CString error;
    error.Format("Internal XML error in result file [%d] %s",p_msg.GetInternalError(),p_msg.GetInternalErrorString().GetString());
    throw StdException(error);
  }

  // Check that it is our message type
  if(p_msg.GetRootNodeName().Compare("StepResult"))
  {
    throw StdException("XRES file is not a StepResult definition: " + p_filename);
  }

  // Find names
  m_name          = p_msg.GetElement("Name");
  m_documentation = p_msg.GetElement("Documentation");
  CString timing  = p_msg.GetElement("Timing");
  m_seconds = atof(timing);

  // Load all validations and results
  XMLElement* validations = p_msg.FindElement("Validations");
  if(validations)
  {
    XMLElement* validation = p_msg.FindElement(validations,"Validation",false);
    while(validation)
    {
      ValStep vali;
      vali.m_number      = p_msg.GetElementInteger(validation,"Number");
      vali.m_validation  = p_msg.GetElement(validation,"Name");
      vali.m_filename    = p_msg.GetElement(validation,"File");
      vali.m_ok          = p_msg.GetElement(validation,"Result") == "OK" ? true : false;
      vali.m_global      = p_msg.GetElementBoolean(validation,"Global");

      m_validations.push_back(vali);
      validation = p_msg.GetElementSibling(validation);
    }
  }
}

bool
StepResult::WriteToXML(XMLMessage& p_msg,CString p_filename)
{
  p_msg.SetRootNodeName("StepResult");

  // Result of the step
  CString timing;
  timing.Format("%.6G",m_seconds);

  m_documentation.Remove('\r');

  XMLElement* root = p_msg.GetRoot();
  p_msg.AddElement(root,"Name",         XDT_String | XDT_CDATA,m_name);
  p_msg.AddElement(root,"Documentation",XDT_String | XDT_CDATA,m_documentation);
  p_msg.AddElement(root,"Timing",       XDT_String,            timing);

  // All validation steps and results
  XMLElement* validations = p_msg.AddElement(root,"Validations",XDT_String,"");
  for(auto& val : m_validations)
  {
    XMLElement* vali = p_msg.AddElement(validations,"Validation",XDT_String,"");
    p_msg.SetElement(vali,"Number",val.m_number);
    p_msg.SetElement(vali,"Name",  val.m_validation);
    p_msg.SetElement(vali,"File",  val.m_filename);
    p_msg.SetElement(vali,"Result",val.m_ok     ? "OK"   : "ERROR");
    p_msg.SetElement(vali,"Global",val.m_global ? "true" : "false");
  }
  return true;
}

// THIS IS WHY WE ARE HERE
// Get the total test result of the step
// TRUE if all steps are true
bool
StepResult::GetTotalResult()
{
  for(auto& step : m_validations)
  {
    if (step.m_ok == false)
    {
      return false;
    }
  }
  return true;
}

// Add an extra validation
void
StepResult::AddValidation(int p_step,CString p_name,CString p_filename,bool p_result,bool p_global)
{
  ValStep step;
  WinFile file(p_filename);
  
  step.m_number     = p_step;
  step.m_validation = p_name;
  step.m_filename   = file.GetFilenamePartFilename();
  step.m_ok         = p_result;
  step.m_global     = p_global;

  m_validations.push_back(step);
}
