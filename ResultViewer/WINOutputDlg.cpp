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
// This File   : WINOutputDlg.cpp
// What it does: Shows the standard output from a command line program
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ResultViewer.h"
#include "WINOutputDlg.h"
#include "StepResult.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// WINOutputDlg dialog

IMPLEMENT_DYNAMIC(WINOutputDlg,StyleTab)

WINOutputDlg::WINOutputDlg(CWnd* pParent /*=nullptr*/)
             :StyleTab(IDD_WINOUTPUT, pParent)
{

}

WINOutputDlg::~WINOutputDlg()
{
}

void WINOutputDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleTab::DoDataExchange(pDX);

  DDX_Control(pDX, IDC_RETURN, m_editReturn, m_returnValue);
  DDX_Control(pDX, IDC_OUTPUT, m_editLogging,m_winLogging);
  DDX_Control(pDX, IDC_ERROR,  m_editErrors, m_winErrors);
}

BEGIN_MESSAGE_MAP(WINOutputDlg, StyleTab)
END_MESSAGE_MAP()

BOOL
WINOutputDlg::OnInitDialog()
{
  StyleTab::OnInitDialog();

  SetCanResize();
  UpdateData(FALSE);
  return TRUE;
}

void
WINOutputDlg::LoadVariables(StepResultWIN* p_stepResult)
{
  // Remember our object
  m_stepResult = p_stepResult;

  // Load our variables
  m_returnValue = m_stepResult->GetLastOSError();
  m_winLogging  = m_stepResult->GetResultString();
  m_winErrors   = m_stepResult->GetErrorString();

  m_winLogging.Replace("\n","\r\n");
  m_winErrors .Replace("\n","\r\n");

  UpdateData(FALSE);
}

void
WINOutputDlg::SetupDynamicLayout()
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

// ENTER/ESCAPE keys are caught here
BOOL
WINOutputDlg::PreTranslateMessage(MSG* pMsg)
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

// WINOutputDlg message handlers
