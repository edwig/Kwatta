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
// This File   : UrlParameterDlg.cpp
// What it does: Request URL parameters for a HTTP internet test call
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StepEditor.h"
#include "TestStepNET.h"
#include "StepInternetDlg.h"
#include "URLParameterDlg.h"
#include <SearchVarDlg.h>
#include <afxdialogex.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// HTTPParameterDlg dialog

IMPLEMENT_DYNAMIC(URLParameterDlg, StyleTab)

URLParameterDlg::URLParameterDlg(CWnd* pParent /*=nullptr*/)
                 :StyleTab(IDD_HTTP_PARAM,pParent)
{

}

URLParameterDlg::~URLParameterDlg()
{
}

void URLParameterDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleTab::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_GRID,       m_list);
  DDX_Control(pDX,IDC_NEW,        m_buttonNew);
  DDX_Control(pDX,IDC_DELETE,     m_buttonDelete);
  DDX_Control(pDX,IDC_PARM_PARM,  m_buttonParmParm);
  DDX_Control(pDX,IDC_ANCHOR,     m_editAnchor,m_anchor);
  DDX_Control(pDX,IDC_ANCHOR_PARM,m_buttonAnchorParm);
}

BEGIN_MESSAGE_MAP(URLParameterDlg, StyleTab)
  ON_BN_CLICKED  (IDC_NEW,            &URLParameterDlg::OnBnClickedNew)
  ON_BN_CLICKED  (IDC_DELETE,         &URLParameterDlg::OnBnClickedDelete)
  ON_BN_CLICKED  (IDC_PARM_PARM,      &URLParameterDlg::OnBnClickedParmParm)
  ON_EN_KILLFOCUS(IDC_ANCHOR,         &URLParameterDlg::OnEnKillfocusAnchor)
  ON_BN_CLICKED  (IDC_ANCHOR_PARM,    &URLParameterDlg::OnBnClickedAnchorParm)
  ON_NOTIFY(LVN_ITEMCHANGED,IDC_GRID, &URLParameterDlg::OnLvnItemchangedGrid)
END_MESSAGE_MAP()

BOOL 
URLParameterDlg::OnInitDialog()
{
  StyleTab::OnInitDialog();

  m_buttonParmParm  .SetIconImage(IDI_LIST);
  m_buttonAnchorParm.SetIconImage(IDI_LIST);
  m_buttonNew       .SetIconImage(IDI_NEW);
  m_buttonDelete    .SetIconImage(IDI_DELETE);
  EnableToolTips();
  RegisterTooltip(m_buttonParmParm,  _T("Choose global/test parameter(s) for URL parameter."));
  RegisterTooltip(m_buttonAnchorParm,_T("Choose global/test parameter(s) for URL anchor."));

  InitLijst();

  SetCanResize();
  return TRUE;
}

void
URLParameterDlg::SetupDynamicLayout()
{
  StyleTab::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif

  manager.AddItem(IDC_GRP_EXTRA,  CMFCDynamicLayout::MoveNone(),       CMFCDynamicLayout::SizeHorizontalAndVertical(100,100));
  manager.AddItem(IDC_GRID,       CMFCDynamicLayout::MoveNone(),       CMFCDynamicLayout::SizeHorizontalAndVertical(100,100));
  manager.AddItem(IDC_NEW,        CMFCDynamicLayout::MoveVertical(100),CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_DELETE,     CMFCDynamicLayout::MoveVertical(100),CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_PARM_PARM,  CMFCDynamicLayout::MoveVertical(100),CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_GRP_ANCHOR, CMFCDynamicLayout::MoveVertical(100),CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_ANCHOR,     CMFCDynamicLayout::MoveVertical(100),CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_ANCHOR_PARM,CMFCDynamicLayout::MoveHorizontalAndVertical(100,100),CMFCDynamicLayout::SizeNone());
}

void
URLParameterDlg::InitLijst()
{
  m_list.SetColumnCount(2);
  m_list.SetRowCount(1);
  m_list.SetFixedRowCount(1);
  m_list.GetCell(0, 0)->SetText(_T("Parameter"));
  m_list.GetCell(0, 1)->SetText(_T("Value"));
  m_list.SetColumnWidth(0, 150);
  m_list.SetColumnWidth(1, 500);
  m_list.SetEditable();
  m_list.SetSingleRowSelection();
  m_list.SetSortColumn(0);
}

void
URLParameterDlg::InitTab(TestStepNET* p_step,Parameters* p_parameters)
{
  m_step       = p_step;
  m_parameters = p_parameters;
  int count    = 0;

  // Fill in all variables
  for(auto& pair: m_step->GetParameters())
  {
    int row = m_list.InsertRow(pair.m_name);
    m_list.GetCell(row, 1)->SetText(pair.m_value);
    ++count;
  }

  // If something read-in
  if(count)
  {
    // Set focus to first cell
    m_list.SetFocusCell(0, 0);
  }

  // Getting the anchor
  m_anchor = m_step->GetAnchor();
  UpdateData(FALSE);
}

bool
URLParameterDlg::IsFilled()
{
  return m_list.GetRowCount() > 1;
}

void
URLParameterDlg::EffectiveParameters()
{
  if(m_step)
  {
    // Check parameters
    StepInternetDlg* step = reinterpret_cast<StepInternetDlg*>(GetParent()->GetParent());
    step->EffectiveParameters();
  }
}

void
URLParameterDlg::StoreVariables()
{
  UrlParameters& map = m_step->GetParameters();
  map.clear();

  for(int ind = 1; ind < m_list.GetRowCount(); ++ind)
  {
    XString param = m_list.GetCell(ind,0)->GetText();
    XString value = m_list.GetCell(ind,1)->GetText();

    m_step->SetParameter(param,value);
  }

  if(m_list.GetRowCount() > 1)
  {
    // Set changes on rest of the dialog
    StepInternetDlg* parent = reinterpret_cast<StepInternetDlg*>(GetParent()->GetParent());
    if(parent)
    {
      parent->EffectiveParameters();
    }
  }

  // Store the anchor
  m_step->SetAnchor(m_anchor);
}

// HTTPParameterDlg message handlers

void 
URLParameterDlg::OnBnClickedNew()
{
  m_list.InsertRowAndSetFocus(_T(""));
}

void 
URLParameterDlg::OnBnClickedDelete()
{
  CCellRange range = m_list.GetSelectedCellRange();
  int row = range.GetMinRow();
  if (row >= 0)
  {
    // Remove from the OS
    CGridCellBase* cell = m_list.GetCell(row, 0);
    if(cell)
    {
      XString variable = cell->GetText();
      if(!variable.IsEmpty())
      {
        XString ask;
        ask.Format(_T("Do you want to delete the URL parameter [%s] ?"), variable.GetString());
        if(StyleMessageBox(this,ask,_T(KWATTA),MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION) == IDNO)
        {
          return;
        }
        // Delete from the teststep
        m_step->DeleteParameter(variable);
        // Remove from the grid
        range.SetMinCol(0);
        range.SetMaxCol(1);
        m_list.SetSelectedRange(range);
        m_list.DeleteRow(row);
        m_list.Invalidate();

        // Re-calculate the completeness
        EffectiveParameters();
      }
    }
  }
}

void 
URLParameterDlg::OnBnClickedParmParm()
{
  CCellRange range = m_list.GetSelectedCellRange();
  int row = range.GetMinRow();
  if (row >= 0)
  {
    // Remove from the OS
    CGridCellBase* cell = m_list.GetCell(row,1);
    if(cell)
    {
      AutoFocus focus;
      SearchVarDlg dlg(this,m_parameters,true,true,true,true);
      if(dlg.DoModal() == IDOK || dlg.GetSaved())
      {
        XString variable = dlg.GetVariable();
        XString txt = cell->GetText() + variable;
        cell->SetText(txt);
        m_list.Refresh();

        EffectiveParameters();
      }
    }
  }
}

void
URLParameterDlg::OnLvnItemchangedGrid(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

  if (pNMLV->uNewState)
  {
    EffectiveParameters();
    *pResult = 1;
  }
}

void 
URLParameterDlg::OnEnKillfocusAnchor()
{
  UpdateData();
  EffectiveParameters();
}

void 
URLParameterDlg::OnBnClickedAnchorParm()
{
  AutoFocus focus;
  SearchVarDlg dlg(this,m_parameters,true,true,true,true);
  if(dlg.DoModal() == IDOK || dlg.GetSaved())
  {
    XString variable = dlg.GetVariable();
    m_editAnchor.InsertAtCurPos(variable,0);
    OnEnKillfocusAnchor();
  }
}
