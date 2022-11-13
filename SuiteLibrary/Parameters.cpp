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
// This File   : Parameters.cpp
// What it does: Parameters in three categories (global, return, buffer)
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Parameters.h"
#include <XMLMessage.h>
#include <StdException.h>
#include <Crypto.h>
#include <io.h>

#define KWATTA_NOAUTOLINK
#include "SuiteLibrary.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

void
Parameters::Reset()
{
  m_globals.clear();
  m_buffers.clear();
  m_returns.clear();
  m_locals .clear();
}

void
Parameters::ReadFromXML(CString p_filename,bool p_global /*=true*/)
{
  XMLMessage msg;

  // Check that we have a *.xpar extension
  CheckFilename(p_filename);

  // Remember our name
  if (p_global)
  {
    m_filenameGlobal = p_filename;
  }
  else
  {
    m_filenameLocal = p_filename;
  }

  // Check if the file even exists
  if(_access(p_filename,0))
  {
    return;
  }

  // Load the file (if any)
  if (msg.LoadFile(p_filename) == false)
  {
    throw StdException("Could not load the XML file: " + p_filename);
  }

  // CHeck for XML error
  if (msg.GetInternalError() != XmlError::XE_NoError)
  {
    CString error;
    error.Format("Internal XML error in XPAR file [%d] %s", msg.GetInternalError(), msg.GetInternalErrorString().GetString());
    throw StdException(error);
  }

  // Check that it is our message type
  if (msg.GetRootNodeName().Compare("Parameters"))
  {
    throw StdException("XPAR file is not a parameters definition: " + p_filename);
  }

  // Find names
  XMLElement* root = msg.GetRoot();

  // Read all global variables
  XMLElement* system = msg.FindElement(root,"System",false);
  if(system)
  {
    XMLElement* var = msg.GetElementFirstChild(system);
    while(var)
    {
      CString name  = var->GetName();
      CString value = var->GetValue();
      CheckPasswordProtection(name,value);
      if(AddSystemParameter(name,value) == false)
      {
        TRACE("Duplicate system parameter found: %s%s\n",name.GetString(),value.GetString());
      }
      var = msg.GetElementSibling(var);
    }
  }

  // Read all global variables
  XMLElement* globals = msg.FindElement(root,"Globals",false);
  if(globals)
  {
    XMLElement* var = msg.GetElementFirstChild(globals);
    while(var)
    {
      CString name  = var->GetName();
      CString value = var->GetValue();
      if(AddGlobalParameter(name,value) == false)
      {
        TRACE("Duplicate global parameter found: %s%s\n",name.GetString(),value.GetString());
      }
      var = msg.GetElementSibling(var);
    }
  }

  // Read all local variables
  XMLElement* locals = msg.FindElement(root,"Locals",false);
  if(locals)
  {
    XMLElement* var = msg.GetElementFirstChild(locals);
    while(var)
    {
      CString name  = var->GetName();
      CString value = var->GetValue();
      if(AddLocalParameter(name,value) == false)
      {
        TRACE("Duplicate local parameter found: %s%s\n",name.GetString(),value.GetString());
      }
      var = msg.GetElementSibling(var);
    }
  }

  // Read all return variables
  XMLElement* returns = msg.FindElement(root, "Returns", false);
  if(returns)
  {
    XMLElement* var = msg.GetElementFirstChild(returns);
    while(var)
    {
      CString name  = var->GetName();
      CString value = var->GetValue();
      if(AddReturnParameter(name,value) == false)
      {
        TRACE("Duplicate return parameter found: %s%s\n",name.GetString(),value.GetString());
      }
      var = msg.GetElementSibling(var);
    }
  }

  // Read all buffer variables
  XMLElement* buffers = msg.FindElement(root,"Buffers",false);
  if(buffers)
  {
    XMLElement* var = msg.GetElementFirstChild(buffers);
    while(var)
    {
      CString name  = var->GetName();
      CString value = var->GetValue();
      if(AddBufferParameter(name,value) == false)
      {
        TRACE("Duplicate buffer parameter found: %s%s\n",name.GetString(),value.GetString());
      }
      var = msg.GetElementSibling(var);
    }
  }
}

bool
Parameters::WriteToXML(bool p_locals /*=true*/,bool p_globals /*=false*/)
{
  if(m_changed == false)
  {
    return true;
  }

  XMLMessage msg;
  int count = 0;

  msg.SetRootNodeName("Parameters");

  XMLElement* root = msg.GetRoot();

  // Save all globals
  if(p_globals)
  {
    XMLElement* system = msg.AddElement(root,"System",XDT_String,"");
    for(auto& var : m_system)
    {
      if(!var.first.IsEmpty())
      {
        msg.AddElement(system,var.first,XDT_CDATA,var.second);
        ++count;
      }
    }

    XMLElement* globals = msg.AddElement(root,"Globals",XDT_String,"");
    for(auto& var : m_globals)
    {
      if(!var.first.IsEmpty())
      {
        msg.AddElement(globals,var.first,XDT_CDATA,var.second);
        ++count;
      }
    }
  }

  // Save all locals
  if(p_locals)
  {
    // Save all local variables
    XMLElement* locals = msg.AddElement(root,"Locals",XDT_String,"");
    for(auto& var : m_locals)
    {
      if(!var.first.IsEmpty())
      {
        msg.AddElement(locals,var.first,XDT_CDATA,var.second);
        ++count;
      }
    }

    // Save all return values
    XMLElement* returns = msg.AddElement(root,"Returns",XDT_String,"");
    for(auto& var : m_returns)
    {
      if(!var.first.IsEmpty())
      {
        msg.AddElement(returns, var.first, XDT_Integer | XDT_Type, var.second);
        ++count;
      }
    }

    // Save all buffer values
    XMLElement* buffers = msg.AddElement(root,"Buffers",XDT_String,"");
    for(auto& var : m_buffers)
    {
      if(!var.first.IsEmpty())
      {
        msg.AddElement(buffers,var.first,XDT_CDATA,var.second);
        ++count;
      }
    }
  }

  // See if we have work to do
  if(count == 0)
  {
    return true;
  }

  // Now save it
  if(msg.SaveFile(p_locals ? m_filenameLocal : m_filenameGlobal))
  {
    m_changed = false;
    return true;
  }
  return false;
}

//////////////////////////////////////////////////////////////////////////
//
// Programmatic interface
//
//////////////////////////////////////////////////////////////////////////

bool
Parameters::ExistsAsSystemParameter(CString p_name)
{
  ParMap::iterator it = m_system.find(p_name);
  if(it != m_system.end())
  {
    return true;
  }
  return false;
}

bool
Parameters::ExistsAsGlobalParameter(CString p_name)
{
  // If we protect our password, the variable exists!
  if(p_name.CompareNoCase("password") == 0)
  {
    return true;
  }

  ParMap::iterator it = m_globals.find(p_name);
  if(it != m_globals.end())
  {
    return true;
  }
  return false;
}

bool
Parameters::ExistsAsLocalParameter(CString p_name)
{
  ParMap::iterator it = m_locals.find(p_name);
  if(it != m_locals.end())
  {
    return true;
  }
  return false;
}

bool
Parameters::ExistsAsReturnParameter(CString p_name)
{
  ParMap::iterator it = m_returns.find(p_name);
  if(it != m_returns.end())
  {
    return true;
  }
  return false;
}

bool
Parameters::ExistsAsBufferParameter(CString p_name)
{
  ParMap::iterator it = m_buffers.find(p_name);
  if(it != m_buffers.end())
  {
    return true;
  }
  return false;
}

bool
Parameters::ExistsAsEnvironParameter(CString p_name)
{
  CString value;
  if(value.GetEnvironmentVariable(p_name))
  {
    return true;
  }
  return false;
}


CString
Parameters::FindBufferParameter(CString p_name)
{
  ParMap::iterator it = m_buffers.find(p_name);
  if(it != m_buffers.end())
  {
    return it->second;
  }
  return "";
}

CString
Parameters::FindReturnParameter(CString p_name)
{
  ParMap::iterator it = m_returns.find(p_name);
  if (it != m_returns.end())
  {
    return it->second;
  }
  return "";
}

CString
Parameters::FindGlobalParameter(CString p_name,bool p_forDisplay)
{
  // In case we protect our password, it comes from the environment variable
  if(p_name.CompareNoCase("password") == 0)
  {
    if(p_forDisplay)
    {
      return "############";
    }

    if(m_password)
    {
      CString pwd;
      if(pwd.GetEnvironmentVariable(KWATTA_PASSWORD))
      {
        Crypto crypt;
        pwd = crypt.Decryption(pwd,KWATTA_ENCRYPT);
        return pwd;
      }
      else return "";
    }
  }

  ParMap::iterator it = m_globals.find(p_name);
  if (it != m_globals.end())
  {
    return it->second;
  }
  return "";
}

CString
Parameters::FindLocalParameter(CString p_name)
{
  ParMap::iterator it = m_locals.find(p_name);
  if (it != m_locals.end())
  {
    return it->second;
  }
  return "";
}

CString
Parameters::FindSystemParameter(CString p_name)
{
  ParMap::iterator it = m_system.find(p_name);
  if (it != m_system.end())
  {
    return it->second;
  }
  return "";
}

CString
Parameters::FindEnvironParameter(CString p_name)
{
  if(m_environmentValue.GetEnvironmentVariable(p_name))
  {
    if(m_environmentValue.IsEmpty())
    {
      return "";
    }
    return m_environmentValue;
  }
  return "";
}

bool
Parameters::AddBufferParameter(CString p_name,CString p_value)
{
  if(NameNotYetUsed(p_name))
  {
    m_changed = true;
    m_buffers.insert(std::make_pair(p_name,p_value));
    return true;
  }
  return false;
}

bool
Parameters::AddReturnParameter(CString p_name,CString p_value)
{
  if(NameNotYetUsed(p_name))
  {
    m_changed = true;
    m_returns.insert(std::make_pair(p_name,p_value));
    return true;
  }
  return false;
}

bool
Parameters::AddSystemParameter(CString p_name,CString p_value)
{
  if(NameNotYetUsed(p_name))
  {
    m_changed = true;
    m_system.insert(std::make_pair(p_name, p_value));
    return true;
  }
  return false;
}

bool
Parameters::AddLocalParameter(CString p_name,CString p_value)
{
  if(NameNotYetUsed(p_name))
  {
    m_changed = true;
    m_locals.insert(std::make_pair(p_name, p_value));
    return true;
  }
  return false;
}

bool
Parameters::AddGlobalParameter(CString p_name,CString p_value)
{
  // Protected password always succeeds
  if(m_password && p_value.CompareNoCase("password") == 0)
  {
    return true;
  }

  if(NameNotYetUsed(p_name))
  {
    m_changed = true;
    m_globals.insert(std::make_pair(p_name,p_value));
    return true;
  }
  return false;
}

void
Parameters::OverwriteBufferParameter(CString p_name,CString p_value)
{
  ParMap::iterator it = m_buffers.find(p_name);
  if(it == m_buffers.end())
  {
    m_buffers.insert(std::make_pair(p_name,p_value));
  }
  else
  {
    it->second = p_value;
  }
  m_changed = true;
}

void
Parameters::OverwriteReturnParameter(CString p_name,CString p_value)
{
  ParMap::iterator it = m_returns.find(p_name);
  if(it == m_returns.end())
  {
    m_returns.insert(std::make_pair(p_name,p_value));
  }
  else
  {
    it->second = p_value;
  }
  m_changed = true;
}

void
Parameters::OverwriteLocalParameter(CString p_name,CString p_value)
{
  ParMap::iterator it = m_locals.find(p_name);
  if(it == m_locals.end())
  {
    m_locals.insert(std::make_pair(p_name,p_value));
  }
  else
  {
    it->second = p_value;
  }
  m_changed = true;
}

void
Parameters::OverwriteGlobalParameter(CString p_name,CString p_value)
{
  // Password protection
  if(m_password && p_name.CompareNoCase("password") == 0)
  {
    return;
  }

  ParMap::iterator it = m_globals.find(p_name);
  if (it == m_globals.end())
  {
    m_globals.insert(std::make_pair(p_name,p_value));
  }
  else
  {
    it->second = p_value;
  }
  m_changed = true;
}

void
Parameters::OverwriteSystemParameter(CString p_name,CString p_value)
{
  ParMap::iterator it = m_system.find(p_name);
  if(it == m_system.end())
  {
    m_system.insert(std::make_pair(p_name, p_value));
  }
  else
  {
    it->second = p_value;
  }
  m_changed = true;
}

bool
Parameters::RemoveBufferParameter(CString p_name)
{
  ParMap::iterator it = m_buffers.find(p_name);
  if(it != m_buffers.end())
  {
    m_buffers.erase(it);
    m_changed = true;
    return true;
  }
  return false;
}

bool
Parameters::RemoveSystemParameter(CString p_name)
{
  ParMap::iterator it = m_system.find(p_name);
  if (it != m_system.end())
  {
    m_system.erase(it);
    m_changed = true;
    return true;
  }
  return false;
}

bool
Parameters::RemoveReturnParameter(CString p_name)
{
  ParMap::iterator it = m_returns.find(p_name);
  if(it != m_returns.end())
  {
    m_returns.erase(it);
    m_changed = true;
    return true;
  }
  return false;
}

bool
Parameters::RemoveLocalParameter(CString p_name)
{
  ParMap::iterator it = m_locals.find(p_name);
  if(it != m_locals.end())
  {
    m_locals.erase(it);
    m_changed = true;
    return true;
  }
  return false;
}

bool
Parameters::RemoveGlobalParameter(CString p_name)
{
  if(m_password && p_name.CompareNoCase("password") == 0)
  {
    return true;
  }

  ParMap::iterator it = m_globals.find(p_name);
  if(it != m_globals.end())
  {
    m_globals.erase(it);
    m_changed = true;
    return true;
  }
  return false;
}

//////////////////////////////////////////////////////////////////////////
//
// THIS IS WHY WE EXIST
//
//////////////////////////////////////////////////////////////////////////

// The general replace function for bound parameters
int
Parameters::Replace(CString p_input,CString& p_output,bool p_forDisplay,ParType p_exclude /*= PAR_NONE*/)
{
  int notfound = 0;

  // Do all replacements
  notfound += Replace(p_input,'$','$',ParType::PAR_GLOBAL, p_forDisplay,p_exclude);  // Global variables
  notfound += Replace(p_input,'[',']',ParType::PAR_RETURN, p_forDisplay,p_exclude);  // Return variables
  notfound += Replace(p_input,'<','>',ParType::PAR_BUFFER, p_forDisplay,p_exclude);  // Buffer variables
  notfound += Replace(p_input,'%','%',ParType::PAR_ENVIRON,p_forDisplay,p_exclude);  // Environment variables
  notfound += Replace(p_input,'#','#',ParType::PAR_LOCAL,  p_forDisplay,p_exclude);  // Environment variables

  // Set the output
  p_output = p_input;
  return notfound;
}

//////////////////////////////////////////////////////////////////////////
//
// PRIVATE
//
//////////////////////////////////////////////////////////////////////////

// Check if we should do password protection
void
Parameters::CheckPasswordProtection(CString p_name, CString p_value)
{
  if(p_name.Compare("HidePassword") == 0)
  {
    m_password = atoi(p_value) > 0;
  }
}

void
Parameters::CheckFilename(CString p_filename)
{
  // Split of only the extension
  char extension[_MAX_EXT];
  _splitpath_s(p_filename, NULL, 0, NULL, 0, NULL, 0, extension, _MAX_EXT);

  // Check that we have the right one
  if (_strnicmp(extension, ".xpar", 5))
  {
    throw StdException("A parameters XML definition file must be saved as a *.XPAR");
  }
}

// Name not yet used in other maps
// This ensures global uniqueness within a testset
bool
Parameters::NameNotYetUsed(CString p_name)
{
  if(m_system .find(p_name) != m_system .end()) return false;
  if(m_buffers.find(p_name) != m_buffers.end()) return false;
  if(m_returns.find(p_name) != m_returns.end()) return false;
  if(m_globals.find(p_name) != m_globals.end()) return false;
  if(m_locals .find(p_name) != m_locals .end()) return false;

  return true;
}

// This is our general workhorse to replace a variable in a string
// processes all strings and all types of variables
//
int
Parameters::Replace(CString& p_string
                   ,char     p_first
                   ,char     p_last
                   ,ParType  p_find
                   ,bool     p_forDisplay
                   ,ParType  p_exclude /*= PAR_NONE*/)
{
  int last = 0;
  int notReplaced = 0;
  CString replaced;

  // Check if we should perform our actions
  if(p_find == p_exclude)
  {
    return notReplaced;
  }

  for(int ind = 0; ind < p_string.GetLength(); ++ind)
  {
    int ch = p_string.GetAt(ind);
    if(ch == p_first)
    {
      //// "str$name$test
      int pos = p_string.Find(p_last,ind + 1);
      if(pos > ind)
      {
        CString varName = p_string.Mid(ind + 1,pos - ind - 1);

        CString value;
        bool exists(false);
        switch (p_find)
        {
          case ParType::PAR_GLOBAL:  exists = ExistsAsGlobalParameter(varName);
                                     value  = FindGlobalParameter(varName,p_forDisplay); 
                                     break;
          case ParType::PAR_LOCAL:   exists = ExistsAsLocalParameter(varName);
                                     value  = FindLocalParameter(varName); 
                                     break;
          case ParType::PAR_RETURN:  exists = ExistsAsReturnParameter(varName);
                                     value  = FindReturnParameter(varName); 
                                     break;
          case ParType::PAR_BUFFER:  exists = ExistsAsBufferParameter(varName);
                                     value  = FindBufferParameter(varName); 
                                     break;
          case ParType::PAR_ENVIRON: exists = ExistsAsEnvironParameter(varName);
                                     value  = FindEnvironParameter(varName); 
                                     break;
        }
        
        if(exists)
        {
          // All OK, variable found
          replaced += value;
          // Skip past parameter name
          ind = pos;
        }
        else
        {
          // Variable not found
          replaced += p_first;
          ++notReplaced;
        }
      }
      else
      {
        // End symbol not found: Stray symbol in the string
        replaced += p_first;
      }
    }
    else if (ch == '\\')
    {
      // Skip passed a meta character
      int meta = p_string.GetAt(ind + 1);
      if(meta)
      {
        if(meta == '$' || meta == '%' || meta == '#' || 
           meta == '[' || meta == ']' || meta == '<' || meta == '>')
        {
          replaced += (char)meta;
        }
        else
        {
          replaced += (char)ch;
          replaced += (char)meta;
        }
      }
      ++ind;
    }
    else
    {
      // Normal character
      replaced += (unsigned char) ch;
    }
    last = ch;
  }

  // Result
  p_string = replaced;
  return notReplaced;
}
