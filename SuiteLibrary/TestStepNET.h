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
// This File   : TestStepIN.h
// What it does: One (1) test step. Subclass for a HTTP call
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "TestStep.h"
#include <HTTPClient.h>
#include <vector>

typedef struct _pair
{
  CString m_name;
  CString m_value;
}
INPair;

using UrlParameters = std::vector<INPair>;
using UrlHeaders    = std::vector<INPair>;

class TestStepNET : public TestStep
{
public:
  TestStepNET() = default;
  virtual ~TestStepNET() = default;

  // Interface with the file system
  virtual void    ReadFromXML(CString p_filename) override;  // Throws in case of an error
  virtual bool    WriteToXML(CString p_filename) override;
  // RE-Calculate the effective strings, returning the number of unbound parameters
  virtual int     EffectiveReplacements(Parameters* p_parameters,bool p_forDisplay);
  // Check our filenames extension
  virtual void    CheckFilename(CString p_filename);
  // Reset effective parameter replacements
  virtual void    ResetEffective();

  // GETTERS
  CString         GetVerb()                 { return m_verb;              }
  CString         GetURL()                  { return m_url;               }
  CString         GetAnchor()               { return m_anchor;            }
  CString         GetBody()                 { return m_body;              }
  CString         GetMimeType()             { return m_mimeType;          }
  bool            GetBodyInputIsFile()      { return m_bodyInputIsFile;   }
  bool            GetBodyOutputIsFile()     { return m_bodyOutputIsFile;  }
  CString         GetFilenameInput()        { return m_filenameInput;     }
  CString         GetFilenameOutput()       { return m_filenameOutput;    }

  CString         GetParameter(CString p_parameter);
  CString         GetHeader   (CString p_header);
  UrlParameters&  GetParameters()           { return m_parameters; }
  UrlHeaders&     GetHeaders()              { return m_headers;    }
  CString         GetRawRequest();
  CString         GetCredential()           { return m_credential; }
  CString         GetAuthType()             { return m_authType;   }
  CString         GetAuthUser()             { return m_userName;   }
  CString         GetAuthPassword()         { return m_password;   }
  CString         GetAuthGrant()            { return m_oauthGrant; }
  CString         GetAuthTokenServer()      { return m_tokenServer;}
  CString         GetAuthClientID()         { return m_clientID;   }
  CString         GetAuthClientKey()        { return m_clientKey;  }
  CString         GetAuthClientScope()      { return m_clientScope;}
  CString         GetAuthBearerToken()      { return m_bearerToken;}
  CString         GetKeyHeaderName()        { return m_keyHeader;  }
  CString         GetKeyHeaderValue()       { return m_keyValue;   }
  bool            GetUseStatus()            { return m_useStatus;  }
  bool            GetUseHeaders()           { return m_useHeaders; }
  bool            GetUseBody()              { return m_useBody;    }
  CString         GetEffectiveURL()         { return m_effectiveUrl;        }
  CString         GetEffectiveAnchor()      { return m_effectiveAnchor;     }
  CString         GetEffectiveBody()        { return m_effectiveBody;       }
  CString         GetEffectiveFileInput()   { return m_effectiveFileInput;  }
  CString         GetEffectiveFileOutput()  { return m_effectiveFileOutput; }
  UrlParameters&  GetEffectiveParameters()  { return m_effectiveParameters; }
  UrlHeaders&     GetEffectiveHeaders()     { return m_effectiveHeaders;    }
  int             GetTimeoutResolve()       { return m_timeoutResolve;      };
  int             GetTimeoutConnect()       { return m_timeoutConnect;      };
  int             GetTimeoutSend()          { return m_timeoutSend;         };
  int             GetTimeoutReceive()       { return m_timeoutReceive;      };
  CString         GetEffectiveCombinedURL();
  
  // SETTERS
  void      SetVerb  (CString p_verb)             { m_verb              = p_verb;       }
  void      SetURL   (CString p_url)              { m_url               = p_url;        }
  void      SetAnchor(CString p_anchor)           { m_anchor            = p_anchor;     }
  void      SetBody  (CString p_body)             { m_body              = p_body;       }
  void      SetBodyInputIsFile (bool p_file)      { m_bodyInputIsFile   = p_file;       }
  void      SetBodyOutputIsFile(bool p_file)      { m_bodyOutputIsFile  = p_file;       }
  void      SetFilenameInput(CString p_file)      { m_filenameInput     = p_file;       }
  void      SetFilenameOutput(CString p_file)     { m_filenameOutput    = p_file;       }
  void      SetMimeType(CString p_type)           { m_mimeType          = p_type;       }
  void      SetParameter(CString p_parameter,CString p_value);
  void      SetHeader   (CString p_parameter,CString p_value);
  void      SetAuthType(CString p_type)           { m_authType          = p_type;       }
  void      SetAuthUser(CString p_user)           { m_userName          = p_user;       }
  void      SetAuthPassword(CString p_passsword)  { m_password          = p_passsword;  }
  void      SetAuthGrant(CString p_grant)         { m_oauthGrant        = p_grant;      }
  void      SetAuthTokenServer(CString p_server)  { m_tokenServer       = p_server;     }
  void      SetAuthClientID(CString p_clientID)   { m_clientID          = p_clientID;   }
  void      SetAuthClientKey(CString p_key)       { m_clientKey         = p_key;        }
  void      SetAuthClientScope(CString p_scope)   { m_clientScope       = p_scope;      }
  void      SetAuthBearerToken(CString p_token)   { m_bearerToken       = p_token;      }
  void      SetKeyHeaderName(CString p_name)      { m_keyHeader         = p_name;       }
  void      SetKeyHeaderValue(CString p_value)    { m_keyValue          = p_value;      }
  void      SetUseStatus(bool p_use)              { m_useStatus         = p_use;        }
  void      SetUseHeaders(bool p_use)             { m_useHeaders        = p_use;        }
  void      SetUseBody(bool p_use)                { m_useBody           = p_use;        }
  void      SetTimeoutResolve(int p_timeout)      { m_timeoutResolve    = p_timeout;    }
  void      SetTimeoutConnect(int p_timeout)      { m_timeoutConnect    = p_timeout;    }
  void      SetTimeoutSend   (int p_timeout)      { m_timeoutSend       = p_timeout;    }
  void      SetTimeoutReceive(int p_timeout)      { m_timeoutReceive    = p_timeout;    }
  void      SetCredential(CString p_credential);

  void      DeleteParameter(CString p_parameter);

protected:
  CString       m_verb;
  CString       m_url;
  CString       m_anchor;
  UrlParameters m_parameters;
  UrlHeaders    m_headers;
  CString       m_body;
  CString       m_mimeType;
  bool          m_bodyInputIsFile;
  bool          m_bodyOutputIsFile;
  CString       m_filenameInput;
  CString       m_filenameOutput;

  // Call timeouts
  unsigned      m_timeoutResolve  { DEF_TIMEOUT_RESOLVE };        // Timeout resolving URL
  unsigned      m_timeoutConnect  { DEF_TIMEOUT_CONNECT };        // Timeout in connecting to URL
  unsigned      m_timeoutSend     { DEF_TIMEOUT_SEND    };        // Timeout in sending 
  unsigned      m_timeoutReceive  { DEF_TIMEOUT_RECEIVE };        // Timeout in receiving

  // Credential template
  CString       m_credential;     // Short name of the credential set
  // Our own authentication set
  CString       m_authType;       // Type of authentication
  CString       m_userName;       // User name - Basic or NTLM
  CString       m_password;       // Password  - Basic or NTL
  CString       m_oauthGrant;     // OAuth2 grant type
  CString       m_tokenServer;    // OAuth2 token server
  CString       m_clientID;       // OAuth2 CCG client-id
  CString       m_clientKey;      // OAuth2 CCG client-key
  CString       m_clientScope;    // OAuth2 CCG client-scope
  CString       m_bearerToken;    // OAuth2 effective bearer token
  CString       m_keyHeader;      // X-KEY authorization header name
  CString       m_keyValue;       // X-KEY authorization header value

  // After replacement
  CString       m_effectiveUrl;
  CString       m_effectiveAnchor;
  CString       m_effectiveBody;
  CString       m_effectiveFileInput;
  CString       m_effectiveFileOutput;
  UrlParameters m_effectiveParameters;
  UrlHeaders    m_effectiveHeaders;

  // How to execute
  bool          m_useStatus  { false };
  bool          m_useHeaders { false };
  bool          m_useBody    { false };
};
