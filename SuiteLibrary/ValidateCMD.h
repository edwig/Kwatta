﻿///////////////////////////////////////////////////////////////////////////////////////////////////////
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
// This File   : ValidateCL.h
// What it does: A validation step for a teststep. The validation of a command-line step
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Validate.h"

class ValidateCMD : public Validate
{
public:
  ValidateCMD() = default;
  virtual ~ValidateCMD() = default;

  // Interface with the file system
  virtual void    ReadFromXML(CString p_filename) override;  // Throws in case of an error
  virtual bool    WriteToXML (CString p_filename) override;
  // RE-Calculate the effective strings, returning the number of unbound parameters
  virtual int     EffectiveReplacements(Parameters* p_parameters,bool p_forDisplay);
  // Check our filenames extension
  virtual void    CheckFilename(CString p_filename) override;

  // OUR MAIN FUNCTION: Performing our VALIDATIONS
  bool            ValidateReturnValue(int p_value);
  bool            ValidateOutputBuffer(CString p_buffer);
  bool            ValidateErrorBuffer(CString p_buffer);

  // GETTERS
  bool            GetReturnValueIsSigned()  { return m_returnIsSigned;    }
  bool            GetCheckReturnValue()     { return m_checkReturnValue;  }
  bool            GetCheckOutputValue()     { return m_checkOutput;       }
  bool            GetCheckErrorValue()      { return m_checkError;        }
  ReturnOperator  GetReturnOperator()       { return m_returnOperator;    }
  BufferOperator  GetOutputOperator()       { return m_outputOperator;    }
  BufferOperator  GetErrorOperator()        { return m_errorOperator;     }
  CString         GetExpectedReturn()       { return m_expectedReturn;    }
  CString         GetExpectedOutput()       { return m_expectedOutput;    }
  CString         GetExpectedError()        { return m_expectedError;     }
  CString         GetEffectiveReturn()      { return m_effectiveReturn;   }
  CString         GetEffectiveOutput()      { return m_effectiveOutput;   }
  CString         GetEffectiveError()       { return m_effectiveError;    }

  // SETTERS
  void            SetReturnValueIsSigned(bool p_signed)   { m_returnIsSigned    = p_signed;   }
  void            SetCheckReturnValue(bool p_check)       { m_checkReturnValue  = p_check;    }
  void            SetCheckOutputValue(bool p_check)       { m_checkOutput       = p_check;    }
  void            SetCheckErrorValue(bool p_check)        { m_checkError        = p_check;    }
  void            SetReturnOperator(ReturnOperator p_oper){ m_returnOperator    = p_oper;     }
  void            SetOutputOperator(BufferOperator p_oper){ m_outputOperator    = p_oper;     }
  void            SetErrorOperator (BufferOperator p_oper){ m_errorOperator     = p_oper;     }
  void            SetExpectedReturn(CString p_retval)     { m_expectedReturn    = p_retval;   }
  void            SetExpectedOutput(CString p_buffer)     { m_expectedOutput    = p_buffer;   }
  void            SetExpectedError (CString p_buffer)     { m_expectedError     = p_buffer;   }

protected:

  // Definition
  bool            m_returnIsSigned  { true };
  bool            m_checkReturnValue{ true };
  bool            m_checkOutput     { true };
  bool            m_checkError      { true };

  ReturnOperator  m_returnOperator { ReturnOperator::ROP_EQUAL   };
  BufferOperator  m_outputOperator { BufferOperator::BOP_EXACT   };
  BufferOperator  m_errorOperator  { BufferOperator::BOP_ISEMPTY };

  CString         m_expectedReturn;
  CString         m_expectedOutput;
  CString         m_expectedError;

  CString         m_effectiveReturn;
  CString         m_effectiveOutput;
  CString         m_effectiveError;
};
