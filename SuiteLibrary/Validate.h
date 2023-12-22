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
// This File   : Validate.h
// What it does: A validation step for a teststep. The main class.
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "XMLMessage.h"
#include <vector>

enum class ReturnOperator
{
  ROP_NOP
 ,ROP_EQUAL
 ,ROP_GREATER
 ,ROP_GREATEREQUAL
 ,ROP_SMALLER
 ,ROP_SMALLEREQUAL
 ,ROP_NOTEQUAL
 ,ROP_BETWEEN
 ,ROP_IN
};

enum class BufferOperator
{
   BOP_NOP
  ,BOP_EXACT
  ,BOP_CONTAINS
  ,BOP_BEGINSWITH
  ,BOP_ENDSWITH
  ,BOP_ISEMPTY
  ,BOP_NOTEMPTY
  ,BOP_NOTFOUND
  ,BOP_FILEMATCH
  ,BOP_EXISTFILE
};

class Parameters;
class Validate;

using Validations = std::vector<Validate*>;

// Read a validation file (factory)
Validate* ReadValidate(CString p_filename);

enum class ValidateType
{
   Validate_unknown
  ,Validate_command
  ,Validate_inet
  ,Validate_sql
  ,Validate_win
};

class Validate
{
public:
  Validate() = default;
  virtual ~Validate() = default;

  // Interface with the file system
  virtual void    ReadFromXML(CString p_filename) = 0;  // Throws in case of an error
  virtual bool    WriteToXML (CString p_filename) = 0;
  // RE-Calculate the effective strings, returning the number of unbound parameters
  virtual int     EffectiveReplacements(Parameters* p_parameters,bool p_forDisplay) = 0;
  // Check our filenames extension
  virtual void    CheckFilename(CString p_filename) = 0;

  // GETTERS
  virtual CString GetName()          { return m_name;          }
  virtual CString GetDocumentation() { return m_documentation; }
  virtual CString GetFilename()      { return m_filename;      }
  virtual bool    GetGlobal()        { return m_global;        }

  // SETTERS
  virtual void    SetName(CString p_name)         { m_name          = p_name;   }
  virtual void    SetDocumentation(CString p_doc) { m_documentation = p_doc;    }
  virtual void    SetFilename(CString p_file)     { m_filename      = p_file;   }
  virtual void    SetGlobal(bool p_global)        { m_global        = p_global; }

protected:
  // Interface with the file system
  virtual void    ReadFromXML(XMLMessage& p_msg,CString p_filename);  // Throws in case of an error
  virtual bool    WriteToXML (XMLMessage& p_msg,CString p_filename);

  // Values <-> String
  CString         ReturnOperatorToString(ReturnOperator p_oper);
  CString         BufferOperatorToString(BufferOperator p_oper);
  ReturnOperator  StringToReturnOperator(CString p_oper);
  BufferOperator  StringToBufferOperator(CString p_oper);

  // Comparison functions
  bool            Between(CString p_effective,int p_value);
  bool            ValueIn(CString p_effecitve,int p_value);
  bool            FileMatch(CString p_file1,CString p_file2);
  bool            FileExist(CString p_file);
  char*           ReadFileInBuffer(CString p_filename,ULONG& p_size);

  // Identity
  CString         m_name;
  CString         m_documentation;
  CString         m_filename;
  bool            m_global { false };
};
