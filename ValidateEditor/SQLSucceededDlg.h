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
// This File   : SQLSucceededDlg.h
// What it does: Validation of the succeeded status of an SQL database statement
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "ValidateSQL.h"
#include "Parameters.h"

// SQLSucceededDlg dialog

class SQLSucceededDlg : public StyleTab
{
  DECLARE_DYNAMIC(SQLSucceededDlg)

public:
  SQLSucceededDlg(CWnd* p_parent);
  virtual ~SQLSucceededDlg();
  void     InitTab(ValidateSQL* p_validate,Parameters* p_parameters);
  void     StoreVariables();

// Dialog Data
  enum { IDD = IDD_SQL_RETURN};

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual void SetupDynamicLayout() override;
  virtual BOOL OnInitDialog() override;
          void LoadVariables();
          void FillCombos();
          void SetCombos();

  DECLARE_MESSAGE_MAP()

  ValidateSQL*   m_validate;
  Parameters*    m_parameters;

  bool           m_useSucceeded;
  ReturnOperator m_succeededOperator;
  CString        m_succeeded;
  CString        m_effectiveSucceeded;
  CString        m_succeededVariable;

  StyleEdit      m_editSucceeded;
  StyleEdit      m_editEffective;
  StyleCheckbox  m_buttonCheck;
  StyleComboBox  m_comboOperator;
  StyleButton    m_buttonVariable;
  StyleComboBox  m_comboVariable;
  StyleButton    m_buttonParm;

public:
  afx_msg void OnBnClickedUseSucceeded();
  afx_msg void OnEnChangeSucceeded();
  afx_msg void OnBnClickedNewvar();
  afx_msg void OnEnChangeEffSucceeded();
  afx_msg void OnCbnSelchangeOperator();
  afx_msg void OnCbnSelchangeSucceededVariable();
  afx_msg void OnBnClickedParm();
};
