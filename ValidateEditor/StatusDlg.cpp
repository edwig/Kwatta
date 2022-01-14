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
// This File   : StatusDlg.cpp
// What it does: Defining the validation of the status value of a HTTP response message
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ValidateEditor.h"
#include "ValidateInternetDlg.h"
#include "StatusDlg.h"
#include <SearchVarDlg.h>
#include <NewVariableDlg.h>
#include <afxdialogex.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// StatusDlg dialog

IMPLEMENT_DYNAMIC(StatusDlg, StyleDialog)

StatusDlg::StatusDlg(CWnd* pParent /*=nullptr*/)
          :StyleDialog(IDD_STATUS, pParent)
{
}

StatusDlg::~StatusDlg()
{
}

void 
StatusDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_USE_STATUS, m_buttonCheck);
  DDX_Control(pDX, IDC_OPERATOR,   m_comboOperator);
  DDX_Control(pDX, IDC_STATUS,     m_editExpected, m_expectedStatus);
  DDX_Control(pDX, IDC_EFF_STATUS, m_editEffective,m_effectiveStatus);
  DDX_Control(pDX, IDC_NEWVAR,     m_buttonVariable);
  DDX_Control(pDX, IDC_VAR_STATUS, m_comboStatusVar);
  DDX_Control(pDX, IDC_NEW_STATUS, m_buttonStatusVar);

  if(pDX->m_bSaveAndValidate == false)
  {
    m_comboOperator  .EnableWindow(m_useStatusValue);
    m_editExpected   .EnableWindow(m_useStatusValue);
    m_editEffective  .EnableWindow(m_useStatusValue);
    m_comboStatusVar .EnableWindow(m_useStatusValue);
    m_buttonVariable .EnableWindow(m_useStatusValue);
    m_buttonStatusVar.EnableWindow(m_useStatusValue);
  }
}

BEGIN_MESSAGE_MAP(StatusDlg, StyleDialog)
  ON_BN_CLICKED   (IDC_USE_STATUS, &StatusDlg::OnBnClickedUseStatus)
  ON_CBN_SELCHANGE(IDC_OPERATOR,   &StatusDlg::OnCbnSelchangeOperator)
  ON_EN_CHANGE    (IDC_STATUS,     &StatusDlg::OnEnChangeStatus)
  ON_EN_KILLFOCUS (IDC_STATUS,     &StatusDlg::OnEnKillfocusStatus)
  ON_BN_CLICKED   (IDC_NEWVAR,     &StatusDlg::OnBnClickedNewvar)
  ON_EN_CHANGE    (IDC_EFF_RETURN, &StatusDlg::OnEnChangeEffStatus)
  ON_CBN_SELCHANGE(IDC_VAR_STATUS, &StatusDlg::OnCbnSelchangeStatusVar)
  ON_BN_CLICKED   (IDC_NEW_STATUS, &StatusDlg::OnBnClickedStatusVar)
END_MESSAGE_MAP()

BOOL
StatusDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();

  m_buttonVariable.SetIconImage(IDI_LIST);
  m_buttonStatusVar.SetIconImage(IDI_RETURN);

  SetCanResize();
  return TRUE;
}

void
StatusDlg::SetupDynamicLayout()
{
  StyleDialog::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif
  manager.AddItem(IDC_STATUS,     CMFCDynamicLayout::MoveNone(),          CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_EFF_STATUS, CMFCDynamicLayout::MoveNone(),          CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_NEWVAR,     CMFCDynamicLayout::MoveHorizontal(100), CMFCDynamicLayout::SizeNone());
}

void
StatusDlg::InitTab(ValidateIN* p_validate,Parameters* p_parameters)
{
  m_validate   = p_validate;
  m_parameters = p_parameters;

  LoadVariables();
  FillCombos();
  SetCombos();
}

void
StatusDlg::LoadVariables()
{
  m_useStatusValue  = m_validate->GetCheckStatus();
  m_statusOperator  = m_validate->GetStatusOperator();
  m_expectedStatus  = m_validate->GetExpectedStatus();
  m_effectiveStatus = m_validate->GetEffectiveStatus();
  m_statusVariable  = m_validate->GetStatusVariable();

  m_buttonCheck.SetCheck(m_useStatusValue);
  m_comboOperator.SetCurSel((int)m_statusOperator - 1);

  UpdateData(FALSE);
}

void
StatusDlg::FillCombos()
{
  m_comboOperator .ResetContent();
  m_comboStatusVar.ResetContent();

  m_comboOperator.AddString("");
  m_comboOperator.AddString("=    Exactly equal to");
  m_comboOperator.AddString(">    Greater than");
  m_comboOperator.AddString(">= Greater than or equal to");
  m_comboOperator.AddString("<    Smaller than");
  m_comboOperator.AddString("<= Smaller than or equal to");
  m_comboOperator.AddString("<> Not equal to");
  m_comboOperator.AddString("~~ Between two values");
  m_comboOperator.AddString("[  ] In a range of values");

  m_comboStatusVar.AddString("");
  for(auto& ret : m_parameters->GetReturns())
  {
    m_comboStatusVar.AddString(ret.first);
  }
}

void
StatusDlg::SetCombos()
{
  m_comboOperator.SetCurSel((int)m_statusOperator);

  int ind = m_comboStatusVar.FindStringExact(0,m_statusVariable);
  if (ind >= 0)
  {
    m_comboStatusVar.SetCurSel(ind);
  }
}

void
StatusDlg::StoreVariables()
{
  m_validate->SetCheckStatus(m_useStatusValue);
  m_validate->SetStatusOperator(m_statusOperator);
  m_validate->SetExpectedStatus(m_expectedStatus);
  m_validate->SetStatusVariable(m_statusVariable);
}

// StatusDlg message handlers

void 
StatusDlg::OnBnClickedUseStatus()
{
  m_useStatusValue= m_buttonCheck.GetCheck() > 0;

  if(!m_useStatusValue)
  {
    m_statusOperator = ReturnOperator::ROP_NOP;
    m_comboOperator.SetCurSel(-1);
    m_expectedStatus.Empty();
    m_effectiveStatus.Empty();
    m_statusVariable.Empty();
    m_comboStatusVar.SetCurSel(-1);
  }

  UpdateData(FALSE);
}

void 
StatusDlg::OnCbnSelchangeOperator()
{
  int ind = m_comboOperator.GetCurSel();
  if (ind >= 0)
  {
    m_statusOperator = (ReturnOperator)ind;
  }
}

void 
StatusDlg::OnEnChangeStatus()
{
  UpdateData();
}

void
StatusDlg::OnEnKillfocusStatus()
{
  UpdateData();
  m_validate->SetExpectedStatus(m_expectedStatus);

  ValidateInternetDlg* parent = (ValidateInternetDlg*)GetParent()->GetParent();
  StoreVariables();
  parent->EffectiveParameters();
  LoadVariables();
}

void 
StatusDlg::OnBnClickedNewvar()
{
  SearchVarDlg dlg(m_parameters,true,true,false);
  if (dlg.DoModal() == IDOK || dlg.GetSaved())
  {
    CString var = dlg.GetChosenVariable();
    CString variable;
    switch (dlg.GetResultType())
    {
      case ParType::PAR_GLOBAL: variable.Format("$%s$", var); break;
      case ParType::PAR_RETURN: variable.Format("[%s]", var); break;
      case ParType::PAR_BUFFER: variable.Format("<%s>", var); break;
    }

    m_expectedStatus += variable;
    UpdateData(FALSE);

    OnEnKillfocusStatus();
  }
}

void 
StatusDlg::OnEnChangeEffStatus()
{
  UpdateData();
}

void 
StatusDlg::OnCbnSelchangeStatusVar()
{
  int ind = m_comboStatusVar.GetCurSel();
  if (ind >= 0)
  {
    CString var;
    m_comboStatusVar.GetLBText(ind, var);
    m_statusVariable = var;
  }
}

void 
StatusDlg::OnBnClickedStatusVar()
{
  NewVariableDlg dlg(ParType::PAR_RETURN, m_parameters->GetReturns());
  if(dlg.DoModal() == IDOK)
  {
    CString newvar = dlg.GetNewName();
    if(!newvar.IsEmpty())
    {
      m_statusVariable = newvar;
      FillCombos();
      SetCombos();
    }
  }
}
