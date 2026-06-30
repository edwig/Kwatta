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

class TestStepCMD;
class Parameters;

// ParametersDlg dialog

class ParametersDlg : public StyleTab
{
	DECLARE_DYNAMIC(ParametersDlg)

public:
	ParametersDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~ParametersDlg();
  void     InitTab(TestStepCMD* p_step,Parameters* p_param);
  void     StoreVariables();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PARAMETERS };
#endif

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual void SetupDynamicLayout() override;
  virtual BOOL OnInitDialog() override;
          void Effectuate();
          void InitControls();

	DECLARE_MESSAGE_MAP()

  XString m_effectiveRuntimer;
  XString m_effectiveCommandline;
  int     m_startWindow       { 0 };
  bool    m_waitForIdle       { false };
  int     m_handleEnvironment { 0 };
  bool    m_killOnTimeout     { false };
  XString m_waitBeforeRun;
  XString m_waitAfterRun;
  XString m_maxExecution;
  XString m_effectiveBefore;
  XString m_effectiveExec;
  XString m_effectiveAfter;
  XString m_charset;

  TestStepCMD*   m_step       { nullptr };
  Parameters*   m_parameters { nullptr };

  StyleEdit     m_editEffRuntimer;
  StyleEdit     m_editEffCommand;
  StyleComboBox m_comboStartWindow;
  StyleComboBox m_comboEnvironment;
  StyleCheckbox m_buttonWaitForIdle;
  StyleCheckbox m_buttonKillOnTimeout;
  StyleEdit     m_editWaitBeforeRun;
  StyleEdit     m_editWaitAfterRun;
  StyleEdit     m_editMaxExecution;
  StyleButton   m_buttonBefore;
  StyleButton   m_buttonExec;
  StyleButton   m_buttonAfter;
  StyleEdit     m_editEffWaitBefore;
  StyleEdit     m_editEffWaitExec;
  StyleEdit     m_editEffWaitAfter;
  StyleComboBox m_comboCharset;

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
  afx_msg void OnBnClickedBefore();
  afx_msg void OnBnClickedExec();
  afx_msg void OnBnClickedAfter();
  afx_msg void OnCbnSelchangeCharset();
};
