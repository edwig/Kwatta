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
// This File   : CmdResultDlg.cpp
// What it does: Shows the standard output from a command line program
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StepEditor.h"
#include "CmdResultDlg.h"
#include "StepResult.h"
#include <RunRedirect.h>
#include <afxdialogex.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// CmdResultDlg dialog

IMPLEMENT_DYNAMIC(CmdResultDlg,StyleTab)

CmdResultDlg::CmdResultDlg(CWnd* pParent /*=nullptr*/)
             :StyleTab(IDD_CMDRESULT, pParent)
{
}

CmdResultDlg::~CmdResultDlg()
{
}

void CmdResultDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleTab::DoDataExchange(pDX);

  DDX_Control(pDX, IDC_RETURN, m_editReturn,m_returnValue);
  DDX_Control(pDX, IDC_OUTPUT, m_editOutput,m_standardOutput);
  DDX_Control(pDX, IDC_ERROR,  m_editError, m_standardError);
}

BEGIN_MESSAGE_MAP(CmdResultDlg, StyleTab)
  ON_MESSAGE(WM_CONSOLE_TEXT,OnConsoleText)
END_MESSAGE_MAP()

BOOL
CmdResultDlg::OnInitDialog()
{
  StyleTab::OnInitDialog();

  SetCanResize();
  UpdateData(FALSE);
  return TRUE;
}

void
CmdResultDlg::InitTab()
{
  m_returnValue = 0;
  m_standardOutput.Empty();
  m_standardOutput.Empty();

  UpdateData(FALSE);
}

void
CmdResultDlg::LoadVariables(StepResultCMD* p_stepResult)
{
  // Remember our object
  m_stepResult = p_stepResult;

  // Load our variables
  m_returnValue    = m_stepResult->GetReturnValue();
  m_standardOutput = m_stepResult->GetStandardOutput();
  m_standardError  = m_stepResult->GetStandardError();

  m_standardOutput.Replace(_T("\n"),_T("\r\n"));
  m_standardError .Replace(_T("\n"),_T("\r\n"));

  // Entry here from another thread in the CMDRunner
  // So we have to divert the "UpdateData" with a PostMessage
  PostMessage(WM_CONSOLE_TEXT,0,0);
}

void
CmdResultDlg::SetupDynamicLayout()
{
  StyleTab::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif

  manager.AddItem(IDC_GRP_OUTPUT, CMFCDynamicLayout::MoveNone(),      CMFCDynamicLayout::SizeHorizontalAndVertical(100,100));
  manager.AddItem(IDC_OUTPUT,     CMFCDynamicLayout::MoveNone(),      CMFCDynamicLayout::SizeHorizontalAndVertical(100,50));
  manager.AddItem(IDC_ERROR,      CMFCDynamicLayout::MoveVertical(50),CMFCDynamicLayout::SizeHorizontalAndVertical(100,50));
  manager.AddItem(IDC_ST_ERROR,   CMFCDynamicLayout::MoveVertical(50),CMFCDynamicLayout::SizeNone());
}

LRESULT 
CmdResultDlg::OnConsoleText(WPARAM /*wParam*/,LPARAM /*lParam*/)
{
  UpdateData(FALSE);
  return 0;
}

// ENTER/ESCAPE keys are caught here
BOOL
CmdResultDlg::PreTranslateMessage(MSG* pMsg)
{
  if (pMsg->message == WM_KEYUP ||
      pMsg->message == WM_KEYDOWN)
  {
    if (pMsg->wParam == VK_RETURN ||
        pMsg->wParam == VK_ESCAPE )
    {
      return TRUE;
    }
  }
  // Nothing done yet
  return FALSE;
}

// CmdResultDlg message handlers
