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
// This File   : WINOutputDlg.h
// What it does: Shows the output from a Windows UI test
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "StepResultWIN.h"

// WINOutputDlg dialog

class WINOutputDlg : public StyleTab
{
	DECLARE_DYNAMIC(WINOutputDlg)

public:
	WINOutputDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~WINOutputDlg();
  void     LoadVariables(StepResultWIN* p_stepResult);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WINOUTPUT };
#endif

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL PreTranslateMessage(MSG* pMsg) override;
  virtual void SetupDynamicLayout();
  virtual BOOL OnInitDialog() override;

	DECLARE_MESSAGE_MAP()

  int       m_returnValue;
  CString   m_winErrors;
  CString   m_winLogging;
  StyleEdit m_editReturn;
  StyleEdit m_editLogging;
  StyleEdit m_editErrors;

  StepResultWIN* m_stepResult{ nullptr };
};
