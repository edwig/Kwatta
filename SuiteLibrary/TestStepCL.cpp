#include "stdafx.h"
#include "TestStepCL.h"
#include "XMLMessage.h"
#include "StdException.h"
#include "ExtraExtensions.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// Interface with the file system
void
TestStepCL::ReadFromXML(CString p_filename)
{
  XMLMessage msg;
  TestStep::ReadFromXML(msg,p_filename);
  XMLElement* root = msg.GetRoot();

  // Find type
  XMLElement* typ = msg.FindElement(root,"Type",false);
  if(typ)
  {
    CString type = typ->GetValue();
    if (type.Compare("CommandLine"))
    {
      throw StdException("XRUN file is not a Command-Line test: " + p_filename);
    }
  }
  else
  {
    throw StdException("Missing <Type> in XRUN file: " + p_filename);
  }

  // Find definition
  XMLElement* def = msg.FindElement(root,"Definition",false);
  if(def)
  {
    m_directoryPath = FindElementString(msg,def,"Directory");
    m_runtimer      = FindElementString(msg,def,"Runtimer");
    m_commandLine   = FindElementString(msg,def,"CommandLine");

    CString input = FindElementString(msg,def,"StandardInput");
    if(input.GetLength())
    {
      m_standardInput.SetBuffer((unsigned char*)input.GetBuffer(), input.GetLength());
    }

    XMLElement* env = msg.FindElement(root,"Environment",false);
    if(env)
    {
      XMLElement* envvar = msg.FindElement(env,"EnvironmentVariable");
      while(envvar)
      {
        XMLAttribute* attrib = msg.FindAttribute(envvar,"Name");
        if(attrib)
        {
          CString name = attrib->m_value;
          CString value = envvar->GetValue();

          m_environment.insert(std::make_pair(name, value));
        }
        envvar = msg.GetElementSibling(envvar);
      }
    }
  }

  // Find Parameters
  XMLElement* param = msg.FindElement(root,"Parameters",false);
  if(param)
  {
    m_maxExecution      = FindElementInteger(msg, param, "MaxExecutionTime");
    m_waitForIdle       = FindElementBoolean(msg, param, "WaitForIdle");
    m_killOnTimeout     = FindElementBoolean(msg, param, "KillOnTimeout");
    m_waitBeforeRun     = FindElementInteger(msg, param, "WaitBeforeRun");
    m_waitAfterRun      = FindElementInteger(msg, param, "WaitAfterRun");
    m_startWindow       = FindElementInteger(msg, param, "StartWindow");
    m_handleEnvironment = FindElementInteger(msg, param, "HandleEnvironment");

    m_useReturnValue    = FindElementBoolean(msg, param, "UseReturnValue");
    m_useOutputValue    = FindElementBoolean(msg, param, "UseOutputStream");
    m_useErrorValue     = FindElementBoolean(msg, param, "UseErrorStream");

    m_varReturn         = FindElementString (msg, param, "ReturnVariable");
    m_varOutput         = FindElementString (msg, param, "OutputVariable");
    m_varError          = FindElementString (msg, param, "ErrorVariable");
  }
}

bool
TestStepCL::WriteToXML(CString p_filename)
{
  XMLMessage msg;
  if(!TestStep::WriteToXML(msg, p_filename))
  {
    return false;
  }
  XMLElement* root = msg.GetRoot();

  // This is our SUB-CLASS type
  msg.AddElement(root,"Type",XDT_String,"CommandLine");

  XMLElement* definition = msg.AddElement(root, "Definition", XDT_String, "");
  msg.AddElement(definition, "Directory",   XDT_String, m_directoryPath);
  msg.AddElement(definition, "Runtimer",    XDT_String, m_runtimer);
  msg.AddElement(definition, "CommandLine", XDT_String, m_commandLine);

  XMLElement* parameters = msg.AddElement(root, "Parameters", XDT_String, "");
  msg.SetElement(parameters, "MaxExecutionTime",  m_maxExecution);
  msg.SetElement(parameters, "WaitForIdle",       m_waitForIdle);
  msg.SetElement(parameters, "KillOnTimeout",     m_killOnTimeout);
  msg.SetElement(parameters, "WaitBeforeRun",     m_waitBeforeRun);
  msg.SetElement(parameters, "WaitAfterRun",      m_waitAfterRun);
  msg.SetElement(parameters, "StartWindow",       m_startWindow);
  msg.SetElement(parameters, "HandleEnvironment", m_handleEnvironment);

  msg.SetElement(parameters, "UseReturnValue",    m_useReturnValue);
  msg.SetElement(parameters, "UseOutputStream",   m_useOutputValue);
  msg.SetElement(parameters, "UseErrorStream",    m_useErrorValue);
  msg.SetElement(parameters, "ReturnVariable",    m_varReturn);
  msg.SetElement(parameters, "OutputVariable",    m_varOutput);
  msg.SetElement(parameters, "ErrorVariable",     m_varError);

  // Add environment variables (if any)
  if(!m_environment.empty())
  {
    XMLElement* env = msg.AddElement(root, "Environment", XDT_String, "");

    for (auto& parm : m_environment)
    {
      XMLElement* envvar = msg.AddElement(env, "EnvironmentVariable", XDT_String, parm.second);
      msg.SetAttribute(envvar, "Name", parm.first);
    }
  }

  // Add input stream buffer (if any)
  if(m_standardInput.GetUse())
  {
    msg.AddElement(definition, "StandardInput", XDT_CDATA, GetStandardInput());
  }

  // Now save it
  return msg.SaveFile(p_filename);
}


CString
TestStepCL::GetStandardInput()
{
  unsigned char* buffer = nullptr;
  unsigned int   length = 0;
  m_standardInput.GetBuffer(buffer, length);

  return CString(buffer);
}

void
TestStepCL::SetStandardInput(CString p_input)
{
  m_standardInput.SetBuffer((unsigned char*)p_input.GetString(), p_input.GetLength());
}

// RE-Calculate the effective strings, returning the number of unbound parameters
int
TestStepCL::EffectiveReplacements(Parameters* p_parameters)
{
  int unbound = TestStep::EffectiveReplacements(p_parameters);
  CString input = GetStandardInput();

  unbound += p_parameters->Replace(m_directoryPath,m_effectiveDirectory);
  unbound += p_parameters->Replace(m_runtimer,     m_effectiveRuntimer);
  unbound += p_parameters->Replace(m_commandLine,  m_effectiveCommandLine);
  unbound += p_parameters->Replace(input,          m_effectiveInput);

  return unbound;
}

void
TestStepCL::CheckFilename(CString p_filename)
{
  // Split of only the extension
  char extension[_MAX_EXT];
  _splitpath_s(p_filename, NULL, 0, NULL, 0, NULL, 0, extension, _MAX_EXT);

  // Check that we have the right one
  if (_strnicmp(extension, EXTENSION_TESTSTEP_CL, 5))
  {
    throw StdException("A TestStep XML definition file must be saved as a *.XRUN");
  }
}

// Reset effective parameter replacements
void
TestStepCL::ResetEffective()
{
  m_effectiveDirectory.Empty();
  m_effectiveRuntimer.Empty();
  m_effectiveCommandLine.Empty();
  m_effectiveInput.Empty();
}
