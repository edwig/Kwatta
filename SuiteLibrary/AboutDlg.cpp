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
// This program: SuiteLibrary
// This File   : AboutDlg.cpp
// What it does: General About dialog for all programs
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SuiteLibrary.h"
#include "AboutDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

using namespace ThemeColor;

// AboutDlg dialog

IMPLEMENT_DYNAMIC(AboutDlg, StyleDialog)

AboutDlg::AboutDlg(CWnd* pParent /*=nullptr*/)
	       :StyleDialog(IDD_ABOUT, pParent)
{
}

AboutDlg::~AboutDlg()
{
}

void AboutDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_ABOUT,m_editText);
  DDX_Control(pDX,IDOK,     m_buttonOK);

  if(pDX->m_bSaveAndValidate == FALSE)
  {
    m_editText.SetRTFText(m_text);
    CString test = m_editText.GetRTFText();
  }
}

BEGIN_MESSAGE_MAP(AboutDlg, StyleDialog)
END_MESSAGE_MAP()

BOOL
AboutDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();
  SetWindowText(_T("About"));
  ShowCloseButton();

  m_text = _T("{\\rtf1\\ansi\\ansicpg1252\\deff0\\deflang1043{\\fonttbl{\\f0\\fnil\\fcharset0 Verdana;}}\n")
           _T("\\viewkind4\\uc1\\pard\\f0\\fs20\n")
           _T("\\tab{\\b KWATTA}\\par\n")
           _T("\\par\n")
#ifdef _UNICODE
           _T("\\tab UNICODE VERSION !!\\par\n")
           _T("\\par\n")
#endif
           _T("KWALITY TEST API - Program\\par\n")
           _T("\\par\n")
           _T("Named after the 'Ateles Paniscus'\\par\n")
           _T("monkey. Also named the spider-monkey,\\par\n")
           _T("Jungle-devil or Bosduivel.\\par\n")
           _T("\\par\n")
           _T("This program is named {\\b Kwatta} as a\\par\n")
           _T("short for {\\b Kwa}li{\\b T}y {\\b T}est {\\b A}pi program.\\par\n")
           _T("To programmers the protests of a\\par\n")
           _T("professional tester is somewhat like\\par\n")
           _T("the screeching of the spider-monkey.\\par\n")
           _T("After having accomplished a difficult\\par\n")
           _T("task in programming, the programmer\\par\n")
           _T("sits back in humble satisfaction.\\par\n")
           _T("\\par\n")
           _T("But his/her peace gets quickly disturbed\\par\n")
           _T("by the noises of the quality testers\\par\n")
           _T("who gets busy in the aftermath of the\\par\n")
           _T("programming day......\\par\n")
           _T("\\par\n");

  CString version;
  version.Format(_T("{\\b VERSION: %s}}"),_T(KWATTA_VERSION));
  m_text += version;

  // Perform the streaming
  m_editText.SetBorderColor(ThemeColor::GetColor(Colors::AccentColor1));
  m_editText.SetTextMode(TM_RICHTEXT);

  UpdateData(FALSE);
  return TRUE;
}

// AboutDlg message handlers
