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
// This File   : Credentials.h
// What it does: Store global credentials for all test sets and steps
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <map>

// Currently recognized authentication credentials
enum class CredType
{
  ANONYMOUS  = 0
 ,BASIC
 ,NTLM_SSO
 ,NTLM
 ,OAUTH2
 ,HEADER
};

// Reserved credentials type names
#define CREDNAME_ANONYMOUS  _T("Anonymous")
#define CREDNAME_BASIC      _T("Basic authentication")
#define CREDNAME_NTLM_SSO   _T("NTLM Single-signon")
#define CREDNAME_NTLM       _T("NTLM Logon")
#define CREDNAME_OAUTH2     _T("OAuth2")
#define CREDNAME_HEADER     _T("Key-Header")

// Saved credentials structure
typedef struct _cred
{
  CredType  m_type;         // Type to handle
  XString   m_typeName;     // Type in human readable form
  XString   m_identifier;   // Stored under this identifier
  XString   m_username;
  XString   m_password;
  XString   m_oauthGrant;
  XString   m_tokenServer;
  XString   m_clientID;
  XString   m_clientKey;
  XString   m_clientScope;
  XString   m_keyHeader;
  XString   m_keyValue;
}
Credential;

typedef struct _dbconn
{
  XString  m_identifier;  // Stored under this identifier
  XString  m_datasource;  // ODBC Datasource name (DNS)
  XString  m_username;    // Username for the database
  XString  m_password;    // Password for the database
}
DBSConnect;

// Saved credentials by name
using CredentialMap = std::map<XString,Credential>;
using DBSConnectMap = std::map<XString,DBSConnect>;

//////////////////////////////////////////////////////////////////////////
//
// The credentials
//
class Credentials
{
public:
  Credentials() = default;

  // Reset all credentials
  void  Reset();
  // Force changed
  void  SetChanged() { m_changed = true;  }
  bool  GetChanged() { return m_changed;  }

  // Interface with the file system
  void  ReadFromXML(XString p_filename);
  bool  WriteToXML();

  // Finding a set of credentials
  Credential*   FindCredential(XString p_identifier);
  DBSConnect*   FindConnection(XString p_identifier);

  // Adding credentials (add and update)
  bool          SetBasicCredential(XString p_identifier,XString p_username,XString p_password);
  bool          SetWNTLMCredential(XString p_identifier,XString p_username = _T(""),XString p_password = _T(""));
  bool          SetOAuthCredential(XString p_identifier,XString p_grant,XString p_tserver,XString p_id,XString p_key,XString p_scope);
  bool          SetKeyHdCredential(XString p_identifier,XString p_headerName,XString p_headervalue);
  bool          SetODBCConnection (XString p_identifier,XString p_datasource,XString p_username,XString p_password);

  // Remove a credentials set
  bool          DeleteCredentials(XString p_identifier);
  bool          DeleteConnection (XString p_identifier);

  // Convert strings for credential types
  static XString  CredTypeToString(CredType p_type);
  static CredType StringToCredType(XString  p_type);

  CredentialMap& GetAllCredentials() { return m_credentials; }
  DBSConnectMap& GetAllConnections() { return m_connections; }

private:
  bool          m_changed { false };
  XString       m_filename;
  CredentialMap m_credentials;
  DBSConnectMap m_connections;
};
