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
// This File   : StepCommandDlg.h
// What it does: Main dialog for editing a command-line test step
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "ParametersDlg.h"
#include "InputDlg.h"
#include "OutputDlg.h"
#include "TestStepCMD.h"
#include "Parameters.h"
#include "EnvironmentDlg.h"
#include "ScriptDlg.h"

// CStepEditorDlg dialog
class StepCommandDlg : public StyleDialog
{
// Construction
public:
	StepCommandDlg(CWnd* pParent = nullptr);	// standard constructor
  virtual ~StepCommandDlg();
  void    EffectiveParameters();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STEPEDITOR_CMD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual void SetupDynamicLayout() override;
  virtual BOOL OnInitDialog() override;

// Implementation
protected:
  XString m_name;
  XString m_documentation;
  XString m_directory;
  XString m_runtimer;
  XString m_commandLine;
  XString m_bound;
  int     m_unbound { 0 };
  bool    m_busy    { false };

  TestStepCMD*     m_testStep { nullptr };
  Parameters      m_parameters;
	
  StyleEdit       m_editName;
  StyleButton     m_buttonGlobal;
  StyleEdit       m_editDocumentation;
  StyleEdit       m_editBound;
  StyleEdit       m_editDirectory;
  StyleEdit       m_editRuntimer;
  StyleEdit       m_editCommandLine;
  StyleButton     m_buttonDirParm;
  StyleButton     m_buttonRunParm;
  StyleButton     m_buttonComParm;
  StyleTabCtrl    m_tab;
  StyleButton     m_buttonOK;
  StyleButton     m_buttonCancel;

  HICON           m_hIcon;
  InputDlg*       m_tab1;
  OutputDlg*      m_tab2;
  ParametersDlg*  m_tab3;
  EnvironmentDlg* m_tab4;
  ScriptDlg*      m_tab5;
  
	// Generated message map functions
  void         InitButtons();
  void         InitTabs();
  void         InitStep();
  void         InitParameters();
  void         InitGlobalParameters();
  void         InitControls();
  void         ReadParameters(XString p_file,bool p_global = true);
  void         LoadVariables();
  void         LoadVariablesTabs();
  void         StoreVariables();
  bool         SaveStep();
  void         ChooseVariable(StyleEdit& p_edit);

  DECLARE_MESSAGE_MAP()

  afx_msg void OnPaint();
  afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg HCURSOR OnQueryDragIcon();
  afx_msg void OnEnChangeStepname();
  afx_msg void OnBnClickedGlobal();
  afx_msg void OnEnChangeComment();
  afx_msg void OnEnChangeDirectory();
  afx_msg void OnEnChangeRuntimer();
  afx_msg void OnEnChangeCommandline();
  afx_msg void OnBnClickedDirParm();
  afx_msg void OnBnClickedRunParm();
  afx_msg void OnBnClickedComParm();
  afx_msg void OnBnClickedOk();
  afx_msg void OnAbout();
  afx_msg void OnExit();
};
