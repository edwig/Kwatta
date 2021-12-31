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
// This File   : StepResultCL.cpp
// What it does: Result of one (1) command-line step in a test set
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StepResultCL.h"
#include "ExtraExtensions.h"
#include "StdException.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

void
StepResultCL::ReadFromXML(CString p_filename)
{
  XMLMessage msg;
  StepResult::ReadFromXML(msg,p_filename);

  // Load result of the step
  CString timing;
  XMLElement* output = msg.FindElement("Output");
  if(output)
  {
    m_returnValue    = msg.GetElementInteger(output,"ReturnValue");
    m_standardOutput = msg.GetElement(output,"StandardOutput");
    m_standardError  = msg.GetElement(output,"StandardError");
  }

  // Load all validations and results
  XMLElement* validations = msg.FindElement("Validations");
  if(validations)
  {
    XMLElement* validation = msg.FindElement(validations,"Validation",false);
    while(validation)
    {
      ValStep vali;
      vali.m_number      = msg.GetElementInteger(validation,"Number");
      vali.m_validation  = msg.GetElement(validation,"Name");
      vali.m_filename    = msg.GetElement(validation,"File");
      vali.m_ok          = msg.GetElement(validation,"Result") == "OK" ? true : false;

      m_validations.push_back(vali);
      validation = msg.GetElementSibling(validation);
    }
  }
}

bool 
StepResultCL::WriteToXML(CString p_filename)
{
  XMLMessage msg;
  if(!StepResult::WriteToXML(msg,p_filename))
  {
    return false;
  }
  XMLElement* root = msg.GetRoot();

  XMLElement* output = msg.AddElement(root,"Output",XDT_String,"");
  msg.SetElement(output,"ReturnValue",m_returnValue);
  msg.AddElement(output,"StandardOutput",XDT_CDATA,m_standardOutput);
  msg.AddElement(output,"StandardError", XDT_CDATA,m_standardError);

  // All validation steps and results
  XMLElement* validations = msg.AddElement(root,"Validations",XDT_String,"");
  for(auto& val : m_validations)
  {
    XMLElement* vali = msg.AddElement(validations,"Validation",XDT_String,"");
    msg.SetElement(vali,"Number",val.m_number);
    msg.SetElement(vali,"Name",  val.m_validation);
    msg.SetElement(vali,"File",  val.m_filename);
    msg.SetElement(vali,"Result",val.m_ok ? "OK" : "ERROR");
  }
  // Now save it
  return msg.SaveFile(p_filename);
}

void
StepResultCL::CheckFilename(CString p_filename)
{
  // Split of only the extension
  char extension[_MAX_EXT];
  _splitpath_s(p_filename,NULL,0,NULL,0,NULL,0,extension,_MAX_EXT);

  // Check that we have the right one
  if(_strnicmp(extension,EXTENSION_RESULT_CL,5))
  {
    throw StdException("A StepResult XML definition file must be saved as a *.XRES");
  }
}

void 
StepResultCL::ResetEffective()
{
  // NOP
}
