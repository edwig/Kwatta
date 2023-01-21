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
// This File   : StatusDlg.h
// What it does: Defining the validation of the status value of a HTTP response message
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "ValidateNET.h"
#include "Parameters.h"

// StatusDlg dialog

class StatusDlg : public StyleTab
{
  DECLARE_DYNAMIC(StatusDlg)

public:
  StatusDlg(CWnd* pParent = nullptr);   // standard constructor
  virtual ~StatusDlg();
  void     InitTab(ValidateNET* p_validate,Parameters* p_parameters);
  void     StoreVariables();

// Dialog Data
#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_STATUS };
#endif

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual void SetupDynamicLayout() override;
  virtual BOOL OnInitDialog() override;
          void LoadVariables();
          void FillCombos();
          void SetCombos();

  DECLARE_MESSAGE_MAP()

  ValidateNET* m_validate;
  Parameters* m_parameters;

  bool           m_useStatusValue;
  ReturnOperator m_statusOperator;
  CString        m_expectedStatus;
  CString        m_effectiveStatus;
  CString        m_statusVariable;

  StyleEdit      m_editExpected;
  StyleEdit      m_editEffective;
  StyleCheckbox  m_buttonCheck;
  StyleComboBox  m_comboOperator;
  StyleButton    m_buttonVariable;
  StyleComboBox  m_comboStatusVar;
  StyleButton    m_buttonStatusVar;

public:
  afx_msg void OnBnClickedUseStatus();
  afx_msg void OnEnChangeStatus();
  afx_msg void OnEnKillfocusStatus();
  afx_msg void OnBnClickedNewvar();
  afx_msg void OnEnChangeEffStatus();
  afx_msg void OnCbnSelchangeOperator();
  afx_msg void OnCbnSelchangeStatusVar();
  afx_msg void OnBnClickedStatusVar();
};
