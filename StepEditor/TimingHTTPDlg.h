﻿///////////////////////////////////////////////////////////////////////////////////////////////////////
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
// This File   : TimingHTTPDlg.h
// What it does: Parameters for running a internet test step
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

class TestStepNET;
class Parameters;

// ParametersDlg dialog

class TimingHTTPDlg : public StyleTab
{
	DECLARE_DYNAMIC(TimingHTTPDlg)

public:
	TimingHTTPDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~TimingHTTPDlg();
  void     InitTab(TestStep* p_step,Parameters* p_param);
  void     StoreVariables();
  bool     IsFilled();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TIMING_HTTP };
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
  int     m_timeoutResolve;
  int     m_timeoutConnect;
  int     m_timeoutSend;
  int     m_timeoutReceive;


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
  StyleEdit     m_editTOResolve;
  StyleEdit     m_editTOConnect;
  StyleEdit     m_editTOSend;
  StyleEdit     m_editTOReceive;

public:
  afx_msg void OnBnClickedKillontimeout();
  afx_msg void OnEnChangeWaitBeforerun();
  afx_msg void OnEnChangeWaitafterrun();
  afx_msg void OnEnChangeTimeout();
  afx_msg void OnBnClickedBefore();
  afx_msg void OnBnClickedExec();
  afx_msg void OnBnClickedAfter();
  afx_msg void OnEnChangeTOResolve();
  afx_msg void OnEnChangeTOConnect();
  afx_msg void OnEnChangeTOSend();
  afx_msg void OnEnChangeTOReceive();
};
