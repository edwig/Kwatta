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
// This File   : StepResultCL.h
// What it does: Result of one (1) command-line step in a test set
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "StepResult.h"

class StepResultCMD : public StepResult
{
public:
  StepResultCMD() = default;
  virtual ~StepResultCMD() = default;

  // Interface with the file system
  virtual void ReadFromXML(CString p_filename);  // Throws in case of an error
  virtual bool WriteToXML (CString p_filename);
  virtual void CheckFilename(CString p_filename);
  virtual void ResetEffective();

  // GETTERS
  int       GetReturnValue()    { return m_returnValue; };
  CString   GetStandardOutput() { return m_standardOutput; };
  CString   GetStandardError()  { return m_standardError; };

  // SETTERS
  void      SetReturnValue(int p_value)               { m_returnValue = p_value; }
  void      SetStandardOutput(CString p_output)       { m_standardOutput = p_output; }
  void      SetStandardError(CString p_error)         { m_standardError = p_error; }

protected:
  // Output Command line
  int       m_returnValue { 0 };
  CString   m_standardOutput;
  CString   m_standardError;

private:
};

