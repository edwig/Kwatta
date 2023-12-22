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
// This File   : WINReturnDlg.h
// What it does: Defining the validation of the return value of a Windows UI test
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "ValidateWIN.h"
#include "Parameters.h"

// WINReturnDlg dialog

class WINReturnDlg : public StyleTab
{
	DECLARE_DYNAMIC(WINReturnDlg)

public:
	WINReturnDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~WINReturnDlg();
  void     InitTab(ValidateWIN* p_validate, Parameters* p_parameters);
  void     StoreVariables();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WINRETURN };
#endif

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual void SetupDynamicLayout() override;
  virtual BOOL OnInitDialog() override;
          void LoadVariables();

	DECLARE_MESSAGE_MAP()

  ValidateWIN*   m_validate       { nullptr };
  Parameters*    m_parameters     { nullptr };

  bool           m_useReturnValue { false};
  ReturnOperator m_returnOperator { ReturnOperator::ROP_NOP };
  CString        m_expectedReturn;
  CString        m_effectiveReturn;

  StyleEdit      m_editExpected;
  StyleEdit      m_editEffective;
  StyleCheckbox  m_buttonCheck;
  StyleComboBox  m_comboOperator;
  StyleButton    m_buttonVariable;

public:
  afx_msg void OnBnClickedUseReturn();
  afx_msg void OnEnChangeReturn();
  afx_msg void OnEnKillfocusReturn();
  afx_msg void OnBnClickedNewvar();
  afx_msg void OnEnChangeEffReturn();
  afx_msg void OnCbnSelchangeOperator();
};
