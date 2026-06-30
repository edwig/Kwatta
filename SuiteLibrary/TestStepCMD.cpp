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
// This File   : TestStepCL.cpp
// What it does: One (1) test step. Subclass for a command-line test
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TestStepCMD.h"
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
TestStepCMD::ReadFromXML(XString p_filename)
{
  XMLMessage msg;
  TestStep::ReadFromXML(msg,p_filename);
  XMLElement* root = msg.GetRoot();

  // Find type
  XMLElement* typ = msg.FindElement(root,_T("Type"),false);
  if(typ)
  {
    XString type = typ->GetValue();
    if (type.Compare(_T("CommandLine")))
    {
      throw StdException(_T("XRUN file is not a Command-Line test: ") + p_filename);
    }
  }
  else
  {
    throw StdException(_T("Missing <Type> in XRUN file: ") + p_filename);
  }

  // Find definition
  XMLElement* def = msg.FindElement(root,_T("Definition"),false);
  if(def)
  {
    m_directoryPath = FindElementString(msg,def,_T("Directory"));
    m_runtimer      = FindElementString(msg,def,_T("Runtimer"));
    m_commandLine   = FindElementString(msg,def,_T("CommandLine"));
    m_streamCharset = FindElementString(msg,def,_T("StreamCharset"));

    XString input = FindElementString(msg,def,_T("StandardInput"));
    if(input.GetLength())
    {
      m_standardInput.SetBuffer((_TUCHAR*)input.GetString(), input.GetLength());
    }

    XMLElement* env = msg.FindElement(root,_T("Environment"),false);
    if(env)
    {
      XMLElement* envvar = msg.FindElement(env,_T("EnvironmentVariable"));
      while(envvar)
      {
        XMLAttribute* attrib = msg.FindAttribute(envvar,_T("Name"));
        if(attrib)
        {
          XString name = attrib->m_value;
          XString value = envvar->GetValue();

          m_environment.insert(std::make_pair(name, value));
        }
        envvar = msg.GetElementSibling(envvar);
      }
    }
  }

  // Find Parameters
  XMLElement* param = msg.FindElement(root,_T("Parameters"),false);
  if(param)
  {
    m_waitForIdle       = FindElementBoolean(msg, param, _T("WaitForIdle"));
    m_startWindow       = FindElementInteger(msg, param, _T("StartWindow"));
    m_handleEnvironment = FindElementInteger(msg, param, _T("HandleEnvironment"));

    m_useReturnValue    = FindElementBoolean(msg, param, _T("UseReturnValue"));
    m_useOutputValue    = FindElementBoolean(msg, param, _T("UseOutputStream"));
    m_useErrorValue     = FindElementBoolean(msg, param, _T("UseErrorStream"));

    m_varReturn         = FindElementString (msg, param, _T("ReturnVariable"));
    m_varOutput         = FindElementString (msg, param, _T("OutputVariable"));
    m_varError          = FindElementString (msg, param, _T("ErrorVariable"));
  }
}

bool
TestStepCMD::WriteToXML(XString p_filename)
{
  XMLMessage msg;
  if(!TestStep::WriteToXML(msg, p_filename))
  {
    return false;
  }
  XMLElement* root = msg.GetRoot();

  // This is our SUB-CLASS type
  msg.AddElement(root,_T("Type"),_T("CommandLine"));

  XMLElement* definition = msg.AddElement(root, _T("Definition"), _T(""));
  msg.AddElement(definition, _T("Directory"),     m_directoryPath);
  msg.AddElement(definition, _T("Runtimer"),      m_runtimer);
  msg.AddElement(definition, _T("CommandLine"),   m_commandLine);
  msg.AddElement(definition, _T("StreamCharset"), m_streamCharset);

  XMLElement* parameters = msg.FindElement(root,_T("Parameters"));
  msg.SetElement(parameters, _T("WaitForIdle"),       m_waitForIdle);
  msg.SetElement(parameters, _T("StartWindow"),       m_startWindow);
  msg.SetElement(parameters, _T("HandleEnvironment"), m_handleEnvironment);
  msg.SetElement(parameters, _T("UseReturnValue"),    m_useReturnValue);
  msg.SetElement(parameters, _T("UseOutputStream"),   m_useOutputValue);
  msg.SetElement(parameters, _T("UseErrorStream"),    m_useErrorValue);
  msg.SetElement(parameters, _T("ReturnVariable"),    m_varReturn);
  msg.SetElement(parameters, _T("OutputVariable"),    m_varOutput);
  msg.SetElement(parameters, _T("ErrorVariable"),     m_varError);

  // Add environment variables (if any)
  if(!m_environment.empty())
  {
    XMLElement* env = msg.AddElement(root, _T("Environment"),_T(""));

    for (auto& parm : m_environment)
    {
      XMLElement* envvar = msg.AddElement(env, _T("EnvironmentVariable"),parm.second);
      msg.SetAttribute(envvar, _T("Name"), parm.first);
    }
  }

  // Add input stream buffer (if any)
  if(m_standardInput.GetUse())
  {
    msg.AddElement(definition, _T("StandardInput"), GetStandardInput(),XmlDataType::XDT_CDATA | XmlDataType::XDT_String);
  }

  // Now save it
  return msg.SaveFile(p_filename);
}


XString
TestStepCMD::GetStandardInput()
{
  _TUCHAR* buffer = nullptr;
  unsigned int   length = 0;
  m_standardInput.GetBuffer(buffer, length);

  return XString(buffer);
}

void
TestStepCMD::SetStandardInput(XString p_input)
{
  m_standardInput.SetBuffer((_TUCHAR*)p_input.GetString(), p_input.GetLength());
}

// RE-Calculate the effective strings, returning the number of unbound parameters
int
TestStepCMD::EffectiveReplacements(Parameters* p_parameters,bool p_forDisplay)
{
  int unbound = TestStep::EffectiveReplacements(p_parameters,p_forDisplay);
  XString input = GetStandardInput();

  unbound += p_parameters->Replace(m_directoryPath,m_effectiveDirectory,  p_forDisplay);
  unbound += p_parameters->Replace(m_runtimer,     m_effectiveRuntimer,   p_forDisplay);
  unbound += p_parameters->Replace(m_commandLine,  m_effectiveCommandLine,p_forDisplay);
  unbound += p_parameters->Replace(input,          m_effectiveInput,      p_forDisplay);

  return unbound;
}

void
TestStepCMD::CheckFilename(XString p_filename)
{
  // Split of only the extension
  TCHAR extension[_MAX_EXT];
  _tsplitpath_s(p_filename, NULL, 0, NULL, 0, NULL, 0, extension, _MAX_EXT);

  // Check that we have the right one
  if (_tcsncicmp(extension, EXTENSION_TESTSTEP_CMD, 5))
  {
    throw StdException(_T("A TestStep XML definition file must be saved as a *.XRUN"));
  }
}

// Reset effective parameter replacements
void
TestStepCMD::ResetEffective()
{
  m_effectiveDirectory.Empty();
  m_effectiveRuntimer.Empty();
  m_effectiveCommandLine.Empty();
  m_effectiveInput.Empty();
}
