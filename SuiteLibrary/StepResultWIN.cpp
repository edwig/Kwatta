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
// This File   : StepResultWIN.cpp
// What it does: Result of one (1) Windows UI test
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StepResultWIN.h"
#include "ExtraExtensions.h"
#include "StdException.h"
#include <HTTPError.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// Interface with the file system
void
StepResultWIN::ReadFromXML(CString p_filename)
{
  XMLMessage msg;
  StepResult::ReadFromXML(msg,p_filename);

  m_lastOSerror   = msg.GetElementInteger("LastOSError");
  m_errorString   = msg.GetElement("ErrorString");
  m_resultLogging = msg.GetElement("ResultLogging");
}

bool
StepResultWIN::WriteToXML(CString p_filename)
{
  XMLMessage msg;
  if(!StepResult::WriteToXML(msg,p_filename))
  {
    return false;
  }
  XMLElement* root = msg.GetRoot();

  CString error;
  error.Format("%u",m_lastOSerror);

  msg.AddElement(root,"LastOSError",  XDT_Integer,error);
  msg.AddElement(root,"ErrorString",  XDT_String,m_errorString);
  msg.AddElement(root,"ResultLogging",XDT_String,m_resultLogging);

  // Now save it
  return msg.SaveFile(p_filename);
}

void
StepResultWIN::CheckFilename(CString p_filename)
{
  // Split of only the extension
  char extension[_MAX_EXT];
  _splitpath_s(p_filename,NULL,0,NULL,0,NULL,0,extension,_MAX_EXT);

  // Check that we have the right one
  if(_strnicmp(extension, EXTENSION_RESULT_WIN,5))
  {
    throw StdException("A StepResult XML definition file must be saved as a *.WRES");
  }
}

void
StepResultWIN::ResetEffective()
{
  m_lastOSerror = 0;
  m_errorString.Empty();
  m_resultLogging.Empty();
}
