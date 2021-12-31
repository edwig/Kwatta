#pragma once
#include "TestStep.h"

class TestStepCL: public TestStep
{
public:
  TestStepCL() = default;
  virtual ~TestStepCL() = default;

  // Interface with the file system
  virtual void    ReadFromXML(CString p_filename) override;  // Throws in case of an error
  virtual bool    WriteToXML (CString p_filename) override;
  // RE-Calculate the effective strings, returning the number of unbound parameters
  virtual int     EffectiveReplacements(Parameters* p_parameters);
  // Check our filenames extension
  virtual void    CheckFilename(CString p_filename);
  // Reset effective parameter replacements
  virtual void    ResetEffective();

  // GETTERS
  CString GetDirectoryPath()        { return m_directoryPath; };
  CString GetRuntimer()             { return m_runtimer;      };
  CString GetCommandLine()          { return m_commandLine;   };
  ParMap& GetEnvironmentVars()      { return m_environment;   };
  int     GetMaxExecution()         { return m_maxExecution;  };
  bool    GetWaitForIdle()          { return m_waitForIdle;   };
  bool    GetKillOnTimeout()        { return m_killOnTimeout; };
  int     GetWaitBeforeRun()        { return m_waitBeforeRun; };
  int     GetWaitAfterRun()         { return m_waitAfterRun;  };
  int     GetStartWindow()          { return m_startWindow;   };
  int     GetHandleEnvironment()    { return m_handleEnvironment;     };
  CString GetEffectiveDirectory()   { return m_effectiveDirectory;    };
  CString GetEffectiveRuntimer()    { return m_effectiveRuntimer;     };
  CString GetEffectiveCommandLine() { return m_effectiveCommandLine;  };
  CString GetEffectiveInput()       { return m_effectiveInput;        };
  bool    GetUseReturnValue()       { return m_useReturnValue;        };
  bool    GetUseOutputValue()       { return m_useOutputValue;        };
  bool    GetUseErrorValue()        { return m_useErrorValue;         };
  CString GetReturnVariable()       { return m_varReturn;             };
  CString GetOutputVariable()       { return m_varOutput;             };
  CString GetErrorVariable()        { return m_varError;              };
  CString GetStandardInput();

  // SETTERS
  void    SetDirectoryPath(CString p_path)  { m_directoryPath     = p_path;     };
  void    SetRuntimer(CString p_runtimer)   { m_runtimer          = p_runtimer; };
  void    SetCommandLine(CString p_line)    { m_commandLine       = p_line;     };
  void    SetMaxExecution(int p_exec)       { m_maxExecution      = p_exec;     };
  void    SetWaitForIdle(bool p_wait)       { m_waitForIdle       = p_wait;     };
  void    SetKillOnTimeout(bool p_kill)     { m_killOnTimeout     = p_kill;     };
  void    SetWaitBeforeRun(int p_wait)      { m_waitBeforeRun     = p_wait;     };
  void    SetWaitAfterRun(int p_wait)       { m_waitAfterRun      = p_wait;     };
  void    SetStartWindow(int p_start)       { m_startWindow       = p_start;    };
  void    SetHandleEnvironment(int p_handle){ m_handleEnvironment = p_handle;   };
  void    SetUseReturnValue(bool p_use)     { m_useReturnValue    = p_use;      };
  void    SetUseOutputValue(bool p_use)     { m_useOutputValue    = p_use;      };
  void    SetUseErrorValue(bool p_use)      { m_useErrorValue     = p_use;      };
  void    SetReturnVariable(CString p_var)  { m_varReturn         = p_var;      };
  void    SetOutputVariable(CString p_var)  { m_varOutput         = p_var;      };
  void    SetErrorVariable (CString p_var)  { m_varError          = p_var;      };
  void    SetStandardInput(CString p_input);

protected:
  // Direct definition
  CString       m_directoryPath;
  CString       m_runtimer;
  CString       m_commandLine;
  ParMap        m_environment;
  StreamBuffer  m_standardInput;
  // Parameters
  int           m_maxExecution        { 100000  };    // Time in ms.
  bool          m_waitForIdle         { true    };
  bool          m_killOnTimeout       { false   };
  int           m_waitBeforeRun       { 100     };    // Time in ms.
  int           m_waitAfterRun        { 100     };    // Time in ms.
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

