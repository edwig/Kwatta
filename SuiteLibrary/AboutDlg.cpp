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
    m_editText.FormatRTFText(m_text);
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

  m_text = _T("\t{\\b KWATTA}\n\n")
#ifdef _UNICODE
           _T("\tUNICODE VERSION !!\n")
#else
           _T("\tMBCS VERSION !!\n")
#endif
           _T("\n")
           _T("KWALITY TEST API - Program\n")
           _T("\n")
           _T("Named after the 'Ateles Paniscus'\n")
           _T("monkey. Also named the spider-monkey,\n")
           _T("Jungle-devil or Bosduivel.\n")
           _T("\n")
           _T("This program is named {\\b Kwatta} as a\n")
           _T("short for {\\b Kwa}li{\\b T}y {\\b T}est {\\b A}pi program.\n")
           _T("To programmers the protests of a\n")
           _T("professional tester is somewhat like\n")
           _T("the screeching of the spider-monkey.\n")
           _T("After having accomplished a difficult\n")
           _T("task in programming, the programmer\n")
           _T("sits back in humble satisfaction.\n")
           _T("\n")
           _T("But his/her peace gets quickly disturbed\n")
           _T("by the noises of the quality testers\n")
           _T("who gets busy in the aftermath of the\n")
           _T("programming day......\n")
           _T("\n");

  CString version;
  version.Format(_T("{\\b VERSION: %s}}"),_T(KWATTA_VERSION));
  m_text += version;

  UpdateData(FALSE);
  return TRUE;
}

// AboutDlg message handlers
