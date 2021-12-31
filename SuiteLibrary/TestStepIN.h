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

class TestStepIN : public TestStep
{
public:
  TestStepIN() = default;
  virtual ~TestStepIN() = default;

  // Interface with the file system
  virtual void    ReadFromXML(CString p_filename) override;  // Throws in case of an error
  virtual bool    WriteToXML(CString p_filename) override;
  // RE-Calculate the effective strings, returning the number of unbound parameters
  virtual int     EffectiveReplacements(Parameters* p_parameters);
  // Check our filenames extension
  virtual void    CheckFilename(CString p_filename);
  // Reset effective parameter replacements
  virtual void    ResetEffective();

  // GETTERS
  CString         GetVerb()                 { return m_verb;    }
  CString         GetURL()                  { return m_url;     }
  CString         GetAnchor()               { return m_anchor;  }
  CString         GetBody()                 { return m_body;    }
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
  UrlParameters&  GetEffectiveParameters()  { return m_effectiveParameters; }
  UrlHeaders&     GetEffectiveHeaders()     { return m_effectiveHeaders;    }
  CString         GetEffectiveCombinedURL();
  
  // SETTERS
  void      SetVerb  (CString p_verb)   { m_verb    = p_verb;   }
  void      SetURL   (CString p_url)    { m_url     = p_url;    }
  void      SetAnchor(CString p_anchor) { m_anchor  = p_anchor; }
  void      SetBody  (CString p_body)   { m_body    = p_body;   }
  void      SetParameter(CString p_parameter,CString p_value);
  void      SetHeader   (CString p_parameter,CString p_value);
  void      SetAuthType(CString p_type)           { m_authType    = p_type;       }
  void      SetAuthUser(CString p_user)           { m_userName    = p_user;       }
  void      SetAuthPassword(CString p_passsword)  { m_password    = p_passsword;  }
  void      SetAuthGrant(CString p_grant)         { m_oauthGrant  = p_grant;      }
  void      SetAuthTokenServer(CString p_server)  { m_tokenServer = p_server;     }
  void      SetAuthClientID(CString p_clientID)   { m_clientID    = p_clientID;   }
  void      SetAuthClientKey(CString p_key)       { m_clientKey   = p_key;        }
  void      SetAuthClientScope(CString p_scope)   { m_clientScope = p_scope;      }
  void      SetAuthBearerToken(CString p_token)   { m_bearerToken = p_token;      }
  void      SetUseStatus(bool p_use)              { m_useStatus   = p_use;        }
  void      SetUseHeaders(bool p_use)             { m_useHeaders  = p_use;        }
  void      SetUseBody(bool p_use)                { m_useBody     = p_use;        }

  void      DeleteParameter(CString p_parameter);

protected:
  CString       m_verb;
  CString       m_url;
  CString       m_anchor;
  UrlParameters m_parameters;
  UrlHeaders    m_headers;
  CString       m_body;

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
  UrlParameters m_effectiveParameters;
  UrlHeaders    m_effectiveHeaders;

  // How to execute
  bool          m_useStatus  { false };
  bool          m_useHeaders { false };
  bool          m_useBody    { false };
};
