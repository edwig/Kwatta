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
// This File   : StepWindowsDlg.h
// What it does: Main editor for one Windows UI test call
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <Parameters.h>

class TestStepWIN;
class StepResultWIN;
class WinActionsDlg;
class TimingDlg;
class ScriptDlg;
class WinResultDlg;

// StepInternetDlg dialog

class StepWindowsDlg : public StyleDialog
{
  DECLARE_DYNAMIC(StepWindowsDlg)

public:
  StepWindowsDlg(CWnd* pParent = nullptr);   // standard constructor
  virtual ~StepWindowsDlg();
  void EffectiveParameters();
  void ResetStepResult();
  void SetStepResult(StepResultWIN* p_result);
  void RefreshEffective();

  // Dialog Data
#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_STEPEDITOR_WIN };
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

  CString           m_name;
  CString           m_comment;
  CString           m_bound;

  StyleEdit         m_editName;
  StyleButton       m_buttonGlobal;
  StyleEdit         m_editComment;
  StyleEdit         m_editBound;
  StyleButton       m_buttonGO;
  StyleTabCtrl      m_tabs;
  StyleButton       m_buttonOK;
  StyleButton       m_buttonCancel;
  HICON             m_hIcon;

  TestStepWIN*      m_testStep { nullptr };
  Parameters        m_parameters;
  WinActionsDlg*    m_page1;
  TimingDlg*        m_page2;
  ScriptDlg*        m_page3;
  WinResultDlg*     m_page4;

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
