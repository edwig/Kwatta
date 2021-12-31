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
#include "EnsureFile.h"
#include "ExtraExtensions.h"
#include "StepResultCL.h"
#include "StepResultIN.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

StepResult*
ReadStepResult(CString p_filename)
{
  EnsureFile ensure;
  CString extension = ensure.ExtensionPart(p_filename);

  if(extension.CompareNoCase(EXTENSION_RESULT_CL) == 0)
  {
    StepResultCL* result = new StepResultCL();
    result->ReadFromXML(p_filename);
    return result;
  }
  if(extension.CompareNoCase(EXTENSION_RESULT_IN) == 0)
  {
    StepResultIN* result = new StepResultIN();
    result->ReadFromXML(p_filename);
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
}

bool
StepResult::WriteToXML(XMLMessage& p_msg,CString p_filename)
{
  p_msg.SetRootNodeName("StepResult");

  // Result of the step
  CString timing;
  timing.Format("%.6G",m_seconds);


  XMLElement* root = p_msg.GetRoot();
  p_msg.AddElement(root,"Name",         XDT_String | XDT_CDATA,m_name);
  p_msg.AddElement(root,"Documentation",XDT_String | XDT_CDATA,m_documentation);
  p_msg.AddElement(root,"Timing",       XDT_String,            timing);

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
StepResult::AddValidation(int p_step,CString p_name,CString p_filename,bool p_result)
{
  ValStep step;
  EnsureFile ensure;
  
  step.m_number     = p_step;
  step.m_validation = p_name;
  step.m_filename   = ensure.FilenamePart(p_filename);
  step.m_ok         = p_result;

  m_validations.push_back(step);
}
