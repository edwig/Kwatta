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
// This File   : Parameters.h
// What it does: Parameters in three categories (global, return, buffer)
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <map>

// $name$ -> Global parameter(valid within testsuite)
// #name# -> Local parameter(valid within testset)
// [name] -> return value result(valid within testset)
// <name> -> Buffer result parameter(valid within testset)

struct ParamCompare
{
  bool operator() (const CString& p_left, const CString& p_right) const
  {
    return p_left.CompareNoCase(p_right) < 0;
  }
};

using ParMap = std::map<CString,CString,ParamCompare>;

enum class ParType
{
  PAR_NONE   = 0
 ,PAR_SYSTEM
 ,PAR_GLOBAL
 ,PAR_LOCAL
 ,PAR_RETURN
 ,PAR_BUFFER
 ,PAR_ENVIRON
};

class Parameters
{
public:
  Parameters() = default;

  // Reset all variables
  void     Reset();

  // Force changed
  void     SetChanged() { m_changed = true;  }

  // The general replace function for bound parameters
  int      Replace(CString p_input,CString& p_output,bool p_forDisplay,ParType p_exclude = ParType::PAR_NONE);

  // Interface with the file system
  void     ReadFromXML(CString p_filename, bool p_globals = true);  // Throws in case of an error
  bool     WriteToXML(bool p_locals = true,bool p_globals = false);

  bool     ExistsAsSystemParameter (CString p_name);
  bool     ExistsAsGlobalParameter (CString p_name);
  bool     ExistsAsLocalParameter  (CString p_name);
  bool     ExistsAsReturnParameter (CString p_name);
  bool     ExistsAsBufferParameter (CString p_name);
  bool     ExistsAsEnvironParameter(CString p_name);

  CString  FindSystemParameter (CString p_name);
  CString  FindGlobalParameter (CString p_name,bool p_forDisplay);
  CString  FindLocalParameter  (CString p_name);
  CString  FindReturnParameter (CString p_name);
  CString  FindBufferParameter (CString p_name);
  CString  FindEnvironParameter(CString p_name);

  bool     AddSystemParameter(CString p_name,CString p_value);
  bool     AddGlobalParameter(CString p_name,CString p_value);
  bool     AddLocalParameter (CString p_name,CString p_value);
  bool     AddReturnParameter(CString p_name,CString p_value);
  bool     AddBufferParameter(CString p_name,CString p_value);

  void     OverwriteSystemParameter(CString p_name,CString p_value);
  void     OverwriteGlobalParameter(CString p_name,CString p_value);
  void     OverwriteLocalParameter (CString p_name,CString p_value);
  void     OverwriteReturnParameter(CString p_name,CString p_value);
  void     OverwriteBufferParameter(CString p_name,CString p_value);

  bool     RemoveSystemParameter(CString p_name);
  bool     RemoveBufferParameter(CString p_name);
  bool     RemoveReturnParameter(CString p_name);
  bool     RemoveGlobalParameter(CString p_name);
  bool     RemoveLocalParameter (CString p_name);

  // Get the maps
  ParMap&  GetSystem()  { return m_system;  }
  ParMap&  GetGlobals() { return m_globals; }
  ParMap&  GetLocals()  { return m_locals;  }
  ParMap&  GetReturns() { return m_returns; }
  ParMap&  GetBuffers() { return m_buffers; }

private:
  // Check the filename's extension
  void    CheckFilename(CString p_filename);
  // Name not yet used in other maps
  bool    NameNotYetUsed(CString p_name);
  // Check if we should do password protection
  void    CheckPasswordProtection(CString p_name,CString p_value);
  // Replacement of values
  int     Replace(CString& p_string,char p_first,char p_last,ParType p_find,bool p_forDisplay,ParType p_exclude = ParType::PAR_NONE);

  ParMap  m_system;   // System parameters
  ParMap  m_buffers;  // [name]   values
  ParMap  m_returns;  // <name>   values
  ParMap  m_globals;  // $global$ values
  ParMap  m_locals;   // #local#  values

  CString m_environmentValue;

  bool    m_changed  { false }; // Any parameters changed?
  bool    m_password { false }; // Use password protection?
  CString m_filenameGlobal;     // Where we reside
  CString m_filenameLocal;      // Where we change regularly
};
