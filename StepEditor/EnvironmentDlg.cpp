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
// This File   : EnvironmentDlg.cpp
// What it does: Changing the environment before a command-line test step
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StepEditor.h"
#include "TestStepCMD.h"
#include "StepEditorDlg.h"
#include "EnvironmentDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// EnvironmentDlg dialog

IMPLEMENT_DYNAMIC(EnvironmentDlg, StyleDialog)

EnvironmentDlg::EnvironmentDlg(CWnd* pParent /*=nullptr*/)
               :StyleDialog(IDD_ENVIRON, pParent)
{

}

EnvironmentDlg::~EnvironmentDlg()
{
}

void EnvironmentDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_GRID,   m_list);
  DDX_Control(pDX,IDC_NEW,    m_buttonNew);
  DDX_Control(pDX,IDC_DELETE, m_buttonDelete);
}

BEGIN_MESSAGE_MAP(EnvironmentDlg, StyleDialog)
  ON_BN_CLICKED(IDC_NEW,    &EnvironmentDlg::OnBnClickedNew)
  ON_BN_CLICKED(IDC_DELETE, &EnvironmentDlg::OnBnClickedDelete)

  ON_NOTIFY(LVN_ITEMCHANGED,IDC_GRID, &EnvironmentDlg::OnLvnItemchangedGrid)
END_MESSAGE_MAP()

BOOL 
EnvironmentDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();

  m_buttonNew   .SetIconImage(IDI_NEW);
  m_buttonDelete.SetIconImage(IDI_DELETE);

  InitLijst();

  SetCanResize();
  return TRUE;
}

void
EnvironmentDlg::SetupDynamicLayout()
{
  StyleDialog::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif

  manager.AddItem(IDC_GRP_EXTRA,CMFCDynamicLayout::MoveNone(),CMFCDynamicLayout::SizeHorizontalAndVertical(100,100));
  manager.AddItem(IDC_GRID,     CMFCDynamicLayout::MoveNone(),CMFCDynamicLayout::SizeHorizontalAndVertical(100,100));
  manager.AddItem(IDC_NEW,      CMFCDynamicLayout::MoveVertical(100),CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_DELETE,   CMFCDynamicLayout::MoveVertical(100),CMFCDynamicLayout::SizeNone());
}

void
EnvironmentDlg::InitLijst()
{
  m_list.SetColumnCount(2);
  m_list.SetRowCount(1);
  m_list.SetFixedRowCount(1);
  m_list.GetCell(0, 0)->SetText("Variable");
  m_list.GetCell(0, 1)->SetText("Value");
  m_list.SetColumnWidth(0, 150);
  m_list.SetColumnWidth(1, 500);
  m_list.SetEditable();
  m_list.SetSingleRowSelection();
  m_list.SetSortColumn(0);
}

void
EnvironmentDlg::InitTab(TestStepCMD* p_step)
{
  m_step = p_step;
  int count = 0;

  // Fill in all variables
  for(auto& var : m_step->GetEnvironmentVars())
  {
    int row = m_list.InsertRow(var.first);
    m_list.GetCell(row, 1)->SetText(var.second);

    ::SetEnvironmentVariable(var.first, var.second);
    ++count;
  }

  // If something read-in
  if(count)
  {
    // Set focus to first cell
    m_list.SetFocusCell(0, 0);
  }
}

void
EnvironmentDlg::StoreVariables()
{
  ParMap& map = m_step->GetEnvironmentVars();
  map.clear();

  for(int ind = 1; ind < m_list.GetRowCount(); ++ind)
  {
    CString variable = m_list.GetCell(ind,0)->GetText();
    CString value    = m_list.GetCell(ind,1)->GetText();

    map.insert(std::make_pair(variable,value));

    // And export to the OS shell
    ::SetEnvironmentVariable(variable,value);
  }

  if(m_list.GetRowCount() > 1)
  {
    // Set changes on rest of the dialog
    StepEditorDlg* parent = (StepEditorDlg*)GetParent()->GetParent();
    if(parent)
    {
      parent->EffectiveParameters();
    }
  }
}

// EnvironmentDlg message handlers

void 
EnvironmentDlg::OnBnClickedNew()
{
  int row = m_list.InsertRow("");
  m_list.GetCell(row, 1)->SetText("");
  m_list.SetFocusCell(row, 0);
  m_list.Invalidate();
}

void 
EnvironmentDlg::OnBnClickedDelete()
{
  CCellRange range = m_list.GetSelectedCellRange();
  int row = range.GetMinRow();
  if (row >= 0)
  {
    // Remove from the OS
    CGridCellBase* cell = m_list.GetCell(row, 0);
    if(cell)
    {
      CString variable = cell->GetText();
      if(!variable.IsEmpty())
      {
        ::SetEnvironmentVariable(variable,"");
      }

      // Remove from the grid
      range.SetMinCol(0);
      range.SetMaxCol(1);
      m_list.SetSelectedRange(range);
      m_list.DeleteRow(row);
      m_list.Invalidate();

      StoreVariables();
    }
  }
}

void
EnvironmentDlg::OnLvnItemchangedGrid(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

  if (pNMLV->uNewState)
  {
    StoreVariables();
    *pResult = 1;
  }
}

