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
// This program: StepEditor
// This File   : OutputDlg.h
// What it does: Standard output definition for a test step
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

class TestStepCMD;
class Parameters;

// OutputDlg dialog

class OutputDlg : public StyleTab
{
	DECLARE_DYNAMIC(OutputDlg)

public:
	OutputDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~OutputDlg();
  void     InitTab(TestStepCMD* p_step,Parameters* p_param);
  void     StoreVariables();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OUTPUT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  void InitButtons();
  void FillCombos();
  void SetCombos();

	DECLARE_MESSAGE_MAP()

  bool        m_useReturn;
  bool        m_useOutput;
  bool        m_useError;

  CString     m_varReturn;
  CString     m_varOutput;
  CString     m_varError;

  TestStepCMD* m_step;
  Parameters* m_parameters;

  StyleCheckbox   m_buttonUseReturn;
  StyleCheckbox   m_buttonUseOutput;
  StyleCheckbox   m_buttonUseError;

  StyleComboBox   m_comboReturnVars;
  StyleComboBox   m_comboOutputVars;
  StyleComboBox   m_comboErrorVars;

  StyleButton     m_buttonNewReturnVar;
  StyleButton     m_buttonNewOutputVar;
  StyleButton     m_buttonNewErrorVar;
public:
  afx_msg void OnBnClickedUseReturn();
  afx_msg void OnCbnSelchangeVarReturn();
  afx_msg void OnBnClickedNewReturn();
  afx_msg void OnBnClickedUseOutput();
  afx_msg void OnCbnSelchangeVarOutput();
  afx_msg void OnBnClickedNewOutput();
  afx_msg void OnBnClickedUseError();
  afx_msg void OnCbnSelchangeVarError();
  afx_msg void OnBnClickedNewError();
};
