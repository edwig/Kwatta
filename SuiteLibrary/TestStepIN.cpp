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
// This File   : TestStepIN.cpp
// What it does: One (1) test step. Subclass for a HTTP call
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TestStepIN.h"
#include "XMLMessage.h"
#include "StdException.h"
#include "ExtraExtensions.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// Interface with the file system
void
TestStepIN::ReadFromXML(CString p_filename)
{
  XMLMessage msg;
  TestStep::ReadFromXML(msg, p_filename);
  XMLElement* root = msg.GetRoot();

  // Find type
  XMLElement* typ = msg.FindElement(root, "Type", false);
  if (typ)
  {
    CString type = typ->GetValue();
    if (type.Compare("Internet"))
    {
      throw StdException("IRUN file is not a internet test: " + p_filename);
    }
  }
  else
  {
    throw StdException("Missing <Type> in IRUN file: " + p_filename);
  }
  XMLElement* def = msg.FindElement("Definition");
  if(!def)
  {
    throw StdException("Missing <Definition> in IRUN file: " + p_filename);
  }
  m_verb   = msg.GetElement(def,"VERB");
  m_url    = msg.GetElement(def,"URL");
  m_anchor = msg.GetElement(def,"Anchor");
  m_body   = msg.GetElement(def,"Body");

  m_useStatus  = msg.GetElementBoolean(def,"UseStatus");
  m_useHeaders = msg.GetElementBoolean(def,"UseHeaders");
  m_useBody    = msg.GetElementBoolean(def,"UseBody");

  XMLElement* params = msg.FindElement(def,"Parameters");
  if(params)
  {
    XMLElement* parameter = msg.FindElement(params,"Parameter");
    while(parameter)
    {
      INPair pair;
      pair.m_name  = msg.GetElement(parameter,"Name");
      pair.m_value = msg.GetElement(parameter,"Value");
      m_parameters.push_back(pair);

      parameter = msg.GetElementSibling(parameter);
    }
  }

  XMLElement* headers = msg.FindElement(def,"Headers");
  if(headers)
  {
    XMLElement* header = msg.FindElement(headers,"Header");
    while(header)
    {
      INPair head;
      head.m_name  = msg.GetElement(header,"Name");
      head.m_value = msg.GetElement(header,"Value");
      m_headers.push_back(head);

      header = msg.GetElementSibling(header);
    }
  }

  XMLElement* auth = msg.FindElement(def,"Authentication");
  if(auth)
  {
    m_authType    = msg.GetElement(auth,"Type");
    m_userName    = msg.GetElement(auth,"Username");
    m_password    = msg.GetElement(auth,"Password");
    m_oauthGrant  = msg.GetElement(auth,"OAuthGrant");
    m_tokenServer = msg.GetElement(auth,"TokenServer");
    m_clientID    = msg.GetElement(auth,"ClientID");
    m_clientKey   = msg.GetElement(auth,"ClientKey");
    m_clientScope = msg.GetElement(auth,"ClientScope");
  }
}

bool
TestStepIN::WriteToXML(CString p_filename)
{
  XMLMessage msg;
  if (!TestStep::WriteToXML(msg, p_filename))
  {
    return false;
  }
  XMLElement* root = msg.GetRoot();

  // This is our SUB-CLASS type
  msg.AddElement(root,"Type",XDT_String,"Internet");

  XMLElement* def = msg.AddElement(root,"Definition",XDT_String,"");

  // Main elements
  msg.AddElement(def,"VERB",  XDT_String,m_verb);
  msg.AddElement(def,"URL",   XDT_String |XDT_CDATA,m_url);
  msg.AddElement(def,"Anchor",XDT_String |XDT_CDATA,m_anchor);

  msg.SetElement(def,"UseStatus", m_useStatus);
  msg.SetElement(def,"UseHeaders",m_useHeaders);
  msg.SetElement(def,"UseBody",   m_useBody);

  // Parameters
  XMLElement* params = msg.AddElement(def,"Parameters",XDT_String,"");
  for(auto& parm : m_parameters)
  {
    XMLElement* param = msg.AddElement(params,"Parameter",XDT_String,"");
    msg.AddElement(param,"Name", XDT_String,          parm.m_name);
    msg.AddElement(param,"Value",XDT_String|XDT_CDATA,parm.m_value);
  }

  // Headers
  XMLElement* headers = msg.AddElement(def,"Headers",XDT_String,"");
  for(auto& head : m_headers)
  {
    XMLElement* header = msg.AddElement(headers,"Header",XDT_String,"");
    msg.AddElement(header,"Name",XDT_String,           head.m_name);
    msg.AddElement(header,"Value",XDT_String|XDT_CDATA,head.m_value);
  }

  // Authentication
  XMLElement* auth = msg.AddElement(def,"Authentication",XDT_String,"");
  msg.AddElement(auth,"Type",       XDT_String,m_authType);
  msg.AddElement(auth,"Username",   XDT_String,m_userName);
  msg.AddElement(auth,"Password",   XDT_String,m_password);
  msg.AddElement(auth,"OAuthGrant", XDT_String,m_oauthGrant);
  msg.AddElement(auth,"TokenServer",XDT_String,m_tokenServer);
  msg.AddElement(auth,"ClientID",   XDT_String,m_clientID);
  msg.AddElement(auth,"ClientKey",  XDT_String,m_clientKey);
  msg.AddElement(auth,"ClientScope",XDT_String,m_clientScope);

  // And our payload body
  msg.AddElement(def,"Body",XDT_String|XDT_CDATA,m_body);

  // Now save it
  return msg.SaveFile(p_filename);
}

// RE-Calculate the effective strings, returning the number of unbound parameters
int
TestStepIN::EffectiveReplacements(Parameters* p_parameters,bool p_forDisplay)
{
  int unbound = TestStep::EffectiveReplacements(p_parameters,p_forDisplay);

  unbound += p_parameters->Replace(m_url,   m_effectiveUrl,   p_forDisplay);
  unbound += p_parameters->Replace(m_anchor,m_effectiveAnchor,p_forDisplay);
  unbound += p_parameters->Replace(m_body,  m_effectiveBody,  p_forDisplay,ParType::PAR_BUFFER);

  m_effectiveParameters.clear();
  m_effectiveHeaders.clear();

  CString result;
  for(auto& param : m_parameters)
  {
    unbound += p_parameters->Replace(param.m_value,result,p_forDisplay);
    INPair pair{ param.m_name,result };
    m_effectiveParameters.push_back(pair);
  }
  for(auto& head : m_headers)
  {
    unbound += p_parameters->Replace(head.m_value,result,p_forDisplay);
    INPair pair{ head.m_name,result };
    m_effectiveHeaders.push_back(pair);
  }
  return unbound;
}

void
TestStepIN::CheckFilename(CString p_filename)
{
  // Split of only the extension
  char extension[_MAX_EXT];
  _splitpath_s(p_filename,NULL,0,NULL,0,NULL,0,extension,_MAX_EXT);

  // Check that we have the right one
  if(_strnicmp(extension, EXTENSION_TESTSTEP_IN, 5))
  {
    throw StdException("A TestStep XML definition file must be saved as a *.XRUN");
  }
}

CString
TestStepIN::GetParameter(CString p_parameter)
{
  for(auto& param : m_parameters)
  {
    if(param.m_name.CompareNoCase(p_parameter) == 0)
    {
      return param.m_value;
    }
  }
  return CString();
}

CString
TestStepIN::GetHeader(CString p_header)
{
  for(auto& head : m_headers)
  {
    if(head.m_name.CompareNoCase(p_header) == 0)
    {
      return head.m_value;
    }
  }
  return CString();
}

void
TestStepIN::SetParameter(CString p_parameter,CString p_value)
{
  for(auto& param : m_parameters)
  {
    if(param.m_name.CompareNoCase(p_parameter) == 0)
    {
      param.m_value = p_value;
      return;
    }
  }
  INPair pair = {p_parameter,p_value};
  m_parameters.push_back(pair);
}

void
TestStepIN::DeleteParameter(CString p_parameter)
{
  UrlParameters::iterator it = m_parameters.begin();
  while(it != m_parameters.end())
  {
    if(p_parameter.CompareNoCase(it->m_name) == 0)
    {
      m_parameters.erase(it);
      return;
    }
  }
}

void
TestStepIN::SetHeader(CString p_header,CString p_value)
{
  for(auto& head : m_headers)
  {
    if(head.m_name.CompareNoCase(p_header) == 0)
    {
      head.m_value = p_value;
      return;
    }
  }
  INPair pair = {p_header,p_value};
  m_headers.push_back(pair);
}

CString
TestStepIN::GetEffectiveCombinedURL()
{
  CString url(m_effectiveUrl);

  // Add URL parameters
  if(!m_effectiveParameters.empty())
  {
    url += "?";
    bool extra = false;
    for(auto& param : m_effectiveParameters)
    {
      if(extra)
      {
        url += "&";
      }
      url += param.m_name;
      url += "=";
      url += param.m_value;
      extra = true;
    }
  }

  // Add anchor
  if(!m_effectiveAnchor.IsEmpty())
  {
    url += "#";
    url += m_effectiveAnchor;
  }

  // Ready
  return url;
}


CString
TestStepIN::GetRawRequest()
{
  CString raw(m_verb);
  raw += " ";
  raw += GetEffectiveCombinedURL();

  // Add protocol version
  raw += " HTTP/1.1\r\n";

  // Add all headers
  for(auto& head : m_effectiveHeaders)
  {
    raw += head.m_name;
    raw += ": ";
    raw += head.m_value;
    raw += "\r\n";
  }

  // Header body separator
  raw += "\r\n";

  CString body(m_effectiveBody);
  body.Replace("\n","\r\n");
  raw += body;
 
  return raw;
}

// Reset effective parameter replacements
void
TestStepIN::ResetEffective()
{
  m_bearerToken.Empty();
  m_effectiveUrl.Empty();
  m_effectiveAnchor.Empty();
  m_effectiveBody.Empty();
  m_effectiveParameters.clear();
  m_effectiveHeaders.clear();
}
