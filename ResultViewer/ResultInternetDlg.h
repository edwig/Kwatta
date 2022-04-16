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
// This File   : ResultInternetDlg.h
// What it does: Shows the total HTTP internet message result
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "StepResultNET.h"
#include "ValidationsDlg.h"
#include "RawResponseDlg.h"
#include "RespHeadersDlg.h"
#include "ResponseDlg.h"

// CResultInternetDlg dialog
class ResultInternetDlg : public StyleDialog
{
// Construction
public:
	ResultInternetDlg(CWnd* pParent = nullptr);	// standard constructor
  virtual ~ResultInternetDlg();

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

  StepResultNET    m_stepResult;
	HICON           m_hIcon;
  CBitmapButton   m_picture;
  ValidationsDlg* m_tab1;
  ResponseDlg*    m_tab2;
  RawResponseDlg* m_tab3;
  RespHeadersDlg* m_tab4;

	// Generated message map functions
	afx_msg void    OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void    OnPaint();
  afx_msg void    OnAbout();
  afx_msg void    OnExit();
  afx_msg HCURSOR OnQueryDragIcon();

	DECLARE_MESSAGE_MAP()
public:
};
