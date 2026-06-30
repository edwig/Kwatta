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
Validate* ReadValidate(XString p_filename);

enum class ValidateType
{
   Validate_unknown
  ,Validate_command
  ,Validate_inet
  ,Validate_sql
  ,Validate_win
};

enum class StepType;

class Validate
{
public:
  Validate() = default;
  virtual ~Validate() = default;

  // Interface with the file system
  virtual void    ReadFromXML(XString p_filename) = 0;  // Throws in case of an error
  virtual bool    WriteToXML (XString p_filename) = 0;
  // RE-Calculate the effective strings, returning the number of unbound parameters
  virtual int     EffectiveReplacements(Parameters* p_parameters,bool p_forDisplay) = 0;
  // Check our filenames extension
  virtual void    CheckFilename(XString p_filename) = 0;

  // GETTERS
  virtual XString GetName()          { return m_name;          }
  virtual XString GetDocumentation() { return m_documentation; }
  virtual XString GetFilename()      { return m_filename;      }
  virtual bool    GetGlobal()        { return m_global;        }

  // SETTERS
  virtual void    SetName(XString p_name)         { m_name          = p_name;   }
  virtual void    SetDocumentation(XString p_doc) { m_documentation = p_doc;    }
  virtual void    SetFilename(XString p_file)     { m_filename      = p_file;   }
  virtual void    SetGlobal(bool p_global)        { m_global        = p_global; }

  // Various
  static StepType FindStepTypeFromFile(XString p_filename);

protected:
  // Interface with the file system
  virtual void    ReadFromXML(XMLMessage& p_msg,XString p_filename);  // Throws in case of an error
  virtual bool    WriteToXML (XMLMessage& p_msg,XString p_filename);

  // Values <-> String
  XString         ReturnOperatorToString(ReturnOperator p_oper);
  XString         BufferOperatorToString(BufferOperator p_oper);
  ReturnOperator  StringToReturnOperator(XString p_oper);
  BufferOperator  StringToBufferOperator(XString p_oper);

  // Comparison functions
  bool            Between(XString p_effective,int p_value);
  bool            ValueIn(XString p_effecitve,int p_value);
  bool            FileMatch(XString p_file1,XString p_file2);
  bool            FileExist(XString p_file);
  TCHAR*           ReadFileInBuffer(XString p_filename,ULONG& p_size);

  // Identity
  XString         m_name;
  XString         m_documentation;
  XString         m_filename;
  bool            m_global { false };
};
