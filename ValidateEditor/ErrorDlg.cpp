﻿///////////////////////////////////////////////////////////////////////////////////////////////////////
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
// This File   : ErrorDlg.cpp
// What it does: Defining the validation of the standard-error channel of a command-line 
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ValidateEditor.h"
#include "ValidateEditorDlg.h"
#include "ErrorDlg.h"
#include "SearchVarDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// ErrorDlg dialog

IMPLEMENT_DYNAMIC(ErrorDlg,StyleTab)

ErrorDlg::ErrorDlg(CWnd* pParent /*=nullptr*/)
         :StyleTab(IDD_OUTPUT, pParent)
{
}

ErrorDlg::~ErrorDlg()
{
}

void
ErrorDlg::DoDataExchange(CDataExchange* pDX)
{
  StyleTab::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_USE_ERROR, m_buttonUseError);
  DDX_Control(pDX, IDC_OPERATOR,  m_comboOperator);
  DDX_Control(pDX, IDC_SEARCHVAR, m_buttonSearchvar);
  DDX_Control(pDX, IDC_ERROR,     m_editExpected, m_expectedError);
  DDX_Control(pDX, IDC_EFFECTIVE, m_editEffective,m_effectiveError);

  if (pDX->m_bSaveAndValidate == false)
  {
    m_comboOperator  .EnableWindow(m_useError);
    m_buttonSearchvar.EnableWindow(m_useError);
    m_editExpected   .EnableWindow(m_useError);
    m_editEffective  .EnableWindow(m_useError);
  }
}

BEGIN_MESSAGE_MAP(ErrorDlg, StyleTab)
  ON_BN_CLICKED   (IDC_USE_ERROR, &ErrorDlg::OnBnClickedUseError)
  ON_CBN_SELCHANGE(IDC_OPERATOR,  &ErrorDlg::OnCbnSelchangeOperator)
  ON_BN_CLICKED   (IDC_SEARCHVAR, &ErrorDlg::OnBnClickedSearchvar)
  ON_EN_CHANGE    (IDC_ERROR,     &ErrorDlg::OnEnChangeError)
  ON_EN_KILLFOCUS (IDC_ERROR,     &ErrorDlg::OnEnKillfocusError)
  ON_EN_CHANGE    (IDC_EFFECTIVE, &ErrorDlg::OnEnChangeEffective)
END_MESSAGE_MAP()

BOOL
ErrorDlg::OnInitDialog()
{
  StyleTab::OnInitDialog();

  m_buttonSearchvar.SetIconImage(IDI_LIST);

  m_comboOperator.AddString(_T("EXACT buffer contents"));
  m_comboOperator.AddString(_T("CONTAINS this text"));
  m_comboOperator.AddString(_T("BEGINSWITH this text"));
  m_comboOperator.AddString(_T("ENDSWITH this text"));
  m_comboOperator.AddString(_T("IS EMPTY"));
  m_comboOperator.AddString(_T("NOT EMPTY"));
  m_comboOperator.AddString(_T("NOTFOUND"));
  m_comboOperator.AddString(_T("FILE contents MATCH"));
  m_comboOperator.AddString(_T("FILE EXISTS"));

  SetCanResize();
  return TRUE;
}

void
ErrorDlg::SetupDynamicLayout()
{
  StyleTab::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif
  manager.AddItem(IDC_SEARCHVAR, CMFCDynamicLayout::MoveHorizontal(100),CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_ERROR,     CMFCDynamicLayout::MoveNone(),         CMFCDynamicLayout::SizeHorizontalAndVertical(100, 50));
  manager.AddItem(IDC_ST_EFF,    CMFCDynamicLayout::MoveVertical(50),   CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_EFFECTIVE, CMFCDynamicLayout::MoveVertical(50),   CMFCDynamicLayout::SizeHorizontalAndVertical(100, 50));
}

void
ErrorDlg::InitTab(ValidateCMD* p_validate, Parameters* p_param)
{
  m_validate = p_validate;
  m_parameters = p_param;

  LoadVariables();
}

void
ErrorDlg::LoadVariables()
{
  m_useError       = m_validate->GetCheckErrorValue();
  m_errorOperator  = m_validate->GetErrorOperator();
  m_expectedError  = m_validate->GetExpectedError();
  m_effectiveError = m_validate->GetEffectiveError();

  m_buttonUseError.SetCheck(m_useError);
  m_comboOperator.SetCurSel((int)m_errorOperator - 1);

  UpdateData(FALSE);
}

void
ErrorDlg::StoreVariables()
{
  m_validate->SetCheckErrorValue(m_useError);
  m_validate->SetErrorOperator(m_errorOperator);
  m_validate->SetExpectedError(m_expectedError);
}

// ErrorDlg message handlers

void
ErrorDlg::OnBnClickedUseError()
{
  m_useError = m_buttonUseError.GetCheck() > 0;

  if(!m_useError)
  {
    m_errorOperator = BufferOperator::BOP_NOP;
    m_comboOperator.SetCurSel(-1);
    m_expectedError.Empty();
    m_effectiveError.Empty();
  }
  UpdateData(FALSE);
}

void
ErrorDlg::OnCbnSelchangeOperator()
{
  int ind = m_comboOperator.GetCurSel();
  if (ind >= 0)
  {
    m_errorOperator = (BufferOperator)(ind + 1);
  }
}

void
ErrorDlg::OnBnClickedSearchvar()
{
  AutoFocus focus;
  SearchVarDlg dlg(this,m_parameters,true,true,true,false);
  if (dlg.DoModal() == IDOK || dlg.GetSaved())
  {
    m_expectedError += dlg.GetVariable();
    UpdateData(FALSE);

    OnEnKillfocusError();
  }
}

void
ErrorDlg::OnEnChangeError()
{
  UpdateData();
}

void
ErrorDlg::OnEnKillfocusError()
{
  UpdateData();
  m_validate->SetExpectedError(m_expectedError);

  ValidateEditorDlg* parent = (ValidateEditorDlg*)GetParent()->GetParent();
  StoreVariables();
  parent->EffectiveParameters();
  LoadVariables();
}

void
ErrorDlg::OnEnChangeEffective()
{
  UpdateData();
}
