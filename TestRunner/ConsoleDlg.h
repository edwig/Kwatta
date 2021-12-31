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
// This program: TestRunner
// This File   : ConsoleDlg.h
// What it does: Showing the running executable in a fake console window
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "resource.h"

#define WM_CONSOLE_TITLE (WM_USER + 1)
#define WM_CONSOLE_TEXT  (WM_USER + 2)

// ConsoleDlg dialog

class ConsoleDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ConsoleDlg)

public:
	ConsoleDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~ConsoleDlg();
  virtual BOOL OnInitDialog() override;

  void AddLine(const char* p_line);
  void Move();

// Dialog Data
	enum { IDD = IDD_CONSOLE };

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  void PumpMessage();

  StyleEdit m_editText;
  CString   m_text;

	DECLARE_MESSAGE_MAP()

  afx_msg LRESULT OnConsoleText (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnConsoleTitle(WPARAM wParam,LPARAM lParam);
  afx_msg void    OnShowWindow(BOOL p_show,UINT p_how);
};
