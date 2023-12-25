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
// This File   : StepDatabaseDlg.h
// What it does: Main editor for one database SQL test call
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <Parameters.h>

class TestStepSQL;
class StepResultSQL;
class SQLDlg;
class DatabaseDlg;
class TimingDlg;
class RawSQLDlg;
class SQLResultDlg;
class SQLOutputDlg;
class ScriptDlg;

// StepInternetDlg dialog

class StepDatabaseDlg : public StyleDialog
{
  DECLARE_DYNAMIC(StepDatabaseDlg)

public:
  StepDatabaseDlg(CWnd* pParent = nullptr);   // standard constructor
  virtual ~StepDatabaseDlg();
  void EffectiveParameters();
  void ResetStepResult();
  void SetStepResult(StepResultSQL* p_result);
  void RefreshEffective();

  // Dialog Data
#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_STEPEDITOR_INET };
#endif

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;
  virtual void SetupDynamicLayout() override;

  void InitTabs();
  void InitStep();
  void InitGlobalParameters();
  void InitParameters();
  void ReadParameters(CString p_file, bool p_global = true);
  bool SaveStep();
  void StoreVariables();
  void LoadVariables();
  void LoadVariablesTabs();
  void Redraw();

  CString           m_name;
  CString           m_comment;
  CString           m_bound;

  StyleEdit         m_editName;
  StyleButton       m_buttonGlobal;
  StyleEdit         m_editComment;
  StyleEdit         m_editBound;
  StyleButton       m_buttonGO;
  StyleTabCtrl      m_tabsRequest;
  StyleTabCtrl      m_tabsResponse;
  StyleButton       m_buttonOK;
  StyleButton       m_buttonCancel;
  HICON             m_hIcon;

  TestStepSQL*      m_testStep { nullptr };
  Parameters        m_parameters;
  SQLDlg*           m_page1;
  DatabaseDlg*      m_page2;
  TimingDlg*        m_page3;
  RawSQLDlg*        m_page4;
  ScriptDlg*        m_page5;
  SQLResultDlg*     m_page10;
  SQLOutputDlg*     m_page11;

  bool              m_busy    { false };
  int               m_unbound { 0     };

  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnEnChangeStepname();
  afx_msg void OnBnClickedGlobal();
  afx_msg void OnEnChangeComment();
  afx_msg void OnBnClickedGO();
  afx_msg void OnBnClickedOk();
  afx_msg void OnAbout();
  afx_msg void OnExit();
};
