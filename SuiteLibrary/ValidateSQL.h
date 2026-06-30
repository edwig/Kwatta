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
using ColumnData = std::map<XString,XString>;

class ValidateSQL : public Validate
{
public:
  ValidateSQL() = default;
  virtual ~ValidateSQL() = default;

  // Interface with the file system
  virtual void    ReadFromXML(XString p_filename) override;  // Throws in case of an error
  virtual bool    WriteToXML(XString p_filename) override;
  // RE-Calculate the effective strings, returning the number of unbound parameters
  virtual int     EffectiveReplacements(Parameters* p_parameters, bool p_forDisplay);
  // Check our filenames extension
  virtual void    CheckFilename(XString p_filename) override;

  // OUR MAIN FUNCTION: Performing our VALIDATIONS
  bool            ValidateSucceeded   (Parameters* p_parameters, int p_status);
  bool            ValidateReturnedRows(Parameters* p_parameters, int p_rows);
  bool            ValidateReturnedCols(Parameters* p_parameters, int p_cols);
  bool            ValidateSQLState    (Parameters* p_parameters, XString p_sqlState);
  bool            ValidateNativeStatus(Parameters* p_parameters, XString p_nativeStatus);
  bool            ValidateFirstData   (Parameters* p_parameters, XString p_data);
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
  XString         GetSucceeded()                { return m_succeeded;                 }
  XString         GetReturnedRows()             { return m_returnedRows;              }
  XString         GetReturnedCols()             { return m_returnedCols;              }
  XString         GetSQLState()                 { return m_sqlState;                  }
  XString         GetNativeStatus()             { return m_nativeStatus;              }
  ColumnData&     GetData()                     { return m_data;                      }
  XString         GetSucceededVariable()        { return m_succeededVariable;         }
  XString         GetReturnedRowsVariable()     { return m_returnedRowsVariable;      }
  XString         GetReturnedColsVariable()     { return m_returnedColsVariable;      }
  XString         GetReturnedSQLStateVariable() { return m_returnedSQLStateVariable;  }
  XString         GetReturnedNativeVariable()   { return m_returnedNativeVariable;    }
  XString         GetReturnedDataVariable()     { return m_returnedDataVariable;      }
  XString         GetEffectiveSucceded()        { return m_effectiveSucceeded;        }
  XString         GetEffectiveReturnedRows()    { return m_effectiveReturnedRows;     }
  XString         GetEffectiveReturnedCols()    { return m_effectiveReturnedCols;     }
  XString         GetEffectiveSQLStateState()   { return m_effectiveSQLState;         }
  XString         GetEffectiveNativeStatus()    { return m_effectiveNativeStatus;     }
  XString         GetEffectiveData()            { return m_effectiveData;             }

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
  void            SetSucceeded(XString p_succeed)                 { m_succeeded                 = p_succeed;  }
  void            SetReturnedRows(XString p_rows)                 { m_returnedRows              = p_rows;     }
  void            SetReturnedCols(XString p_cols)                 { m_returnedCols              = p_cols;     }
  void            SetSQLState(XString p_state)                    { m_sqlState                  = p_state;    }
  void            SetNativeStatus(XString p_status)               { m_nativeStatus              = p_status;   }
  void            SetData(XString p_column,XString p_value);
  void            SetSucceededVariable(XString p_variable)        { m_succeededVariable         = p_variable; }
  void            SetReturnedRowsVariable(XString p_variable)     { m_returnedRowsVariable      = p_variable; }
  void            SetReturnedColsVariable(XString p_variable)     { m_returnedColsVariable      = p_variable; }
  void            SetReturnedSQLStateVariable(XString p_variable) { m_returnedSQLStateVariable  = p_variable; }
  void            SetReturnedNativeVariable(XString p_variable)   { m_returnedNativeVariable    = p_variable; }
  void            SetReturnedDataVariable(XString p_variable)     { m_returnedDataVariable      = p_variable; }

  // Functions
  bool            DeleteData(XString p_column);

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
  XString     m_succeeded;
  XString     m_returnedRows;
  XString     m_returnedCols;
  XString     m_sqlState;
  XString     m_nativeStatus;
  ColumnData  m_data;

  // Return variables after the test
  XString     m_succeededVariable;
  XString     m_returnedRowsVariable;
  XString     m_returnedColsVariable;
  XString     m_returnedSQLStateVariable;
  XString     m_returnedNativeVariable;
  XString     m_returnedDataVariable;     // One data field only!!

  // After parameter replacement
  // Cached values: never stored in an XML file
  XString     m_effectiveSucceeded;
  XString     m_effectiveReturnedRows;
  XString     m_effectiveReturnedCols;
  XString     m_effectiveSQLState;
  XString     m_effectiveNativeStatus;
  XString     m_effectiveData;
};
