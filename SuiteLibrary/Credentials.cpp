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
// This File   : Credentials.cpp
// What it does: Store global credentials for all test sets and steps
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Credentials.h"
#include "SuiteLibrary.h"
#include <Crypto.h>
#include <XMLMessage.h>
#include <io.h>

// Reset all credentials
void
Credentials::Reset()
{
  m_changed = false;
  m_credentials.clear();
}

// Interface with the file system
void
Credentials::ReadFromXML(XString p_filename)
{
  XMLMessage msg;

  // Remember where we read it
  m_filename = p_filename;

  // Check if the file exists
  if(_taccess(p_filename,0) == -1)
  {
    return;
  }

  if(!msg.LoadFile(p_filename))
  {
    throw StdException(_T("Could not load the credentials XML file: ") + p_filename);
  }
  // CHeck for XML error
  if(msg.GetInternalError() != XmlError::XE_NoError)
  {
    CString error;
    error.Format(_T("Internal XML error in credentials file [%d] %s"),msg.GetInternalError(),msg.GetInternalErrorString().GetString());
    throw StdException(error);
  }
  // Check that it is our message type
  if(msg.GetRootNodeName().Compare(_T("Credentials")))
  {
    throw StdException(_T("CRED file is not a credentials definition: ") + p_filename);
  }

  // Find credentials sets
  XMLElement* root = msg.GetRoot();

  XMLElement* credelem = msg.FindElement(root,_T("Credential"),false);
  while(credelem)
  {
    // Read this credential
    Credential cred;
    cred.m_typeName    = msg.GetElement(credelem,_T("TypeName"));
    cred.m_identifier  = msg.GetElement(credelem,_T("Identifier"));
    cred.m_username    = msg.GetElement(credelem,_T("Username"));
    cred.m_password    = msg.GetElement(credelem,_T("Password"));
    cred.m_oauthGrant  = msg.GetElement(credelem,_T("OAuth2Grant"));
    cred.m_tokenServer = msg.GetElement(credelem,_T("TokenServer"));
    cred.m_clientID    = msg.GetElement(credelem,_T("ClientID"));
    cred.m_clientKey   = msg.GetElement(credelem,_T("ClientKey"));
    cred.m_clientScope = msg.GetElement(credelem,_T("ClientScope"));

    // Convert type
    cred.m_type = StringToCredType(cred.m_typeName);
    // Decrypt passwords
    if(!cred.m_password.IsEmpty())
    {
      Crypto crypt;
      cred.m_password = crypt.Decryption(cred.m_password,_T(KWATTA_ENCRYPT));
    }
    if(!cred.m_clientKey.IsEmpty())
    {
      Crypto crypt;
      cred.m_clientKey = crypt.Decryption(cred.m_clientKey,_T(KWATTA_ENCRYPT));
    }

    // Store in the credentials mapping
    XString identifier(cred.m_identifier);
    if(FindCredential(identifier))
    {
      throw StdException(_T("Credential set defined more than once: ") + identifier);
    }
    m_credentials[identifier] = cred;
  
    // Next set of credentials
    credelem = msg.GetElementSibling(credelem);
  }

  XMLElement* connelem = msg.FindElement(root,_T("Connection"),false);
  while(connelem)
  {
    DBSConnect conn;
    conn.m_identifier = msg.GetElement(connelem,_T("Identifier"));
    conn.m_datasource = msg.GetElement(connelem,_T("Datasource"));
    conn.m_username   = msg.GetElement(connelem,_T("Username"));
    conn.m_password   = msg.GetElement(connelem,_T("Password"));

    if(!conn.m_password.IsEmpty())
    {
      Crypto crypt;
      conn.m_password = crypt.Decryption(conn.m_password,_T(KWATTA_ENCRYPT));
    }

    // Store in connections mapping
    XString identifier(conn.m_identifier);
    if(FindConnection(identifier))
    {
      throw StdException(_T("Database connection set defined more than once: ") + identifier);
    }
    m_connections[identifier] = conn;

    // Next database connection
    connelem = msg.GetElementSibling(connelem);
  }

  // Succeeded: Remember where to store it again
  m_changed  = false;
}

bool
Credentials::WriteToXML()
{
  // Check that we have a filename
  if(m_filename.IsEmpty() || m_changed == false)
  {
    return false;
  }
  XMLMessage msg;
  msg.SetRootNodeName(_T("Credentials"));

  // Save all credential sets
  for(auto& cred : m_credentials)
  {
    XMLElement* credelem = msg.AddElement(nullptr,_T("Credential"));

    XString password(cred.second.m_password);
    if(!password.IsEmpty())
    {
      Crypto crypt;
      password = crypt.Encryption(password,_T(KWATTA_ENCRYPT));
    }
    XString clientKey(cred.second.m_clientKey);
    if(!clientKey.IsEmpty())
    {
      Crypto crypt;
      clientKey = crypt.Encryption(clientKey,_T(KWATTA_ENCRYPT));
    }
    msg.AddElement(credelem,_T("TypeName"),   XDT_String,cred.second.m_typeName);
    msg.AddElement(credelem,_T("Identifier"), XDT_String,cred.second.m_identifier);
    msg.AddElement(credelem,_T("Username"),   XDT_String,cred.second.m_username);
    msg.AddElement(credelem,_T("Password"),   XDT_String,password);
    msg.AddElement(credelem,_T("OAuth2Grant"),XDT_String,cred.second.m_oauthGrant);
    msg.AddElement(credelem,_T("TokenServer"),XDT_String,cred.second.m_tokenServer);
    msg.AddElement(credelem,_T("ClientID"),   XDT_String,cred.second.m_clientID);
    msg.AddElement(credelem,_T("ClientKey"),  XDT_String,clientKey);
    msg.AddElement(credelem,_T("ClientScope"),XDT_String,cred.second.m_clientScope);
  }

  // Now save all the database connections
  for(auto& conn : m_connections)
  {
    XMLElement* connelem = msg.AddElement(nullptr,_T("Connection"));
    
    XString password(conn.second.m_password);
    if(!password.IsEmpty())
    {
      Crypto crypt;
      password = crypt.Encryption(password,_T(KWATTA_ENCRYPT));
    }
    msg.AddElement(connelem,_T("Identifier"), XDT_String,conn.second.m_identifier);
    msg.AddElement(connelem,_T("Datasource"), XDT_String,conn.second.m_datasource);
    msg.AddElement(connelem,_T("Username"),   XDT_String,conn.second.m_username);
    msg.AddElement(connelem,_T("Password"),   XDT_String,password);
  }

  // Now save it to file
  if(msg.SaveFile(m_filename))
  {
    m_changed = false;
    return true;
  }
  return false;
}

// Finding a set of credentials
Credential* 
Credentials::FindCredential(XString p_identifier)
{
  CredentialMap::iterator it = m_credentials.find(p_identifier);
  if(it != m_credentials.end())
  {
    return &(it->second);
  }
  return nullptr;
}

// Remove a credentials set
bool
Credentials::DeleteCredentials(XString p_identifier)
{
  CredentialMap::iterator it = m_credentials.find(p_identifier);
  if(it != m_credentials.end())
  {
    m_credentials.erase(it);
    return (m_changed = true);
  }
  return false;
}

  // Adding credentials (add and update)
bool
Credentials::SetBasicCredential(XString p_identifier,XString p_username,XString p_password)
{
  if(FindCredential(p_identifier))
  {
    return false;
  }

  Credential cred;
  cred.m_type       = CredType::BASIC;
  cred.m_typeName   = _T("Basic");
  cred.m_username   = p_username;
  cred.m_password   = p_password;
  cred.m_identifier = p_identifier;

  m_credentials[p_identifier] = cred;
  return (m_changed = true);
}

bool
Credentials::SetWNTLMCredential(XString p_identifier,XString p_username /*= _T("")*/,XString p_password /*= _T("")*/)
{
  if(FindCredential(p_identifier))
  {
    return false;
  }

  Credential cred;
  cred.m_type       = CredType::NTLM;
  cred.m_typeName   = _T("NTLM Logon");
  cred.m_username   = p_username;
  cred.m_password   = p_password;
  cred.m_identifier = p_identifier;

  if(p_username.IsEmpty() && p_password.IsEmpty())
  {
    cred.m_type = CredType::NTLM_SSO;
    cred.m_typeName = _T("NTLM Single-signon");
  }
  m_credentials[p_identifier] = cred;
  return (m_changed = true);
}

bool
Credentials::SetOAuthCredential(XString p_identifier,XString p_grant,XString p_tserver,XString p_id,XString p_key,XString p_scope)
{
  if(FindCredential(p_identifier))
  {
    return false;
  }

  Credential cred;
  cred.m_type        = CredType::OAUTH2;
  cred.m_typeName    = _T("OAuth2");
  cred.m_oauthGrant  = p_grant;
  cred.m_tokenServer = p_tserver;
  cred.m_clientID    = p_id;
  cred.m_clientKey   = p_key;
  cred.m_clientScope = p_scope;
  cred.m_identifier  = p_identifier;

  m_credentials[p_identifier] = cred;
  return (m_changed = true);
}

// Convert strings for credential types

XString
Credentials::CredTypeToString(CredType p_type)
{
  XString type;
  switch(p_type)
  {
    case CredType::BASIC:     type = _T("Basic");             break;
    case CredType::NTLM_SSO:  type = _T("NTLM Single-signon");break;
    case CredType::NTLM:      type = _T("NTLM Logon");        break;
    case CredType::OAUTH2:    type = _T("OAuth2");            break;
    default:                  type = _T("Unknown");           break;
  }
  return type;
}

CredType
Credentials::StringToCredType(XString p_type)
{
  if(p_type.Compare(_T("Basic"))              == 0) return CredType::BASIC;
  if(p_type.Compare(_T("NTLM Single-signon")) == 0) return CredType::NTLM_SSO;
  if(p_type.Compare(_T("NTLM Login"))         == 0) return CredType::NTLM;
  if(p_type.Compare(_T("OAuth2"))             == 0) return CredType::OAUTH2;

  return CredType::ANONYMOUS;
}

//////////////////////////////////////////////////////////////////////////
//
// Database connections
//
//////////////////////////////////////////////////////////////////////////

DBSConnect* 
Credentials::FindConnection(XString p_identifier)
{
  DBSConnectMap::iterator it = m_connections.find(p_identifier);
  if(it != m_connections.end())
  {
    return &(it->second);
  }
  return nullptr;
}

bool
Credentials::SetODBCConnection(XString p_identifier,XString p_datasource,XString p_username,XString p_password)
{
  if(FindConnection(p_identifier))
  {
    return false;
  }
  DBSConnect conn;
  conn.m_identifier = p_identifier;
  conn.m_datasource = p_datasource;
  conn.m_username   = p_username;
  conn.m_password   = p_password;

  m_connections[p_identifier] = conn;
  return (m_changed = true);
}

bool
Credentials::DeleteConnection(XString p_identifier)
{
  DBSConnectMap::iterator it = m_connections.find(p_identifier);
  if(it != m_connections.end())
  {
    m_connections.erase(it);
    return (m_changed = true);
  }
  return false;
}

