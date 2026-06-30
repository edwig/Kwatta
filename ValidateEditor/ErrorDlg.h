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
// This File   : ErrorDlg.h
// What it does: Defining the validation of the standard-error channel of a command-line 
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Parameters.h"
#include "ValidateCMD.h"

// OutputDlg dialog

class ErrorDlg : public StyleTab
{
  DECLARE_DYNAMIC(ErrorDlg)

public:
  ErrorDlg(CWnd* pParent = nullptr);   // standard constructor
  virtual ~ErrorDlg();
  void     InitTab(ValidateCMD* p_validate, Parameters* p_param);
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

  bool            m_useError;
  BufferOperator  m_errorOperator;
  XString         m_expectedError;
  XString         m_effectiveError;

  StyleCheckbox   m_buttonUseError;
  StyleComboBox   m_comboOperator;
  StyleButton     m_buttonSearchvar;
  StyleEdit       m_editExpected;
  StyleEdit       m_editEffective;

  ValidateCMD*     m_validate{ nullptr };
  Parameters*     m_parameters{ nullptr };
public:
  afx_msg void OnBnClickedUseError();
  afx_msg void OnCbnSelchangeOperator();
  afx_msg void OnBnClickedSearchvar();
  afx_msg void OnEnChangeError();
  afx_msg void OnEnKillfocusError();
  afx_msg void OnEnChangeEffective();
};
