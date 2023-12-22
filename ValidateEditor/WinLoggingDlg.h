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
// This File   : WINLoggingDlg.h
// What it does: Defining the validation of the Windows UI test logging
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Parameters.h"
#include "ValidateWIN.h"

// WINLoggingDlg dialog

class WINLoggingDlg : public StyleTab
{
	DECLARE_DYNAMIC(WINLoggingDlg)

public:
	WINLoggingDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~WINLoggingDlg();
  void     InitTab(ValidateWIN* p_validate, Parameters* p_param);
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

  bool            m_useOutput      { false };
  BufferOperator  m_outputOperator { BufferOperator::BOP_NOP };
  CString         m_expectedOutput;
  CString         m_effectiveOutput;

  StyleCheckbox   m_buttonUseOutput;
  StyleComboBox   m_comboOperator;
  StyleButton     m_buttonSearchvar;
  StyleEdit       m_editExpected;
  StyleEdit       m_editEffective;

  ValidateWIN*    m_validate   { nullptr };
  Parameters*     m_parameters { nullptr };
public:
  afx_msg void OnBnClickedUseOutput();
  afx_msg void OnCbnSelchangeOperator();
  afx_msg void OnBnClickedSearchvar();
  afx_msg void OnEnChangeOutput();
  afx_msg void OnEnKillfocusOutput();
  afx_msg void OnEnChangeEffective();
};
