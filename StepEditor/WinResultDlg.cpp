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
// This program: StepEditor
// This File   : WinResultDlg.cpp
// What it does: Resulting response of a Windows UIT test
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "StepEditor.h"
#include "WinResultDlg.h"
#include "StepWindowsDlg.h"
#include <TestStepWIN.h>
#include <StepResultWIN.h>
#include <HTTPError.h>
#include <JSONMessage.h>
#include <XMLMessage.h>
#include <afxdialogex.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// WinResultDlg dialog

IMPLEMENT_DYNAMIC(WinResultDlg,StyleTab)

WinResultDlg::WinResultDlg(CWnd* pParent /*=nullptr*/)
	           :StyleTab(IDD_RESPONSE, pParent)
{
}

WinResultDlg::~WinResultDlg()
{
}

void 
WinResultDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleTab::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_STATUS,  m_editStatus, m_status);
  DDX_Control(pDX,IDC_ERRORS,  m_editErrors, m_errors);
  DDX_Control(pDX,IDC_LOGGING, m_editLogging,m_logging);
  DDX_Control(pDX,IDC_CLEAR,   m_buttonClear);
}

BEGIN_MESSAGE_MAP(WinResultDlg, StyleTab)
  ON_EN_CHANGE (IDC_STATUS, &WinResultDlg::OnEnChangeStatus)
  ON_EN_CHANGE (IDC_ERRORS, &WinResultDlg::OnEnChangeErrors)
  ON_EN_CHANGE (IDC_LOGGING,&WinResultDlg::OnEnChangeLogging)
  ON_BN_CLICKED(IDC_CLEAR,  &WinResultDlg::OnBnClickedClear)
END_MESSAGE_MAP()

BOOL
WinResultDlg::OnInitDialog()
{
  StyleTab::OnInitDialog();

  m_editStatus .SetMutable(false);
  m_editErrors .SetMutable(false);
  m_editLogging.SetMutable(false);

  EnableToolTips();
  RegisterTooltip(m_buttonClear, "Remove test results for Windows UI test!");
  m_buttonClear.SetIconImage(IDI_CLEAR);

  InitTextFields();
  SetCanResize();
  return TRUE;
}

void
WinResultDlg::SetupDynamicLayout()
{
  StyleTab::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif
  manager.AddItem(IDC_CLEAR,   CMFCDynamicLayout::MoveHorizontal(100), CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_STATUS,  CMFCDynamicLayout::MoveNone(),          CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_ERRORS,  CMFCDynamicLayout::MoveNone(),          CMFCDynamicLayout::SizeHorizontalAndVertical(100,25));
  manager.AddItem(IDC_LOGGING, CMFCDynamicLayout::MoveVertical(25),    CMFCDynamicLayout::SizeHorizontalAndVertical(100,75));
  manager.AddItem(IDC_ST_LOG,  CMFCDynamicLayout::MoveVertical(25),    CMFCDynamicLayout::SizeNone());
}

void 
WinResultDlg::InitTab()
{
  m_status = 0;
  m_errors.Empty();
  m_logging.Empty();

  UpdateData(FALSE);
}

void
WinResultDlg::InitTextFields()
{
  m_editErrors .SetFontName("Courier new",100);
  m_editLogging.SetFontName("Courier new",100);
}

void 
WinResultDlg::SetResult(StepResultWIN* p_result)
{
  m_status  = p_result->GetLastOSError();
  m_errors  = p_result->GetErrorString();
  m_logging = p_result->GetResultString();

  m_errors .Replace("\n","\r\n");
  m_logging.Replace("\n","\r\n");
  UpdateData(FALSE);
}

// WinResultDlg message handlers

void 
WinResultDlg::OnEnChangeStatus()
{
  UpdateData();
}

void 
WinResultDlg::OnEnChangeErrors()
{
  UpdateData();
}

void 
WinResultDlg::OnEnChangeLogging()
{
  UpdateData();
}

void 
WinResultDlg::OnBnClickedClear()
{
  StepWindowsDlg* dlg = dynamic_cast<StepWindowsDlg*>(GetParent()->GetParent());
  if(dlg)
  {
    dlg->ResetStepResult();
  }
}
