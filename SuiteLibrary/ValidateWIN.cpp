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
// This File   : ValidateSQL.cpp
// What it does: A validation step for a teststep. The validation of a Windows UI step
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ValidateWIN.h"
#include "Parameters.h"
#include "ExtraExtensions.h"
#include <StdException.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// Interface with the file system
void
ValidateWIN::ReadFromXML(CString p_filename)
{
  XMLMessage msg;
  Validate::ReadFromXML(msg, p_filename);

  // Return value validation
  XMLElement* retval = msg.FindElement(_T("CheckReturn"));
  if(retval)
  {
    m_returnIsSigned   = msg.GetElementBoolean(retval,_T("IsSigned"));
    m_checkReturnValue = msg.GetElementBoolean(retval,_T("Check"));
    m_returnOperator   = StringToReturnOperator(msg.GetElement(retval,_T("Operator")));
    m_expectedReturn   = msg.GetElement(retval,_T("Expected"));
  }

  // Standard output validation
  XMLElement* logging = msg.FindElement(_T("CheckLogging"));
  if(logging)
  {
    m_checkLogging    = msg.GetElementBoolean(logging,_T("Check"));
    m_loggingOperator = StringToBufferOperator(msg.GetElement(logging,_T("Operator")));
    m_expectedLogging = msg.GetElement(logging,_T("Expected"));
  }

  // Standard error validation
  XMLElement* errors = msg.FindElement(_T("CheckErrors"));
  if(errors)
  {
    m_checkErrors    = msg.GetElementBoolean(errors,_T("Check"));
    m_errorsOperator = StringToBufferOperator(msg.GetElement(errors,_T("Operator")));
    m_expectedErrors = msg.GetElement(errors,_T("Expected"));
  }
}

bool
ValidateWIN::WriteToXML(CString p_filename)
{
  XMLMessage msg;
  if (!Validate::WriteToXML(msg, p_filename))
  {
    return false;
  }
  XMLElement* root   = msg.GetRoot();
  XMLElement* retval = msg.AddElement(root,_T("CheckReturn"),XDT_String,_T(""));
  msg.SetElement(retval,_T("IsSigned"),m_returnIsSigned);
  msg.SetElement(retval,_T("Check"),   m_checkReturnValue);
  msg.SetElement(retval,_T("Operator"),ReturnOperatorToString(m_returnOperator));
  msg.AddElement(retval,_T("Expected"),XDT_CDATA,m_expectedReturn);

  XMLElement* output = msg.AddElement(root,_T("CheckLogging"),XDT_String,_T(""));
  msg.SetElement(output,_T("Check"),   m_checkLogging);
  msg.SetElement(output,_T("Operator"),BufferOperatorToString(m_loggingOperator));
  msg.AddElement(output,_T("Expected"),XDT_CDATA,m_expectedLogging);

  XMLElement* error = msg.AddElement(root,_T("CheckErrors"),XDT_String,_T(""));
  msg.SetElement(error,_T("Check"),   m_checkErrors);
  msg.SetElement(error,_T("Operator"),BufferOperatorToString(m_errorsOperator));
  msg.AddElement(error,_T("Expected"),XDT_CDATA,m_expectedErrors);

  // Now save it
  return msg.SaveFile(m_filename);
}

// RE-Calculate the effective strings, returning the number of unbound parameters
int
ValidateWIN::EffectiveReplacements(Parameters* p_parameters, bool p_forDisplay)
{
  int unbound = 0;

  unbound += p_parameters->Replace(m_expectedReturn, m_effectiveReturn, p_forDisplay);
  unbound += p_parameters->Replace(m_expectedLogging,m_effectiveLogging,p_forDisplay);
  unbound += p_parameters->Replace(m_expectedErrors, m_effectiveErrors, p_forDisplay);

  return unbound;
}

// Check our filenames extension
void
ValidateWIN::CheckFilename(CString p_filename)
{
  // Split of only the extension
  TCHAR extension[_MAX_EXT];
  _tsplitpath_s(p_filename,NULL,0,NULL,0,NULL,0,extension,_MAX_EXT);

  // Check that we have the right one
  if (_tcsncicmp(extension,EXTENSION_VALIDATE_WIN,5))
  {
    throw StdException(_T("A Validation XML definition file must be saved as a *.WVAL"));
  }
}

// Check the return value
bool
ValidateWIN::ValidateReturnValue(int p_value)
{
  // If we need not check it, all-OK
  if(!m_checkReturnValue)
  {
    return true;
  }

  // Convert expected result to an int
  int expected = _ttoi(m_effectiveReturn);

  // Check result, depending on the operator
  bool result = false;
  switch(m_returnOperator)
  {
    case ReturnOperator::ROP_NOP:         result = false;               break;
    case ReturnOperator::ROP_EQUAL:       result = p_value == expected; break;
    case ReturnOperator::ROP_GREATER:     result = p_value >  expected; break;
    case ReturnOperator::ROP_GREATEREQUAL:result = p_value >= expected; break;
    case ReturnOperator::ROP_SMALLER:     result = p_value <  expected; break;
    case ReturnOperator::ROP_SMALLEREQUAL:result = p_value <= expected; break;
    case ReturnOperator::ROP_NOTEQUAL:    result = p_value != expected; break;
    case ReturnOperator::ROP_BETWEEN:     result = Between(m_effectiveReturn,p_value); break;
    case ReturnOperator::ROP_IN:          result = ValueIn(m_effectiveReturn,p_value); break;
  }
  return result;
}

bool
ValidateWIN::ValidateLogging(CString p_buffer)
{
  // See if we must check the output value
  if(!m_checkLogging)
  {
    return true;
  }

  bool result = false;
  switch (m_loggingOperator)
  {
    case BufferOperator::BOP_NOP:       result = false;                                     break;
    case BufferOperator::BOP_ISEMPTY:   result = p_buffer.IsEmpty();                        break;
    case BufferOperator::BOP_NOTEMPTY:  result = p_buffer.IsEmpty() == false;               break;
    case BufferOperator::BOP_EXACT:     result = p_buffer.Compare(m_effectiveLogging) == 0; break;
    case BufferOperator::BOP_CONTAINS:  result = p_buffer.Find(m_effectiveLogging) >= 0;    break;
    case BufferOperator::BOP_BEGINSWITH:result = p_buffer.Find(m_effectiveLogging) == 0;    break;
    case BufferOperator::BOP_NOTFOUND:  result = p_buffer.Find(m_effectiveLogging)  < 0;    break;
    case BufferOperator::BOP_ENDSWITH:  result = p_buffer.Right(m_effectiveLogging.GetLength()).Compare(m_effectiveLogging) == 0; break;
    case BufferOperator::BOP_FILEMATCH: result = FileMatch(p_buffer,m_effectiveLogging);    break;
    case BufferOperator::BOP_EXISTFILE: result = FileExist(p_buffer);                       break;
  }
  return result;
}

bool
ValidateWIN::ValidateErrors(CString p_buffer)
{
  // See if we must check the error output
  // But if we DO HAVE AN ERROR, always consider it for an error
  if(!m_checkErrors && p_buffer.IsEmpty())
  {
    return true;
  }

  bool result = false;
  switch (m_errorsOperator)
  {
    case BufferOperator::BOP_NOP:       result = false;                                    break;
    case BufferOperator::BOP_ISEMPTY:   result = p_buffer.IsEmpty();                       break;
    case BufferOperator::BOP_NOTEMPTY:  result = p_buffer.IsEmpty() == false;              break;
    case BufferOperator::BOP_EXACT:     result = p_buffer.Compare(m_effectiveErrors) == 0; break;
    case BufferOperator::BOP_CONTAINS:  result = p_buffer.Find(m_effectiveErrors) >= 0;    break;
    case BufferOperator::BOP_BEGINSWITH:result = p_buffer.Find(m_effectiveErrors) == 0;    break;
    case BufferOperator::BOP_NOTFOUND:  result = p_buffer.Find(m_effectiveErrors)  < 0;    break;
    case BufferOperator::BOP_ENDSWITH:  result = p_buffer.Right(m_effectiveErrors.GetLength()).Compare(m_effectiveErrors) == 0; break;
    case BufferOperator::BOP_FILEMATCH: result = FileMatch(p_buffer,m_effectiveErrors);    break;
    case BufferOperator::BOP_EXISTFILE: result = FileExist(p_buffer);                      break;
  }
  return result;
}
