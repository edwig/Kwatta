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
// This File   : ValidateSQL.h
// What it does: A validation step for a teststep. The validation of a database SQL step
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Validate.h"
#include <map>

// Column name and string value of the returned data cell
using ColumnData = std::map<CString,CString>;

class ValidateSQL : public Validate
{
public:
  ValidateSQL() = default;
  virtual ~ValidateSQL() = default;

  // Interface with the file system
  virtual void    ReadFromXML(CString p_filename) override;  // Throws in case of an error
  virtual bool    WriteToXML(CString p_filename) override;
  // RE-Calculate the effective strings, returning the number of unbound parameters
  virtual int     EffectiveReplacements(Parameters* p_parameters, bool p_forDisplay);
  // Check our filenames extension
  virtual void    CheckFilename(CString p_filename) override;

  // OUR MAIN FUNCTION: Performing our VALIDATIONS
  bool            ValidateSucceeded   (Parameters* p_parameters, int p_status);
  bool            ValidateReturnedRows(Parameters* p_parameters, int p_rows);
  bool            ValidateReturnedCols(Parameters* p_parameters, int p_cols);
  bool            ValidateSQLState    (Parameters* p_parameters, CString p_sqlState);
  bool            ValidateNativeStatus(Parameters* p_parameters, CString p_nativeStatus);
  bool            ValidateFirstData   (Parameters* p_parameters, CString p_data);
  bool            ValidateColumnData  (ColumnData& p_data);

  // GETTERS
  bool            GetCheckSucceeded()           { return m_checkSucceeded;            }
  bool            GetCheckRows()                { return m_checkRows;                 }
  bool            GetCheckCols()                { return m_checkCols;                 }
  bool            GetCheckSQLState()            { return m_checkSQLState;             }
  bool            GetCheckNativeStatus()        { return m_checkNativeStatus;         }
  bool            GetCheckData()                { return m_checkData;                 }
  ReturnOperator  GetSucceededOperator()        { return m_succeedOperator;           }
  ReturnOperator  GetRowsOperator()             { return m_rowsOperator;              }
  ReturnOperator  GetColsOperator()             { return m_colsOperator;              }
  BufferOperator  GetStateOperator()            { return m_stateOperator;             }
  BufferOperator  GetNativeOperator()           { return m_nativeOperator;            }
  BufferOperator  GetDataOperator()             { return m_dataOperator;              }
  CString         GetSucceeded()                { return m_succeeded;                 }
  CString         GetReturnedRows()             { return m_returnedRows;              }
  CString         GetReturnedCols()             { return m_returnedCols;              }
  CString         GetSQLState()                 { return m_sqlState;                  }
  CString         GetNativeStatus()             { return m_nativeStatus;              }
  ColumnData&     GetData()                     { return m_data;                      }
  CString         GetSucceededVariable()        { return m_succeededVariable;         }
  CString         GetReturnedRowsVariable()     { return m_returnedRowsVariable;      }
  CString         GetReturnedColsVariable()     { return m_returnedColsVariable;      }
  CString         GetReturnedSQLStateVariable() { return m_returnedSQLStateVariable;  }
  CString         GetReturnedNativeVariable()   { return m_returnedNativeVariable;    }
  CString         GetReturnedDataVariable()     { return m_returnedDataVariable;      }
  CString         GetEffectiveSucceded()        { return m_effectiveSucceeded;        }
  CString         GetEffectiveReturnedRows()    { return m_effectiveReturnedRows;     }
  CString         GetEffectiveReturnedCols()    { return m_effectiveReturnedCols;     }
  CString         GetEffectiveSQLStateState()   { return m_effectiveSQLState;         }
  CString         GetEffectiveNativeStatus()    { return m_effectiveNativeStatus;     }
  CString         GetEffectiveData()            { return m_effectiveData;             }

  // SETTERS
  void            SetCheckSucceeded(bool p_check)                 { m_checkSucceeded            = p_check;    }
  void            SetCheckRows(bool p_check)                      { m_checkRows                 = p_check;    }
  void            SetCheckCols(bool p_check)                      { m_checkCols                 = p_check;    }
  void            SetCheckSQLState(bool p_check)                  { m_checkSQLState             = p_check;    }
  void            SetCheckNativeStatus(bool p_check)              { m_checkNativeStatus         = p_check;    }
  void            SetCheckData(bool p_check)                      { m_checkData                 = p_check;    }
  void            SetSucceededOperator(ReturnOperator p_oper)     { m_succeedOperator           = p_oper;     }
  void            SetRowsOperator(ReturnOperator p_oper)          { m_rowsOperator              = p_oper;     }
  void            SetColsOperator(ReturnOperator p_oper)          { m_colsOperator              = p_oper;     }
  void            SetStateOperator(BufferOperator p_oper)         { m_stateOperator             = p_oper;     }
  void            SetNativeOperator(BufferOperator p_oper)        { m_nativeOperator            = p_oper;     }
  void            SetDataOperator(BufferOperator p_oper)          { m_dataOperator              = p_oper;     }
  void            SetSucceeded(CString p_succeed)                 { m_succeeded                 = p_succeed;  }
  void            SetReturnedRows(CString p_rows)                 { m_returnedRows              = p_rows;     }
  void            SetReturnedCols(CString p_cols)                 { m_returnedCols              = p_cols;     }
  void            SetSQLState(CString p_state)                    { m_sqlState                  = p_state;    }
  void            SetNativeStatus(CString p_status)               { m_nativeStatus              = p_status;   }
  void            SetData(CString p_column,CString p_value);
  void            SetSucceededVariable(CString p_variable)        { m_succeededVariable         = p_variable; }
  void            SetReturnedRowsVariable(CString p_variable)     { m_returnedRowsVariable      = p_variable; }
  void            SetReturnedColsVariable(CString p_variable)     { m_returnedColsVariable      = p_variable; }
  void            SetReturnedSQLStateVariable(CString p_variable) { m_returnedSQLStateVariable  = p_variable; }
  void            SetReturnedNativeVariable(CString p_variable)   { m_returnedNativeVariable    = p_variable; }
  void            SetReturnedDataVariable(CString p_variable)     { m_returnedDataVariable      = p_variable; }

  // Functions
  bool            DeleteData(CString p_column);

protected:
  // What to check
  bool        m_checkSucceeded    { false };
  bool        m_checkRows         { false };
  bool        m_checkCols         { false };
  bool        m_checkSQLState     { false };
  bool        m_checkNativeStatus { false };
  bool        m_checkData         { false };

  // How to check
  ReturnOperator m_succeedOperator  { ReturnOperator::ROP_EQUAL };
  ReturnOperator m_rowsOperator     { ReturnOperator::ROP_EQUAL };
  ReturnOperator m_colsOperator     { ReturnOperator::ROP_EQUAL };
  BufferOperator m_stateOperator    { BufferOperator::BOP_NOP   };
  BufferOperator m_nativeOperator   { BufferOperator::BOP_NOP   };
  BufferOperator m_dataOperator     { BufferOperator::BOP_NOP   };

  // What we expect
  CString     m_succeeded;
  CString     m_returnedRows;
  CString     m_returnedCols;
  CString     m_sqlState;
  CString     m_nativeStatus;
  ColumnData  m_data;

  // Return variables after the test
  CString     m_succeededVariable;
  CString     m_returnedRowsVariable;
  CString     m_returnedColsVariable;
  CString     m_returnedSQLStateVariable;
  CString     m_returnedNativeVariable;
  CString     m_returnedDataVariable;     // One data field only!!

  // After parameter replacement
  // Cached values: never stored in an XML file
  CString     m_effectiveSucceeded;
  CString     m_effectiveReturnedRows;
  CString     m_effectiveReturnedCols;
  CString     m_effectiveSQLState;
  CString     m_effectiveNativeStatus;
  CString     m_effectiveData;
};
