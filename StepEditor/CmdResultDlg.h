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
// This File   : CmdResultDlg.h
// What it does: Shows the standard output from a command line program
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "StepResultCMD.h"

// CmdResultDlg dialog

class CmdResultDlg : public StyleTab
{
	DECLARE_DYNAMIC(CmdResultDlg)

public:
	CmdResultDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CmdResultDlg();
  void     LoadVariables(StepResultCMD* p_stepResult);
  void     InitTab();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CMDRESULT };
#endif

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL PreTranslateMessage(MSG* pMsg) override;
  virtual void SetupDynamicLayout();
  virtual BOOL OnInitDialog() override;

  afx_msg LRESULT OnConsoleText(WPARAM wParam,LPARAM lParam);
  
  DECLARE_MESSAGE_MAP()

  int       m_returnValue;
  XString   m_standardOutput;
  XString   m_standardError;
  StyleEdit m_editReturn;
  StyleEdit m_editOutput;
  StyleEdit m_editError;

  StepResultCMD* m_stepResult{ nullptr };
};
