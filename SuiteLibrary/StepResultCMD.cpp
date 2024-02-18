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
#include "StepResultCMD.h"
#include "ExtraExtensions.h"
#include "StdException.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

void
StepResultCMD::ReadFromXML(CString p_filename)
{
  XMLMessage msg;
  StepResult::ReadFromXML(msg,p_filename);

  // Load result of the step
  CString timing;
  XMLElement* output = msg.FindElement(_T("Output"));
  if(output)
  {
    m_returnValue    = msg.GetElementInteger(output,_T("ReturnValue"));
    m_standardOutput = msg.GetElement(output,_T("StandardOutput"));
    m_standardError  = msg.GetElement(output,_T("StandardError"));
  }
}

bool 
StepResultCMD::WriteToXML(CString p_filename)
{
  XMLMessage msg;
  if(!StepResult::WriteToXML(msg,p_filename))
  {
    return false;
  }
  XMLElement* root = msg.GetRoot();

  XMLElement* output = msg.AddElement(root,_T("Output"),XDT_String,_T(""));
  msg.SetElement(output,_T("ReturnValue"),m_returnValue);

  m_standardOutput.Remove('\r');
  m_standardError.Remove('\r');

  msg.AddElement(output,_T("StandardOutput"),XDT_CDATA,m_standardOutput);
  msg.AddElement(output,_T("StandardError"), XDT_CDATA,m_standardError);

  // Now save it
  return msg.SaveFile(p_filename);
}

void
StepResultCMD::CheckFilename(CString p_filename)
{
  // Split of only the extension
  TCHAR extension[_MAX_EXT];
  _tsplitpath_s(p_filename,NULL,0,NULL,0,NULL,0,extension,_MAX_EXT);

  // Check that we have the right one
  if(_tcsncicmp(extension,EXTENSION_RESULT_CMD,5))
  {
    throw StdException(_T("A StepResult XML definition file must be saved as a *.XRES"));
  }
}

void 
StepResultCMD::ResetEffective()
{
  // NOP
}
