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
// This File   : StepResultIN.cpp
// What it does: Result of one (1) internet step in a test set
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StepResultNET.h"
#include "ExtraExtensions.h"
#include "StdException.h"
#include <HTTPError.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

void
StepResultNET::Reset()
{
  StepResult::Reset();
  ResetEffective();

  m_status  = 0;
  m_osError = 0;

  m_headers.clear();
  m_body.Empty();
  m_bodyFile.Empty();
  m_bearerToken.Empty();
  m_osErrorString.Empty();
}

void
StepResultNET::ReadFromXML(CString p_filename)
{
  XMLMessage msg;
  StepResult::ReadFromXML(msg,p_filename);

  // Find status
  m_status        = msg.GetElementInteger("Status");
  m_osError       = msg.GetElementInteger("OSError");
  m_osErrorString = msg.GetElement("OSErrorString");
  m_bearerToken   = msg.GetElement("BearerToken");

  // Find all headers
  XMLElement* allheaders = msg.FindElement("ResponseHeaders");
  if(allheaders)
  {
    XMLElement* header = msg.FindElement(allheaders,"Header");
    while(header)
    {
      INPair head;
      head.m_name  = msg.GetElement(header,"Name");
      head.m_value = msg.GetElement(header,"Value");
      m_headers.push_back(head);

      header = msg.GetElementSibling(header);
    }
  }

  // Find the body
  m_body = msg.GetElement("ResponseBody");
}

bool
StepResultNET::WriteToXML(CString p_filename)
{
  XMLMessage msg;
  if(!StepResult::WriteToXML(msg,p_filename))
  {
    return false;
  }
  XMLElement* root = msg.GetRoot();

  m_osErrorString.Remove('\r');
  m_body.Remove('\r');

  // Status
  CString status;
  CString oserror;
  status .Format("%d",m_status);
  oserror.Format("%d",m_osError);
  msg.AddElement(root,"Status",       XDT_String,status);
  msg.AddElement(root,"OSError",      XDT_String,oserror);
  msg.AddElement(root,"OSErrorString",XDT_String,m_osErrorString);
  msg.AddElement(root,"BearerToken",  XDT_String,m_bearerToken);

  // Headers
  XMLElement* allheaders = msg.AddElement(root,"ResponseHeaders",XDT_String,"");
  for(auto& head : m_headers)
  {
    XMLElement* header = msg.AddElement(allheaders,"Header",XDT_String,"");
    msg.AddElement(header,"Name", XDT_String,            head.m_name);
    msg.AddElement(header,"Value",XDT_String | XDT_CDATA,head.m_value);
  }

  // Body
  if(m_body.Find("<![CDATA[") > 0)
  {
    // Recursive restartable CDATA section
    m_body.Replace("]]>","]]]]><![CDATA[>");
  }
  // Save the body
  msg.AddElement(root,"ResponseBody",XDT_CDATA,m_body);

  // Now save it
  return msg.SaveFile(p_filename);
}

void
StepResultNET::CheckFilename(CString p_filename)
{
  // Split of only the extension
  char extension[_MAX_EXT];
  _splitpath_s(p_filename,NULL,0,NULL,0,NULL,0,extension,_MAX_EXT);

  // Check that we have the right one
  if(_strnicmp(extension,EXTENSION_RESULT_NET,5))
  {
    throw StdException("A StepResult XML definition file must be saved as a *.IRES");
  }
}

CString 
StepResultNET::GetHeader(CString p_header)
{
  for(auto& header : m_headers)
  {
    if(header.m_name.CompareNoCase(p_header) == 0)
    {
      return header.m_value;
    }
  }
  return "";
}

CString 
StepResultNET::GetRawResponse()
{
  // Getting the raw HTTP line
  CString raw;
  raw.Format("HTTP/1.1 %d %s\n",m_status,GetHTTPStatusText(m_status));

  // Add the headers
  for(auto& header : m_headers)
  {
    raw.AppendFormat("%s: %s\n",header.m_name.GetString(),header.m_value.GetString());
  }

  // Add the body
  raw += "\n";
  raw += m_body;

  return raw;
}

void
StepResultNET::ResetEffective()
{
  // NOP
}