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

IMPLEMENT_DYNAMIC(ValidationsDlg, StyleDialog)

ValidationsDlg::ValidationsDlg(CWnd* pParent /*=nullptr*/)
               :StyleDialog(IDD_VALIDATIONS, pParent)
{
}

ValidationsDlg::~ValidationsDlg()
{
}

void 
ValidationsDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_GRID, m_grid);
}


BEGIN_MESSAGE_MAP(ValidationsDlg, StyleDialog)
  ON_NOTIFY(LVN_ITEMCHANGED,IDC_GRID,&ValidationsDlg::OnLvnItemchangedGrid)
  ON_NOTIFY(NM_DBLCLK,IDC_GRID,      &ValidationsDlg::OnLvnDoubleClickGrid)
END_MESSAGE_MAP()

BOOL
ValidationsDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();

  InitLijst();

  SetCanResize();
  return FALSE;
}

void
ValidationsDlg::SetupDynamicLayout()
{
  StyleDialog::SetupDynamicLayout();

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
  m_grid.SetColumnCount(3);
  m_grid.SetRowCount(1);
  m_grid.SetFixedRowCount(1);
  m_grid.SetEditable(FALSE);
  m_grid.GetCell(0, 0)->SetText("Number");
  m_grid.GetCell(0, 1)->SetText("Validation");
  m_grid.GetCell(0, 2)->SetText("Result");
  m_grid.SetColumnWidth(0,  80);
  m_grid.SetColumnWidth(1, 800);
  m_grid.SetColumnWidth(2, 150);
  m_grid.SetSingleRowSelection();
  m_grid.SetSortColumn(0);

  // Create image list from bitmap
  m_images.Create(MAKEINTRESOURCE(IDB_STATUS),16,1,RGB(256,256,256));
  m_grid.SetImageList(&m_images);
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
    CString num;
    num.Format("%d", val.m_number);
    int row = m_grid.InsertRow(num);
    m_grid.GetCell(row, 1)->SetText(val.m_validation);

    GV_ITEM item;
    item.mask = GVIF_TEXT | GVIF_IMAGE;
    item.row  = row;
    item.col   = 2;
    item.strText = val.m_ok ? "OK" : "ERROR";
    item.iImage  = val.m_ok ? 1    : 0;
    m_grid.SetItem(&item);

    ++count;
  }

  // If something read-in
  if (count)
  {
    // Set focus to first cell
    m_grid.SetFocusCell(0, 0);
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
    int number = atoi(m_grid.GetCell(item, 0)->GetText());
    for (auto& val : m_stepResult->GetValidations())
    {
      if(val.m_number == number)
      {
        CString filename = val.m_filename;
        // Todo : Start ValidateEditor op basis van deze file
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
    CString validation = m_grid.GetCell(item,1)->GetText();

    for(auto& val : m_stepResult->GetValidations())
    {
      if(val.m_validation.CompareNoCase(validation) == 0)
      {
        theApp.StartValidateEditor(val.m_filename,GetSafeHwnd(),item);
      }
    }
  }
}

// ENTER toets leidt tot instellingen dialoog met leeg tabblad
// Daarom wordt de 'VK_RETURN' hier afgevangen en als afgehandeld beschouwd
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
  // Niet afgehandeld
  return FALSE;
}

