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
// This File   : OutputDlg.h
// What it does: Defining the validation of the standard-output channel of a command-line executable
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Parameters.h"
#include "ValidateCMD.h"

// OutputDlg dialog

class OutputDlg : public StyleTab
{
	DECLARE_DYNAMIC(OutputDlg)

public:
	OutputDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~OutputDlg();
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

  bool            m_useOutput;
  BufferOperator  m_outputOperator;
  XString         m_expectedOutput;
  XString         m_effectiveOutput;

  StyleCheckbox   m_buttonUseOutput;
  StyleComboBox   m_comboOperator;
  StyleButton     m_buttonSearchvar;
  StyleEdit       m_editExpected;
  StyleEdit       m_editEffective;

  ValidateCMD*     m_validate   { nullptr };
  Parameters*     m_parameters { nullptr };
public:
  afx_msg void OnBnClickedUseOutput();
  afx_msg void OnCbnSelchangeOperator();
  afx_msg void OnBnClickedSearchvar();
  afx_msg void OnEnChangeOutput();
  afx_msg void OnEnKillfocusOutput();
  afx_msg void OnEnChangeEffective();
};
