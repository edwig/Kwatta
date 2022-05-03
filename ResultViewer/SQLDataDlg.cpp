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
// This File   : SQLDataDlg.cpp
// What it does: Shows the resulting data row from a SQL test
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "ResultViewer.h"
#include "SQLDataDlg.h"
#include <StepResultSQL.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// SQLDataDlg dialog

IMPLEMENT_DYNAMIC(SQLDataDlg,StyleDialog)

SQLDataDlg::SQLDataDlg(CWnd* pParent /*=nullptr*/)
           :StyleDialog(IDD_SQL_DATA,pParent)
{
}

SQLDataDlg::~SQLDataDlg()
{
}

void SQLDataDlg::DoDataExchange(CDataExchange* pDX)
{
  StyleDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_GRID,m_list);
}

BEGIN_MESSAGE_MAP(SQLDataDlg,StyleDialog)
END_MESSAGE_MAP()

BOOL
SQLDataDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();
  InitLijst();
  SetCanResize();
  return TRUE;
}

void
SQLDataDlg::SetupDynamicLayout()
{
  StyleDialog::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif
  manager.AddItem(IDC_GRID,CMFCDynamicLayout::MoveNone(),CMFCDynamicLayout::SizeHorizontalAndVertical(100,100));
}

void
SQLDataDlg::InitLijst()
{
  m_list.SetColumnCount(2);
  m_list.SetRowCount(1);
  m_list.SetFixedRowCount(1);
  m_list.GetCell(0,0)->SetText("Column");
  m_list.GetCell(0,1)->SetText("Data");
  m_list.SetColumnWidth(0,150);
  m_list.SetColumnWidth(1,500);
  m_list.SetEditable(FALSE);
  m_list.SetSingleRowSelection();
  m_list.SetSortColumn(0);
}

void
SQLDataDlg::LoadVariables(StepResultSQL* p_result)
{
  int count = 0;

  // Fill in all variables
  for(auto& data : p_result->GetResultMap())
  {
    int row = m_list.InsertRow(data.first);
    m_list.GetCell(row,1)->SetText(data.second);
    ++count;
  }

  // If something read-in
  if(count)
  {
    // Set focus to first cell
    m_list.SetFocusCell(0,0);
  }
}

// SQLDataDlg message handlers

