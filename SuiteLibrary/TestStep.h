//////////////////////////////////////////////////////////////////////////
//
// TEST STEP
//
//////////////////////////////////////////////////////////////////////////

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
};

class TestStep;

// General factory to read-in a test step
TestStep* ReadTestStep(CString p_filename);

class TestStep
{
public:
  TestStep() = default;
  virtual ~TestStep() = default;

  // Interface with the file system
  virtual void  ReadFromXML(CString p_filename) = 0;
  virtual bool  WriteToXML (CString p_filename) = 0;
  // RE-Calculate the effective strings, returning the number of unbound parameters
  virtual int   EffectiveReplacements(Parameters* p_parameters,bool p_forDisplay);
  virtual void  CheckFilename(CString p_filename) = 0;

  // GETTERS
  CString  GetName()                 { return m_name;          };
  StepType GetType()                 { return m_type;          };
  CString  GetDocumentation()        { return m_documentation; };

  // SETTERS
  void    SetName(CString p_name)           { m_name              = p_name;     };
  void    SetType(StepType p_type)          { m_type              = p_type;     };
  void    SetDocumentation(CString p_doc)   { m_documentation     = p_doc;      };

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
};
