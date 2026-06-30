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
// This File   : ReturnDlg.h
// What it does: Defining the validation of the return value of a command-line executable
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "ValidateCMD.h"
#include "Parameters.h"

// ReturnDlg dialog

class ReturnDlg : public StyleTab
{
	DECLARE_DYNAMIC(ReturnDlg)

public:
	ReturnDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~ReturnDlg();
  void     InitTab(ValidateCMD* p_validate, Parameters* p_parameters);
  void     StoreVariables();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RETURN };
#endif

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual void SetupDynamicLayout() override;
  virtual BOOL OnInitDialog() override;
          void LoadVariables();

	DECLARE_MESSAGE_MAP()

  ValidateCMD*    m_validate;
  Parameters*    m_parameters;

  bool           m_useReturnValue;
  ReturnOperator m_returnOperator;
  XString        m_expectedReturn;
  XString        m_effectiveReturn;

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
