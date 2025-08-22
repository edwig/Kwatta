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
#include "TestStepNET.h"
#include "XMLMessage.h"
#include "StdException.h"
#include "ExtraExtensions.h"
#include <CrackURL.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// Interface with the file system
void
TestStepNET::ReadFromXML(CString p_filename)
{
  XMLMessage msg;
  TestStep::ReadFromXML(msg, p_filename);
  XMLElement* root = msg.GetRoot();

  // Find type
  XMLElement* typ = msg.FindElement(root, _T("Type"), false);
  if (typ)
  {
    CString type = typ->GetValue();
    if (type.Compare(_T("Internet")))
    {
      throw StdException(_T("IRUN file is not an internet test: ") + p_filename);
    }
  }
  else
  {
    throw StdException(_T("Missing <Type> in IRUN file: ") + p_filename);
  }
  XMLElement* def = msg.FindElement(_T("Definition"));
  if(!def)
  {
    throw StdException(_T("Missing <Definition> in IRUN file: ") + p_filename);
  }
  m_verb       = msg.GetElement(def,_T("VERB"));
  m_url        = msg.GetElement(def,_T("URL"));
  m_anchor     = msg.GetElement(def,_T("Anchor"));
  m_mimeType   = msg.GetElement(def,_T("MimeType"));

  m_useStatus  = msg.GetElementBoolean(def,_T("UseStatus"));
  m_useHeaders = msg.GetElementBoolean(def,_T("UseHeaders"));
  m_useBody    = msg.GetElementBoolean(def,_T("UseBody"));

  XMLElement* params = msg.FindElement(def,_T("Parameters"));
  if(params)
  {
    XMLElement* parameter = msg.FindElement(params,_T("Parameter"));
    while(parameter)
    {
      INPair pair;
      pair.m_name  = msg.GetElement(parameter,_T("Name"));
      pair.m_value = msg.GetElement(parameter,_T("Value"));
      m_parameters.push_back(pair);

      parameter = msg.GetElementSibling(parameter);
    }
  }

  XMLElement* allheaders = msg.FindElement(def,_T("Headers"));
  if(allheaders)
  {
    XMLElement* header = msg.FindElement(allheaders,_T("Header"));
    while(header)
    {
      INPair head;
      head.m_name  = msg.GetElement(header,_T("Name"));
      head.m_value = msg.GetElement(header,_T("Value"));
      m_headers.push_back(head);

      header = msg.GetElementSibling(header);
    }
  }

  XMLElement* auth = msg.FindElement(def,_T("Authentication"));
  if(auth)
  {
    m_credential  = msg.GetElement(auth,_T("Credentials"));
    m_authType    = msg.GetElement(auth,_T("Type"));
    m_userName    = msg.GetElement(auth,_T("Username"));
    m_password    = msg.GetElement(auth,_T("Password"));
    m_oauthGrant  = msg.GetElement(auth,_T("OAuthGrant"));
    m_tokenServer = msg.GetElement(auth,_T("TokenServer"));
    m_clientID    = msg.GetElement(auth,_T("ClientID"));
    m_clientKey   = msg.GetElement(auth,_T("ClientKey"));
    m_clientScope = msg.GetElement(auth,_T("ClientScope"));
    m_keyHeader   = msg.GetElement(auth,_T("KeyHeaderName"));
    m_keyValue    = msg.GetElement(auth,_T("KeyHeaderValue"));

    SetCredential(m_credential);
  }

  // Timeouts
  XMLElement* timeouts = msg.FindElement(def,_T("Timeouts"));
  if(timeouts)
  {
    m_timeoutResolve = _ttoi(msg.GetElement(timeouts,_T("TimeoutResolve")));
    m_timeoutConnect = _ttoi(msg.GetElement(timeouts,_T("TimeoutConnect")));
    m_timeoutSend    = _ttoi(msg.GetElement(timeouts,_T("TimeoutSend")));
    m_timeoutReceive = _ttoi(msg.GetElement(timeouts,_T("TimeoutReceive")));
  }

  // Read the body
  m_body              = msg.GetElement(def, _T("Body"));
  m_bodyInputIsFile   = msg.GetElementBoolean(def,_T("BodyInputIsFile"));
  m_bodyOutputIsFile  = msg.GetElementBoolean(def,_T("BodyOutputIsFile"));
  m_filenameInput     = msg.GetElement(def,_T("FileInput"));
  m_filenameOutput    = msg.GetElement(def,_T("FileOutput"));
}

bool
TestStepNET::WriteToXML(CString p_filename)
{
  XMLMessage msg;
  if (!TestStep::WriteToXML(msg, p_filename))
  {
    return false;
  }
  XMLElement* root = msg.GetRoot();

  // This is our SUB-CLASS type
  msg.AddElement(root,_T("Type"),XDT_String,_T("Internet"));

  XMLElement* def = msg.AddElement(root,_T("Definition"),XDT_String,_T(""));

  // Main elements
  msg.AddElement(def,_T("VERB"),  XDT_String,m_verb);
  msg.AddElement(def,_T("URL"),   XDT_String |XDT_CDATA,m_url);
  msg.AddElement(def,_T("Anchor"),XDT_String |XDT_CDATA,m_anchor);

  msg.SetElement(def,_T("UseStatus"), m_useStatus);
  msg.SetElement(def,_T("UseHeaders"),m_useHeaders);
  msg.SetElement(def,_T("UseBody"),   m_useBody);

  // Parameters
  XMLElement* params = msg.AddElement(def,_T("Parameters"),XDT_String,_T(""));
  for(auto& parm : m_parameters)
  {
    XMLElement* param = msg.AddElement(params,_T("Parameter"),XDT_String,_T(""));
    msg.AddElement(param,_T("Name"), XDT_String,          parm.m_name);
    msg.AddElement(param,_T("Value"),XDT_String|XDT_CDATA,parm.m_value);
  }

  // Headers
  XMLElement* allheaders = msg.AddElement(def,_T("Headers"),XDT_String,_T(""));
  for(auto& head : m_headers)
  {
    XMLElement* header = msg.AddElement(allheaders,_T("Header"),XDT_String,_T(""));
    msg.AddElement(header,_T("Name"),XDT_String,           head.m_name);
    msg.AddElement(header,_T("Value"),XDT_String|XDT_CDATA,head.m_value);
  }

  // Authentication
  XMLElement* auth = msg.AddElement(def,_T("Authentication"),XDT_String,_T(""));
  msg.AddElement(auth,_T("Credentials"),XDT_String,m_credential);
  if(m_credential.IsEmpty())
  {
    msg.AddElement(auth,_T("Type"),          XDT_String,m_authType);
    msg.AddElement(auth,_T("Username"),      XDT_String,m_userName);
    msg.AddElement(auth,_T("Password"),      XDT_String,m_password);
    msg.AddElement(auth,_T("OAuthGrant"),    XDT_String,m_oauthGrant);
    msg.AddElement(auth,_T("TokenServer"),   XDT_String,m_tokenServer);
    msg.AddElement(auth,_T("ClientID"),      XDT_String,m_clientID);
    msg.AddElement(auth,_T("ClientKey"),     XDT_String,m_clientKey);
    msg.AddElement(auth,_T("ClientScope"),   XDT_String,m_clientScope);
    msg.AddElement(auth,_T("KeyHeaderName"), XDT_String,m_keyHeader);
    msg.AddElement(auth,_T("KeyHeaderValue"),XDT_String,m_keyValue);
  }
  // And our payload body
  msg.AddElement(def,_T("MimeType"),        XDT_String, m_mimeType);
  msg.AddElement(def,_T("BodyInputIsFile"), XDT_Boolean,m_bodyInputIsFile  ? _T("true") : _T("false") );
  msg.AddElement(def,_T("BodyOutputIsFile"),XDT_Boolean,m_bodyOutputIsFile ? _T("true") : _T("false") );
  msg.AddElement(def,_T("FileInput"),       XDT_String, m_filenameInput);
  msg.AddElement(def,_T("FileOutput"),      XDT_String, m_filenameOutput);
  msg.AddElement(def,_T("Body"),            XDT_String|XDT_CDATA,m_body);

  // Timeouts
  XMLElement* timeouts = msg.AddElement(def,_T("Timeouts"),XDT_String,_T(""));
  msg.AddElement(timeouts,_T("TimeoutResolve"), XDT_Integer,IntegerToString(m_timeoutResolve));
  msg.AddElement(timeouts,_T("TimeoutConnect"), XDT_Integer,IntegerToString(m_timeoutConnect));
  msg.AddElement(timeouts,_T("TimeoutSend"),    XDT_Integer,IntegerToString(m_timeoutSend));
  msg.AddElement(timeouts,_T("TimeoutReceive"), XDT_Integer,IntegerToString(m_timeoutReceive));

  // Now save it
  return msg.SaveFile(p_filename);
}

// RE-Calculate the effective strings, returning the number of unbound parameters
int
TestStepNET::EffectiveReplacements(Parameters* p_parameters,bool p_forDisplay)
{
  // Clear the error stack
  p_parameters->ResetUnboundErrors();

  int unbound = TestStep::EffectiveReplacements(p_parameters,p_forDisplay);

  unbound += p_parameters->Replace(m_url,           m_effectiveUrl,       p_forDisplay);
  unbound += p_parameters->Replace(m_anchor,        m_effectiveAnchor,    p_forDisplay);
  unbound += p_parameters->Replace(m_body,          m_effectiveBody,      p_forDisplay,ParType::PAR_BUFFER);
  unbound += p_parameters->Replace(m_filenameInput, m_effectiveFileInput, p_forDisplay);
  unbound += p_parameters->Replace(m_filenameOutput,m_effectiveFileOutput,p_forDisplay);

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
TestStepNET::CheckFilename(CString p_filename)
{
  // Split of only the extension
  TCHAR extension[_MAX_EXT];
  _tsplitpath_s(p_filename,NULL,0,NULL,0,NULL,0,extension,_MAX_EXT);

  // Check that we have the right one
  if(_tcsncicmp(extension, EXTENSION_TESTSTEP_NET, 5))
  {
    throw StdException(_T("A TestStep XML definition file must be saved as a *.XRUN"));
  }
}

CString
TestStepNET::GetParameter(CString p_parameter)
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
TestStepNET::GetHeader(CString p_header)
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
TestStepNET::SetParameter(CString p_parameter,CString p_value)
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
TestStepNET::DeleteParameter(CString p_parameter)
{
  UrlParameters::iterator it = m_parameters.begin();
  while(it != m_parameters.end())
  {
    if(p_parameter.CompareNoCase(it->m_name) == 0)
    {
      m_parameters.erase(it);
      return;
    }
    ++it;
  }
}

void
TestStepNET::SetHeader(CString p_header,CString p_value)
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
TestStepNET::GetEffectiveCombinedURL()
{
  CString url = CrackedURL::EncodeURLChars(m_effectiveUrl);

  // Add URL parameters
  if(!m_effectiveParameters.empty())
  {
    url += _T("?");
    bool extra = false;
    for(auto& param : m_effectiveParameters)
    {
      if(extra)
      {
        url += _T("&");
      }
      url += param.m_name;
      url += _T("=");
      url += CrackedURL::EncodeURLChars(param.m_value);
      extra = true;
    }
  }

  // Add anchor
  if(!m_effectiveAnchor.IsEmpty())
  {
    url += _T("#");
    url += CrackedURL::EncodeURLChars(m_effectiveAnchor);
  }
 
  // Ready
  return url;
}

CString
TestStepNET::GetRawRequest()
{
  CString raw(m_verb);
  raw += _T(" ");
  raw += GetEffectiveCombinedURL();

  // Add protocol version
  raw += _T(" HTTP/1.1\r\n");

  // Add all headers
  for(auto& head : m_effectiveHeaders)
  {
    raw += head.m_name;
    raw += _T(": ");
    raw += head.m_value;
    raw += _T("\r\n");
  }

  // Header body separator
  raw += _T("\r\n");

  CString body(m_effectiveBody);
  body.Replace(_T("\n"),_T("\r\n"));
  raw += body;

  return raw;
}

// Reset effective parameter replacements
void
TestStepNET::ResetEffective()
{
  m_bearerToken.Empty();
  m_effectiveUrl.Empty();
  m_effectiveAnchor.Empty();
  m_effectiveBody.Empty();
  m_effectiveParameters.clear();
  m_effectiveHeaders.clear();
}

void
TestStepNET::SetCredential(CString p_credential)
{
  // Remember credential template
  m_credential = p_credential;

  if(!m_credential.IsEmpty())
  {
    m_authType.Empty();
    m_userName.Empty();
    m_password.Empty();
    m_oauthGrant.Empty();
    m_tokenServer.Empty();
    m_clientID.Empty();
    m_clientKey.Empty();
    m_clientScope.Empty();
    m_keyHeader.Empty();
    m_keyValue.Empty();
  }
}
