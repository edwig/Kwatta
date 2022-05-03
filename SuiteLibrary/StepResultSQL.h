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
// This File   : StepResultSQLh
// What it does: Result of one (1) database step in a test set
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "StepResult.h"
#include "TestStepSQL.h"
#include <map>

// Mapping the column name to the column value
using ResultMap = std::map<CString,CString>;

class StepResultSQL : public StepResult
{
public:
  StepResultSQL() = default;
  virtual ~StepResultSQL() = default;

  // Interface with the file system
  virtual void ReadFromXML(CString p_filename);  // Throws in case of an error
  virtual bool WriteToXML(CString p_filename);
  virtual void CheckFilename(CString p_filename);
  virtual void ResetEffective();
  // SETTERS
  void    SetSucceeded(bool p_success)      { m_succeeded  = p_success; }
  void    SetResultRows(int p_rows)         { m_resultRows = p_rows;    }
  void    SetResultCols(int p_cols)         { m_resultCols = p_cols;    }
  void    SetSQLState(CString p_state)      { m_sqlState   = p_state;   }
  void    SetNativeStatus(CString p_native) { m_nativeStatus = p_native;}
  void    AddResult(CString p_name,CString p_value);
  // GETTERS
  CString GetFirstData();
  int     GetSucceeded()    { return m_succeeded;     }
  int     GetResultRows()   { return m_resultRows;    }
  int     GetResultCols()   { return m_resultCols;    }
  CString GetSQLState()     { return m_sqlState;      }
  CString GetNativeStatus() { return m_nativeStatus;  }
  ResultMap& GetResultMap() { return m_data;          }

protected:
  bool    m_succeeded  { false };
  int     m_resultRows { 0     };
  int     m_resultCols { 0     };
  CString m_sqlState;
  CString m_nativeStatus;

  // Result data set
  ResultMap m_data;
};
