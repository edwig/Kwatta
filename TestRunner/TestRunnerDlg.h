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
// This program: TestRunner
// This File   : TestRunnerDlg.h
// What it does: Showing the result of the test on the screen in the lower right corner
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

// CTestRunnerDlg dialog
class TestRunnerDlg : public StyleDialog
{
// Construction
public:
	TestRunnerDlg(CWnd* pParent = nullptr);	// standard constructor

  void SetTest(XString p_test);
  void SetStep(XString p_step);
  void SetTotal   (int p_percent);
  void SetProgress(int p_percent);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TESTRUNNER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
  virtual BOOL OnInitDialog();

// Implementation
protected:
  int   GetTaskBarHeight();
  void  PlaceInRightBottomCorner();
  void  PumpMessages();

  StyleProgressCtrl m_total;
  StyleProgressCtrl m_progress;
  XString           m_test;
  XString           m_step;
  StyleEdit         m_editTest;
  StyleEdit         m_editStep;

  DECLARE_MESSAGE_MAP()

	// Generated message map functions
	afx_msg void OnPaint();
  afx_msg HCURSOR OnQueryDragIcon();
  afx_msg void    OnSysCommand(UINT nID,     LPARAM lParam);
  afx_msg LRESULT OnTestName  (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnStepName  (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnSetTotal  (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnProgress  (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnUpdate    (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnEndTesting(WPARAM wParam,LPARAM lParam);
};
