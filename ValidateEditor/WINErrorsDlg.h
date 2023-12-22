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
// This File   : WINErrorsDlg.h
// What it does: Defining the validation of the error list of a Windows UI test
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Parameters.h"
#include "ValidateWIN.h"

// OutputDlg dialog

class WINErrorsDlg : public StyleTab
{
  DECLARE_DYNAMIC(WINErrorsDlg)

public:
  WINErrorsDlg(CWnd* pParent = nullptr);   // standard constructor
  virtual ~WINErrorsDlg();
  void     InitTab(ValidateWIN* p_validate,Parameters* p_param);
  void     StoreVariables();

  // Dialog Data
#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_OUTPUT };
#endif

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual void SetupDynamicLayout() override;
  virtual BOOL OnInitDialog() override;
  void LoadVariables();

  DECLARE_MESSAGE_MAP()

  bool            m_useError      { false };
  BufferOperator  m_errorOperator { BufferOperator::BOP_NOP };
  CString         m_expectedError;
  CString         m_effectiveError;

  StyleCheckbox   m_buttonUseError;
  StyleComboBox   m_comboOperator;
  StyleButton     m_buttonSearchvar;
  StyleEdit       m_editExpected;
  StyleEdit       m_editEffective;

  ValidateWIN*    m_validate   { nullptr };
  Parameters*     m_parameters { nullptr };
public:
  afx_msg void OnBnClickedUseError();
  afx_msg void OnCbnSelchangeOperator();
  afx_msg void OnBnClickedSearchvar();
  afx_msg void OnEnChangeError();
  afx_msg void OnEnKillfocusError();
  afx_msg void OnEnChangeEffective();
};
