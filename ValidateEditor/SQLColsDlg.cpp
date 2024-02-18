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
// This File   : SQLColsDlg.cpp
// What it does: Validation of the returned columns of an SQL database statement
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ValidateEditor.h"
#include "ValidateDatabaseDlg.h"
#include "SQLColsDlg.h"
#include <SearchVarDlg.h>
#include <NewVariableDlg.h>
#include <afxdialogex.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// SQLColsDlg dialog

IMPLEMENT_DYNAMIC(SQLColsDlg,StyleTab)

SQLColsDlg::SQLColsDlg(CWnd* p_parent)
           :StyleTab(IDD_SQL_COLS,p_parent)
{
}

SQLColsDlg::~SQLColsDlg()
{
}

void 
SQLColsDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleTab::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_USE_RETURN,  m_buttonCheck);
  DDX_Control(pDX, IDC_OPERATOR,    m_comboOperator);
  DDX_Control(pDX, IDC_RETURN,      m_editCols, m_returnedCols);
  DDX_Control(pDX, IDC_BUTT_PARAM,  m_buttonParm);
  DDX_Control(pDX, IDC_EFF_RETURN,  m_editEffective,m_effectiveReturnedCols);
  DDX_Control(pDX, IDC_SUCCEED_VAR, m_comboVariable);
  DDX_Control(pDX, IDC_NEWVAR,      m_buttonVariable);

  if(pDX->m_bSaveAndValidate == false)
  {
    m_comboOperator .EnableWindow(m_useCheckCols);
    m_editCols      .EnableWindow(m_useCheckCols);
    m_editEffective .EnableWindow(m_useCheckCols);
    m_comboVariable .EnableWindow(m_useCheckCols);
    m_buttonVariable.EnableWindow(m_useCheckCols);
    m_buttonParm    .EnableWindow(m_useCheckCols);
  }
}

BEGIN_MESSAGE_MAP(SQLColsDlg, StyleTab)
  ON_BN_CLICKED   (IDC_USE_RETURN,  &SQLColsDlg::OnBnClickedUseReturnedCols)
  ON_CBN_SELCHANGE(IDC_OPERATOR,    &SQLColsDlg::OnCbnSelchangeOperator)
  ON_EN_KILLFOCUS (IDC_RETURN,      &SQLColsDlg::OnEnChangeReturnedCols)
  ON_BN_CLICKED   (IDC_BUTT_PARAM,  &SQLColsDlg::OnBnClickedParm)
  ON_EN_CHANGE    (IDC_EFF_RETURN,  &SQLColsDlg::OnEnChangeEffSucceeded)
  ON_CBN_SELCHANGE(IDC_SUCCEED_VAR, &SQLColsDlg::OnCbnSelchangeReturnedColsVariable)
  ON_BN_CLICKED   (IDC_NEWVAR,      &SQLColsDlg::OnBnClickedNewvar)
END_MESSAGE_MAP()

BOOL
SQLColsDlg::OnInitDialog()
{
  StyleTab::OnInitDialog();

  m_buttonVariable.SetIconImage(IDI_LIST);
  m_buttonParm .SetIconImage(IDI_RETURN);

  SetCanResize();
  return TRUE;
}

void
SQLColsDlg::SetupDynamicLayout()
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
SQLColsDlg::InitTab(ValidateSQL* p_validate,Parameters* p_parameters)
{
  m_validate   = p_validate;
  m_parameters = p_parameters;

  LoadVariables();
  FillCombos();
  SetCombos();
}

void 
SQLColsDlg::FillCombos()
{
  m_comboOperator.ResetContent();
  m_comboVariable.ResetContent();

  m_comboOperator.AddString(_T(""));
  m_comboOperator.AddString(_T("=    Exactly equal to"));
  m_comboOperator.AddString(_T(">    Greater than"));
  m_comboOperator.AddString(_T(">= Greater than or equal to"));
  m_comboOperator.AddString(_T("<    Smaller than"));
  m_comboOperator.AddString(_T("<= Smaller than or equal to"));
  m_comboOperator.AddString(_T("<> Not equal to"));
  m_comboOperator.AddString(_T("~~ Between two values"));
  m_comboOperator.AddString(_T("[  ] In a range of values"));

  m_comboVariable.AddString(_T(""));
  for(auto& ret : m_parameters->GetReturns())
  {
    m_comboVariable.AddString(ret.first);
  }
}

void 
SQLColsDlg::SetCombos()
{
  m_comboOperator.SetCurSel((int)m_colsOperator);

  int ind = m_comboVariable.FindStringExact(0, m_returnedColsVariable);
  if (ind >= 0)
  {
    m_comboVariable.SetCurSel(ind);
  }
}

void
SQLColsDlg::LoadVariables()
{
  m_useCheckCols          = m_validate->GetCheckCols();
  m_colsOperator          = m_validate->GetColsOperator();
  m_returnedCols          = m_validate->GetReturnedCols();
  m_effectiveReturnedCols = m_validate->GetEffectiveReturnedCols();
  m_returnedColsVariable  = m_validate->GetReturnedColsVariable();

  m_buttonCheck.SetCheck(m_useCheckCols);
  m_comboOperator.SetCurSel((int)m_colsOperator - 1);

  UpdateData(FALSE);
}

void
SQLColsDlg::StoreVariables()
{
  m_validate->SetCheckCols(m_useCheckCols);
  m_validate->SetReturnedCols(m_returnedCols);
  m_validate->SetColsOperator(m_colsOperator);
  m_validate->SetReturnedColsVariable(m_returnedColsVariable);
}

// SQLColsDlg message handlers

void 
SQLColsDlg::OnBnClickedUseReturnedCols()
{
  m_useCheckCols= m_buttonCheck.GetCheck() > 0;

  if(!m_useCheckCols)
  {
    m_colsOperator = ReturnOperator::ROP_NOP;
    m_comboOperator.SetCurSel(-1);
    m_effectiveReturnedCols.Empty();
  }
  UpdateData(FALSE);
}

void 
SQLColsDlg::OnCbnSelchangeOperator()
{
  int ind = m_comboOperator.GetCurSel();
  if (ind >= 0)
  {
    m_colsOperator = (ReturnOperator)ind;
  }
}

void
SQLColsDlg::OnEnChangeReturnedCols()
{
  UpdateData(); 
  m_validate->SetSucceeded(m_returnedCols);

  ValidateDatabaseDlg* parent = dynamic_cast<ValidateDatabaseDlg*>(GetParent()->GetParent());
  if(parent)
  {
    StoreVariables();
    parent->EffectiveParameters();
    LoadVariables();
  }
}

void 
SQLColsDlg::OnBnClickedParm()
{
  AutoFocus focus;
  SearchVarDlg dlg(this,m_parameters,true,true,true,true);
  if (dlg.DoModal() == IDOK || dlg.GetSaved())
  {
    m_returnedCols += dlg.GetVariable();
    UpdateData(FALSE);
  }
}

void 
SQLColsDlg::OnEnChangeEffSucceeded()
{
  UpdateData();
}

void 
SQLColsDlg::OnCbnSelchangeReturnedColsVariable()
{
  int ind = m_comboVariable.GetCurSel();
  if(ind >= 0)
  {
    CString var;
    m_comboVariable.GetLBText(ind,var);
    m_returnedColsVariable = var;
  }
}

void 
SQLColsDlg::OnBnClickedNewvar()
{
  AutoFocus focus;
  NewVariableDlg dlg(ParType::PAR_RETURN, m_parameters->GetReturns());
  if(dlg.DoModal() == IDOK)
  {
    CString newvar = dlg.GetNewName();
    if(!newvar.IsEmpty())
    {
      m_returnedColsVariable = newvar;
      FillCombos();
      SetCombos();
    }
  }
}
