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
// This File   : ReturnDlg.cpp
// What it does: Defining the validation of the return value of a command-line executable
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ValidateEditor.h"
#include "ValidateEditorDlg.h"
#include "ReturnDlg.h"
#include "SearchVarDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// ReturnDlg dialog

IMPLEMENT_DYNAMIC(ReturnDlg,StyleTab)

ReturnDlg::ReturnDlg(CWnd* pParent /*=nullptr*/)
          :StyleTab(IDD_RETURN, pParent)
{
}

ReturnDlg::~ReturnDlg()
{
}

void 
ReturnDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleTab::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_USE_RETURN, m_buttonCheck);
  DDX_Control(pDX, IDC_OPERATOR,   m_comboOperator);
  DDX_Control(pDX, IDC_RETURN,     m_editExpected, m_expectedReturn);
  DDX_Control(pDX, IDC_EFF_RETURN, m_editEffective,m_effectiveReturn);
  DDX_Control(pDX, IDC_NEWVAR,     m_buttonVariable);

  if(pDX->m_bSaveAndValidate == false)
  {
    m_comboOperator .EnableWindow(m_useReturnValue);
    m_editExpected  .EnableWindow(m_useReturnValue);
    m_editEffective .EnableWindow(m_useReturnValue);
    m_buttonVariable.EnableWindow(m_useReturnValue);
  }
}

BEGIN_MESSAGE_MAP(ReturnDlg, StyleTab)
  ON_BN_CLICKED   (IDC_USE_RETURN, &ReturnDlg::OnBnClickedUseReturn)
  ON_CBN_SELCHANGE(IDC_OPERATOR,   &ReturnDlg::OnCbnSelchangeOperator)
  ON_EN_CHANGE    (IDC_RETURN,     &ReturnDlg::OnEnChangeReturn)
  ON_EN_KILLFOCUS (IDC_RETURN,     &ReturnDlg::OnEnKillfocusReturn)
  ON_BN_CLICKED   (IDC_NEWVAR,     &ReturnDlg::OnBnClickedNewvar)
  ON_EN_CHANGE    (IDC_EFF_RETURN, &ReturnDlg::OnEnChangeEffReturn)
END_MESSAGE_MAP()

BOOL
ReturnDlg::OnInitDialog()
{
  StyleTab::OnInitDialog();

  m_buttonVariable.SetIconImage(IDI_LIST);

  m_comboOperator.AddString(_T("=    Exactly equal to"));
  m_comboOperator.AddString(_T(">    Greater than"));
  m_comboOperator.AddString(_T(">= Greater than or equal to"));
  m_comboOperator.AddString(_T("<    Smaller than"));
  m_comboOperator.AddString(_T("<= Smaller than or equal to"));
  m_comboOperator.AddString(_T("<> Not equal to"));
  m_comboOperator.AddString(_T("~~ Between two values"));
  m_comboOperator.AddString(_T("[  ] In a range of values"));

  SetCanResize();
  return TRUE;
}

void
ReturnDlg::SetupDynamicLayout()
{
  StyleTab::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif
  manager.AddItem(IDC_RETURN,     CMFCDynamicLayout::MoveNone(),          CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_EFF_RETURN, CMFCDynamicLayout::MoveNone(),          CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_NEWVAR,     CMFCDynamicLayout::MoveHorizontal(100), CMFCDynamicLayout::SizeNone());
}

void
ReturnDlg::InitTab(ValidateCMD* p_validate,Parameters* p_parameters)
{
  m_validate   = p_validate;
  m_parameters = p_parameters;

  LoadVariables();
}

void
ReturnDlg::LoadVariables()
{
  m_useReturnValue  = m_validate->GetCheckReturnValue();
  m_returnOperator  = m_validate->GetReturnOperator();
  m_expectedReturn  = m_validate->GetExpectedReturn();
  m_effectiveReturn = m_validate->GetEffectiveReturn();

  m_buttonCheck.SetCheck(m_useReturnValue);
  m_comboOperator.SetCurSel((int)m_returnOperator - 1);

  UpdateData(FALSE);
}

void
ReturnDlg::StoreVariables()
{
  m_validate->SetCheckReturnValue(m_useReturnValue);
  m_validate->SetReturnOperator(m_returnOperator);
  m_validate->SetExpectedReturn(m_expectedReturn);
}

// ReturnDlg message handlers

void 
ReturnDlg::OnBnClickedUseReturn()
{
  m_useReturnValue = m_buttonCheck.GetCheck() > 0;

  if(!m_useReturnValue)
  {
    m_returnOperator = ReturnOperator::ROP_NOP;
    m_comboOperator.SetCurSel(-1);
    m_expectedReturn.Empty();
    m_effectiveReturn.Empty();
  }

  UpdateData(FALSE);
}

void 
ReturnDlg::OnCbnSelchangeOperator()
{
  int ind = m_comboOperator.GetCurSel();
  if (ind >= 0)
  {
    m_returnOperator = (ReturnOperator)(ind + 1);
  }
}

void 
ReturnDlg::OnEnChangeReturn()
{
  UpdateData();
}

void
ReturnDlg::OnEnKillfocusReturn()
{
  UpdateData();
  m_validate->SetExpectedReturn(m_expectedReturn);

  ValidateEditorDlg* parent = (ValidateEditorDlg*)GetParent()->GetParent();
  StoreVariables();
  parent->EffectiveParameters();
  LoadVariables();
}

void 
ReturnDlg::OnBnClickedNewvar()
{
  AutoFocus focus;
  SearchVarDlg dlg(this,m_parameters,true,true,true,false);
  if (dlg.DoModal() == IDOK || dlg.GetSaved())
  {
    m_expectedReturn += dlg.GetVariable();
    UpdateData(FALSE);

    OnEnKillfocusReturn();
  }
}

void 
ReturnDlg::OnEnChangeEffReturn()
{
  UpdateData();
}


