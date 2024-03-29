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
// This File   : OutputDlg.cpp
// What it does: Defining the validation of the standard-output channel of a command-line executable
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ValidateEditor.h"
#include "ValidateEditorDlg.h"
#include "OutputDlg.h"
#include "SearchVarDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// OutputDlg dialog

IMPLEMENT_DYNAMIC(OutputDlg,StyleTab)

OutputDlg::OutputDlg(CWnd* pParent /*=nullptr*/)
          :StyleTab(IDD_OUTPUT, pParent)
{
}

OutputDlg::~OutputDlg()
{
}

void 
OutputDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleTab::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_USE_OUTPUT, m_buttonUseOutput);
  DDX_Control(pDX, IDC_OPERATOR,   m_comboOperator);
  DDX_Control(pDX, IDC_SEARCHVAR,  m_buttonSearchvar);
  DDX_Control(pDX, IDC_OUTPUT,     m_editExpected, m_expectedOutput);
  DDX_Control(pDX, IDC_EFFECTIVE,  m_editEffective,m_effectiveOutput);

  if(pDX->m_bSaveAndValidate == false)
  {
    m_comboOperator  .EnableWindow(m_useOutput);
    m_buttonSearchvar.EnableWindow(m_useOutput);
    m_editExpected   .EnableWindow(m_useOutput);
    m_editEffective  .EnableWindow(m_useOutput);
  }
}

BEGIN_MESSAGE_MAP(OutputDlg, StyleTab)
  ON_BN_CLICKED   (IDC_USE_OUTPUT,&OutputDlg::OnBnClickedUseOutput)
  ON_CBN_SELCHANGE(IDC_OPERATOR,  &OutputDlg::OnCbnSelchangeOperator)
  ON_BN_CLICKED   (IDC_SEARCHVAR, &OutputDlg::OnBnClickedSearchvar)
  ON_EN_CHANGE    (IDC_OUTPUT,    &OutputDlg::OnEnChangeOutput)
  ON_EN_KILLFOCUS (IDC_OUTPUT,    &OutputDlg::OnEnKillfocusOutput)
  ON_EN_CHANGE    (IDC_EFFECTIVE, &OutputDlg::OnEnChangeEffective)
END_MESSAGE_MAP()

BOOL
OutputDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_buttonSearchvar.SetIconImage(IDI_LIST);

  m_comboOperator.AddString(_T(""));
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
OutputDlg::SetupDynamicLayout()
{
  StyleTab::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif
  manager.AddItem(IDC_SEARCHVAR,  CMFCDynamicLayout::MoveHorizontal(100), CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_OUTPUT,     CMFCDynamicLayout::MoveNone(),          CMFCDynamicLayout::SizeHorizontalAndVertical(100, 50));
  manager.AddItem(IDC_ST_EFF,     CMFCDynamicLayout::MoveVertical(50),    CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_EFFECTIVE,  CMFCDynamicLayout::MoveVertical(50),    CMFCDynamicLayout::SizeHorizontalAndVertical(100, 50));
}

void
OutputDlg::InitTab(ValidateCMD* p_validate, Parameters* p_param)
{
  m_validate   = p_validate;
  m_parameters = p_param;

  LoadVariables();
}

void
OutputDlg::LoadVariables()
{
  m_useOutput       = m_validate->GetCheckOutputValue();
  m_outputOperator  = m_validate->GetOutputOperator();
  m_expectedOutput  = m_validate->GetExpectedOutput();
  m_effectiveOutput = m_validate->GetEffectiveOutput();

  m_buttonUseOutput.SetCheck(m_useOutput);
  m_comboOperator.SetCurSel((int)m_outputOperator);

  UpdateData(FALSE);
}

void
OutputDlg::StoreVariables()
{
  m_validate->SetCheckOutputValue(m_useOutput);
  m_validate->SetOutputOperator(m_outputOperator);
  m_validate->SetExpectedOutput(m_expectedOutput);
}

// OutputDlg message handlers

void 
OutputDlg::OnBnClickedUseOutput()
{
  m_useOutput = m_buttonUseOutput.GetCheck() > 0;

  if(!m_useOutput)
  {
    m_outputOperator = BufferOperator::BOP_NOP;
    m_comboOperator.SetCurSel(-1);
    m_expectedOutput.Empty();
    m_effectiveOutput.Empty();
  }
  UpdateData(FALSE);
}

void 
OutputDlg::OnCbnSelchangeOperator()
{
  int ind = m_comboOperator.GetCurSel();
  if (ind >= 0)
  {
    m_outputOperator = (BufferOperator)(ind);
    UpdateData(FALSE);
  }
}

void 
OutputDlg::OnBnClickedSearchvar()
{
  AutoFocus focus;
  SearchVarDlg dlg(this,m_parameters,true,true,true,true);
  if (dlg.DoModal() == IDOK || dlg.GetSaved())
  {
    m_expectedOutput += dlg.GetVariable();
    UpdateData(FALSE);

    OnEnKillfocusOutput();
  }
}

void 
OutputDlg::OnEnChangeOutput()
{
  UpdateData();
}

void
OutputDlg::OnEnKillfocusOutput()
{
  UpdateData();
  m_validate->SetExpectedOutput(m_expectedOutput);

  ValidateEditorDlg* parent = (ValidateEditorDlg*)GetParent()->GetParent();
  StoreVariables();
  parent->EffectiveParameters();
  LoadVariables();
}

void 
OutputDlg::OnEnChangeEffective()
{
  UpdateData();
}
