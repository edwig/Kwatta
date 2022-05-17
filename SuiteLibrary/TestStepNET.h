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
  CString         GetAuthType()             { return m_authType;   }
  CString         GetAuthUser()             { return m_userName;   }
  CString         GetAuthPassword()         { return m_password;   }
  CString         GetAuthGrant()            { return m_oauthGrant; }
  CString         GetAuthTokenServer()      { return m_tokenServer;}
  CString         GetAuthClientID()         { return m_clientID;   }
  CString         GetAuthClientKey()        { return m_clientKey;  }
  CString         GetAuthClientScope()      { return m_clientScope;}
  CString         GetAuthBearerToken()      { return m_bearerToken;}
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
  void      SetUseStatus(bool p_use)              { m_useStatus         = p_use;        }
  void      SetUseHeaders(bool p_use)             { m_useHeaders        = p_use;        }
  void      SetUseBody(bool p_use)                { m_useBody           = p_use;        }

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

  // Authenticate
  CString       m_authType;
  CString       m_userName;
  CString       m_password;
  CString       m_oauthGrant;
  CString       m_tokenServer;
  CString       m_clientID;
  CString       m_clientKey;
  CString       m_clientScope;
  CString       m_bearerToken;

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
