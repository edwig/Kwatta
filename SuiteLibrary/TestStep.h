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
  Step_command
 ,Step_http
 ,Step_sql
};

class TestStep;

// General factory to read-in a test step
TestStep* ReadTestStep(CString p_filename);

class TestStep
{
public:
  TestStep();
  virtual ~TestStep();

  // Interface with the file system
  virtual void  ReadFromXML(CString p_filename) = 0;
  virtual bool  WriteToXML (CString p_filename) = 0;
  // RE-Calculate the effective strings, returning the number of unbound parameters
  virtual int   EffectiveReplacements(Parameters* p_parameters,bool p_forDisplay);
  virtual void  CheckFilename(CString p_filename) = 0;

  // GETTERS
  CString  GetName()                   { return m_name;                   }
  StepType GetType()                   { return m_type;                   }
  CString  GetDocumentation()          { return m_documentation;          }
  bool     GetKillOnTimeout()          { return m_killOnTimeout;          }
  CString  GetMaxExecution()           { return m_maxExecution;           }
  CString  GetWaitBeforeRun()          { return m_waitBeforeRun;          }
  CString  GetWaitAfterRun()           { return m_waitAfterRun;           }
  CString  GetEffectiveMaxExecution()  { return m_effectiveMaxEcecution;  }
  CString  GetEffectiveWaitBeforeRun() { return m_effectiveWaitBeforeRun; }
  CString  GetEffectiveWaitAfterRun()  { return m_effectiveWaitAfterRun;  }

  // SETTERS
  void    SetName(CString p_name)           { m_name              = p_name;     }
  void    SetType(StepType p_type)          { m_type              = p_type;     }
  void    SetDocumentation(CString p_doc)   { m_documentation     = p_doc;      }
  void    SetKillOnTimeout(bool p_kill)     { m_killOnTimeout     = p_kill;     }
  void    SetMaxExecution(CString p_exec)   { m_maxExecution      = p_exec;     }
  void    SetWaitBeforeRun(CString p_wait)  { m_waitBeforeRun     = p_wait;     }
  void    SetWaitAfterRun (CString p_wait)  { m_waitAfterRun      = p_wait;     }

protected:
  // Interface with the file system
  virtual void    ReadFromXML(XMLMessage& p_msg,CString p_filename);  // Throws in case of an error
  virtual bool    WriteToXML (XMLMessage& p_msg,CString p_filename);

  CString FindElementString (XMLMessage& p_msg,XMLElement* p_start,CString p_name);
  int     FindElementInteger(XMLMessage& p_msg,XMLElement* p_start,CString p_name);
  bool    FindElementBoolean(XMLMessage& p_msg,XMLElement* p_start,CString p_name);
  // Check the filename's extension of the XML file

  // Names
  CString       m_name;
  CString       m_documentation;
  StepType      m_type;
  // General parameters
  bool          m_killOnTimeout       { false   };    // Do kill (or not) after m_maxExecution
  CString       m_maxExecution        { "90000" };    // Time in ms. 90 seconds
  CString       m_waitBeforeRun;                      // Time in ms.  0 seconds
  CString       m_waitAfterRun;                       // Time in ms.  0 seconds

  CString       m_effectiveMaxEcecution;
  CString       m_effectiveWaitBeforeRun;
  CString       m_effectiveWaitAfterRun;
};
