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
// This File   : ValidateIN.h
// What it does: A validation step for a teststep. The validation of a HTTP step
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Validate.h"

class ValidateNET : public Validate
{
public:
  ValidateNET() = default;
  virtual ~ValidateNET() = default;

  // Interface with the file system
  virtual void    ReadFromXML(XString p_filename) override;  // Throws in case of an error
  virtual bool    WriteToXML(XString p_filename) override;
  // RE-Calculate the effective strings, returning the number of unbound parameters
  virtual int     EffectiveReplacements(Parameters* p_parameters,bool p_forDisplay);
  // Check our filenames extension
  virtual void    CheckFilename(XString p_filename) override;

  // OUR MAIN FUNCTION: Performing our VALIDATIONS
  bool            ValidateStatusValue(Parameters* p_parameters,int p_status);
  bool            ValidateHeaderValue(Parameters* p_parameters,XString p_headerValue);
  bool            ValidateBodyValue  (Parameters* p_parameters,XString p_body);
  bool            ValidateXMLValue   (Parameters* p_parameters,XString p_body);
  bool            ValidateJSONValue  (Parameters* p_parameters,XString p_body);

  // GETTERS
  bool            GetCheckStatus()          { return m_checkStatus;         }
  bool            GetCheckHeader()          { return m_checkHeader;         }
  bool            GetCheckBody()            { return m_checkBody;           }
  bool            GetCheckXML()             { return m_checkXML;            }
  bool            GetCheckJSON()            { return m_checkJSON;           }
  ReturnOperator  GetStatusOperator()       { return m_statusOperator;      }
  BufferOperator  GetHeaderOperator()       { return m_headerOperator;      }
  BufferOperator  GetBodyOperator()         { return m_bodyOperator;        }
  BufferOperator  GetXMLOperator()          { return m_xmlOperator;         }
  BufferOperator  GetJSONOperator()         { return m_jsonOperator;        }
  XString         GetExpectedStatus()       { return m_expectedStatus;      }
  XString         GetVerifyHeader()         { return m_verifyHeader;        }
  XString         GetExpectedHeaderValue()  { return m_expectedHeader;      }
  XString         GetExpectedBody()         { return m_expectedBody;        }
  XString         GetVerifyXmlPath()        { return m_verifyXmlPath;       }
  XString         GetExpectedXMLValue()     { return m_expectedXML;         }
  XString         GetVerifyJSONPath()       { return m_verifyJSONPath;      }
  XString         GetExpectedJSONValue()    { return m_expectedJSON;        }
  XString         GetStatusVariable()       { return m_statusVariable;      }
  XString         GetHeaderVariable()       { return m_headerVariable;      }
  XString         GetBodyVariable()         { return m_bodyVariable;        }
  XString         GetXmlVariable()          { return m_xmlVariable;         }
  XString         GetJsonVariable()         { return m_jsonVariable;        }
  // Total effective returned values
  XString         GetEffectiveStatus()      { return m_effectiveStatus;     }
  XString         GetEffectiveHeaderValue() { return m_effectiveHeaderValue;}
  XString         GetEffectiveBody()        { return m_effectiveBody;       }
  XString         GetEffectiveXMLValue()    { return m_effectiveXMLValue;   }
  XString         GetEffectiveJSONValue()   { return m_effectiveJSONValue;  }

  // SETTERS
  void      SetCheckStatus(bool p_check)              { m_checkStatus         = p_check;  }
  void      SetCheckHeader(bool p_check)              { m_checkHeader         = p_check;  }
  void      SetCheckBody(bool p_check)                { m_checkBody           = p_check;  }
  void      SetCheckXML(bool p_check)                 { m_checkXML            = p_check;  }
  void      SetCheckJSON(bool p_check)                { m_checkJSON           = p_check;  }
  void      SetStatusOperator(ReturnOperator p_oper)  { m_statusOperator      = p_oper;   }
  void      SetHeaderOperator(BufferOperator p_oper)  { m_headerOperator      = p_oper;   }
  void      SetBodyOperator  (BufferOperator p_oper)  { m_bodyOperator        = p_oper;   }
  void      SetXMLOperator   (BufferOperator p_oper)  { m_xmlOperator         = p_oper;   }
  void      SetJSONOperator  (BufferOperator p_oper)  { m_jsonOperator        = p_oper;   }
  void      SetExpectedStatus(XString p_status)       { m_expectedStatus      = p_status; }
  void      SetVerifyHeader(XString p_header)         { m_verifyHeader        = p_header; }
  void      SetExpectedHeaderValue(XString p_value)   { m_expectedHeader      = p_value;  }
  void      SetExpectedBody(XString p_body)           { m_expectedBody        = p_body;   }
  void      SetVerifyXmlPath(XString p_path)          { m_verifyXmlPath       = p_path;   }
  void      SetExpectedXMLValue(XString p_value)      { m_expectedXML         = p_value;  }
  void      SetVerifyJSONPath(XString p_path)         { m_verifyJSONPath      = p_path;   }
  void      SetExpectedJSONValue(XString p_value)     { m_expectedJSON        = p_value;  }
  void      SetStatusVariable(XString p_value)        { m_statusVariable      = p_value;  }
  void      SetHeaderVariable(XString p_value)        { m_headerVariable      = p_value;  }
  void      SetBodyVariable(XString p_value)          { m_bodyVariable        = p_value;  }
  void      SetXmlVariable(XString p_value)           { m_xmlVariable         = p_value;  }
  void      SetJsonVariable(XString p_value)          { m_jsonVariable        = p_value;  }

protected:

  // What to check
  bool      m_checkStatus  { false };
  bool      m_checkHeader  { false };
  bool      m_checkBody    { false };
  bool      m_checkXML     { false };
  bool      m_checkJSON    { false };

  // How to check
  ReturnOperator m_statusOperator { ReturnOperator::ROP_EQUAL };
  BufferOperator m_headerOperator { BufferOperator::BOP_NOP   };
  BufferOperator m_bodyOperator   { BufferOperator::BOP_NOP   };
  BufferOperator m_xmlOperator    { BufferOperator::BOP_NOP   };
  BufferOperator m_jsonOperator   { BufferOperator::BOP_NOP   };

  // What we expect
  XString   m_expectedStatus;
  XString   m_verifyHeader;
  XString   m_expectedHeader;
  XString   m_expectedBody;
  XString   m_verifyXmlPath;
  XString   m_expectedXML;
  XString   m_verifyJSONPath;
  XString   m_expectedJSON;

  // Return variables after the test
  XString   m_statusVariable;
  XString   m_headerVariable;
  XString   m_bodyVariable;
  XString   m_xmlVariable;
  XString   m_jsonVariable;

  // What we expect after parameter translation
  XString   m_effectiveStatus;
  XString   m_effectiveHeaderValue;
  XString   m_effectiveBody;
  XString   m_effectiveXMLValue;
  XString   m_effectiveJSONValue;
};
