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
// This program: ValidateEditor
// This File   : SQLDataDlg.cpp
// What it does: Validation of returned data row of a SQL database statement
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SQLDataDlg.h"
#include "ValidateEditor.h"
#include "ValidateDatabaseDlg.h"
#include <SearchVarDlg.h>
#include <NewVariableDlg.h>

// SQLDataDlg dialog

IMPLEMENT_DYNAMIC(SQLDataDlg,StyleTab)

SQLDataDlg::SQLDataDlg(CWnd* p_parent)
	         :StyleTab(IDD_SQL_DATA,p_parent)
{
}

SQLDataDlg::~SQLDataDlg()
{
}

void 
SQLDataDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleTab::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_USE_RETURN, m_checkData);
	DDX_Control(pDX,IDC_GRID,				m_list);
	DDX_Control(pDX,IDC_NEW,				m_buttonNew);
	DDX_Control(pDX,IDC_DELETE,			m_buttonDelete);
	DDX_Control(pDX,IDC_PARM,				m_buttonParm);
	DDX_Control(pDX,IDC_OPERATOR,		m_comboOperator);
	DDX_Control(pDX,IDC_EFF_RET,    m_editEffective,m_effectiveData);
	DDX_Control(pDX,IDC_SUCCEED_VAR,m_comboVariable);
	DDX_Control(pDX,IDC_NEWDATAVAR, m_buttonVariable);

  if(pDX->m_bSaveAndValidate == false)
  {
    m_list          .EnableWindow(m_useData);
    m_buttonNew     .EnableWindow(m_useData);
    m_buttonDelete  .EnableWindow(m_useData);
    m_buttonParm    .EnableWindow(m_useData);
    m_comboOperator .EnableWindow(m_useData);
    m_editEffective .EnableWindow(m_useData);
    m_comboVariable .EnableWindow(m_useData);
    m_buttonVariable.EnableWindow(m_useData);

    bool one = m_list.GetRowCount() == 2;

    m_comboOperator.EnableWindow(one);
    m_editEffective.EnableWindow(one);
    m_comboVariable.EnableWindow(one);
    m_buttonVariable.EnableWindow(one);
  }
}

BEGIN_MESSAGE_MAP(SQLDataDlg, StyleTab)
  ON_BN_CLICKED   (IDC_USE_RETURN,		&SQLDataDlg::OnBnClickedUseData)
  ON_NOTIFY(LVN_ITEMCHANGED,IDC_GRID,	&SQLDataDlg::OnLvnItemchangedGrid)
	ON_BN_CLICKED   (IDC_NEW,						&SQLDataDlg::OnBnClickedNew)
	ON_BN_CLICKED   (IDC_DELETE,				&SQLDataDlg::OnBnClickedDelete)
	ON_BN_CLICKED   (IDC_PARM,					&SQLDataDlg::OnBnClickedParm)
  ON_CBN_SELCHANGE(IDC_OPERATOR,			&SQLDataDlg::OnCbnSelchangeOperator)
  ON_EN_CHANGE    (IDC_EFF_RET,   		&SQLDataDlg::OnEnChangeEffData)
  ON_CBN_SELCHANGE(IDC_SUCCEED_VAR,		&SQLDataDlg::OnCbnSelchangeDataVariable)
  ON_BN_CLICKED   (IDC_NEWDATAVAR,    &SQLDataDlg::OnBnClickedNewvar)
END_MESSAGE_MAP()

BOOL
SQLDataDlg::OnInitDialog()
{
	StyleTab::OnInitDialog();

	m_buttonParm.SetIconImage(IDI_LIST);
	m_buttonNew .SetIconImage(IDI_NEW);
	m_buttonDelete.SetIconImage(IDI_DELETE);
  m_buttonVariable.SetIconImage(IDI_LIST);
	EnableToolTips();
	RegisterTooltip(m_buttonNew,		  "Add a column name and data cell value.");
	RegisterTooltip(m_buttonDelete,   "Delete a result column name and data cell value");
	RegisterTooltip(m_buttonParm,     "Choose global/test parameter(s) for a data cell.");
	RegisterTooltip(m_buttonVariable, "Choose global/test parameter(s) for the resulting return value.");

	InitLijst();

	SetCanResize();
	return TRUE;
}

void
SQLDataDlg::SetupDynamicLayout()
{
	StyleTab::SetupDynamicLayout();

	CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
	manager.AssertValid();
#endif
	manager.AddItem(IDC_GRID,					CMFCDynamicLayout::MoveNone(),				CMFCDynamicLayout::SizeHorizontalAndVertical(100, 100));
	manager.AddItem(IDC_NEW,					CMFCDynamicLayout::MoveVertical(100), CMFCDynamicLayout::SizeNone());
	manager.AddItem(IDC_DELETE,				CMFCDynamicLayout::MoveVertical(100), CMFCDynamicLayout::SizeNone());
	manager.AddItem(IDC_PARM,					CMFCDynamicLayout::MoveVertical(100), CMFCDynamicLayout::SizeNone());
	manager.AddItem(IDC_ST_OPERATOR,	CMFCDynamicLayout::MoveVertical(100), CMFCDynamicLayout::SizeNone());
	manager.AddItem(IDC_OPERATOR,			CMFCDynamicLayout::MoveVertical(100), CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_ST_EFFECTIVE, CMFCDynamicLayout::MoveVertical(100), CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_EFF_RET,      CMFCDynamicLayout::MoveVertical(100), CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_ST_VAR,       CMFCDynamicLayout::MoveVertical(100), CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_SUCCEED_VAR,  CMFCDynamicLayout::MoveVertical(100), CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_NEWDATAVAR,   CMFCDynamicLayout::MoveVertical(100), CMFCDynamicLayout::SizeNone());
}

void
SQLDataDlg::InitLijst()
{
	m_list.SetColumnCount(2);
	m_list.SetRowCount(1);
	m_list.SetFixedRowCount(1);
	m_list.GetCell(0, 0)->SetText("Column");
	m_list.GetCell(0, 1)->SetText("Value");
	m_list.SetColumnWidth(0, 150);
	m_list.SetColumnWidth(1, 500);
	m_list.SetEditable();
	m_list.SetSingleRowSelection();
	m_list.SetSortColumn(0);
}

void
SQLDataDlg::InitTab(ValidateSQL* p_validate,Parameters* p_parameters)
{
  m_validate   = p_validate;
  m_parameters = p_parameters;

  FillCombos();
  LoadVariables();
  SetCombos();
}

void 
SQLDataDlg::FillCombos()
{
  m_comboOperator.ResetContent();
  m_comboVariable.ResetContent();

  m_comboOperator.AddString("");
  m_comboOperator.AddString("EXACT buffer contents");
  m_comboOperator.AddString("CONTAINS this text");
  m_comboOperator.AddString("BEGINSWITH this text");
  m_comboOperator.AddString("ENDSWITH this text");
  m_comboOperator.AddString("IS EMPTY");
  m_comboOperator.AddString("NOT EMPTY");
  m_comboOperator.AddString("NOTFOUND");
  m_comboOperator.AddString("FILE contents MATCH");
  m_comboOperator.AddString("FILE EXISTS");

  m_comboVariable.AddString("");
  for (auto& ret : m_parameters->GetReturns())
  {
    m_comboVariable.AddString(ret.first);
  }
}

void 
SQLDataDlg::SetCombos()
{
  m_comboOperator.SetCurSel((int)m_dataOperator);

  int ind = m_comboVariable.FindStringExact(0, m_returnedDataVariable);
  if (ind >= 0)
  {
    m_comboVariable.SetCurSel(ind);
  }
}

void
SQLDataDlg::LoadVariables()
{
  m_useData              = m_validate->GetCheckData();
  m_dataOperator         = m_validate->GetDataOperator();
  m_effectiveData        = m_validate->GetEffectiveData();
  m_returnedDataVariable = m_validate->GetReturnedDataVariable();

  m_list.DeleteNonFixedRows();
  ColumnData& data = m_validate->GetData();
  for(auto& dat : data)
  {
    int row = m_list.InsertRow(dat.first);
    m_list.GetCell(row,1)->SetText(dat.second);
  }
  m_list.Invalidate();

  m_checkData.SetCheck(m_useData);
  m_comboOperator.SetCurSel((int)m_dataOperator);

  UpdateData(FALSE);
}

void
SQLDataDlg::StoreVariables()
{
  m_validate->SetCheckData(m_useData);
  m_validate->SetDataOperator(m_dataOperator);
  m_validate->SetReturnedDataVariable(m_returnedDataVariable);

  for(int index = 1; index < m_list.GetRowCount(); ++index)
  {
    m_validate->SetData(m_list.GetCell(index,0)->GetText()
                       ,m_list.GetCell(index,1)->GetText());
  }

}

// SQLDataDlg message handlers

void
SQLDataDlg::OnLvnItemchangedGrid(NMHDR* pNMHDR, LRESULT* pResult)
{
  LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

  if(pNMLV->uNewState)
  {
    // EffectiveParameters();
    *pResult = 1;
  }
  UpdateData(FALSE);
}

void 
SQLDataDlg::OnBnClickedNew()
{
  int row = m_list.InsertRow("");
  m_list.GetCell(row, 1)->SetText("");
  m_list.SetFocusCell(row, 0);
  m_list.Invalidate();
}

void 
SQLDataDlg::OnBnClickedDelete()
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
        ask.Format("Do you want to delete the column [%s] ?", variable.GetString());
        if(StyleMessageBox(this,ask,KWATTA,MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION) == IDNO)
        {
          return;
        }
        // Delete from the teststep
        m_validate->DeleteData(variable);
        // Remove from the grid
        range.SetMinCol(0);
        range.SetMaxCol(1);
        m_list.SetSelectedRange(range);
        m_list.DeleteRow(row);
        m_list.Invalidate();

        // Re-calculate the completeness
        // EffectiveParameters();
      }
    }
  }
}

void 
SQLDataDlg::OnBnClickedParm()
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
        CString variable = dlg.GetVariable();
        CString txt = cell->GetText() + variable;
        cell->SetText(txt);
        m_list.Refresh();

        // EffectiveParameters();
      }
    }
  }
}

void
SQLDataDlg::OnBnClickedUseData()
{
  m_useData = m_checkData.GetCheck() > 0;

  if (!m_useData)
  {
    m_dataOperator = BufferOperator::BOP_NOP;
    m_comboOperator.SetCurSel(-1);
    m_effectiveData.Empty();
  }
  UpdateData(FALSE);
}

void
SQLDataDlg::OnCbnSelchangeOperator()
{
  int ind = m_comboOperator.GetCurSel();
  if (ind >= 0)
  {
    m_dataOperator = (BufferOperator)ind;
  }
}

// void
// SQLDataDlg::OnEnChangeData()
// {
//   UpdateData();
//   m_validate->SetData(m_data);
// 
//   ValidateDatabaseDlg* parent = dynamic_cast<ValidateDatabaseDlg*>(GetParent()->GetParent());
//   if (parent)
//   {
//     StoreVariables();
//     parent->EffectiveParameters();
//     LoadVariables();
//   }
// }

// void
// SQLDataDlg::OnBnClickedDataParm()
// {
//   SearchVarDlg dlg(this, m_parameters, true, true, true, true);
//   if (dlg.DoModal() == IDOK || dlg.GetSaved())
//   {
//     m_data += dlg.GetVariable();
//     UpdateData(FALSE);
//   }
// }

void
SQLDataDlg::OnEnChangeEffData()
{
  UpdateData();
}

void
SQLDataDlg::OnCbnSelchangeDataVariable()
{
  int ind = m_comboVariable.GetCurSel();
  if (ind >= 0)
  {
    CString var;
    m_comboVariable.GetLBText(ind, var);
    m_returnedDataVariable = var;
  }
}

void
SQLDataDlg::OnBnClickedNewvar()
{
  AutoFocus focus;
  NewVariableDlg dlg(ParType::PAR_RETURN, m_parameters->GetReturns());
  if (dlg.DoModal() == IDOK)
  {
    CString newvar = dlg.GetNewName();
    if (!newvar.IsEmpty())
    {
      m_returnedDataVariable = newvar;
      FillCombos();
      SetCombos();
    }
  }
}
