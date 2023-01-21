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
// This File   : ValidateDatabaseDlg.h
// What it does: Main dialog for the validation of a SQL Database call
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "ValidateSQL.h"
#include "Parameters.h"

class SQLSucceededDlg;
class SQLRowsDlg;
class SQLColsDlg;
class SQLSQLStateDlg;
class SQLNativeDlg;
class SQLDataDlg;

class ValidateDatabaseDlg : public StyleDialog
{
// Construction
public:
	ValidateDatabaseDlg(CWnd* pParent = nullptr);
  virtual ~ValidateDatabaseDlg();
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

  ValidateSQL       m_validate;
  Parameters        m_parameters;
  SQLSucceededDlg*  m_tab1;
  SQLRowsDlg*       m_tab2;
  SQLColsDlg*       m_tab3;
  SQLSQLStateDlg*   m_tab4;
  SQLNativeDlg*     m_tab5;
  SQLDataDlg*       m_tab6;

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
