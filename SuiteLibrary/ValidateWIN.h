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
// This File   : ValidateSQL.h
// What it does: A validation step for a teststep. The validation of a Windows UIstep
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Validate.h"
#include <map>

// Column name and string value of the returned data cell
using ColumnData = std::map<CString, CString>;

class ValidateWIN : public Validate
{
public:
  ValidateWIN() = default;
  virtual ~ValidateWIN() = default;

  // Interface with the file system
  virtual void    ReadFromXML(CString p_filename) override;  // Throws in case of an error
  virtual bool    WriteToXML (CString p_filename) override;
  // RE-Calculate the effective strings, returning the number of unbound parameters
  virtual int     EffectiveReplacements(Parameters* p_parameters, bool p_forDisplay);
  // Check our filenames extension
  virtual void    CheckFilename(CString p_filename) override;

  // OUR MAIN FUNCTION: Performing our VALIDATIONS
  bool            ValidateReturnValue(int p_value);
  bool            ValidateLogging(CString p_buffer);
  bool            ValidateErrors(CString p_buffer);

  // GETTERS
  bool            GetReturnValueIsSigned()  { return m_returnIsSigned;    }
  bool            GetCheckReturnValue()     { return m_checkReturnValue;  }
  bool            GetCheckLoggingValue()    { return m_checkLogging;      }
  bool            GetCheckErrorsValue()     { return m_checkErrors;       }
  ReturnOperator  GetReturnOperator()       { return m_returnOperator;    }
  BufferOperator  GetLoggingOperator()      { return m_loggingOperator;   }
  BufferOperator  GetErrorsOperator()       { return m_errorsOperator;    }
  CString         GetExpectedReturn()       { return m_expectedReturn;    }
  CString         GetExpectedLogging()      { return m_expectedLogging;   }
  CString         GetExpectedErrors()       { return m_expectedErrors;    }
  CString         GetEffectiveReturn()      { return m_effectiveReturn;   }
  CString         GetEffectiveLogging()     { return m_effectiveLogging;  }
  CString         GetEffectiveErrors()      { return m_effectiveErrors;   }

  // SETTERS
  void            SetReturnValueIsSigned(bool p_signed)    { m_returnIsSigned   = p_signed;   }
  void            SetCheckReturnValue(bool p_check)        { m_checkReturnValue = p_check;    }
  void            SetCheckLoggingValue(bool p_check)       { m_checkLogging     = p_check;    }
  void            SetCheckErrorsValue(bool p_check)        { m_checkErrors      = p_check;    }
  void            SetReturnOperator (ReturnOperator p_oper){ m_returnOperator   = p_oper;     }
  void            SetLoggingOperator(BufferOperator p_oper){ m_loggingOperator  = p_oper;     }
  void            SetErrorsOperator (BufferOperator p_oper){ m_errorsOperator   = p_oper;     }
  void            SetExpectedReturn (CString p_retval)     { m_expectedReturn   = p_retval;   }
  void            SetExpectedLogging(CString p_buffer)     { m_expectedLogging  = p_buffer;   }
  void            SetExpectedErrors (CString p_buffer)     { m_expectedErrors   = p_buffer;   }

protected:

  // Definition
  bool            m_returnIsSigned  { false };
  bool            m_checkReturnValue{ false };
  bool            m_checkLogging    { false };
  bool            m_checkErrors     { false };

  ReturnOperator  m_returnOperator  { ReturnOperator::ROP_EQUAL   };
  BufferOperator  m_loggingOperator { BufferOperator::BOP_EXACT   };
  BufferOperator  m_errorsOperator  { BufferOperator::BOP_ISEMPTY };

  CString         m_expectedReturn;
  CString         m_expectedLogging;
  CString         m_expectedErrors;

  CString         m_effectiveReturn;
  CString         m_effectiveLogging;
  CString         m_effectiveErrors;
};