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
// This File   : SQLResultDlg.cpp
// What it does: Resulting columns of a SQL test call
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "StepEditor.h"
#include "SQLResultDlg.h"
#include <StepResultSQL.h>
#include "StepDatabaseDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// SQLResultDlg dialog

IMPLEMENT_DYNAMIC(SQLResultDlg,StyleTab)

SQLResultDlg::SQLResultDlg(CWnd* pParent /*=nullptr*/)
             :StyleTab(IDD_SQLRESULT, pParent)
{
}

SQLResultDlg::~SQLResultDlg()
{
}

void SQLResultDlg::DoDataExchange(CDataExchange* pDX)
{
  StyleTab::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_GRID, m_list);
  DDX_Control(pDX,IDC_SPEED,m_editSpeed,m_speed);
  DDX_Control(pDX,IDC_CLEAR,m_buttonClear);
}

BEGIN_MESSAGE_MAP(SQLResultDlg, StyleTab)
  ON_BN_CLICKED(IDC_CLEAR,OnBnClickedClear)
END_MESSAGE_MAP()

BOOL
SQLResultDlg::OnInitDialog()
{
  StyleTab::OnInitDialog();

  InitLijst();
  m_editSpeed.SetMutable(false);
  m_buttonClear.SetIconImage(IDI_CLEAR);

  SetCanResize();
  return TRUE;
}

void
SQLResultDlg::SetupDynamicLayout()
{
  StyleTab::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif
  manager.AddItem(IDC_GRID, CMFCDynamicLayout::MoveNone(),                        CMFCDynamicLayout::SizeHorizontalAndVertical(100,100));
  manager.AddItem(IDC_SPEED,CMFCDynamicLayout::MoveVertical(100),                 CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_CLEAR,CMFCDynamicLayout::MoveHorizontalAndVertical(100,100),CMFCDynamicLayout::SizeNone());
}

void
SQLResultDlg::InitLijst()
{
  m_list.SetColumnCount(2);
  m_list.SetRowCount(1);
  m_list.SetFixedRowCount(1);
  m_list.GetCell(0, 0)->SetText(_T("Column name"));
  m_list.GetCell(0, 1)->SetText(_T("Value"));
  m_list.SetColumnWidth(0, 150);
  m_list.SetColumnWidth(1, 700);
  m_list.SetEditable(FALSE);
  m_list.SetSingleRowSelection();
  m_list.SetSortColumn(0);
}

void
SQLResultDlg::InitTab()
{
  m_list.DeleteNonFixedRows();
}

void
SQLResultDlg::SetResult(StepResultSQL* p_result)
{
  m_result = p_result;

  int count = 0;
  ResultMap& data = p_result->GetResultMap();

  m_list.DeleteNonFixedRows();
  for (auto& col : data)
  {
    int row = m_list.InsertRow(col.first);
    m_list.GetCell(row, 1)->SetText(col.second);
    ++count;
  }
  m_list.Invalidate();
  if(count)
  {
    m_list.SetFocusCell(1, 0);
  }

  m_speed.Format(_T("%.3f"),p_result->GetTiming());
  UpdateData(FALSE);
}

// SQLResultDlg message handlers

void 
SQLResultDlg::OnBnClickedClear()
{
  StepDatabaseDlg* parent = dynamic_cast<StepDatabaseDlg*>(GetParent()->GetParent());
  if(parent)
  {
    StepResultSQL empty;
    parent->SetStepResult(&empty);
  }
  UpdateData(FALSE);
}
