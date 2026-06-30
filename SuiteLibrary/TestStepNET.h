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
  XString m_name;
  XString m_value;
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
  virtual void    ReadFromXML(XString p_filename) override;  // Throws in case of an error
  virtual bool    WriteToXML(XString p_filename) override;
  // RE-Calculate the effective strings, returning the number of unbound parameters
  virtual int     EffectiveReplacements(Parameters* p_parameters,bool p_forDisplay);
  // Check our filenames extension
  virtual void    CheckFilename(XString p_filename);
  // Reset effective parameter replacements
  virtual void    ResetEffective();

  // GETTERS
  XString         GetVerb()                 { return m_verb;              }
  XString         GetURL()                  { return m_url;               }
  XString         GetAnchor()               { return m_anchor;            }
  XString         GetBody()                 { return m_body;              }
  XString         GetMimeType()             { return m_mimeType;          }
  bool            GetBodyInputIsFile()      { return m_bodyInputIsFile;   }
  bool            GetBodyOutputIsFile()     { return m_bodyOutputIsFile;  }
  XString         GetFilenameInput()        { return m_filenameInput;     }
  XString         GetFilenameOutput()       { return m_filenameOutput;    }

  XString         GetParameter(XString p_parameter);
  XString         GetHeader   (XString p_header);
  UrlParameters&  GetParameters()           { return m_parameters; }
  UrlHeaders&     GetHeaders()              { return m_headers;    }
  XString         GetRawRequest();
  XString         GetCredential()           { return m_credential; }
  XString         GetAuthType()             { return m_authType;   }
  XString         GetAuthUser()             { return m_userName;   }
  XString         GetAuthPassword()         { return m_password;   }
  XString         GetAuthGrant()            { return m_oauthGrant; }
  XString         GetAuthTokenServer()      { return m_tokenServer;}
  XString         GetAuthClientID()         { return m_clientID;   }
  XString         GetAuthClientKey()        { return m_clientKey;  }
  XString         GetAuthClientScope()      { return m_clientScope;}
  XString         GetAuthBearerToken()      { return m_bearerToken;}
  XString         GetKeyHeaderName()        { return m_keyHeader;  }
  XString         GetKeyHeaderValue()       { return m_keyValue;   }
  bool            GetUseStatus()            { return m_useStatus;  }
  bool            GetUseHeaders()           { return m_useHeaders; }
  bool            GetUseBody()              { return m_useBody;    }
  XString         GetEffectiveURL()         { return m_effectiveUrl;        }
  XString         GetEffectiveAnchor()      { return m_effectiveAnchor;     }
  XString         GetEffectiveBody()        { return m_effectiveBody;       }
  XString         GetEffectiveFileInput()   { return m_effectiveFileInput;  }
  XString         GetEffectiveFileOutput()  { return m_effectiveFileOutput; }
  UrlParameters&  GetEffectiveParameters()  { return m_effectiveParameters; }
  UrlHeaders&     GetEffectiveHeaders()     { return m_effectiveHeaders;    }
  int             GetTimeoutResolve()       { return m_timeoutResolve;      };
  int             GetTimeoutConnect()       { return m_timeoutConnect;      };
  int             GetTimeoutSend()          { return m_timeoutSend;         };
  int             GetTimeoutReceive()       { return m_timeoutReceive;      };
  XString         GetEffectiveCombinedURL();
  
  // SETTERS
  void      SetVerb  (XString p_verb)             { m_verb              = p_verb;       }
  void      SetURL   (XString p_url)              { m_url               = p_url;        }
  void      SetAnchor(XString p_anchor)           { m_anchor            = p_anchor;     }
  void      SetBody  (XString p_body)             { m_body              = p_body;       }
  void      SetBodyInputIsFile (bool p_file)      { m_bodyInputIsFile   = p_file;       }
  void      SetBodyOutputIsFile(bool p_file)      { m_bodyOutputIsFile  = p_file;       }
  void      SetFilenameInput(XString p_file)      { m_filenameInput     = p_file;       }
  void      SetFilenameOutput(XString p_file)     { m_filenameOutput    = p_file;       }
  void      SetMimeType(XString p_type)           { m_mimeType          = p_type;       }
  void      SetParameter(XString p_parameter,XString p_value);
  void      SetHeader   (XString p_parameter,XString p_value);
  void      SetAuthType(XString p_type)           { m_authType          = p_type;       }
  void      SetAuthUser(XString p_user)           { m_userName          = p_user;       }
  void      SetAuthPassword(XString p_passsword)  { m_password          = p_passsword;  }
  void      SetAuthGrant(XString p_grant)         { m_oauthGrant        = p_grant;      }
  void      SetAuthTokenServer(XString p_server)  { m_tokenServer       = p_server;     }
  void      SetAuthClientID(XString p_clientID)   { m_clientID          = p_clientID;   }
  void      SetAuthClientKey(XString p_key)       { m_clientKey         = p_key;        }
  void      SetAuthClientScope(XString p_scope)   { m_clientScope       = p_scope;      }
  void      SetAuthBearerToken(XString p_token)   { m_bearerToken       = p_token;      }
  void      SetKeyHeaderName(XString p_name)      { m_keyHeader         = p_name;       }
  void      SetKeyHeaderValue(XString p_value)    { m_keyValue          = p_value;      }
  void      SetUseStatus(bool p_use)              { m_useStatus         = p_use;        }
  void      SetUseHeaders(bool p_use)             { m_useHeaders        = p_use;        }
  void      SetUseBody(bool p_use)                { m_useBody           = p_use;        }
  void      SetTimeoutResolve(int p_timeout)      { m_timeoutResolve    = p_timeout;    }
  void      SetTimeoutConnect(int p_timeout)      { m_timeoutConnect    = p_timeout;    }
  void      SetTimeoutSend   (int p_timeout)      { m_timeoutSend       = p_timeout;    }
  void      SetTimeoutReceive(int p_timeout)      { m_timeoutReceive    = p_timeout;    }
  void      SetCredential(XString p_credential);

  void      DeleteParameter(XString p_parameter);

protected:
  XString       m_verb;
  XString       m_url;
  XString       m_anchor;
  UrlParameters m_parameters;
  UrlHeaders    m_headers;
  XString       m_body;
  XString       m_mimeType;
  bool          m_bodyInputIsFile;
  bool          m_bodyOutputIsFile;
  XString       m_filenameInput;
  XString       m_filenameOutput;

  // Call timeouts
  unsigned      m_timeoutResolve  { DEF_TIMEOUT_RESOLVE };        // Timeout resolving URL
  unsigned      m_timeoutConnect  { DEF_TIMEOUT_CONNECT };        // Timeout in connecting to URL
  unsigned      m_timeoutSend     { DEF_TIMEOUT_SEND    };        // Timeout in sending 
  unsigned      m_timeoutReceive  { DEF_TIMEOUT_RECEIVE };        // Timeout in receiving

  // Credential template
  XString       m_credential;     // Short name of the credential set
  // Our own authentication set
  XString       m_authType;       // Type of authentication
  XString       m_userName;       // User name - Basic or NTLM
  XString       m_password;       // Password  - Basic or NTL
  XString       m_oauthGrant;     // OAuth2 grant type
  XString       m_tokenServer;    // OAuth2 token server
  XString       m_clientID;       // OAuth2 CCG client-id
  XString       m_clientKey;      // OAuth2 CCG client-key
  XString       m_clientScope;    // OAuth2 CCG client-scope
  XString       m_bearerToken;    // OAuth2 effective bearer token
  XString       m_keyHeader;      // X-KEY authorization header name
  XString       m_keyValue;       // X-KEY authorization header value

  // After replacement
  XString       m_effectiveUrl;
  XString       m_effectiveAnchor;
  XString       m_effectiveBody;
  XString       m_effectiveFileInput;
  XString       m_effectiveFileOutput;
  UrlParameters m_effectiveParameters;
  UrlHeaders    m_effectiveHeaders;

  // How to execute
  bool          m_useStatus  { false };
  bool          m_useHeaders { false };
  bool          m_useBody    { false };
};
