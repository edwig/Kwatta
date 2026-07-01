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
// This File   : OutputDlg.cpp
// What it does: Shows the standard output from a command line program
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ResultViewer.h"
#include "OutputDlg.h"
#include "StepResult.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// OutputDlg dialog

IMPLEMENT_DYNAMIC(OutputDlg,StyleTab)

OutputDlg::OutputDlg(CWnd* pParent /*=nullptr*/)
          :StyleTab(IDD_OUTPUT, pParent)
{
}

OutputDlg::~OutputDlg()
{
}

void OutputDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleTab::DoDataExchange(pDX);

  DDX_Control(pDX, IDC_RETURN, m_editReturn,m_returnValue);
  DDX_Control(pDX, IDC_OUTPUT, m_editOutput,m_standardOutput);
  DDX_Control(pDX, IDC_ERROR,  m_editError, m_standardError);
}

BEGIN_MESSAGE_MAP(OutputDlg, StyleTab)
END_MESSAGE_MAP()

BOOL
OutputDlg::OnInitDialog()
{
  StyleTab::OnInitDialog();

  SetCanResize();
  UpdateData(FALSE);
  return TRUE;
}

void
OutputDlg::LoadVariables(StepResultCMD* p_stepResult)
{
  // Remember our object
  m_stepResult = p_stepResult;

  // Load our variables
  m_returnValue    = m_stepResult->GetReturnValue();
  m_standardOutput = m_stepResult->GetStandardOutput();
  m_standardError  = m_stepResult->GetStandardError();

  m_standardOutput.Replace(_T("\n"),_T("\r\n"));
  m_standardError .Replace(_T("\n"),_T("\r\n"));

  UpdateData(FALSE);
}

void
OutputDlg::SetupDynamicLayout()
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
OutputDlg::PreTranslateMessage(MSG* pMsg)
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

// OutputDlg message handlers
