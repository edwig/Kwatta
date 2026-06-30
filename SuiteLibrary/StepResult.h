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
// This File   : StepResult.h
// What it does: Result of one (1) step in a test set
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <vector>
#include "XMLMessage.h"

typedef struct _valStep
{
  int     m_number;
  XString m_validation;
  XString m_filename;
  bool    m_global;
  bool    m_ok;
}
ValStep;

using ValSteps = std::vector<ValStep>;


// General factory to read-in a test step
class StepResult;

StepResult* ReadStepResult(XString p_filename);

class StepResult
{
public:
  StepResult() = default;
  virtual ~StepResult() = default;
  
  // Interface with the file system
  virtual void ReadFromXML  (XString p_filename) = 0;  // Throws in case of an error
  virtual bool WriteToXML   (XString p_filename) = 0;
  virtual void CheckFilename(XString p_filename) = 0;
  virtual void ResetEffective() = 0;
  virtual void Reset();

  // GETTERS
  XString   GetName()           { return m_name;           }
  XString   GetDocumentation()  { return m_documentation;  }
  ValSteps& GetValidations()    { return m_validations;    }
  double    GetTiming()         { return m_seconds;        }
  bool      GetTotalResult();

  // SETTERS
  void      SetName(XString p_name)                   { m_name           = p_name;          }
  void      SetDocumentation(XString p_documentation) { m_documentation  = p_documentation; }
  void      SetTiming(double p_time)                  { m_seconds        = p_time;          }

  // Add an extra validation
  void      AddValidation(int p_step,XString p_name,XString p_filename,bool p_result,bool p_global);

protected:
  // Interface with the file system
  virtual void    ReadFromXML(XMLMessage& p_msg,XString p_filename);  // Throws in case of an error
  virtual bool    WriteToXML (XMLMessage& p_msg,XString p_filename);

  XString   m_name;
  XString   m_documentation;
  double    m_seconds { 0.0 };

  // Validation steps
  ValSteps  m_validations;
};
