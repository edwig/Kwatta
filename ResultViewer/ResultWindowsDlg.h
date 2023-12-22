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
// This File   : ResultWindowsDlg.h
// What it does: Shows the result of a Windows UI test
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "StepResultWIN.h"
#include "ValidationsDlg.h"
#include "WINOutputDlg.h"

// CResultViewerDlg dialog
class ResultWindowsDlg : public StyleDialog
{
// Construction
public:
	ResultWindowsDlg(CWnd* pParent = nullptr);	// standard constructor
  virtual ~ResultWindowsDlg();

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

  CString         m_name;
  CString         m_documentation;

  StyleEdit       m_editName;
  StyleEdit       m_editDocumentation;
  StyleButton     m_buttonOK;
  StyleTabCtrl    m_tabs;

  StepResultWIN   m_stepResult;
  CBitmapButton   m_picture;
  ValidationsDlg* m_tab1;
  WINOutputDlg*   m_tab2;

	// Generated message map functions
	afx_msg void    OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void    OnPaint();
  afx_msg void    OnAbout();
  afx_msg void    OnExit();
  afx_msg HCURSOR OnQueryDragIcon();

	DECLARE_MESSAGE_MAP()
public:
};
