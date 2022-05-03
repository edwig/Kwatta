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
// This File   : TestStepSQL.cpp
// What it does: One (1) test step in SQL. Subclass for a HTTP call
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TestStepSQL.h"
#include "XMLMessage.h"
#include "StdException.h"
#include "ExtraExtensions.h"
#include <CrackURL.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

void
TestStepSQL::ReadFromXML(CString p_filename)
{
  XMLMessage msg;
  TestStep::ReadFromXML(msg,p_filename);
  XMLElement* root = msg.GetRoot();

  // Find type
  XMLElement* typ = msg.FindElement(root,"Type",false);
  if(typ)
  {
    CString type = typ->GetValue();
    if(type.Compare("SQL"))
    {
      throw StdException("QRUN file is not a SQL test: " + p_filename);
    }
  }
  else
  {
    throw StdException("Missing <Type> in QRUN file: " + p_filename);
  }
  XMLElement* def = msg.FindElement("Definition");
  if(!def)
  {
    throw StdException("Missing <Definition> in QRUN file: " + p_filename);
  }
  m_datasource = msg.GetElement(def,"Datasource");
  m_user       = msg.GetElement(def,"User");
  m_password   = msg.GetElement(def,"Password");
  m_sql        = msg.GetElement(def,"SQL");
}

bool
TestStepSQL::WriteToXML(CString p_filename)
{
  XMLMessage msg;
  if(!TestStep::WriteToXML(msg,p_filename))
  {
    return false;
  }
  XMLElement* root = msg.GetRoot();

  // This is our SUB-CLASS type
  msg.AddElement(root,"Type",XDT_String,"SQL");

  XMLElement* def = msg.AddElement(root,"Definition",XDT_String,"");

  msg.AddElement(def,"Datasource",XDT_String,m_datasource);
  msg.AddElement(def,"User",      XDT_String,m_user);
  msg.AddElement(def,"Password",  XDT_String,m_password);
  msg.AddElement(def,"SQL",       XDT_String|XDT_CDATA,m_sql);

  // Now save it
  return msg.SaveFile(p_filename);
}

// RE-Calculate the effective strings, returning the number of unbound parameters
int
TestStepSQL::EffectiveReplacements(Parameters* p_parameters, bool p_forDisplay)
{
  int unbound = TestStep::EffectiveReplacements(p_parameters,p_forDisplay);

  unbound += p_parameters->Replace(m_datasource,m_effectiveDatasource,p_forDisplay);
  unbound += p_parameters->Replace(m_user,      m_effectiveUser,      p_forDisplay);
  unbound += p_parameters->Replace(m_password,  m_effectivePassword,  p_forDisplay);
  unbound += p_parameters->Replace(m_sql,       m_effectiveSQL,       p_forDisplay);

  return unbound;
}

// Check our filenames extension
void
TestStepSQL::CheckFilename(CString p_filename)
{
  // Split of only the extension
  char extension[_MAX_EXT];
  _splitpath_s(p_filename,NULL,0,NULL,0,NULL,0,extension,_MAX_EXT);

  // Check that we have the right one
  if(_strnicmp(extension,EXTENSION_TESTSTEP_SQL,5))
  {
    throw StdException("A StepResult XML definition file must be saved as a *.QRUN");
  }
}

// Reset effective parameter replacements
void
TestStepSQL::ResetEffective()
{
  m_effectiveDatasource.Empty();
  m_effectivePassword.Empty();
  m_effectiveUser.Empty();
  m_effectiveSQL.Empty();
}
