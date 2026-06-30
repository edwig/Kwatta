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
// This File   : ValidationsDlg.cpp
// What it does: Shows the tab with all validations for this test steps result
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ResultViewer.h"
#include "ValidationsDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// ValidationsDlg dialog

IMPLEMENT_DYNAMIC(ValidationsDlg,StyleTab)

ValidationsDlg::ValidationsDlg(CWnd* pParent /*=nullptr*/)
               :StyleTab(IDD_VALIDATIONS, pParent)
{
}

ValidationsDlg::~ValidationsDlg()
{
}

void 
ValidationsDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleTab::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_GRID, m_grid);
}


BEGIN_MESSAGE_MAP(ValidationsDlg, StyleTab)
  ON_NOTIFY(LVN_ITEMCHANGED,IDC_GRID,&ValidationsDlg::OnLvnItemchangedGrid)
  ON_NOTIFY(NM_DBLCLK,IDC_GRID,      &ValidationsDlg::OnLvnDoubleClickGrid)
END_MESSAGE_MAP()

BOOL
ValidationsDlg::OnInitDialog()
{
  StyleTab::OnInitDialog();

  InitLijst();

  SetCanResize();
  return FALSE;
}

void
ValidationsDlg::SetupDynamicLayout()
{
  StyleTab::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif

  manager.AddItem(IDC_GRID,CMFCDynamicLayout::MoveNone(),CMFCDynamicLayout::SizeHorizontalAndVertical(100,100));
}

void
ValidationsDlg::InitLijst()
{
  // Set the columns
  m_grid.SetColumnCount(4);
  m_grid.SetRowCount(1);
  m_grid.SetFixedRowCount(1);
  m_grid.SetEditable(FALSE);
  m_grid.GetCell(0, COL_NUMBER)->SetText(_T("Number"));
  m_grid.GetCell(0, COL_GLOBAL)->SetText(_T("GT"));
  m_grid.GetCell(0, COL_NAME  )->SetText(_T("Validation"));
  m_grid.GetCell(0, COL_RESULT)->SetText(_T("Result"));
  m_grid.SetColumnWidth(COL_NUMBER,  80);
  m_grid.SetColumnWidth(COL_GLOBAL,  32);
  m_grid.SetColumnWidth(COL_NAME,   800);
  m_grid.SetColumnWidth(COL_RESULT, 150);
  m_grid.SetSingleRowSelection();
  m_grid.SetSortColumn(0);

  // Create image list from bitmap
  m_images.Create(MAKEINTRESOURCE(IDB_STATUS),16,1,RGB(256,256,256));
  m_grid.SetImageList(&m_images);
}

void
ValidationsDlg::SetTextImage(int p_row, int p_col, XString p_text, int p_image)
{
  GV_ITEM item;
  item.mask   = GVIF_IMAGE;
  item.row    = p_row;
  item.col    = p_col;
  item.iImage = p_image;
  if (!p_text.IsEmpty())
  {
    item.mask   |= GVIF_TEXT;
    item.strText = p_text;
  }
  m_grid.SetItem(&item);
}

void
ValidationsDlg::LoadVariables(StepResult* p_stepResult)
{
  // Remember the result object
  m_stepResult = p_stepResult;

  m_grid.SetAutoSizeStyle();

  int count = 0;
  for(auto& val : m_stepResult->GetValidations())
  {
    XString num;
    num.Format(_T("%d"), val.m_number);
    int row = m_grid.InsertRow(num);
    m_grid.GetCell(row,COL_NAME)->SetText(val.m_validation);

    SetTextImage(row,COL_GLOBAL,_T(""),val.m_global ? 2 : 3);
    SetTextImage(row,COL_RESULT,val.m_ok ? _T("OK") : _T("ERROR"),val.m_ok ? 1 : 0);

    ++count;
  }

  // If something read-in
  if(count)
  {
    // Set focus to first cell
    m_grid.SetFocusCell(1,0);
  }
}

// ValidationsDlg message handlers

void 
ValidationsDlg::OnLvnItemchangedGrid(NMHDR* pNMHDR, LRESULT* pResult)
{
  LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

  if(pNMLV->uNewState)
  {
    int item = pNMLV->iItem;
    int number = _ttoi(m_grid.GetCell(item,COL_NUMBER)->GetText());
    for (auto& val : m_stepResult->GetValidations())
    {
      if(val.m_number == number)
      {
        XString filename = val.m_filename;
      }
    }

  }
  *pResult = 1;
}

void 
ValidationsDlg::OnLvnDoubleClickGrid(NMHDR* pNMHDR, LRESULT* pResult)
{
  LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

  if(pNMLV->uNewState)
  {
    int item = pNMLV->iItem;
    XString validation = m_grid.GetCell(item,COL_NAME)->GetText();

    for(auto& val : m_stepResult->GetValidations())
    {
      if(val.m_validation.CompareNoCase(validation) == 0)
      {
        theApp.StartValidateEditor(val.m_filename,val.m_global,GetSafeHwnd(),item);
      }
    }
  }
}

// ENTER key leads to an empty tab dialog
// That's why we catch the VK_RETURN here
BOOL
ValidationsDlg::PreTranslateMessage(MSG* pMsg)
{
  if(pMsg->message == WM_KEYUP ||
     pMsg->message == WM_KEYDOWN)
  {
    if(pMsg->wParam == VK_RETURN ||
       pMsg->wParam == VK_ESCAPE)
    {
      return TRUE;
    }
  }
  // Not handled
  return FALSE;
}
