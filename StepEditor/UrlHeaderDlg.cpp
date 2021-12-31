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
// This File   : UrlHeaderDlg.cpp
// What it does: Request headers for a HTTP internet test call
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StepEditor.h"
#include "TestStepIN.h"
#include "StepInternetDlg.h"
#include "UrlHeaderDlg.h"
#include <SearchVarDlg.h>
#include <afxdialogex.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// UrlHeaderDlg dialog

IMPLEMENT_DYNAMIC(UrlHeaderDlg, StyleDialog)

UrlHeaderDlg::UrlHeaderDlg(CWnd* pParent /*=nullptr*/)
             :StyleDialog(IDD_URL_HEADERS, pParent)
{
}

UrlHeaderDlg::~UrlHeaderDlg()
{
}

void UrlHeaderDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_GRID,   m_list);
  DDX_Control(pDX,IDC_NEW,    m_buttonNew);
  DDX_Control(pDX,IDC_DELETE, m_buttonDelete);
  DDX_Control(pDX,IDC_PARM,   m_buttonParms);
}

BEGIN_MESSAGE_MAP(UrlHeaderDlg, StyleDialog)
  ON_BN_CLICKED(IDC_NEW,    &UrlHeaderDlg::OnBnClickedNew)
  ON_BN_CLICKED(IDC_DELETE, &UrlHeaderDlg::OnBnClickedDelete)
  ON_BN_CLICKED(IDC_PARM,   &UrlHeaderDlg::OnBnClickedParms)
  ON_NOTIFY(LVN_ITEMCHANGED,IDC_GRID, &UrlHeaderDlg::OnLvnItemchangedGrid)
END_MESSAGE_MAP()

BOOL 
UrlHeaderDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();

  EnableToolTips();
  m_buttonParms.SetIconImage(IDI_LIST);
  RegisterTooltip(m_buttonParms,"Choose global/test parameter(s) for a header value.");

  InitLijst();

  SetCanResize();
  return TRUE;
}

void
UrlHeaderDlg::SetupDynamicLayout()
{
  StyleDialog::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif

  manager.AddItem(IDC_GRID,     CMFCDynamicLayout::MoveNone(),CMFCDynamicLayout::SizeHorizontalAndVertical(100,100));
  manager.AddItem(IDC_NEW,      CMFCDynamicLayout::MoveVertical(100),CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_DELETE,   CMFCDynamicLayout::MoveVertical(100),CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_PARM,     CMFCDynamicLayout::MoveVertical(100),CMFCDynamicLayout::SizeNone());
}

void
UrlHeaderDlg::InitLijst()
{
  m_list.SetColumnCount(2);
  m_list.SetRowCount(1);
  m_list.SetFixedRowCount(1);
  m_list.GetCell(0, 0)->SetText("Header");
  m_list.GetCell(0, 1)->SetText("Value");
  m_list.SetColumnWidth(0, 150);
  m_list.SetColumnWidth(1, 500);
  m_list.SetEditable();
  m_list.SetSingleRowSelection();
  m_list.SetSortColumn(0);
}

void
UrlHeaderDlg::InitTab(TestStepIN* p_step,Parameters* p_parameters)
{
  m_step       = p_step;
  m_parameters = p_parameters;
  int count    = 0;

  // Fill in all variables
  for(auto& header : m_step->GetHeaders())
  {
    int row = m_list.InsertRow(header.m_name);
    m_list.GetCell(row, 1)->SetText(header.m_value);
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
UrlHeaderDlg::EffectiveParameters()
{
  if(m_step)
  {
    // Check parameters
    StepInternetDlg* step = reinterpret_cast<StepInternetDlg*>(GetParent()->GetParent());
    step->EffectiveParameters();
  }
}

void
UrlHeaderDlg::StoreVariables()
{
  UrlHeaders& map = m_step->GetHeaders();
  map.clear();

  for(int ind = 1; ind < m_list.GetRowCount(); ++ind)
  {
    CString header = m_list.GetCell(ind,0)->GetText();
    CString value  = m_list.GetCell(ind,1)->GetText();

    m_step->SetHeader(header,value);
  }

  if(m_list.GetRowCount() > 1)
  {
    // Set changes on rest of the dialog
    StepInternetDlg* parent = (StepInternetDlg*)GetParent()->GetParent();
    if(parent)
    {
      parent->EffectiveParameters();
    }
  }
}

// UrlHeaderDlg message handlers

void 
UrlHeaderDlg::OnBnClickedNew()
{
  int row = m_list.InsertRow("");
  m_list.GetCell(row, 1)->SetText("");
  m_list.SetFocusCell(row, 0);
  m_list.Invalidate();
}

void 
UrlHeaderDlg::OnBnClickedDelete()
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

      // Remove from the grid
      range.SetMinCol(0);
      range.SetMaxCol(1);
      m_list.SetSelectedRange(range);
      m_list.DeleteRow(row);
      m_list.Invalidate();

      EffectiveParameters();
    }
  }
}

void
UrlHeaderDlg::OnLvnItemchangedGrid(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

  if (pNMLV->uNewState)
  {
    EffectiveParameters();
    *pResult = 1;
  }
}

void
UrlHeaderDlg::OnBnClickedParms()
{
  CCellRange range = m_list.GetSelectedCellRange();
  int row = range.GetMinRow();
  if (row >= 0)
  {
    // Remove from the OS
    CGridCellBase* cell = m_list.GetCell(row,1);
    if(cell)
    {
      SearchVarDlg dlg(m_parameters,true,true,true);
      if(dlg.DoModal() == IDOK)
      {
        CString var = dlg.GetChosenVariable();
        CString variable;
        switch (dlg.GetResultType())
        {
          case ParType::PAR_GLOBAL: variable.Format("$%s$",var.GetString()); break;
          case ParType::PAR_RETURN: variable.Format("[%s]",var.GetString()); break;
          case ParType::PAR_BUFFER: variable.Format("<%s>",var.GetString()); break;
        }
        CString txt = cell->GetText() + variable;
        cell->SetText(txt);
        m_list.Refresh();

        EffectiveParameters();
      }
    }
  }
}
