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
// This File   : SQLRowsDlg.cpp
// What it does: Validation of the returned rows of an SQL database statement
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ValidateEditor.h"
#include "ValidateDatabaseDlg.h"
#include "SQLRowsDlg.h"
#include <SearchVarDlg.h>
#include <NewVariableDlg.h>
#include <afxdialogex.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// SQLRowsDlg dialog

IMPLEMENT_DYNAMIC(SQLRowsDlg,StyleTab)

SQLRowsDlg::SQLRowsDlg(CWnd* p_parent)
           :StyleTab(IDD_SQL_ROWS,p_parent)
{
}

SQLRowsDlg::~SQLRowsDlg()
{
}

void 
SQLRowsDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleTab::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_USE_RETURN,  m_buttonCheck);
  DDX_Control(pDX, IDC_OPERATOR,    m_comboOperator);
  DDX_Control(pDX, IDC_RETURN,      m_editRows, m_returnedRows);
  DDX_Control(pDX, IDC_BUTT_PARAM,  m_buttonParm);
  DDX_Control(pDX, IDC_EFF_RETURN,  m_editEffective,m_effectiveReturnedRows);
  DDX_Control(pDX, IDC_SUCCEED_VAR, m_comboVariable);
  DDX_Control(pDX, IDC_NEWVAR,      m_buttonVariable);

  if(pDX->m_bSaveAndValidate == false)
  {
    m_comboOperator .EnableWindow(m_useCheckRows);
    m_editRows      .EnableWindow(m_useCheckRows);
    m_editEffective .EnableWindow(m_useCheckRows);
    m_comboVariable .EnableWindow(m_useCheckRows);
    m_buttonVariable.EnableWindow(m_useCheckRows);
    m_buttonParm    .EnableWindow(m_useCheckRows);
  }
}

BEGIN_MESSAGE_MAP(SQLRowsDlg, StyleTab)
  ON_BN_CLICKED   (IDC_USE_RETURN,  &SQLRowsDlg::OnBnClickedUseReturnedRows)
  ON_CBN_SELCHANGE(IDC_OPERATOR,    &SQLRowsDlg::OnCbnSelchangeOperator)
  ON_EN_KILLFOCUS (IDC_RETURN,      &SQLRowsDlg::OnEnChangeReturnedRows)
  ON_BN_CLICKED   (IDC_BUTT_PARAM,  &SQLRowsDlg::OnBnClickedParm)
  ON_EN_CHANGE    (IDC_EFF_RETURN,  &SQLRowsDlg::OnEnChangeEffSucceeded)
  ON_CBN_SELCHANGE(IDC_SUCCEED_VAR, &SQLRowsDlg::OnCbnSelchangeReturnedRowsVariable)
  ON_BN_CLICKED   (IDC_NEWVAR,      &SQLRowsDlg::OnBnClickedNewvar)
END_MESSAGE_MAP()

BOOL
SQLRowsDlg::OnInitDialog()
{
  StyleTab::OnInitDialog();

  m_buttonVariable.SetIconImage(IDI_LIST);
  m_buttonParm .SetIconImage(IDI_RETURN);

  SetCanResize();
  return TRUE;
}

void
SQLRowsDlg::SetupDynamicLayout()
{
  StyleTab::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif

  manager.AddItem(IDC_RETURN,    CMFCDynamicLayout::MoveNone(),          CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_EFF_RETURN,CMFCDynamicLayout::MoveNone(),          CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_BUTT_PARAM,CMFCDynamicLayout::MoveHorizontal(100), CMFCDynamicLayout::SizeNone());
}

void
SQLRowsDlg::InitTab(ValidateSQL* p_validate,Parameters* p_parameters)
{
  m_validate   = p_validate;
  m_parameters = p_parameters;

  LoadVariables();
  FillCombos();
  SetCombos();
}

void 
SQLRowsDlg::FillCombos()
{
  m_comboOperator.ResetContent();
  m_comboVariable.ResetContent();

  m_comboOperator.AddString("");
  m_comboOperator.AddString("=    Exactly equal to");
  m_comboOperator.AddString(">    Greater than");
  m_comboOperator.AddString(">= Greater than or equal to");
  m_comboOperator.AddString("<    Smaller than");
  m_comboOperator.AddString("<= Smaller than or equal to");
  m_comboOperator.AddString("<> Not equal to");
  m_comboOperator.AddString("~~ Between two values");
  m_comboOperator.AddString("[  ] In a range of values");

  m_comboVariable.AddString("");
  for(auto& ret : m_parameters->GetReturns())
  {
    m_comboVariable.AddString(ret.first);
  }
}

void 
SQLRowsDlg::SetCombos()
{
  m_comboOperator.SetCurSel((int)m_rowsOperator);

  int ind = m_comboVariable.FindStringExact(0, m_returnedRowsVariable);
  if (ind >= 0)
  {
    m_comboVariable.SetCurSel(ind);
  }
}

void
SQLRowsDlg::LoadVariables()
{
  m_useCheckRows          = m_validate->GetCheckRows();
  m_rowsOperator          = m_validate->GetRowsOperator();
  m_returnedRows          = m_validate->GetReturnedRows();
  m_effectiveReturnedRows = m_validate->GetEffectiveReturnedRows();
  m_returnedRowsVariable  = m_validate->GetReturnedRowsVariable();

  m_buttonCheck.SetCheck(m_useCheckRows);
  m_comboOperator.SetCurSel((int)m_rowsOperator - 1);

  UpdateData(FALSE);
}

void
SQLRowsDlg::StoreVariables()
{
  m_validate->SetCheckRows(m_useCheckRows);
  m_validate->SetReturnedRows(m_returnedRows);
  m_validate->SetRowsOperator(m_rowsOperator);
  m_validate->SetReturnedRowsVariable(m_returnedRowsVariable);
}

// SQLRowsDlg message handlers

void 
SQLRowsDlg::OnBnClickedUseReturnedRows()
{
  m_useCheckRows= m_buttonCheck.GetCheck() > 0;

  if(!m_useCheckRows)
  {
    m_rowsOperator = ReturnOperator::ROP_NOP;
    m_comboOperator.SetCurSel(-1);
    m_effectiveReturnedRows.Empty();
  }
  UpdateData(FALSE);
}

void 
SQLRowsDlg::OnCbnSelchangeOperator()
{
  int ind = m_comboOperator.GetCurSel();
  if (ind >= 0)
  {
    m_rowsOperator = (ReturnOperator)ind;
  }
}

void
SQLRowsDlg::OnEnChangeReturnedRows()
{
  UpdateData(); 
  m_validate->SetSucceeded(m_returnedRows);

  ValidateDatabaseDlg* parent = dynamic_cast<ValidateDatabaseDlg*>(GetParent()->GetParent());
  if(parent)
  {
    StoreVariables();
    parent->EffectiveParameters();
    LoadVariables();
  }
}

void 
SQLRowsDlg::OnBnClickedParm()
{
  SearchVarDlg dlg(this,m_parameters,true,true,true,true);
  if (dlg.DoModal() == IDOK || dlg.GetSaved())
  {
    m_returnedRows += dlg.GetVariable();
    UpdateData(FALSE);
  }
}

void 
SQLRowsDlg::OnEnChangeEffSucceeded()
{
  UpdateData();
}

void 
SQLRowsDlg::OnCbnSelchangeReturnedRowsVariable()
{
  int ind = m_comboVariable.GetCurSel();
  if(ind >= 0)
  {
    CString var;
    m_comboVariable.GetLBText(ind,var);
    m_returnedRowsVariable = var;
  }
}

void 
SQLRowsDlg::OnBnClickedNewvar()
{
  NewVariableDlg dlg(ParType::PAR_RETURN, m_parameters->GetReturns());
  if(dlg.DoModal() == IDOK)
  {
    CString newvar = dlg.GetNewName();
    if(!newvar.IsEmpty())
    {
      m_returnedRowsVariable = newvar;
      FillCombos();
      SetCombos();
    }
  }
}
