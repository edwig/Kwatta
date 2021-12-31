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
// This File   : ParametersDlg.h
// What it does: Parameters for running a test step
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

class TestStepCL;

// ParametersDlg dialog

class ParametersDlg : public StyleDialog
{
	DECLARE_DYNAMIC(ParametersDlg)

public:
	ParametersDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~ParametersDlg();
  void     InitTab(TestStepCL* p_step);
  void     StoreVariables();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PARAMETERS };
#endif

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual void SetupDynamicLayout() override;
  virtual BOOL OnInitDialog() override;

	DECLARE_MESSAGE_MAP()

  CString m_effectiveRuntimer;
  CString m_effectiveCommandline;
  int     m_startWindow       { 0 };
  bool    m_waitForIdle       { false };
  int     m_handleEnvironment { 0 };
  int     m_waitBeforeRun     { 0 };
  int     m_waitAfterRun      { 0 };
  bool    m_killOnTimeout     { false };
  int     m_maxExecution      { 0 };

  TestStepCL*   m_step { nullptr };

  StyleEdit     m_editEffRuntimer;
  StyleEdit     m_editEffCommand;
  StyleComboBox m_comboStartWindow;
  StyleComboBox m_comboEnvironment;
  StyleCheckbox m_buttonWaitForIdle;
  StyleCheckbox m_buttonKillOnTimeout;
  StyleEdit     m_editWaitBeforeRun;
  StyleEdit     m_editWaitAfterRun;
  StyleEdit     m_editMaxExecution;
public:
  afx_msg void OnEnChangeEffRuntimer();
  afx_msg void OnEnChangeEffComline();
  afx_msg void OnCbnSelchangeStartwindow();
  afx_msg void OnCbnSelchangeHandleenv();
  afx_msg void OnEnChangeWaitBeforerun();
  afx_msg void OnBnClickedWaitforidle();
  afx_msg void OnEnChangeWaitafterrun();
  afx_msg void OnBnClickedKillontimeout();
  afx_msg void OnEnChangeTimeout();
};
