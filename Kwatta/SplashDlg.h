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
// This program: TestSuite
// This File   : SplashDlg.h
// What it does: Displays the Splash screen for 2 seconds at startup
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "resource.h"

// SplashDlg dialog

class SplashDlg : public StyleDialog
{
	DECLARE_DYNAMIC(SplashDlg)

public:
	SplashDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~SplashDlg();

// Dialog Data
	enum { IDD = IDD_SPLASH };

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;

  StyleButton  m_buttonCancel;
  StyleEdit    m_editTitle;
  CString      m_title;
  ThemeColor::Themes m_theme;

  DECLARE_MESSAGE_MAP()

  afx_msg void OnBnClickedCancel();
  afx_msg void OnTimer(UINT_PTR nIDEvent);
  afx_msg void OnOK();
};
