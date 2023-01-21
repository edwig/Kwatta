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
// This File   : TimingDlg.h
// What it does: Parameters for running a internet test step
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

class TestStepNET;
class Parameters;

// ParametersDlg dialog

class TimingDlg : public StyleTab
{
	DECLARE_DYNAMIC(TimingDlg)

public:
	TimingDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~TimingDlg();
  void     InitTab(TestStep* p_step,Parameters* p_param);
  void     StoreVariables();
  bool     IsFilled();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TIMING };
#endif

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;
          void Effectuate();

	DECLARE_MESSAGE_MAP()

  bool    m_killOnTimeout { false };
  CString m_waitBeforeRun;
  CString m_waitAfterRun;
  CString m_maxExecution;
  CString m_effectiveBefore;
  CString m_effectiveExec;
  CString m_effectiveAfter;

  TestStep*     m_step       { nullptr };
  Parameters*   m_parameters { nullptr };

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

public:
  afx_msg void OnBnClickedKillontimeout();
  afx_msg void OnEnChangeWaitBeforerun();
  afx_msg void OnEnChangeWaitafterrun();
  afx_msg void OnEnChangeTimeout();
  afx_msg void OnBnClickedBefore();
  afx_msg void OnBnClickedExec();
  afx_msg void OnBnClickedAfter();
};
