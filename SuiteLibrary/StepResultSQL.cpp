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
// This File   : StepResultSQL.cpp
// What it does: Result of one (1) database step in a test set
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StepResultSQL.h"
#include "ExtraExtensions.h"
#include "StdException.h"
#include <HTTPError.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// Interface with the file system
void 
StepResultSQL::ReadFromXML(CString p_filename)
{
  XMLMessage msg;
  StepResult::ReadFromXML(msg, p_filename);

  m_succeeded    = msg.GetElementBoolean("Succeeded");
  m_resultRows   = msg.GetElementInteger("ResultRows");
  m_resultCols   = msg.GetElementInteger("ResultColumns");
  m_sqlState     = msg.GetElement("SQLState");
  m_nativeStatus = msg.GetElement("NativeStatus");

  m_data.clear();
  XMLElement* data = msg.FindElement("Data");
  if(data)
  {
    XMLElement* column = msg.FindElement(data,"Column");
    while(column)
    {
      CString name  = msg.GetElement(column,"Name");
      CString value = msg.GetElement(column,"Value");
      m_data[name] = value;

      // Next column
      column = msg.GetElementSibling(column);
    }
  }
}

bool 
StepResultSQL::WriteToXML(CString p_filename)
{
  XMLMessage msg;
  if(!StepResult::WriteToXML(msg,p_filename))
  {
    return false;
  }
  XMLElement* root = msg.GetRoot();

  CString rows;
  CString cols;
  CString speed;
  rows.Format("%d",m_resultRows);
  cols.Format("%d",m_resultCols);

  msg.AddElement(root,"Succeeded",    XDT_Boolean,m_succeeded ? "true" : false);
  msg.AddElement(root,"ResultRows",   XDT_Integer,rows);
  msg.AddElement(root,"ResultColumns",XDT_Integer,cols);
  msg.AddElement(root,"SQLState",     XDT_String, m_sqlState);
  msg.AddElement(root,"NativeStatus", XDT_String, m_nativeStatus);
  msg.AddElement(root,"NativeStatus", XDT_String, m_nativeStatus);

  // Add the result data set
  if(!m_data.empty())
  {
    XMLElement* data = msg.AddElement(root,"Data",XDT_String,"");
    for(auto& value : m_data)
    {
      XMLElement* column = msg.AddElement(data,"Column",XDT_String,"");
      msg.AddElement(column,"Name",XDT_String,           value.first);
      msg.AddElement(column,"Value",XDT_String|XDT_CDATA,value.second);
    }
  }

  // Now save it
  return msg.SaveFile(p_filename);
}

void 
StepResultSQL::CheckFilename(CString p_filename)
{
  // Split of only the extension
  char extension[_MAX_EXT];
  _splitpath_s(p_filename,NULL,0,NULL,0,NULL,0,extension,_MAX_EXT);

  // Check that we have the right one
  if(_strnicmp(extension,EXTENSION_RESULT_SQL,5))
  {
    throw StdException("A StepResult XML definition file must be saved as a *.QRES");
  }
}

void 
StepResultSQL::ResetEffective()
{
  m_succeeded  = false;
  m_resultRows = 0;
  m_resultCols = 0;
  m_sqlState.Empty();
  m_nativeStatus.Empty();

  // Result data set
  m_data.clear();
}

void
StepResultSQL::AddResult(CString p_name, CString p_value)
{
  ResultMap::iterator it = m_data.find(p_name);
  if(it != m_data.end())
  {
    it->second = p_value;
  }
  else
  {
    m_data[p_name] = p_value;
  }
}

CString 
StepResultSQL::GetFirstData()
{
  if(m_data.size() == 1)
  {
    return m_data.begin()->second;
  }
  return CString();
}
