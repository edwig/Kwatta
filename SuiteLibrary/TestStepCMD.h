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
  virtual void    ReadFromXML(XString p_filename) override;  // Throws in case of an error
  virtual bool    WriteToXML (XString p_filename) override;
  // RE-Calculate the effective strings, returning the number of unbound parameters
  virtual int     EffectiveReplacements(Parameters* p_parameters,bool p_forDisplay);
  // Check our filenames extension
  virtual void    CheckFilename(XString p_filename);
  // Reset effective parameter replacements
  virtual void    ResetEffective();

  // GETTERS
  XString GetDirectoryPath()        { return m_directoryPath;         }
  XString GetRuntimer()             { return m_runtimer;              }
  XString GetCommandLine()          { return m_commandLine;           }
  XString GetStreamCharset()        { return m_streamCharset;         }
  ParMap& GetEnvironmentVars()      { return m_environment;           }
  bool    GetWaitForIdle()          { return m_waitForIdle;           }
  int     GetStartWindow()          { return m_startWindow;           }
  int     GetHandleEnvironment()    { return m_handleEnvironment;     }
  XString GetEffectiveDirectory()   { return m_effectiveDirectory;    }
  XString GetEffectiveRuntimer()    { return m_effectiveRuntimer;     }
  XString GetEffectiveCommandLine() { return m_effectiveCommandLine;  }
  XString GetEffectiveInput()       { return m_effectiveInput;        }
  bool    GetUseReturnValue()       { return m_useReturnValue;        }
  bool    GetUseOutputValue()       { return m_useOutputValue;        }
  bool    GetUseErrorValue()        { return m_useErrorValue;         }
  XString GetReturnVariable()       { return m_varReturn;             }
  XString GetOutputVariable()       { return m_varOutput;             }
  XString GetErrorVariable()        { return m_varError;              }
  XString GetStandardInput();

  // SETTERS
  void    SetDirectoryPath(XString p_path)  { m_directoryPath     = p_path;     }
  void    SetRuntimer(XString p_runtimer)   { m_runtimer          = p_runtimer; }
  void    SetCommandLine(XString p_line)    { m_commandLine       = p_line;     }
  void    SetStreamCharset(XString p_chars) { m_streamCharset     = p_chars;    }
  void    SetWaitForIdle(bool p_wait)       { m_waitForIdle       = p_wait;     }
  void    SetStartWindow(int p_start)       { m_startWindow       = p_start;    }
  void    SetHandleEnvironment(int p_handle){ m_handleEnvironment = p_handle;   }
  void    SetUseReturnValue(bool p_use)     { m_useReturnValue    = p_use;      }
  void    SetUseOutputValue(bool p_use)     { m_useOutputValue    = p_use;      }
  void    SetUseErrorValue(bool p_use)      { m_useErrorValue     = p_use;      }
  void    SetReturnVariable(XString p_var)  { m_varReturn         = p_var;      }
  void    SetOutputVariable(XString p_var)  { m_varOutput         = p_var;      }
  void    SetErrorVariable (XString p_var)  { m_varError          = p_var;      }
  void    SetStandardInput(XString p_input);

protected:
  // Direct definition
  XString       m_directoryPath;
  XString       m_runtimer;
  XString       m_commandLine;
  XString       m_streamCharset;
  ParMap        m_environment;
  StreamBuffer  m_standardInput;
  // Parameters
  bool          m_waitForIdle         { true    };
  int           m_startWindow         { SW_HIDE };    // SW_HIDE, SW_MAXIMIZED etc
  int           m_handleEnvironment   { 0       };    // 0=nothing, 1=add, 2=replace

  // After considering the global parameter replacements
  XString       m_effectiveDirectory;
  XString       m_effectiveRuntimer;
  XString       m_effectiveCommandLine;
  XString       m_effectiveInput;

  // How to execute the step
  bool          m_useReturnValue      { true  };
  bool          m_useOutputValue      { true  };
  bool          m_useErrorValue       { false };
  XString       m_varReturn;          // Return value  goes into this parameter
  XString       m_varOutput;          // Output stream goes into this parameter
  XString       m_varError;           // Error  stream goes into this parameter
};

