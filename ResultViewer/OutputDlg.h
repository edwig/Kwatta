﻿///////////////////////////////////////////////////////////////////////////////////////////////////////
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
// This File   : OutputDlg.h
// What it does: Shows the standard output from a command line program
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "StepResultCMD.h"

// OutputDlg dialog

class OutputDlg : public StyleTab
{
	DECLARE_DYNAMIC(OutputDlg)

public:
	OutputDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~OutputDlg();
  void     LoadVariables(StepResultCMD* p_stepResult);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OUTPUT };
#endif

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL PreTranslateMessage(MSG* pMsg) override;
  virtual void SetupDynamicLayout();
  virtual BOOL OnInitDialog() override;

	DECLARE_MESSAGE_MAP()

  int       m_returnValue;
  CString   m_standardOutput;
  CString   m_standardError;
  StyleEdit m_editReturn;
  StyleEdit m_editOutput;
  StyleEdit m_editError;

  StepResultCMD* m_stepResult{ nullptr };
};
