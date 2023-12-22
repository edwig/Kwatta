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
// This File   : StepResultWINh
// What it does: Result of one (1) Windows UI test step
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "StepResult.h"
#include "TestStepWIN.h"
#include <map>

class StepResultWIN : public StepResult
{
public:
  StepResultWIN() = default;
  virtual ~StepResultWIN() = default;

  // Interface with the file system
  virtual void ReadFromXML(CString p_filename);  // Throws in case of an error
  virtual bool WriteToXML(CString p_filename);
  virtual void CheckFilename(CString p_filename);
  virtual void ResetEffective();
  // SETTERS
  void    SetLastOSError(UINT p_error)      { m_lastOSerror    = p_error; }
  void    SetErrorString(CString p_error)   { m_errorString    = p_error; }
  void    AddErrorString(CString p_error)   { m_errorString   += p_error; }
  void    AddResultString(CString p_result) { m_resultLogging += p_result; }
  // GETTERS
  int     GetLastOSError()  { return m_lastOSerror;   }
  CString GetErrorString()  { return m_errorString;   }
  CString GetResultString() { return m_resultLogging; }
protected:
  CString m_resultLogging;
  CString m_errorString;
  UINT    m_lastOSerror { 0 };
};
