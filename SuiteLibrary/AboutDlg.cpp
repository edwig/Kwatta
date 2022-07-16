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
  SetWindowText("About");
  ShowCloseButton();

  m_text = "{\\rtf1\\ansi\\ansicpg1252\\deff0\\deflang1043{\\fonttbl{\\f0\\fnil\\fcharset0 Verdana;}}\n"
           "\\viewkind4\\uc1\\pard\\f0\\fs20\n"
           "\\tab{\\b KWATTA}\\par\n"
           "\\par\n"
           "KWALITY TEST API - Program\\par\n"
           "\\par\n"
           "Named after the 'Ateles Paniscus'\\par\n"
           "monkey. Also named the spider-monkey,\\par\n"
           "Jungle-devil or Bosduivel.\\par\n"
           "\\par\n"
           "This program is named {\\b Kwatta} as a\\par\n"
           "short for {\\b Kwa}li{\\b T}y {\\b T}est {\\b A}pi program.\\par\n"
           "To programmers the protests of a\\par\n"
           "professional tester is somewhat like\\par\n"
           "the screeching of the spider-monkey.\\par\n"
           "After having accomplished a difficult\\par\n"
           "task in programming, the programmer\\par\n"
           "sits back in humble satisfaction.\\par\n"
           "\\par\n"
           "But his/her peace gets quickly disturbed\\par\n"
           "by the noises of the quality testers\\par\n"
           "who gets busy in the aftermath of the\\par\n"
           "programming day......\\par\n"
           "\\par\n";

  CString version;
  version.Format("{\\b VERSION: %s}}",KWATTA_VERSION);
  m_text += version;

  // Perform the streaming
  m_editText.SetBorderColor(ThemeColor::GetColor(Colors::AccentColor1));
  m_editText.SetTextMode(TM_RICHTEXT);

  UpdateData(FALSE);
  return TRUE;
}

// AboutDlg message handlers
