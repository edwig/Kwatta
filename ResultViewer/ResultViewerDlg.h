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
// This program: ResultViewer
// This File   : ResultViewerDlg.h
// What it does: Shows the result of a command line test step.
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "StepResultCMD.h"
#include "ValidationsDlg.h"
#include "OutputDlg.h"

// CResultViewerDlg dialog
class ResultViewerDlg : public StyleDialog
{
// Construction
public:
	ResultViewerDlg(CWnd* pParent = nullptr);	// standard constructor
  virtual ~ResultViewerDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RESULTVIEWER_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual void SetupDynamicLayout() override;
  virtual BOOL OnInitDialog() override;

// Implementation
protected:
  void            InitResult();
  void            InitTabs();
  void            LoadVariables();
  void            LoadVariablesTabs();

  XString         m_name;
  XString         m_documentation;

  StyleEdit       m_editName;
  StyleEdit       m_editDocumentation;
  StyleButton     m_buttonOK;
  StyleTabCtrl    m_tabs;

  StepResultCMD   m_stepResult;
  CBitmapButton   m_picture;
  ValidationsDlg* m_tab1;
  OutputDlg*      m_tab2;

	// Generated message map functions
	afx_msg void    OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void    OnPaint();
  afx_msg void    OnAbout();
  afx_msg void    OnExit();
  afx_msg HCURSOR OnQueryDragIcon();

	DECLARE_MESSAGE_MAP()
public:
};
