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
// This File   : SQLNativeDlg.cpp
// What it does: Validation of native error status of an SQL database statement
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ValidateEditor.h"
#include "ValidateDatabaseDlg.h"
#include "SQLNativeDlg.h"
#include <SearchVarDlg.h>
#include <NewVariableDlg.h>
#include <afxdialogex.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// SQLNativeDlg dialog

IMPLEMENT_DYNAMIC(SQLNativeDlg, StyleDialog)

SQLNativeDlg::SQLNativeDlg(CWnd* p_parent)
               :StyleDialog(IDD_SQL_NATIVE,p_parent)
{
}

SQLNativeDlg::~SQLNativeDlg()
{
}

void 
SQLNativeDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_USE_RETURN,  m_buttonCheck);
  DDX_Control(pDX, IDC_OPERATOR,    m_comboOperator);
  DDX_Control(pDX, IDC_RETURN,      m_editNative, m_nativeError);
  DDX_Control(pDX, IDC_BUTT_PARAM,  m_buttonParm);
  DDX_Control(pDX, IDC_EFF_RETURN,  m_editEffective,m_effectiveNative);
  DDX_Control(pDX, IDC_SUCCEED_VAR, m_comboVariable);
  DDX_Control(pDX, IDC_NEWVAR,      m_buttonVariable);

  if(pDX->m_bSaveAndValidate == false)
  {
    m_comboOperator .EnableWindow(m_useCheckNative);
    m_editNative    .EnableWindow(m_useCheckNative);
    m_editEffective .EnableWindow(m_useCheckNative);
    m_comboVariable .EnableWindow(m_useCheckNative);
    m_buttonVariable.EnableWindow(m_useCheckNative);
    m_buttonParm    .EnableWindow(m_useCheckNative);
  }
}

BEGIN_MESSAGE_MAP(SQLNativeDlg, StyleDialog)
  ON_BN_CLICKED   (IDC_USE_RETURN,  &SQLNativeDlg::OnBnClickedUseNative)
  ON_CBN_SELCHANGE(IDC_OPERATOR,    &SQLNativeDlg::OnCbnSelchangeOperator)
  ON_EN_KILLFOCUS (IDC_RETURN,      &SQLNativeDlg::OnEnChangeNative)
  ON_BN_CLICKED   (IDC_BUTT_PARAM,  &SQLNativeDlg::OnBnClickedParm)
  ON_EN_CHANGE    (IDC_EFF_RETURN,  &SQLNativeDlg::OnEnChangeEffSucceeded)
  ON_CBN_SELCHANGE(IDC_SUCCEED_VAR, &SQLNativeDlg::OnCbnSelchangeNativeVariable)
  ON_BN_CLICKED   (IDC_NEWVAR,      &SQLNativeDlg::OnBnClickedNewvar)
END_MESSAGE_MAP()

BOOL
SQLNativeDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();

  m_buttonVariable.SetIconImage(IDI_LIST);
  m_buttonParm .SetIconImage(IDI_RETURN);

  SetCanResize();
  return TRUE;
}

void
SQLNativeDlg::SetupDynamicLayout()
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
SQLNativeDlg::InitTab(ValidateSQL* p_validate,Parameters* p_parameters)
{
  m_validate   = p_validate;
  m_parameters = p_parameters;

  LoadVariables();
  FillCombos();
  SetCombos();
}

void 
SQLNativeDlg::FillCombos()
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
SQLNativeDlg::SetCombos()
{
  m_comboOperator.SetCurSel((int)m_nativeOperator);

  int ind = m_comboVariable.FindStringExact(0, m_nativeErrorVariable);
  if (ind >= 0)
  {
    m_comboVariable.SetCurSel(ind);
  }
}

void
SQLNativeDlg::LoadVariables()
{
  m_useCheckNative       = m_validate->GetCheckNativeStatus();
  m_nativeOperator       = m_validate->GetNativeOperator();
  m_nativeError          = m_validate->GetNativeStatus();
  m_effectiveNative      = m_validate->GetEffectiveNativeStatus();
  m_nativeErrorVariable  = m_validate->GetReturnedNativeVariable();

  m_buttonCheck  .SetCheck(m_useCheckNative);
  m_comboOperator.SetCurSel((int)m_nativeOperator - 1);

  UpdateData(FALSE);
}

void
SQLNativeDlg::StoreVariables()
{
  m_validate->SetCheckNativeStatus(m_useCheckNative);
  m_validate->SetNativeStatus(m_nativeError);
  m_validate->SetNativeOperator(m_nativeOperator);
  m_validate->SetReturnedNativeVariable(m_nativeErrorVariable);
}

// SQLNativeDlg message handlers

void 
SQLNativeDlg::OnBnClickedUseNative()
{
  m_useCheckNative= m_buttonCheck.GetCheck() > 0;

  if(!m_useCheckNative)
  {
    m_nativeOperator = BufferOperator::BOP_NOP;
    m_comboOperator.SetCurSel(-1);
    m_effectiveNative.Empty();
  }
  UpdateData(FALSE);
}

void 
SQLNativeDlg::OnCbnSelchangeOperator()
{
  int ind = m_comboOperator.GetCurSel();
  if (ind >= 0)
  {
    m_nativeOperator = (BufferOperator)ind;
  }
}

void
SQLNativeDlg::OnEnChangeNative()
{
  UpdateData(); 
  m_validate->SetSucceeded(m_nativeError);

  ValidateDatabaseDlg* parent = dynamic_cast<ValidateDatabaseDlg*>(GetParent()->GetParent());
  if(parent)
  {
    StoreVariables();
    parent->EffectiveParameters();
    LoadVariables();
  }
}

void 
SQLNativeDlg::OnBnClickedParm()
{
  SearchVarDlg dlg(this,m_parameters,true,true,true,true);
  if (dlg.DoModal() == IDOK || dlg.GetSaved())
  {
    m_nativeError += dlg.GetVariable();
    UpdateData(FALSE);
  }
}

void 
SQLNativeDlg::OnEnChangeEffSucceeded()
{
  UpdateData();
}

void 
SQLNativeDlg::OnCbnSelchangeNativeVariable()
{
  int ind = m_comboVariable.GetCurSel();
  if(ind >= 0)
  {
    CString var;
    m_comboVariable.GetLBText(ind,var);
    m_nativeErrorVariable = var;
  }
}

void 
SQLNativeDlg::OnBnClickedNewvar()
{
  NewVariableDlg dlg(ParType::PAR_RETURN, m_parameters->GetReturns());
  if(dlg.DoModal() == IDOK)
  {
    CString newvar = dlg.GetNewName();
    if(!newvar.IsEmpty())
    {
      m_nativeErrorVariable = newvar;
      FillCombos();
      SetCombos();
    }
  }
}
