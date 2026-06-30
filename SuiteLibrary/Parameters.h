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
  bool operator() (const XString& p_left, const XString& p_right) const
  {
    return p_left.CompareNoCase(p_right) < 0;
  }
};

using ParMap = std::map<XString,XString,ParamCompare>;

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
  int      Replace(XString p_input,XString& p_output,bool p_forDisplay,ParType p_exclude = ParType::PAR_NONE);

  // Interface with the file system
  void     ReadFromXML(XString p_filename, bool p_globals = true);  // Throws in case of an error
  bool     WriteToXML(bool p_locals = true,bool p_globals = false);

  bool     ExistsAsSystemParameter (XString p_name);
  bool     ExistsAsGlobalParameter (XString p_name);
  bool     ExistsAsLocalParameter  (XString p_name);
  bool     ExistsAsReturnParameter (XString p_name);
  bool     ExistsAsBufferParameter (XString p_name);
  bool     ExistsAsEnvironParameter(XString p_name);

  XString  FindSystemParameter (XString p_name);
  XString  FindGlobalParameter (XString p_name,bool p_forDisplay);
  XString  FindLocalParameter  (XString p_name);
  XString  FindReturnParameter (XString p_name);
  XString  FindBufferParameter (XString p_name);
  XString  FindEnvironParameter(XString p_name);

  bool     AddSystemParameter(XString p_name,XString p_value);
  bool     AddGlobalParameter(XString p_name,XString p_value);
  bool     AddLocalParameter (XString p_name,XString p_value);
  bool     AddReturnParameter(XString p_name,XString p_value);
  bool     AddBufferParameter(XString p_name,XString p_value);

  void     OverwriteSystemParameter(XString p_name,XString p_value);
  void     OverwriteGlobalParameter(XString p_name,XString p_value);
  void     OverwriteLocalParameter (XString p_name,XString p_value);
  void     OverwriteReturnParameter(XString p_name,XString p_value);
  void     OverwriteBufferParameter(XString p_name,XString p_value);

  bool     RemoveSystemParameter(XString p_name);
  bool     RemoveBufferParameter(XString p_name);
  bool     RemoveReturnParameter(XString p_name);
  bool     RemoveGlobalParameter(XString p_name);
  bool     RemoveLocalParameter (XString p_name);

  // Get the maps
  ParMap&  GetSystem()  { return m_system;  }
  ParMap&  GetGlobals() { return m_globals; }
  ParMap&  GetLocals()  { return m_locals;  }
  ParMap&  GetReturns() { return m_returns; }
  ParMap&  GetBuffers() { return m_buffers; }

  // Result of the 
  void     ResetUnboundErrors();
  XString  GetUnboundErrors();

private:
  // Check the filename's extension
  void    CheckFilename(XString p_filename);
  // Name not yet used in other maps
  bool    NameNotYetUsed(XString p_name);
  // Check if we should do password protection
  void    CheckPasswordProtection(XString p_name,XString p_value);
  // Replacement of values
  int     Replace(XString& p_string,TCHAR p_first,TCHAR p_last,ParType p_find,bool p_forDisplay,ParType p_exclude = ParType::PAR_NONE);
  // Add error to the list of errors
  void    AddError(XString p_varname,TCHAR p_first,ParType p_find,XString p_errortext);

  ParMap  m_system;   // System parameters
  ParMap  m_buffers;  // [name]   values
  ParMap  m_returns;  // <name>   values
  ParMap  m_globals;  // $global$ values
  ParMap  m_locals;   // #local#  values

  XString m_environmentValue;
  XString m_errors;

  bool    m_changed  { false }; // Any parameters changed?
  bool    m_password { false }; // Use password protection?
  XString m_filenameGlobal;     // Where we reside
  XString m_filenameLocal;      // Where we change regularly
};
