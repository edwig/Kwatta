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
// This program: ValidateEditor
// This File   : ValidateInternetDlg.h
// What it does: Main dialog for the validation of a HTTPMessage call
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "ValidateNET.h"
#include "Parameters.h"
#include "StatusDlg.h"
#include "HeaderDlg.h"
#include "BodyDlg.h"
#include "XMLNodeDlg.h"
#include "JSONPairDlg.h"

class ValidateInternetDlg : public StyleDialog
{
// Construction
public:
	ValidateInternetDlg(CWnd* pParent = nullptr);
  virtual ~ValidateInternetDlg();
  void     EffectiveParameters();

// Dialog Data
	enum { IDD = IDD_VALIDATEEDITOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual void SetupDynamicLayout() override;
  virtual BOOL OnInitDialog() override;

protected:
  DECLARE_MESSAGE_MAP()

  void         InitTabs();
  void         InitButtons();
  void         InitValidation();
  void         InitParameters();
  void         LoadVariables();
  void         LoadVariablesTabs();
  bool         SaveValidate();
  void         StoreVariables();
  void         SetFirstUsedTab();

  CString      m_name;
  CString      m_documentation;
  CString      m_bound;

  StyleEdit    m_editName;
  StyleButton  m_buttonGlobal;
  StyleEdit    m_editDocumentation;
  StyleEdit    m_editBound;
  StyleTabCtrl m_tabs;
  StyleButton  m_buttonOK;
  StyleButton  m_buttonCancel;

  ValidateNET   m_validate;
  Parameters   m_parameters;
	HICON        m_hIcon;
  StatusDlg*   m_tab1;
  HeaderDlg*   m_tab2;
  BodyDlg*     m_tab3;
  XMLNodeDlg*  m_tab4;
  JSONPairDlg* m_tab5;

  bool         m_busy     { false };
  int          m_unbound  { 0     };

public:
  // Generated message map functions
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
  afx_msg void OnEnChangeName();
  afx_msg void OnBnClckedGlobal();
  afx_msg void OnEnChangeDocumentation();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnAbout();
  afx_msg void OnExit();
};
