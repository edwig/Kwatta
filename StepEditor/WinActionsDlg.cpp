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
// What it does: Request URL parameters for a Windows UI test call
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StepEditor.h"
#include "TestStepWIN.h"
#include "StepWindowsDlg.h"
#include "WinActionsDlg.h"
#include <Grid\GridCellCombo.h>
#include <SearchVarDlg.h>
#include <afxdialogex.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// WinActionsDlg dialog

IMPLEMENT_DYNAMIC(WinActionsDlg, StyleTab)

WinActionsDlg::WinActionsDlg(CWnd* pParent /*=nullptr*/)
              :StyleTab(IDD_WIN_ACTIONS,pParent)
{
}

WinActionsDlg::~WinActionsDlg()
{
}

void WinActionsDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleTab::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_GRID,       m_list);
  DDX_Control(pDX,IDC_NEW,        m_buttonNew);
  DDX_Control(pDX,IDC_DELETE,     m_buttonDelete);
  DDX_Control(pDX,IDC_PARM_PARM,  m_buttonParm);
  DDX_Control(pDX,IDC_DOWN,       m_buttonDown);
  DDX_Control(pDX,IDC_UP,         m_buttonUp);

  if(pDX->m_bSaveAndValidate == FALSE)
  {
    // Take care of correct up/down arrow keys
    CCellID id = m_list.GetFocusCell();
    int maxrow = m_list.GetRowCount();
    m_buttonUp  .EnableWindow(id.row > 1);
    m_buttonDown.EnableWindow(id.row < maxrow - 1);
  }
}

BEGIN_MESSAGE_MAP(WinActionsDlg, StyleTab)
  ON_BN_CLICKED  (IDC_NEW,            &WinActionsDlg::OnBnClickedNew)
  ON_BN_CLICKED  (IDC_DELETE,         &WinActionsDlg::OnBnClickedDelete)
  ON_BN_CLICKED  (IDC_PARM_PARM,      &WinActionsDlg::OnBnClickedParm)
  ON_BN_CLICKED  (IDC_DOWN,           &WinActionsDlg::OnBnClickedDown)
  ON_BN_CLICKED  (IDC_UP,             &WinActionsDlg::OnBnClickedUp)
  ON_NOTIFY(LVN_ITEMCHANGED,IDC_GRID, &WinActionsDlg::OnLvnItemchangedGrid)
END_MESSAGE_MAP()

BOOL 
WinActionsDlg::OnInitDialog()
{
  StyleTab::OnInitDialog();

  m_buttonParm  .SetIconImage(IDI_LIST);
  m_buttonNew   .SetIconImage(IDI_NEW);
  m_buttonDelete.SetIconImage(IDI_DELETE);
  m_buttonUp    .SetIconImage(IDI_UP);
  m_buttonDown  .SetIconImage(IDI_DOWN);
  EnableToolTips();
  RegisterTooltip(m_buttonParm,"Choose global/test parameter(s) for WIN-UI parameter.");

  InitLijst();

  SetCanResize();
  return TRUE;
}

void
WinActionsDlg::SetupDynamicLayout()
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
  manager.AddItem(IDC_DOWN,       CMFCDynamicLayout::MoveVertical(100),CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_UP,         CMFCDynamicLayout::MoveVertical(100),CMFCDynamicLayout::SizeNone());
}

void
WinActionsDlg::InitLijst()
{
  m_list.SetColumnCount(6);
  m_list.SetRowCount(1);
  m_list.SetFixedRowCount(1);
  m_list.GetCell(0, 0)->SetText("Action");
  m_list.GetCell(0, 1)->SetText("WIN Pattern");
  m_list.GetCell(0, 2)->SetText("Parameter 1");
  m_list.GetCell(0, 3)->SetText("Parameter 2");
  m_list.GetCell(0, 4)->SetText("Parameter 3");
  m_list.GetCell(0, 5)->SetText("Extra wait");
  m_list.SetColumnWidth(0, 150);
  m_list.SetColumnWidth(1, 300);
  m_list.SetColumnWidth(2, 150);
  m_list.SetColumnWidth(3, 150);
  m_list.SetColumnWidth(4, 150);
  m_list.SetColumnWidth(5, 100);
  m_list.SetEditable();
  m_list.SetSingleRowSelection();
  m_list.SetSortColumn(0);
}

void
WinActionsDlg::InitTab(TestStepWIN* p_step,Parameters* p_parameters)
{
  m_step       = p_step;
  m_parameters = p_parameters;
  int count    = 0;

  m_list.DeleteNonFixedRows();

  // Fill in all variables
  for(auto& action: m_step->GetActions())
  {
    int row = m_list.InsertRow(p_step->WinUIActionToString(action->m_action));
    m_list.GetCell(row, 1)->SetText(action->m_pattern);
    m_list.GetCell(row, 2)->SetText(action->m_argument1);
    m_list.GetCell(row, 3)->SetText(action->m_argument2);
    m_list.GetCell(row, 4)->SetText(action->m_argument3);
    m_list.GetCell(row, 5)->SetText(action->m_wait);

    AddActions(row);

    ++count;
  }
  m_list.Refresh();

  // If something read-in
  if(count)
  {
    // Set focus to first cell
    m_list.SetFocusCell(0, 0);
  }
  UpdateData(FALSE);
}

void
WinActionsDlg::AddActions(int p_row)
{
  // Change first column to combo box
  m_list.SetCellType(p_row,0,RUNTIME_CLASS(CGridCellCombo));
  CGridCellCombo* combo = reinterpret_cast<CGridCellCombo*>(m_list.GetCell(p_row,0));
  combo->SetStyle(CBS_DROPDOWNLIST | CBS_SORT);

  CStringArray list;
  list.Add("Start");
  list.Add("Close");
  list.Add("Present");
  list.Add("Activate");
  list.Add("Focus");
  list.Add("Caret position");
  list.Add("Mouse Click");
  list.Add("Mouse Double Click");
  list.Add("Character");
  list.Add("String");
  list.Add("Text area");
  list.Add("Nothing");  // Waittime

  combo->SetOptions(list);

  m_list.Invalidate(FALSE);
}

bool
WinActionsDlg::IsFilled()
{
  return m_list.GetRowCount() > 1;
}

void
WinActionsDlg::EffectiveParameters()
{
  if(m_step)
  {
    // Check parameters
    StepWindowsDlg* step = reinterpret_cast<StepWindowsDlg*>(GetParent()->GetParent());
    step->EffectiveParameters();
  }
}

void
WinActionsDlg::StoreVariables()
{
  WinActionList& list = m_step->GetActions();
  m_step->ResetActions();

  for(int ind = 1; ind < m_list.GetRowCount(); ++ind)
  {
    CString action    = m_list.GetCell(ind,0)->GetText();
    CString pattern   = m_list.GetCell(ind,1)->GetText();
    CString argument1 = m_list.GetCell(ind,2)->GetText();
    CString argument2 = m_list.GetCell(ind,3)->GetText();
    CString argument3 = m_list.GetCell(ind,4)->GetText();
    CString wait      = m_list.GetCell(ind,5)->GetText();

    WinAction* act   = new WinAction();
    act->m_action    = m_step->StringToWinUIAction(action);
    act->m_pattern   = pattern;
    act->m_argument1 = argument1;
    act->m_argument2 = argument2;
    act->m_argument3 = argument3;
    act->m_wait      = wait;

    m_step->SetAction(act);
  }

  if(m_list.GetRowCount() > 1)
  {
    // Set changes on rest of the dialog
    StepWindowsDlg* parent = reinterpret_cast<StepWindowsDlg*>(GetParent()->GetParent());
    if(parent)
    {
      parent->EffectiveParameters();
    }
  }
}

// WinActionsDlg message handlers

void 
WinActionsDlg::OnBnClickedNew()
{
  int row = m_list.InsertRowAndSetFocus("");
  AddActions(row);
}

void 
WinActionsDlg::OnBnClickedDelete()
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
        CString ask;
        ask.Format("Do you want to delete the action [%s] ?", variable.GetString());
        if(StyleMessageBox(this,ask,KWATTA,MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION) == IDNO)
        {
          return;
        }
        // Delete from the teststep
        // m_step->DeleteParameter(variable);
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
WinActionsDlg::OnBnClickedParm()
{
  CCellRange range = m_list.GetSelectedCellRange();
  int row = range.GetMinRow();
  if (row >= 0)
  {
    // Remove from the OS
    CGridCellBase* cell = m_list.GetCell(row,1);
    if(cell)
    {
      SearchVarDlg dlg(this,m_parameters,true,true,true,true);
      if(dlg.DoModal() == IDOK || dlg.GetSaved())
      {
        CString variable = dlg.GetVariable();
        CString txt = cell->GetText() + variable;
        cell->SetText(txt);
        m_list.Refresh();

        EffectiveParameters();
      }
    }
  }
}

void 
WinActionsDlg::OnBnClickedDown()
{
  CCellID id = m_list.GetFocusCell();
  int total  = m_list.GetRowCount() - 1;
  int active = id.row - 1;
  if(active >= 0 && active < (total - 1))
  {
    StoreVariables();
    WinActionList& list = m_step->GetActions();
    if(list.size() > active)
    {
      std::swap(list[active],list[active + 1]);
    }
    // Re-Init the list
    InitTab(m_step,m_parameters);
  }
  id.row++;
  CCellRange range(id.row, id.col, id.row, id.col);
  m_list.SetSelectedRange(range,TRUE,TRUE);
  m_list.SetFocusCell(id);
  UpdateData(FALSE);
}

void 
WinActionsDlg::OnBnClickedUp()
{
  CCellID id = m_list.GetFocusCell();
  int total  = m_list.GetRowCount() - 1;
  int active = id.row - 1;
  if(active >= 1 && active < total)
  {
    StoreVariables();
    WinActionList& list = m_step->GetActions();
    if(list.size() > active)
    {
      std::swap(list[active],list[active - 1]);
    }
    // Re-Init the list
    InitTab(m_step,m_parameters);
  }
  id.row--;
  CCellRange range(id.row,id.col,id.row,id.col);
  m_list.SetFocus();
  m_list.SetSelectedRange(range,TRUE,TRUE);
  m_list.SetFocusCell(id);
  UpdateData(FALSE);
}

void
WinActionsDlg::OnLvnItemchangedGrid(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

  if (pNMLV->uNewState)
  {
    EffectiveParameters();
    *pResult = 1;
  }
  UpdateData(FALSE);
}

