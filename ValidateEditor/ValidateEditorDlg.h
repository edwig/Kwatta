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
// This File   : ValidateEditorDlg.h
// What it does: Main dialog for the validation of a command-line executable
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "ReturnDlg.h"
#include "OutputDlg.h"
#include "ErrorDlg.h"
#include "ValidateCMD.h"
#include "Parameters.h"

class ValidateEditorDlg : public StyleDialog
{
// Construction
public:
	ValidateEditorDlg(CWnd* pParent = nullptr);
  virtual ~ValidateEditorDlg();
  void     EffectiveParameters();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VALIDATEEDITOR };
#endif

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

  XString      m_name;
  XString      m_documentation;
  XString      m_bound;

  StyleEdit    m_editName;
  StyleButton  m_buttonGlobal;
  StyleEdit    m_editDocumentation;
  StyleEdit    m_editBound;
  StyleTabCtrl m_tabs;
  StyleButton  m_buttonOK;
  StyleButton  m_buttonCancel;

  ValidateCMD   m_validate;
  Parameters   m_parameters;
  ReturnDlg*   m_tab1;
  OutputDlg*   m_tab2;
  ErrorDlg*    m_tab3;

  bool         m_busy { false };
  int          m_unbound;

public:
  // Generated message map functions
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
  afx_msg void OnEnChangeName();
  afx_msg void OnBnClickedGlobal();
  afx_msg void OnEnChangeDocumentation();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnAbout();
  afx_msg void OnExit();
};
