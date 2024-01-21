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
// This File   : Validate.cpp
// What it does: A validation step for a teststep. The main class.
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Validate.h"
#include "Parameters.h"
#include "ExtraExtensions.h"
#include "ValidateCMD.h"
#include "ValidateNET.h"
#include "ValidateSQL.h"
#include "ValidateWIN.h"
#include "TestStep.h"
#include <StdException.h>
#include <vector>
#include <io.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

Validate* ReadValidate(CString p_filename)
{
  StepType type = Validate::FindStepTypeFromFile(p_filename);

  if(type == StepType::Step_command)
  {
    ValidateCMD* validate = new ValidateCMD();
    try
    {
      validate->ReadFromXML(p_filename);
    }
    catch (StdException& ex)
    {
      delete validate;
      throw ex;
    }
    return validate;
  }
  if(type == StepType::Step_http)
  {
    ValidateNET* validate = new ValidateNET();
    try
    {
      validate->ReadFromXML(p_filename);
    }
    catch (StdException& ex)
    {
      delete validate;
      throw ex;
    }
    return validate;
  }
  if(type == StepType::Step_sql)
  {
    ValidateSQL* validate = new ValidateSQL();
    try
    {
      validate->ReadFromXML(p_filename);
    }
    catch (StdException& ex)
    {
      delete validate;
      throw ex;
    }
    return validate;
  }
  if(type == StepType::Step_win)
  {
    ValidateWIN* validate = new ValidateWIN();
    try
    {
      validate->ReadFromXML(p_filename);
    }
    catch (StdException& ex)
    {
      delete validate;
      throw ex;
    }
    return validate;
  }
  return nullptr;
}

/*static*/ StepType
Validate::FindStepTypeFromFile(CString p_filename)
{
  WinFile file(p_filename);
  CString extension = file.GetFilenamePartExtension();

  if(extension.CompareNoCase(EXTENSION_VALIDATE_CMD) == 0)
  {
    return StepType::Step_command;
  }
  if(extension.CompareNoCase(EXTENSION_VALIDATE_NET) == 0)
  {
    return StepType::Step_http;
  }
  if(extension.CompareNoCase(EXTENSION_VALIDATE_SQL) == 0)
  {
    return StepType::Step_sql;
  }
  if(extension.CompareNoCase(EXTENSION_VALIDATE_WIN) == 0)
  {
    return StepType::Step_win;
  }
  return StepType::Step_unknown;
}

// Interface with the file system
void 
Validate::ReadFromXML(XMLMessage& p_msg,CString p_filename)
{
  // Check that we have a *.xrun extension
  CheckFilename(p_filename);

  // Load the file (if any)
  if(p_msg.LoadFile(p_filename) == false)
  {
    throw StdException("Could not load the XML file: " + p_filename);
  }

  // Remember our file
  m_filename = p_filename;

  // CHeck for XML error
  if(p_msg.GetInternalError() != XmlError::XE_NoError)
  {
    CString error;
    error.Format("Internal XML error in XVAL file [%d] %s",p_msg.GetInternalError(),p_msg.GetInternalErrorString().GetString());
    throw StdException(error);
  }

  // Check that it is our message type
  if(p_msg.GetRootNodeName().Compare("Validate"))
  {
    throw StdException("XVAL file is not a validation definition: " + p_filename);
  }

  // General elements
  m_name          = p_msg.GetElement("Name");
  m_documentation = p_msg.GetElement("Documentation");
}

bool
Validate::WriteToXML(XMLMessage& p_msg,CString p_filename)
{
  p_msg.SetRootNodeName("Validate");

  XMLElement* root = p_msg.GetRoot();
  p_msg.AddElement(root,"Name",XDT_String,m_name);
  p_msg.AddElement(root,"Documentation",XDT_String,m_documentation);

  return true;
}

CString 
Validate::ReturnOperatorToString(ReturnOperator p_oper)
{
  CString string;
  switch (p_oper)
  {
    case ReturnOperator::ROP_NOP:           string = "NOP";               break;
    case ReturnOperator::ROP_EQUAL:         string = "EQUAL";             break;
    case ReturnOperator::ROP_GREATER:       string = "GREATER";           break;
    case ReturnOperator::ROP_GREATEREQUAL:  string = "GREATER_OR_EQUAL";  break;
    case ReturnOperator::ROP_SMALLER:       string = "SMALLER";           break;
    case ReturnOperator::ROP_SMALLEREQUAL:  string = "SMALLER_OR_EQUAL";  break;
    case ReturnOperator::ROP_NOTEQUAL:      string = "NOT_EQUAL";         break;
    case ReturnOperator::ROP_BETWEEN:       string = "BETWEEN";           break;
    case ReturnOperator::ROP_IN:            string = "IN";                break;
  }
  return string;
}

CString 
Validate::BufferOperatorToString(BufferOperator p_oper)
{
  CString string;
  switch (p_oper)
  {
    case BufferOperator::BOP_NOP:         string = "NOP";         break;
    case BufferOperator::BOP_EXACT:       string = "EXACT";       break;
    case BufferOperator::BOP_CONTAINS:    string = "CONTAINS";    break;
    case BufferOperator::BOP_BEGINSWITH:  string = "BEGINSWITH";  break;
    case BufferOperator::BOP_ENDSWITH:    string = "ENDSWITH";    break;
    case BufferOperator::BOP_ISEMPTY:     string = "ISEMPTY";     break;
    case BufferOperator::BOP_NOTEMPTY:    string = "NOTEMPTY";    break;
    case BufferOperator::BOP_NOTFOUND:    string = "NOTFOUND";    break;
    case BufferOperator::BOP_FILEMATCH:   string = "FILEMATCH";   break;
    case BufferOperator::BOP_EXISTFILE:   string = "EXISTFILE";   break;
  }
  return string;
}

ReturnOperator 
Validate::StringToReturnOperator(CString p_oper)
{
  ReturnOperator oper = ReturnOperator::ROP_NOP;

       if(p_oper.Compare("NOP")               == 0) oper = ReturnOperator::ROP_NOP;
  else if(p_oper.Compare("EQUAL")             == 0) oper = ReturnOperator::ROP_EQUAL;
  else if(p_oper.Compare("GREATER")           == 0) oper = ReturnOperator::ROP_GREATER;
  else if(p_oper.Compare("GREATER_OR_EQUAL")  == 0) oper = ReturnOperator::ROP_GREATEREQUAL;
  else if(p_oper.Compare("SMALLER")           == 0) oper = ReturnOperator::ROP_SMALLER;
  else if(p_oper.Compare("SMALLER_OR_EQUAL")  == 0) oper = ReturnOperator::ROP_SMALLEREQUAL;
  else if(p_oper.Compare("NOT_EQUAL")         == 0) oper = ReturnOperator::ROP_NOTEQUAL;
  else if(p_oper.Compare("EQUAL")             == 0) oper = ReturnOperator::ROP_EQUAL;
  else if(p_oper.Compare("BETWEEN")           == 0) oper = ReturnOperator::ROP_BETWEEN;
  else if(p_oper.Compare("IN")                == 0) oper = ReturnOperator::ROP_IN;
  else                                              oper = ReturnOperator::ROP_NOP;

  return oper;
}

BufferOperator 
Validate::StringToBufferOperator(CString p_oper)
{
  BufferOperator oper = BufferOperator::BOP_NOP;

       if(p_oper.Compare("NOP")         == 0) oper = BufferOperator::BOP_NOP;
  else if(p_oper.Compare("EXACT")       == 0) oper = BufferOperator::BOP_EXACT;
  else if(p_oper.Compare("CONTAINS")    == 0) oper = BufferOperator::BOP_CONTAINS;
  else if(p_oper.Compare("BEGINSWITH")  == 0) oper = BufferOperator::BOP_BEGINSWITH;
  else if(p_oper.Compare("ENDSWITH")    == 0) oper = BufferOperator::BOP_ENDSWITH;
  else if(p_oper.Compare("ISEMPTY")     == 0) oper = BufferOperator::BOP_ISEMPTY;
  else if(p_oper.Compare("NOTEMPTY")    == 0) oper = BufferOperator::BOP_NOTEMPTY;
  else if(p_oper.Compare("NOTFOUND")    == 0) oper = BufferOperator::BOP_NOTFOUND;
  else if(p_oper.Compare("FILEMATCH")   == 0) oper = BufferOperator::BOP_FILEMATCH;
  else if(p_oper.Compare("EXISTFILE")   == 0) oper = BufferOperator::BOP_EXISTFILE;
  else                                        oper = BufferOperator::BOP_NOP;

  return oper;
}

bool
Validate::Between(CString p_effective,int p_value)
{
  int pos = p_effective.Find(',');
  if(pos < 0)
  {
    // No two values, so comparison is always false
    return false;
  }

  int lower = atoi(p_effective.Left(pos));
  int upper = atoi(p_effective.Mid(pos + 1));

  // See if we are in this interval (inclusive)
  return (lower <= p_value) && (p_value <= upper);
}

bool
Validate::ValueIn(CString p_effective,int p_value)
{
  std::vector<CString> values;

  // Split effective return in an array
  CString total(p_effective);
  while(total.GetLength())
  {
    int pos = total.Find(',');
    if(pos >= 0)
    {
      values.push_back(total.Left(pos));
      total = total.Mid(pos + 1);
    }
    else
    {
      values.push_back(total);
      total.Empty();
    }
  }

  // See if it is one of the values
  for(auto& val : values)
  {
    if(p_value == atoi(val))
    {
      return true;
    }
  }
  return false;
}

bool
Validate::FileMatch(CString p_file1,CString p_file2)
{
  bool result = false;
  ULONG size1 = 0;
  ULONG size2 = 0;

  // Read in both files
  char* buffer1 = ReadFileInBuffer(p_file1,size1);
  char* buffer2 = ReadFileInBuffer(p_file1,size2);

  // Compare
  if(buffer1 && buffer2 && (size1 == size2) || memcmp(buffer1,buffer2,size1) == 0)
  {
    result = true;
  }

  // Free buffers (caller must free the buffer
  free(buffer1);
  free(buffer2);

  return result;
}

bool
Validate::FileExist(CString p_file)
{
  // Remove extra newlines from reading stdout/stderr
  p_file.Trim('\n');

  // Find out if it exists
  return _access(p_file,0) == 0;
}

// Read a file in a buffer
// Caller must free the buffer again (if any)
char*
Validate::ReadFileInBuffer(CString p_filename,ULONG& p_size)
{
  // Try to open the file
  HANDLE file = CreateFile(p_filename            // Filename
                           ,GENERIC_READ          // Access mode
                           ,FILE_SHARE_READ       // Share mode
                           ,NULL                  // Security
                           ,OPEN_EXISTING         // Create or open
                           ,FILE_ATTRIBUTE_NORMAL // Attributes
                           ,NULL);                // Template file

  if(file == INVALID_HANDLE_VALUE)
  {
    return nullptr;
  }

  // Get the size of the file so we can create an internal buffer
  DWORD sizeHigh = NULL;
  DWORD sizeLow = GetFileSize(file,&sizeHigh);
  if(sizeLow == INVALID_FILE_SIZE || sizeHigh)
  {
    // Cannot get the file size, or file bigger than 4GB
    CloseHandle(file);
    return nullptr;
  }

  // Allocate a big enough buffer
  char* buffer = (char*)malloc(sizeLow + 1);

  // Read file from disk in one (1) go
  bool  result = false;
  DWORD readBytes = 0;
  if(::ReadFile(file,buffer,sizeLow,&readBytes,NULL))
  {
    if(readBytes >= (DWORD)sizeLow)
    {
      result = true;
    }
    // End buffer to be sure
    buffer[sizeLow] = 0;
  }

  // Close file
  CloseHandle(file);

  // In case of an error: free again
  if(!result)
  {
    free(buffer);
    buffer = nullptr;
  }
  else
  {
    // Success: return the size of the buffer
    p_size = sizeLow;
  }
  return buffer;
}
