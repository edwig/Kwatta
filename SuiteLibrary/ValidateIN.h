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

class ValidateIN : public Validate
{
public:
  ValidateIN() = default;
  virtual ~ValidateIN() = default;

  // Interface with the file system
  virtual void    ReadFromXML(CString p_filename) override;  // Throws in case of an error
  virtual bool    WriteToXML(CString p_filename) override;
  // RE-Calculate the effective strings, returning the number of unbound parameters
  virtual int     EffectiveReplacements(Parameters* p_parameters,bool p_forDisplay);
  // Check our filenames extension
  virtual void    CheckFilename(CString p_filename) override;

  // OUR MAIN FUNCTION: Performing our VALIDATIONS
  bool            ValidateStatusValue(Parameters* p_parameters,int p_status);
  bool            ValidateHeaderValue(Parameters* p_parameters,CString p_headerValue);
  bool            ValidateBodyValue  (Parameters* p_parameters,CString p_body);
  bool            ValidateXMLValue   (Parameters* p_parameters,CString p_body);
  bool            ValidateJSONValue  (Parameters* p_parameters,CString p_body);

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
  CString         GetExpectedStatus()       { return m_expectedStatus;      }
  CString         GetVerifyHeader()         { return m_verifyHeader;        }
  CString         GetExpectedHeaderValue()  { return m_expectedHeader;      }
  CString         GetExpectedBody()         { return m_expectedBody;        }
  CString         GetVerifyXmlPath()        { return m_verifyXmlPath;       }
  CString         GetExpectedXMLValue()     { return m_expectedXML;         }
  CString         GetVerifyJSONPath()       { return m_verifyJSONPath;      }
  CString         GetExpectedJSONValue()    { return m_expectedJSON;        }
  CString         GetStatusVariable()       { return m_statusVariable;      }
  CString         GetHeaderVariable()       { return m_headerVariable;      }
  CString         GetBodyVariable()         { return m_bodyVariable;        }
  CString         GetXmlVariable()          { return m_xmlVariable;         }
  CString         GetJsonVariable()         { return m_jsonVariable;        }
  // Total effective returned values
  CString         GetEffectiveStatus()      { return m_effectiveStatus;     }
  CString         GetEffectiveHeaderValue() { return m_effectiveHeaderValue;}
  CString         GetEffectiveBody()        { return m_effectiveBody;       }
  CString         GetEffectiveXMLValue()    { return m_effectiveXMLValue;   }
  CString         GetEffectiveJSONValue()   { return m_effectiveJSONValue;  }

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
  void      SetExpectedStatus(CString p_status)       { m_expectedStatus      = p_status; }
  void      SetVerifyHeader(CString p_header)         { m_verifyHeader        = p_header; }
  void      SetExpectedHeaderValue(CString p_value)   { m_expectedHeader      = p_value;  }
  void      SetExpectedBody(CString p_body)           { m_expectedBody        = p_body;   }
  void      SetVerifyXmlPath(CString p_path)          { m_verifyXmlPath       = p_path;   }
  void      SetExpectedXMLValue(CString p_value)      { m_expectedXML         = p_value;  }
  void      SetVerifyJSONPath(CString p_path)         { m_verifyJSONPath      = p_path;   }
  void      SetExpectedJSONValue(CString p_value)     { m_expectedJSON        = p_value;  }
  void      SetStatusVariable(CString p_value)        { m_statusVariable      = p_value;  }
  void      SetHeaderVariable(CString p_value)        { m_headerVariable      = p_value;  }
  void      SetBodyVariable(CString p_value)          { m_bodyVariable        = p_value;  }
  void      SetXmlVariable(CString p_value)           { m_xmlVariable         = p_value;  }
  void      SetJsonVariable(CString p_value)          { m_jsonVariable        = p_value;  }

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
  CString   m_expectedStatus;
  CString   m_verifyHeader;
  CString   m_expectedHeader;
  CString   m_expectedBody;
  CString   m_verifyXmlPath;
  CString   m_expectedXML;
  CString   m_verifyJSONPath;
  CString   m_expectedJSON;

  // Return variables after the test
  CString   m_statusVariable;
  CString   m_headerVariable;
  CString   m_bodyVariable;
  CString   m_xmlVariable;
  CString   m_jsonVariable;

  // What we expect after parameter translation
  CString   m_effectiveStatus;
  CString   m_effectiveHeaderValue;
  CString   m_effectiveBody;
  CString   m_effectiveXMLValue;
  CString   m_effectiveJSONValue;
};
