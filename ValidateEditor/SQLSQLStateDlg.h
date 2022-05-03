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
// This File   : SQLSQLStateDlg.h
// What it does: Validation of SQLSTATE error status of an SQL database statement
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "ValidateSQL.h"
#include "Parameters.h"

// SQLSQLStateDlg dialog

class SQLSQLStateDlg : public StyleDialog
{
  DECLARE_DYNAMIC(SQLSQLStateDlg)

public:
  SQLSQLStateDlg(CWnd* p_parent);
  virtual ~SQLSQLStateDlg();
  void     InitTab(ValidateSQL* p_validate,Parameters* p_parameters);
  void     StoreVariables();

// Dialog Data
  enum { IDD = IDD_SQL_SQLSTATE};

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

  bool           m_useCheckState    { false };
  BufferOperator m_sqlStateOperator { BufferOperator::BOP_NOP };
  CString        m_SQLState;
  CString        m_effectiveSQLState;
  CString        m_SQLStateVariable;

  StyleEdit      m_editSQLState;
  StyleEdit      m_editEffective;
  StyleCheckbox  m_buttonCheck;
  StyleComboBox  m_comboOperator;
  StyleButton    m_buttonVariable;
  StyleComboBox  m_comboVariable;
  StyleButton    m_buttonParm;

public:
  afx_msg void OnBnClickedUseSQLState();
  afx_msg void OnEnChangeSQLState();
  afx_msg void OnBnClickedNewvar();
  afx_msg void OnEnChangeEffSucceeded();
  afx_msg void OnCbnSelchangeOperator();
  afx_msg void OnCbnSelchangeSQLStateVariable();
  afx_msg void OnBnClickedParm();
};
#pragma once
