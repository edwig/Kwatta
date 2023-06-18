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
// This File   : ConsoleDlg.cpp
// What it does: Showing the running executable in a fake console window
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "ConsoleDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// ConsoleDlg dialog

IMPLEMENT_DYNAMIC(ConsoleDlg, CDialogEx)

ConsoleDlg::ConsoleDlg(CWnd* pParent /*=nullptr*/)
	         :CDialogEx(IDD_CONSOLE, pParent)
{
}

ConsoleDlg::~ConsoleDlg()
{
}

void ConsoleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_TEXT,m_editText,m_text);
}

BEGIN_MESSAGE_MAP(ConsoleDlg, CDialogEx)
  ON_MESSAGE(WM_CONSOLE_TITLE,OnConsoleTitle)
  ON_MESSAGE(WM_CONSOLE_TEXT, OnConsoleText)
  ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

BOOL
ConsoleDlg::OnInitDialog()
{
  CDialogEx::OnInitDialog();

  m_editText.SetMutable(false);
  m_editText.SetBorderColor(RGB(0,0,0));
  m_editText.SetBkColor(RGB(0,0,0));
  m_editText.SetTextColor(RGB(180,180,180));
  m_editText.SetFontName("Lucida console",120);

  UpdateData(FALSE);
  return TRUE;
}

void
ConsoleDlg::Move()
{
  CRect rect;
  GetWindowRect(&rect);
  MoveWindow(40,40,rect.Width(),rect.Height());
}

LRESULT
ConsoleDlg::OnConsoleTitle(WPARAM wParam,LPARAM lParam)
{
  CString title("Console");
  const char* text = reinterpret_cast<const char*>(lParam);
  if(*text)
  {
    title.AppendFormat(" - %s",text);
    ShowWindow(SW_SHOW);
  }
  else
  {
    ShowWindow(SW_HIDE);
  }
  SetWindowText(title);

  return 0;
}

LRESULT
ConsoleDlg::OnConsoleText(WPARAM wParam,LPARAM lParam)
{
  const char* text = reinterpret_cast<const char*>(lParam);
  AddLine(text);
  return 0;
}

void
ConsoleDlg::AddLine(const char* p_line)
{
  int line    = m_editText.GetLineCount() - 1;
  int endChar = m_editText.LineIndex(line);
  m_editText.SetSel(endChar, endChar);
  m_editText.ReplaceSel(p_line);
  m_editText.LineScroll(line + 1);

  PumpMessage();
}

void    
ConsoleDlg::OnShowWindow(BOOL p_show, UINT p_how)
{
  CWnd::OnShowWindow(p_show,p_how);
  PumpMessage();
}

// ConsoleDlg message handlers

void
ConsoleDlg::PumpMessage()
{
  // We just handle the paint messages, so we get visible
  // for larger and longer processes.
  // We could face an eternal loop, so we put a time constriction on it!
  MSG msg;
  UINT ticks = GetTickCount();
  while(GetTickCount() - ticks < 100 &&
        (PeekMessage(&msg,NULL,WM_PAINT,     WM_PAINT,     PM_REMOVE) ||
         PeekMessage(&msg,NULL,WM_SYSCOMMAND,WM_SYSCOMMAND,PM_REMOVE)))
  {
    try
    {
      ::TranslateMessage(&msg);
      ::DispatchMessage(&msg);
    }
    catch(...)
    {
      // How now, brown cow?
    }
  }
}
