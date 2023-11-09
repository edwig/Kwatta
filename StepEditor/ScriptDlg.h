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
// This File   : ScriptDlg.h
// What it does: Resulting response only of a HTTP internet test call
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "afxdialogex.h"

class TestStep;
class Parameters;

// ScriptDlg dialog

class ScriptDlg : public StyleTab
{
	DECLARE_DYNAMIC(ScriptDlg)

public:
	ScriptDlg(CWnd* p_parent = nullptr); 
	virtual ~ScriptDlg();

  void  InitTab(TestStep* p_step,Parameters* p_param);
  void  StoreVariables();
  bool  IsFilled();

// Dialog Data
	enum { IDD = IDD_SCRIPT };

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;
  virtual void SetupDynamicLayout() override;
          void InitCombo();
          void EffectiveParameters();

  TestStep*     m_step       { nullptr };
  Parameters*   m_parameters { nullptr };
  ScriptStatus  m_status     { ScriptStatus::NoScript };
  CString       m_statusOK;
  CString       m_script;

  StyleComboBox m_comboStatus;
  StyleEdit     m_editStatusOK;
  StyleEdit     m_editScript;

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnCbnSelchangeStatus();
  afx_msg void OnEnChangeStatusok();
  afx_msg void OnEnChangeScript();
};
