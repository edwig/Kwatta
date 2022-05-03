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
// This File   : SQLSQLStateDlg.cpp
// What it does: Validation of SQLSTATE error status of an SQL database statement
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ValidateEditor.h"
#include "ValidateDatabaseDlg.h"
#include "SQLSQLStateDlg.h"
#include <SearchVarDlg.h>
#include <NewVariableDlg.h>
#include <afxdialogex.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// SQLSQLStateDlg dialog

IMPLEMENT_DYNAMIC(SQLSQLStateDlg, StyleDialog)

SQLSQLStateDlg::SQLSQLStateDlg(CWnd* p_parent)
               :StyleDialog(IDD_SQL_SQLSTATE,p_parent)
{
}

SQLSQLStateDlg::~SQLSQLStateDlg()
{
}

void 
SQLSQLStateDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_USE_RETURN,  m_buttonCheck);
  DDX_Control(pDX, IDC_OPERATOR,    m_comboOperator);
  DDX_Control(pDX, IDC_RETURN,      m_editSQLState, m_SQLState);
  DDX_Control(pDX, IDC_BUTT_PARAM,  m_buttonParm);
  DDX_Control(pDX, IDC_EFF_RETURN,  m_editEffective,m_effectiveSQLState);
  DDX_Control(pDX, IDC_SUCCEED_VAR, m_comboVariable);
  DDX_Control(pDX, IDC_NEWVAR,      m_buttonVariable);

  if(pDX->m_bSaveAndValidate == false)
  {
    m_comboOperator .EnableWindow(m_useCheckState);
    m_editSQLState  .EnableWindow(m_useCheckState);
    m_editEffective .EnableWindow(m_useCheckState);
    m_comboVariable .EnableWindow(m_useCheckState);
    m_buttonVariable.EnableWindow(m_useCheckState);
    m_buttonParm    .EnableWindow(m_useCheckState);
  }
}

BEGIN_MESSAGE_MAP(SQLSQLStateDlg, StyleDialog)
  ON_BN_CLICKED   (IDC_USE_RETURN,  &SQLSQLStateDlg::OnBnClickedUseSQLState)
  ON_CBN_SELCHANGE(IDC_OPERATOR,    &SQLSQLStateDlg::OnCbnSelchangeOperator)
  ON_EN_KILLFOCUS (IDC_RETURN,      &SQLSQLStateDlg::OnEnChangeSQLState)
  ON_BN_CLICKED   (IDC_BUTT_PARAM,  &SQLSQLStateDlg::OnBnClickedParm)
  ON_EN_CHANGE    (IDC_EFF_RETURN,  &SQLSQLStateDlg::OnEnChangeEffSucceeded)
  ON_CBN_SELCHANGE(IDC_SUCCEED_VAR, &SQLSQLStateDlg::OnCbnSelchangeSQLStateVariable)
  ON_BN_CLICKED   (IDC_NEWVAR,      &SQLSQLStateDlg::OnBnClickedNewvar)
END_MESSAGE_MAP()

BOOL
SQLSQLStateDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();

  m_buttonVariable.SetIconImage(IDI_LIST);
  m_buttonParm .SetIconImage(IDI_RETURN);

  SetCanResize();
  return TRUE;
}

void
SQLSQLStateDlg::SetupDynamicLayout()
{
  StyleDialog::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif

  manager.AddItem(IDC_RETURN,    CMFCDynamicLayout::MoveNone(),          CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_EFF_RETURN,CMFCDynamicLayout::MoveNone(),          CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_BUTT_PARAM,CMFCDynamicLayout::MoveHorizontal(100), CMFCDynamicLayout::SizeNone());
}

void
SQLSQLStateDlg::InitTab(ValidateSQL* p_validate,Parameters* p_parameters)
{
  m_validate   = p_validate;
  m_parameters = p_parameters;

  LoadVariables();
  FillCombos();
  SetCombos();
}

void 
SQLSQLStateDlg::FillCombos()
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
  for(auto& ret : m_parameters->GetReturns())
  {
    m_comboVariable.AddString(ret.first);
  }
}

void 
SQLSQLStateDlg::SetCombos()
{
  m_comboOperator.SetCurSel((int)m_sqlStateOperator);

  int ind = m_comboVariable.FindStringExact(0, m_SQLStateVariable);
  if (ind >= 0)
  {
    m_comboVariable.SetCurSel(ind);
  }
}

void
SQLSQLStateDlg::LoadVariables()
{
  m_useCheckState     = m_validate->GetCheckSQLState();
  m_sqlStateOperator  = m_validate->GetStateOperator();
  m_SQLState          = m_validate->GetSQLState();
  m_effectiveSQLState = m_validate->GetEffectiveSQLStateState();
  m_SQLStateVariable  = m_validate->GetReturnedSQLStateVariable();

  m_buttonCheck.SetCheck(m_useCheckState);
  m_comboOperator.SetCurSel((int)m_sqlStateOperator - 1);

  UpdateData(FALSE);
}

void
SQLSQLStateDlg::StoreVariables()
{
  m_validate->SetCheckSQLState(m_useCheckState);
  m_validate->SetSQLState(m_SQLState);
  m_validate->SetStateOperator(m_sqlStateOperator);
  m_validate->SetReturnedSQLStateVariable(m_SQLStateVariable);
}

// SQLSQLStateDlg message handlers

void 
SQLSQLStateDlg::OnBnClickedUseSQLState()
{
  m_useCheckState= m_buttonCheck.GetCheck() > 0;

  if(!m_useCheckState)
  {
    m_sqlStateOperator = BufferOperator::BOP_NOP;
    m_comboOperator.SetCurSel(-1);
    m_effectiveSQLState.Empty();
  }
  UpdateData(FALSE);
}

void 
SQLSQLStateDlg::OnCbnSelchangeOperator()
{
  int ind = m_comboOperator.GetCurSel();
  if (ind >= 0)
  {
    m_sqlStateOperator = (BufferOperator)ind;
  }
}

void
SQLSQLStateDlg::OnEnChangeSQLState()
{
  UpdateData(); 
  m_validate->SetSucceeded(m_SQLState);

  ValidateDatabaseDlg* parent = dynamic_cast<ValidateDatabaseDlg*>(GetParent()->GetParent());
  if(parent)
  {
    StoreVariables();
    parent->EffectiveParameters();
    LoadVariables();
  }
}

void 
SQLSQLStateDlg::OnBnClickedParm()
{
  SearchVarDlg dlg(this,m_parameters,true,true,true,true);
  if (dlg.DoModal() == IDOK || dlg.GetSaved())
  {
    m_SQLState += dlg.GetVariable();
    UpdateData(FALSE);
  }
}

void 
SQLSQLStateDlg::OnEnChangeEffSucceeded()
{
  UpdateData();
}

void 
SQLSQLStateDlg::OnCbnSelchangeSQLStateVariable()
{
  int ind = m_comboVariable.GetCurSel();
  if(ind >= 0)
  {
    CString var;
    m_comboVariable.GetLBText(ind,var);
    m_SQLStateVariable = var;
  }
}

void 
SQLSQLStateDlg::OnBnClickedNewvar()
{
  NewVariableDlg dlg(ParType::PAR_RETURN, m_parameters->GetReturns());
  if(dlg.DoModal() == IDOK)
  {
    CString newvar = dlg.GetNewName();
    if(!newvar.IsEmpty())
    {
      m_SQLStateVariable = newvar;
      FillCombos();
      SetCombos();
    }
  }
}
