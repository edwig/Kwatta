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
// This File   : SQLStatusDlg.cpp
// What it does: Shows the status output of a SQL test command
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ResultViewer.h"
#include "SQLStatusDlg.h"
#include "StepResult.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// SQLStatusDlg dialog

IMPLEMENT_DYNAMIC(SQLStatusDlg,StyleTab)

SQLStatusDlg::SQLStatusDlg(CWnd* pParent /*=nullptr*/)
             :StyleTab(IDD_SQL_STATUS, pParent)
{
}

SQLStatusDlg::~SQLStatusDlg()
{
}

void SQLStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleTab::DoDataExchange(pDX);

  DDX_Control(pDX, IDC_SUCCEEDED, m_editSucceeded,m_succeeded);
  DDX_Control(pDX, IDC_ROWS,      m_editRows,     m_returnedRows);
  DDX_Control(pDX, IDC_COLS,      m_editCols,     m_returnedCols);
  DDX_Control(pDX, IDC_SQLSTATE,  m_editState,    m_SQLState);
  DDX_Control(pDX, IDC_NATIVE,    m_editNative,   m_nativeError);
}

BEGIN_MESSAGE_MAP(SQLStatusDlg, StyleTab)
END_MESSAGE_MAP()

BOOL
SQLStatusDlg::OnInitDialog()
{
  StyleTab::OnInitDialog();

  m_editSucceeded.SetMutable(false);
  m_editRows     .SetMutable(false);
  m_editCols     .SetMutable(false);
  m_editState    .SetMutable(false);
  m_editNative   .SetMutable(false);

  SetCanResize();
  UpdateData(FALSE);
  return TRUE;
}

void
SQLStatusDlg::SetupDynamicLayout()
{
  StyleTab::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif

  manager.AddItem(IDC_SUCCEEDED,CMFCDynamicLayout::MoveNone(),CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_ROWS,     CMFCDynamicLayout::MoveNone(),CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_COLS,     CMFCDynamicLayout::MoveNone(),CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_SQLSTATE, CMFCDynamicLayout::MoveNone(),CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_NATIVE,   CMFCDynamicLayout::MoveNone(),CMFCDynamicLayout::SizeHorizontal(100));
}

void
SQLStatusDlg::LoadVariables(StepResultSQL* p_stepResult)
{
  // Remember our object
  m_stepResult = p_stepResult;

  // Load our variables
  m_succeeded    = m_stepResult->GetSucceeded();
  m_returnedRows = m_stepResult->GetResultRows();
  m_returnedCols = m_stepResult->GetResultCols();
  m_SQLState     = m_stepResult->GetSQLState();
  m_nativeError  = m_stepResult->GetNativeStatus();

  UpdateData(FALSE);
}

// ENTER/ESCAPE keys are caught here
BOOL
SQLStatusDlg::PreTranslateMessage(MSG* pMsg)
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
  return StyleTab::PreTranslateMessage(pMsg);
}

// SQLStatusDlg message handlers
