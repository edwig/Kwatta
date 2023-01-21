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
// This File   : SQLSucceededDlg.cpp
// What it does: Validation of the succeeded status of an SQL database statement
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ValidateEditor.h"
#include "ValidateDatabaseDlg.h"
#include "SQLSucceededDlg.h"
#include <SearchVarDlg.h>
#include <NewVariableDlg.h>
#include <afxdialogex.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// SQLSucceededDlg dialog

IMPLEMENT_DYNAMIC(SQLSucceededDlg,StyleTab)

SQLSucceededDlg::SQLSucceededDlg(CWnd* p_parent)
                :StyleTab(IDD_SQL_RETURN,p_parent)
{
}

SQLSucceededDlg::~SQLSucceededDlg()
{
}

void 
SQLSucceededDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleTab::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_USE_RETURN,  m_buttonCheck);
  DDX_Control(pDX, IDC_OPERATOR,    m_comboOperator);
  DDX_Control(pDX, IDC_RETURN,      m_editSucceeded, m_succeeded);
  DDX_Control(pDX, IDC_BUTT_PARAM,  m_buttonParm);
  DDX_Control(pDX, IDC_EFF_RETURN,  m_editEffective, m_effectiveSucceeded);
  DDX_Control(pDX, IDC_SUCCEED_VAR, m_comboVariable);
  DDX_Control(pDX, IDC_NEWVAR,      m_buttonVariable);

  if(pDX->m_bSaveAndValidate == false)
  {
    m_comboOperator .EnableWindow(m_useSucceeded);
    m_editSucceeded .EnableWindow(m_useSucceeded);
    m_editEffective .EnableWindow(m_useSucceeded);
    m_comboVariable .EnableWindow(m_useSucceeded);
    m_buttonVariable.EnableWindow(m_useSucceeded);
    m_buttonParm    .EnableWindow(m_useSucceeded);
  }
}

BEGIN_MESSAGE_MAP(SQLSucceededDlg, StyleTab)
  ON_BN_CLICKED   (IDC_USE_RETURN,  &SQLSucceededDlg::OnBnClickedUseSucceeded)
  ON_CBN_SELCHANGE(IDC_OPERATOR,    &SQLSucceededDlg::OnCbnSelchangeOperator)
  ON_EN_KILLFOCUS (IDC_RETURN,      &SQLSucceededDlg::OnEnChangeSucceeded)
  ON_BN_CLICKED   (IDC_BUTT_PARAM,  &SQLSucceededDlg::OnBnClickedParm)
  ON_EN_CHANGE    (IDC_EFF_RETURN,  &SQLSucceededDlg::OnEnChangeEffSucceeded)
  ON_CBN_SELCHANGE(IDC_SUCCEED_VAR, &SQLSucceededDlg::OnCbnSelchangeSucceededVariable)
  ON_BN_CLICKED   (IDC_NEWVAR,      &SQLSucceededDlg::OnBnClickedNewvar)
END_MESSAGE_MAP()

BOOL
SQLSucceededDlg::OnInitDialog()
{
  StyleTab::OnInitDialog();

  m_buttonVariable.SetIconImage(IDI_LIST);
  m_buttonParm .SetIconImage(IDI_RETURN);

  SetCanResize();
  return TRUE;
}

void
SQLSucceededDlg::SetupDynamicLayout()
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
SQLSucceededDlg::InitTab(ValidateSQL* p_validate,Parameters* p_parameters)
{
  m_validate   = p_validate;
  m_parameters = p_parameters;

  LoadVariables();
  FillCombos();
  SetCombos();
}

void 
SQLSucceededDlg::FillCombos()
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
SQLSucceededDlg::SetCombos()
{
  m_comboOperator.SetCurSel((int)m_succeededOperator);

  int ind = m_comboVariable.FindStringExact(0, m_succeededVariable);
  if (ind >= 0)
  {
    m_comboVariable.SetCurSel(ind);
  }
}

void
SQLSucceededDlg::LoadVariables()
{
  m_useSucceeded       = m_validate->GetCheckSucceeded();
  m_succeededOperator  = m_validate->GetSucceededOperator();
  m_succeeded          = m_validate->GetSucceeded();
  m_effectiveSucceeded = m_validate->GetEffectiveSucceded();
  m_succeededVariable  = m_validate->GetSucceededVariable();

  m_buttonCheck.SetCheck(m_useSucceeded);
  m_comboOperator.SetCurSel((int)m_succeededOperator - 1);

  UpdateData(FALSE);
}

void
SQLSucceededDlg::StoreVariables()
{
  m_validate->SetCheckSucceeded(m_useSucceeded);
  m_validate->SetSucceeded(m_succeeded);
  m_validate->SetSucceededOperator(m_succeededOperator);
  m_validate->SetSucceededVariable(m_succeededVariable);
}

// SQLSucceededDlg message handlers

void 
SQLSucceededDlg::OnBnClickedUseSucceeded()
{
  m_useSucceeded= m_buttonCheck.GetCheck() > 0;

  if(!m_useSucceeded)
  {
    m_succeededOperator = ReturnOperator::ROP_NOP;
    m_comboOperator.SetCurSel(-1);
    m_effectiveSucceeded.Empty();
  }
  UpdateData(FALSE);
}

void 
SQLSucceededDlg::OnCbnSelchangeOperator()
{
  int ind = m_comboOperator.GetCurSel();
  if (ind >= 0)
  {
    m_succeededOperator = (ReturnOperator)ind;
  }
}

void
SQLSucceededDlg::OnEnChangeSucceeded()
{
  UpdateData(); 
  m_validate->SetSucceeded(m_succeeded);

  ValidateDatabaseDlg* parent = dynamic_cast<ValidateDatabaseDlg*>(GetParent()->GetParent());
  if(parent)
  {
    StoreVariables();
    parent->EffectiveParameters();
    LoadVariables();
  }
}

void 
SQLSucceededDlg::OnBnClickedParm()
{
  SearchVarDlg dlg(this,m_parameters,true,true,true,true);
  if (dlg.DoModal() == IDOK || dlg.GetSaved())
  {
    m_succeeded += dlg.GetVariable();
    UpdateData(FALSE);
  }
}

void 
SQLSucceededDlg::OnEnChangeEffSucceeded()
{
  UpdateData();
}

void 
SQLSucceededDlg::OnCbnSelchangeSucceededVariable()
{
  int ind = m_comboVariable.GetCurSel();
  if(ind >= 0)
  {
    CString var;
    m_comboVariable.GetLBText(ind,var);
    m_succeededVariable = var;
  }
}

void 
SQLSucceededDlg::OnBnClickedNewvar()
{
  NewVariableDlg dlg(ParType::PAR_RETURN, m_parameters->GetReturns());
  if(dlg.DoModal() == IDOK)
  {
    CString newvar = dlg.GetNewName();
    if(!newvar.IsEmpty())
    {
      m_succeededVariable = newvar;
      FillCombos();
      SetCombos();
    }
  }
}
