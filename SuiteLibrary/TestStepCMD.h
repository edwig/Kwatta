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
// This File   : TestStepCL.h
// What it does: One (1) test step. Subclass for a command-line test
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "TestStep.h"

class TestStepCMD: public TestStep
{
public:
  TestStepCMD() = default;
  virtual ~TestStepCMD() = default;

  // Interface with the file system
  virtual void    ReadFromXML(CString p_filename) override;  // Throws in case of an error
  virtual bool    WriteToXML (CString p_filename) override;
  // RE-Calculate the effective strings, returning the number of unbound parameters
  virtual int     EffectiveReplacements(Parameters* p_parameters,bool p_forDisplay);
  // Check our filenames extension
  virtual void    CheckFilename(CString p_filename);
  // Reset effective parameter replacements
  virtual void    ResetEffective();

  // GETTERS
  CString GetDirectoryPath()        { return m_directoryPath;         }
  CString GetRuntimer()             { return m_runtimer;              }
  CString GetCommandLine()          { return m_commandLine;           }
  CString GetStreamCharset()        { return m_streamCharset;         }
  ParMap& GetEnvironmentVars()      { return m_environment;           }
  bool    GetWaitForIdle()          { return m_waitForIdle;           }
  int     GetStartWindow()          { return m_startWindow;           }
  int     GetHandleEnvironment()    { return m_handleEnvironment;     }
  CString GetEffectiveDirectory()   { return m_effectiveDirectory;    }
  CString GetEffectiveRuntimer()    { return m_effectiveRuntimer;     }
  CString GetEffectiveCommandLine() { return m_effectiveCommandLine;  }
  CString GetEffectiveInput()       { return m_effectiveInput;        }
  bool    GetUseReturnValue()       { return m_useReturnValue;        }
  bool    GetUseOutputValue()       { return m_useOutputValue;        }
  bool    GetUseErrorValue()        { return m_useErrorValue;         }
  CString GetReturnVariable()       { return m_varReturn;             }
  CString GetOutputVariable()       { return m_varOutput;             }
  CString GetErrorVariable()        { return m_varError;              }
  CString GetStandardInput();

  // SETTERS
  void    SetDirectoryPath(CString p_path)  { m_directoryPath     = p_path;     }
  void    SetRuntimer(CString p_runtimer)   { m_runtimer          = p_runtimer; }
  void    SetCommandLine(CString p_line)    { m_commandLine       = p_line;     }
  void    SetStreamCharset(CString p_chars) { m_streamCharset     = p_chars;    }
  void    SetWaitForIdle(bool p_wait)       { m_waitForIdle       = p_wait;     }
  void    SetStartWindow(int p_start)       { m_startWindow       = p_start;    }
  void    SetHandleEnvironment(int p_handle){ m_handleEnvironment = p_handle;   }
  void    SetUseReturnValue(bool p_use)     { m_useReturnValue    = p_use;      }
  void    SetUseOutputValue(bool p_use)     { m_useOutputValue    = p_use;      }
  void    SetUseErrorValue(bool p_use)      { m_useErrorValue     = p_use;      }
  void    SetReturnVariable(CString p_var)  { m_varReturn         = p_var;      }
  void    SetOutputVariable(CString p_var)  { m_varOutput         = p_var;      }
  void    SetErrorVariable (CString p_var)  { m_varError          = p_var;      }
  void    SetStandardInput(CString p_input);

protected:
  // Direct definition
  CString       m_directoryPath;
  CString       m_runtimer;
  CString       m_commandLine;
  CString       m_streamCharset;
  ParMap        m_environment;
  StreamBuffer  m_standardInput;
  // Parameters
  bool          m_waitForIdle         { true    };
  int           m_startWindow         { SW_HIDE };    // SW_HIDE, SW_MAXIMIZED etc
  int           m_handleEnvironment   { 0       };    // 0=nothing, 1=add, 2=replace

  // After considering the global parameter replacements
  CString       m_effectiveDirectory;
  CString       m_effectiveRuntimer;
  CString       m_effectiveCommandLine;
  CString       m_effectiveInput;

  // How to execute the step
  bool          m_useReturnValue      { true  };
  bool          m_useOutputValue      { true  };
  bool          m_useErrorValue       { false };
  CString       m_varReturn;          // Return value  goes into this parameter
  CString       m_varOutput;          // Output stream goes into this parameter
  CString       m_varError;           // Error  stream goes into this parameter
};

