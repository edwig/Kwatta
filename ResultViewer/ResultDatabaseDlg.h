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
// This File   : ResultDatabaseDlg.h
// What it does: Shows the total database test result
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "StepResultSQL.h"
#include "ValidationsDlg.h"
#include "SQLDataDlg.h"
#include "SQLStatusDlg.h"

// CResultDatabaseDlg dialog
class ResultDatabaseDlg : public StyleDialog
{
// Construction
public:
	ResultDatabaseDlg(CWnd* pParent = nullptr);	// standard constructor
  virtual ~ResultDatabaseDlg();

// Dialog Data
	enum { IDD = IDD_RESULTVIEWER_DIALOG };

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

  StepResultSQL   m_stepResult;
	HICON           m_hIcon;
  CBitmapButton   m_picture;
  ValidationsDlg* m_tab1;
  SQLDataDlg*     m_tab2;
  SQLStatusDlg*   m_tab3;

	// Generated message map functions
	afx_msg void    OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void    OnPaint();
  afx_msg void    OnAbout();
  afx_msg void    OnExit();
  afx_msg HCURSOR OnQueryDragIcon();

	DECLARE_MESSAGE_MAP()
public:
};
