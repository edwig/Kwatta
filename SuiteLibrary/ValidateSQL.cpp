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
// What it does: A validation step for a teststep. The validation of a database SQL step
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ValidateSQL.h"
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
ValidateSQL::ReadFromXML(CString p_filename)
{
  XMLMessage msg;
  Validate::ReadFromXML(msg, p_filename);

  // Check the succeeded status
  XMLElement* elem = msg.FindElement(_T("CheckSucceeded"));
  if(elem)
  {
    m_checkSucceeded    = msg.GetElementBoolean(elem,_T("Check"));
    m_succeedOperator   = StringToReturnOperator(msg.GetElement(elem,_T("Operator")));
    m_succeeded         = msg.GetElement(elem, _T("Expected"));
    m_succeededVariable = msg.GetElement(elem,_T("Variable"));
  }

  // Check the number of affected/returned rows
  elem = msg.FindElement(_T("ReturnedRows"));
  if(elem)
  {
    m_checkRows            = msg.GetElementBoolean(elem,_T("Check"));
    m_rowsOperator         = StringToReturnOperator(msg.GetElement(elem,_T("Operator")));
    m_returnedRows         = msg.GetElement(elem,_T("Expected"));
    m_returnedRowsVariable = msg.GetElement(_T("Variable"));
  }

  // Check the number of affected/returned cols
  elem = msg.FindElement(_T("ReturnedColumns"));
  if(elem)
  {
    m_checkCols            = msg.GetElementBoolean(elem,_T("Check"));
    m_colsOperator         = StringToReturnOperator(msg.GetElement(elem,_T("Operator")));
    m_returnedCols         = msg.GetElement(elem,_T("Expected"));
    m_returnedColsVariable = msg.GetElement(_T("Variable"));
  }

  // ODBC Status
  elem = msg.FindElement(_T("SQLState"));
  if(elem)
  {
    m_checkSQLState      = msg.GetElementBoolean(elem,_T("Check"));
    m_stateOperator      = StringToBufferOperator(msg.GetElement(elem,_T("Operator")));
    m_sqlState           = msg.GetElement(elem,_T("Expected"));
    m_returnedSQLStateVariable = msg.GetElement(_T("Variable"));
  }

  // Native Status
  elem = msg.FindElement(_T("NativeStatus"));
  if(elem)
  {
    m_checkNativeStatus      = msg.GetElementBoolean(elem,_T("Check"));
    m_nativeOperator         = StringToBufferOperator(msg.GetElement(elem,_T("Operator")));
    m_nativeStatus           = msg.GetElement(elem,_T("Expected"));
    m_returnedNativeVariable = msg.GetElement(_T("Variable"));
  }

  // Check the returned SQL data
  elem = msg.FindElement(_T("Data"));
  if(elem)
  {
    m_checkData            = msg.GetElementBoolean(elem,_T("Check"));
    m_dataOperator         = StringToBufferOperator(msg.GetElement(elem,_T("Operator")));
    m_returnedDataVariable = msg.GetElement(elem,_T("Variable"));
    XMLElement* row = msg.FindElement(elem,_T("ROW"));
    if(row)
    {
      XMLElement* column = msg.FindElement(row,_T("Column"));
      while(column)
      {
        CString name  = msg.GetElement(column,_T("Name"));
        CString value = msg.GetElement(column,_T("Data"));
        // Store this value
        m_data[name] = value;

        // Next column
        column = msg.GetElementSibling(column);
      }
    }
  }
}

bool
ValidateSQL::WriteToXML(CString p_filename)
{
  XMLMessage msg;
  if(!Validate::WriteToXML(msg,p_filename))
  {
    return false;
  }
  XMLElement* root = msg.GetRoot();

  // Check the succeeded status
  XMLElement* elem = msg.AddElement(root,_T("CheckSucceeded"),XDT_String,_T(""));
  msg.SetElement(elem,_T("Check"),   m_checkSucceeded);
  msg.SetElement(elem,_T("Operator"),ReturnOperatorToString(m_succeedOperator));
  msg.SetElement(elem,_T("Expected"),m_succeeded);
  msg.SetElement(elem,_T("Variable"),m_succeededVariable);

  // Check the number of affected/returned rows
  elem = msg.AddElement(root,_T("ReturnedRows"),XDT_String,_T(""));
  msg.SetElement(elem,_T("Check"),   m_checkRows);
  msg.SetElement(elem,_T("Operator"),ReturnOperatorToString(m_rowsOperator));
  msg.SetElement(elem,_T("Expected"),m_returnedRows);
  msg.SetElement(elem,_T("Variable"),m_returnedRowsVariable);

  // Check the number of affected/returned cols
  elem = msg.AddElement(root,_T("ReturnedColumns"),XDT_String,_T(""));
  msg.SetElement(elem,_T("Check"),   m_checkCols);
  msg.SetElement(elem,_T("Operator"),ReturnOperatorToString(m_colsOperator));
  msg.SetElement(elem,_T("Expected"),m_returnedCols);
  msg.SetElement(elem,_T("Variable"),m_returnedColsVariable);

  // ODBC Status
  elem = msg.AddElement(root,_T("SQLState"),XDT_String,_T(""));
  msg.SetElement(elem,_T("Check"),   m_checkSQLState);
  msg.SetElement(elem,_T("Operator"),BufferOperatorToString(m_stateOperator));
  msg.SetElement(elem,_T("Expected"),m_sqlState);
  msg.SetElement(elem,_T("Variable"),m_returnedSQLStateVariable);

  // Native Status
  elem = msg.AddElement(root,_T("NativeStatus"),XDT_String,_T(""));
  msg.SetElement(elem,_T("Check"),   m_checkNativeStatus);
  msg.SetElement(elem,_T("Operator"),BufferOperatorToString(m_nativeOperator));
  msg.SetElement(elem,_T("Expected"),m_nativeStatus);
  msg.SetElement(elem,_T("Variable"),m_returnedNativeVariable);

  // Check the returned SQL data
  elem = msg.AddElement(root,_T("Data"),XDT_String,_T(""));
  msg.SetElement(elem,_T("Check"),m_checkData);
  msg.SetElement(elem,_T("Operator"),BufferOperatorToString(m_dataOperator));
  msg.SetElement(elem,_T("Variable"),m_returnedDataVariable);
  elem = msg.AddElement(elem,_T("ROW"),XDT_String,_T(""));
  for (auto& coldat : m_data)
  {
    XMLElement* cell = msg.AddElement(elem,_T("Column"), XDT_String, _T(""));
    msg.SetElement(cell,_T("Name"),coldat.first);
    msg.AddElement(cell,_T("Data"),XDT_String|XDT_CDATA,coldat.second);
  }

  // Now save it
  return msg.SaveFile(m_filename);
}

// RE-Calculate the effective strings, returning the number of unbound parameters
int
ValidateSQL::EffectiveReplacements(Parameters* p_parameters, bool p_forDisplay)
{
  int unbound = 0;

  unbound += p_parameters->Replace(m_succeeded,   m_effectiveSucceeded,   p_forDisplay);
  unbound += p_parameters->Replace(m_returnedRows,m_effectiveReturnedRows,p_forDisplay);
  unbound += p_parameters->Replace(m_returnedCols,m_effectiveReturnedCols,p_forDisplay);
  unbound += p_parameters->Replace(m_sqlState,    m_effectiveSQLState,    p_forDisplay);
  unbound += p_parameters->Replace(m_nativeStatus,m_effectiveNativeStatus,p_forDisplay);

  m_effectiveData.Empty();
  if(m_data.size() == 1)
  {
    for(auto& coldat : m_data)
    {
      m_effectiveData = coldat.second;
      unbound += p_parameters->Replace(coldat.second,m_effectiveData,p_forDisplay);
    }
  }
  return unbound;
}

// Check our filenames extension
void
ValidateSQL::CheckFilename(CString p_filename)
{
  // Split of only the extension
  TCHAR extension[_MAX_EXT];
  _tsplitpath_s(p_filename,NULL,0,NULL,0,NULL,0,extension,_MAX_EXT);

  // Check that we have the right one
  if(_tcsncicmp(extension,EXTENSION_VALIDATE_SQL,5))
  {
    throw StdException(_T("A Validation XML definition file must be saved as a *.QVAL"));
  }
}

bool
ValidateSQL::ValidateSucceeded(Parameters* p_parameters,int p_status)
{
  // If we need not check it, all-OK
  if(!m_checkSucceeded)
  {
    return true;
  }

  // Convert expected result to an int
  int expected = _ttoi(m_effectiveSucceeded);

  // Check result, depending on the operator
  bool result = false;
  switch(m_succeedOperator)
  {
    case ReturnOperator::ROP_NOP:         result = false;                break;
    case ReturnOperator::ROP_EQUAL:       result = p_status == expected; break;
    case ReturnOperator::ROP_GREATER:     result = p_status >  expected; break;
    case ReturnOperator::ROP_GREATEREQUAL:result = p_status >= expected; break;
    case ReturnOperator::ROP_SMALLER:     result = p_status <  expected; break;
    case ReturnOperator::ROP_SMALLEREQUAL:result = p_status <= expected; break;
    case ReturnOperator::ROP_NOTEQUAL:    result = p_status != expected; break;
    case ReturnOperator::ROP_BETWEEN:     result = Between(m_effectiveSucceeded,p_status); break;
    case ReturnOperator::ROP_IN:          result = ValueIn(m_effectiveSucceeded,p_status); break;
  }
  // Save as result of the test
  if(result && !m_succeededVariable.IsEmpty())
  {
    CString status;
    status.Format(_T("%d"),p_status);
    p_parameters->OverwriteReturnParameter(m_succeededVariable,status);
  }
  return result;
}

bool
ValidateSQL::ValidateReturnedRows(Parameters* p_parameters,int p_rows)
{
  // If we need not check it, all-OK
  if(!m_checkRows)
  {
    return true;
  }

  // Convert expected result to an int
  int expected = _ttoi(m_effectiveReturnedRows);

  // Check result, depending on the operator
  bool result = false;
  switch(m_rowsOperator)
  {
    case ReturnOperator::ROP_NOP:         result = false;                break;
    case ReturnOperator::ROP_EQUAL:       result = p_rows == expected; break;
    case ReturnOperator::ROP_GREATER:     result = p_rows >  expected; break;
    case ReturnOperator::ROP_GREATEREQUAL:result = p_rows >= expected; break;
    case ReturnOperator::ROP_SMALLER:     result = p_rows <  expected; break;
    case ReturnOperator::ROP_SMALLEREQUAL:result = p_rows <= expected; break;
    case ReturnOperator::ROP_NOTEQUAL:    result = p_rows != expected; break;
    case ReturnOperator::ROP_BETWEEN:     result = Between(m_effectiveReturnedRows,p_rows); break;
    case ReturnOperator::ROP_IN:          result = ValueIn(m_effectiveReturnedRows,p_rows); break;
  }
  // Save as result of the test
  if(result && !m_returnedRowsVariable.IsEmpty())
  {
    CString status;
    status.Format(_T("%d"),p_rows);
    p_parameters->OverwriteReturnParameter(m_returnedRowsVariable,status);
  }
  return result;
}

bool
ValidateSQL::ValidateReturnedCols(Parameters* p_parameters,int p_cols)
{
  // If we need not check it, all-OK
  if(!m_checkCols)
  {
    return true;
  }

  // Convert expected result to an int
  int expected = _ttoi(m_effectiveReturnedCols);

  // Check result, depending on the operator
  bool result = false;
  switch(m_colsOperator)
  {
    case ReturnOperator::ROP_NOP:         result = false;                break;
    case ReturnOperator::ROP_EQUAL:       result = p_cols == expected; break;
    case ReturnOperator::ROP_GREATER:     result = p_cols >  expected; break;
    case ReturnOperator::ROP_GREATEREQUAL:result = p_cols >= expected; break;
    case ReturnOperator::ROP_SMALLER:     result = p_cols <  expected; break;
    case ReturnOperator::ROP_SMALLEREQUAL:result = p_cols <= expected; break;
    case ReturnOperator::ROP_NOTEQUAL:    result = p_cols != expected; break;
    case ReturnOperator::ROP_BETWEEN:     result = Between(m_effectiveReturnedCols,p_cols); break;
    case ReturnOperator::ROP_IN:          result = ValueIn(m_effectiveReturnedCols,p_cols); break;
  }
  // Save as result of the test
  if(result && !m_returnedColsVariable.IsEmpty())
  {
    CString status;
    status.Format(_T("%d"),p_cols);
    p_parameters->OverwriteReturnParameter(m_returnedColsVariable,status);
  }
  return result;
}

bool
ValidateSQL::ValidateSQLState(Parameters* p_parameters,CString p_value)
{
  // See if we must check the output value
  if(!m_checkSQLState)
  {
    return true;
  }

  bool result = false;
  switch(m_stateOperator)
  {
    case BufferOperator::BOP_NOP:       result = false;                                      break;
    case BufferOperator::BOP_ISEMPTY:   result = p_value.IsEmpty();                          break;
    case BufferOperator::BOP_NOTEMPTY:  result = p_value.IsEmpty() == false;                 break;
    case BufferOperator::BOP_EXACT:     result = p_value.Compare(m_effectiveSQLState) == 0;  break;
    case BufferOperator::BOP_CONTAINS:  result = p_value.Find(m_effectiveSQLState)    >= 0;  break;
    case BufferOperator::BOP_BEGINSWITH:result = p_value.Find(m_effectiveSQLState)    == 0;  break;
    case BufferOperator::BOP_NOTFOUND:  result = p_value.Find(m_effectiveSQLState)     < 0;  break;
    case BufferOperator::BOP_ENDSWITH:  result = p_value.Right(m_effectiveSQLState.GetLength()).Compare(m_effectiveSQLState) == 0; break;
    case BufferOperator::BOP_FILEMATCH: result = FileMatch(p_value,m_effectiveSQLState);     break;
    case BufferOperator::BOP_EXISTFILE: result = FileExist(p_value);                         break;
  }
  // Save as result of the test
  if(result && !m_effectiveSQLState.IsEmpty())
  {
    p_parameters->OverwriteReturnParameter(m_returnedSQLStateVariable,p_value);
  }
  return result;
}

bool
ValidateSQL::ValidateNativeStatus(Parameters* p_parameters,CString p_value)
{
  // See if we must check the output value
  if(!m_checkNativeStatus)
  {
    return true;
  }

  bool result = false;
  switch(m_nativeOperator)
  {
    case BufferOperator::BOP_NOP:       result = false;                                         break;
    case BufferOperator::BOP_ISEMPTY:   result = p_value.IsEmpty();                             break;
    case BufferOperator::BOP_NOTEMPTY:  result = p_value.IsEmpty() == false;                    break;
    case BufferOperator::BOP_EXACT:     result = p_value.Compare(m_effectiveNativeStatus) == 0; break;
    case BufferOperator::BOP_CONTAINS:  result = p_value.Find(m_effectiveNativeStatus)    >= 0; break;
    case BufferOperator::BOP_BEGINSWITH:result = p_value.Find(m_effectiveNativeStatus)    == 0; break;
    case BufferOperator::BOP_NOTFOUND:  result = p_value.Find(m_effectiveNativeStatus)     < 0; break;
    case BufferOperator::BOP_ENDSWITH:  result = p_value.Right(m_effectiveNativeStatus.GetLength()).Compare(m_effectiveNativeStatus) == 0; break;
    case BufferOperator::BOP_FILEMATCH: result = FileMatch(p_value, m_effectiveNativeStatus);   break;
    case BufferOperator::BOP_EXISTFILE: result = FileExist(p_value);                            break;
  }
  // Save as result of the test
  if(result && !m_effectiveNativeStatus.IsEmpty())
  {
    p_parameters->OverwriteReturnParameter(m_returnedNativeVariable,p_value);
  }
  return result;
}

bool
ValidateSQL::ValidateFirstData(Parameters* p_parameters,CString p_value)
{
  // See if we must check the output value
  if(!m_checkData)
  {
    return true;
  }

  if(m_data.size() != 1)
  {
    return true;
  }

  bool result = false;
  switch(m_dataOperator)
  {
    case BufferOperator::BOP_NOP:       result = false;                                 break;
    case BufferOperator::BOP_ISEMPTY:   result = p_value.IsEmpty();                     break;
    case BufferOperator::BOP_NOTEMPTY:  result = p_value.IsEmpty() == false;            break;
    case BufferOperator::BOP_EXACT:     result = p_value.Compare(m_effectiveData) == 0; break;
    case BufferOperator::BOP_CONTAINS:  result = p_value.Find(m_effectiveData)    >= 0; break;
    case BufferOperator::BOP_BEGINSWITH:result = p_value.Find(m_effectiveData)    == 0; break;
    case BufferOperator::BOP_NOTFOUND:  result = p_value.Find(m_effectiveData)     < 0; break;
    case BufferOperator::BOP_ENDSWITH:  result = p_value.Right(m_effectiveData.GetLength()).Compare(m_effectiveData) == 0; break;
    case BufferOperator::BOP_FILEMATCH: result = FileMatch(p_value, m_effectiveData);   break;
    case BufferOperator::BOP_EXISTFILE: result = FileExist(p_value);                    break;
  }
  // Save as result of the test
  if(result && !m_effectiveData.IsEmpty())
  {
    p_parameters->OverwriteReturnParameter(m_returnedDataVariable,p_value);
  }
  return result;
}

// If column names exist in the validation data, the value must match EXACTLY !!
bool
ValidateSQL::ValidateColumnData(ColumnData& p_data)
{
  bool result = true;

  if(m_data.size() == 1)
  {
    return true;
  }
  for(auto& dat : m_data)
  {
    ColumnData::iterator it = p_data.find(dat.first);
    if(it == p_data.end())
    {
      return false;
    }
    if(it->second.CompareNoCase(dat.second))
    {
      return false;
    }
  }
  return result;
}

void
ValidateSQL::SetData(CString p_column,CString p_value)
{
  ColumnData::iterator it = m_data.find(p_column);
  if(it != m_data.end())
  {
    it->second = p_value;
  }
  else
  {
    m_data[p_column] = p_value;
  }
}

bool
ValidateSQL::DeleteData(CString p_column)
{
  ColumnData::iterator it = m_data.begin();
  while(it != m_data.end())
  {
    if(it->first.CompareNoCase(p_column) == 0)
    {
      m_data.erase(it);
      return true;
    }
    ++it;
  }
  return false;
}
