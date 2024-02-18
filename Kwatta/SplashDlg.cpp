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
// This File   : SplashDlg.cpp
// What it does: Displays the Splash screen for 2 seconds at startup
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SplashDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// SplashDlg dialog

IMPLEMENT_DYNAMIC(SplashDlg, StyleDialog)

SplashDlg::SplashDlg(CWnd* pParent /*=nullptr*/)
	        :StyleDialog(IDD_SPLASH, pParent)
{
  LoadStyleTheme();
  m_theme = ThemeColor::_theme;
  SetTheme(ThemeColor::Themes::ThemeLime);
}

SplashDlg::~SplashDlg()
{
}

void SplashDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_TITLE,m_editTitle,m_title);
}


BEGIN_MESSAGE_MAP(SplashDlg, StyleDialog)
  ON_WM_TIMER()
  // ON_BN_CLICKED(IDCANCEL,OnBnClickedCancel)
END_MESSAGE_MAP()

BOOL
SplashDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();
  ShowCloseButton(false);

  m_title = _T("KWATTA");

  m_editTitle.SetBorderSize(2);
  m_editTitle.SetBorderColor(RGB(0,255,0));
  m_editTitle.SetBkColor(RGB(200,255,100));
  m_editTitle.SetMutable(false);
  m_editTitle.SetFontName(_T("Verdana"),420);
  m_editTitle.SetFontStyle(true);

  UpdateData(FALSE);

  m_editTitle.PostMessage(WM_KEYDOWN,VK_LEFT,0);
  m_editTitle.PostMessage(WM_KEYUP,  VK_LEFT,0);
  m_editTitle.HideCaret();

  SetTimer(1,2000,nullptr);

  return TRUE;
}

// SplashDlg message handlers

void
SplashDlg::OnTimer(UINT_PTR nIDEvent)
{
  if(nIDEvent == 1)
  {
    OnOK();
  }
}

void
SplashDlg::OnBnClickedCancel()
{
  OnOK();
}

void
SplashDlg::OnOK()
{
  SetTheme(m_theme);
  StyleDialog::OnOK();
}