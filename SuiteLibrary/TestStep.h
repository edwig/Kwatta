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
// This File   : TestStep.h
// What it does: One (1) test step (main class)
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "StreamBuffer.h"
#include "Parameters.h"
#include <map>

class XMLMessage;
class XMLElement;

enum class StepType
{
  Step_unknown  = -1
 ,Step_command  =  0
 ,Step_http
 ,Step_sql
 ,Step_win
};

enum class ScriptStatus
{
  NoScript
 ,SuccessIsZero
 ,SuccessIsPositive
 ,SuccessIsNegative
};

class TestStep;

// General factory to read-in a test step
TestStep* ReadTestStep(XString p_filename);

class TestStep
{
public:
  TestStep();
  virtual ~TestStep();

  // Interface with the file system
  virtual void  ReadFromXML(XString p_filename) = 0;
  virtual bool  WriteToXML (XString p_filename) = 0;
  // RE-Calculate the effective strings, returning the number of unbound parameters
  virtual int   EffectiveReplacements(Parameters* p_parameters,bool p_forDisplay);
  virtual void  CheckFilename(XString p_filename) = 0;

  // GETTERS
  XString  GetName()                   { return m_name;                   }
  StepType GetType()                   { return m_type;                   }
  XString  GetDocumentation()          { return m_documentation;          }
  bool     GetKillOnTimeout()          { return m_killOnTimeout;          }
  XString  GetMaxExecution()           { return m_maxExecution;           }
  XString  GetWaitBeforeRun()          { return m_waitBeforeRun;          }
  XString  GetWaitAfterRun()           { return m_waitAfterRun;           }
  XString  GetEffectiveMaxExecution()  { return m_effectiveMaxEcecution;  }
  XString  GetEffectiveWaitBeforeRun() { return m_effectiveWaitBeforeRun; }
  XString  GetEffectiveWaitAfterRun()  { return m_effectiveWaitAfterRun;  }
  XString  GetStatusOK()               { return m_statusOK;               }
  XString  GetEffectiveStatusOK()      { return m_effectiveStatusOK;      }
  XString  GetScriptToRun()            { return m_scriptToRun;            }
  XString  GetEffectiveScriptToRun()   { return m_effectiveScriptToRun;   }
  ScriptStatus GetScriptStatus()       { return m_scriptStatus;           }

  // SETTERS
  void    SetName(XString p_name)           { m_name              = p_name;     }
  void    SetType(StepType p_type)          { m_type              = p_type;     }
  void    SetDocumentation(XString p_doc)   { m_documentation     = p_doc;      }
  void    SetKillOnTimeout(bool p_kill)     { m_killOnTimeout     = p_kill;     }
  void    SetMaxExecution(XString p_exec)   { m_maxExecution      = p_exec;     }
  void    SetWaitBeforeRun(XString p_wait)  { m_waitBeforeRun     = p_wait;     }
  void    SetWaitAfterRun (XString p_wait)  { m_waitAfterRun      = p_wait;     }
  void    SetScriptToRun(XString p_script)  { m_scriptToRun       = p_script;   }
  void    SetStatusOK(XString p_status)     { m_statusOK          = p_status;   }
  void    SetScriptStatus(ScriptStatus p_s) { m_scriptStatus      = p_s;        }

protected:
  // Interface with the file system
  virtual void    ReadFromXML(XMLMessage& p_msg,XString p_filename);  // Throws in case of an error
  virtual bool    WriteToXML (XMLMessage& p_msg,XString p_filename);

  XString FindElementString (XMLMessage& p_msg,XMLElement* p_start,XString p_name);
  int     FindElementInteger(XMLMessage& p_msg,XMLElement* p_start,XString p_name);
  bool    FindElementBoolean(XMLMessage& p_msg,XMLElement* p_start,XString p_name);
  // Check the filename's extension of the XML file

  // Names
  XString       m_name;
  XString       m_documentation;
  StepType      m_type;
  // General parameters
  bool          m_killOnTimeout       { false   };    // Do kill (or not) after m_maxExecution
  XString       m_maxExecution        { _T("90000") };    // Time in ms. 90 seconds
  XString       m_waitBeforeRun;                      // Time in ms.  0 seconds
  XString       m_waitAfterRun;                       // Time in ms.  0 seconds

  XString       m_effectiveMaxEcecution;
  XString       m_effectiveWaitBeforeRun;
  XString       m_effectiveWaitAfterRun;

  // QL Script to run
  XString       m_scriptToRun;
  XString       m_effectiveScriptToRun;
  XString       m_statusOK;
  XString       m_effectiveStatusOK;
  ScriptStatus  m_scriptStatus { ScriptStatus::NoScript };
};
