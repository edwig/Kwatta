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
// This program: ValidateEditor
// This File   : SQLRowsDlg.h
// What it does: Validation of the number of returned rows of an SQL database statement
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "ValidateSQL.h"
#include "Parameters.h"

// SQLRowsDlg dialog

class SQLRowsDlg : public StyleDialog
{
  DECLARE_DYNAMIC(SQLRowsDlg)

public:
  SQLRowsDlg(CWnd* p_parent);
  virtual ~SQLRowsDlg();
  void     InitTab(ValidateSQL* p_validate,Parameters* p_parameters);
  void     StoreVariables();

// Dialog Data
  enum { IDD = IDD_SQL_ROWS};

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual void SetupDynamicLayout() override;
  virtual BOOL OnInitDialog() override;
          void LoadVariables();
          void FillCombos();
          void SetCombos();

  DECLARE_MESSAGE_MAP()

  ValidateSQL*   m_validate   { nullptr };
  Parameters*    m_parameters { nullptr };

  bool           m_useCheckRows { false };
  ReturnOperator m_rowsOperator { ReturnOperator::ROP_NOP };
  CString        m_returnedRows;
  CString        m_effectiveReturnedRows;
  CString        m_returnedRowsVariable;

  StyleEdit      m_editRows;
  StyleEdit      m_editEffective;
  StyleCheckbox  m_buttonCheck;
  StyleComboBox  m_comboOperator;
  StyleButton    m_buttonVariable;
  StyleComboBox  m_comboVariable;
  StyleButton    m_buttonParm;

public:
  afx_msg void OnBnClickedUseReturnedRows();
  afx_msg void OnEnChangeReturnedRows();
  afx_msg void OnBnClickedNewvar();
  afx_msg void OnEnChangeEffSucceeded();
  afx_msg void OnCbnSelchangeOperator();
  afx_msg void OnCbnSelchangeReturnedRowsVariable();
  afx_msg void OnBnClickedParm();
};
#pragma once
