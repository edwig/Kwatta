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
// This File   : SQLColsDlg.h
// What it does: Validation of the number of returned columns of an SQL database statement
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "ValidateSQL.h"
#include "Parameters.h"

// SQLColsDlg dialog

class SQLColsDlg : public StyleDialog
{
  DECLARE_DYNAMIC(SQLColsDlg)

public:
  SQLColsDlg(CWnd* p_parent);
  virtual ~SQLColsDlg();
  void     InitTab(ValidateSQL* p_validate,Parameters* p_parameters);
  void     StoreVariables();

// Dialog Data
  enum { IDD = IDD_SQL_COLS};

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

  bool           m_useCheckCols { false };
  ReturnOperator m_colsOperator { ReturnOperator::ROP_NOP };
  CString        m_returnedCols;
  CString        m_effectiveReturnedCols;
  CString        m_returnedColsVariable;

  StyleEdit      m_editCols;
  StyleEdit      m_editEffective;
  StyleCheckbox  m_buttonCheck;
  StyleComboBox  m_comboOperator;
  StyleButton    m_buttonVariable;
  StyleComboBox  m_comboVariable;
  StyleButton    m_buttonParm;

public:
  afx_msg void OnBnClickedUseReturnedCols();
  afx_msg void OnEnChangeReturnedCols();
  afx_msg void OnBnClickedNewvar();
  afx_msg void OnEnChangeEffSucceeded();
  afx_msg void OnCbnSelchangeOperator();
  afx_msg void OnCbnSelchangeReturnedColsVariable();
  afx_msg void OnBnClickedParm();
};
#pragma once
