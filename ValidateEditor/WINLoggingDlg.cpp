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
// This File   : WINLoggingDlg.cpp
// What it does: Defining the validation of the Windows UI test logging
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ValidateEditor.h"
#include "ValidateWindowsDlg.h"
#include "WINLoggingDlg.h"
#include "SearchVarDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// WINLoggingDlg dialog

IMPLEMENT_DYNAMIC(WINLoggingDlg,StyleTab)

WINLoggingDlg::WINLoggingDlg(CWnd* pParent /*=nullptr*/)
              :StyleTab(IDD_OUTPUT, pParent)
{
}

WINLoggingDlg::~WINLoggingDlg()
{
}

void 
WINLoggingDlg::DoDataExchange(CDataExchange* pDX)
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

BEGIN_MESSAGE_MAP(WINLoggingDlg, StyleTab)
  ON_BN_CLICKED   (IDC_USE_OUTPUT,&WINLoggingDlg::OnBnClickedUseOutput)
  ON_CBN_SELCHANGE(IDC_OPERATOR,  &WINLoggingDlg::OnCbnSelchangeOperator)
  ON_BN_CLICKED   (IDC_SEARCHVAR, &WINLoggingDlg::OnBnClickedSearchvar)
  ON_EN_CHANGE    (IDC_OUTPUT,    &WINLoggingDlg::OnEnChangeOutput)
  ON_EN_KILLFOCUS (IDC_OUTPUT,    &WINLoggingDlg::OnEnKillfocusOutput)
  ON_EN_CHANGE    (IDC_EFFECTIVE, &WINLoggingDlg::OnEnChangeEffective)
END_MESSAGE_MAP()

BOOL
WINLoggingDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_buttonSearchvar.SetIconImage(IDI_LIST);

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

  SetCanResize();
  return TRUE;
}

void
WINLoggingDlg::SetupDynamicLayout()
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
WINLoggingDlg::InitTab(ValidateWIN* p_validate,Parameters* p_param)
{
  m_validate   = p_validate;
  m_parameters = p_param;

  LoadVariables();
}

void
WINLoggingDlg::LoadVariables()
{
  m_useOutput       = m_validate->GetCheckLoggingValue();
  m_outputOperator  = m_validate->GetLoggingOperator();
  m_expectedOutput  = m_validate->GetExpectedLogging();
  m_effectiveOutput = m_validate->GetEffectiveLogging();

  m_buttonUseOutput.SetCheck(m_useOutput);
  m_comboOperator.SetCurSel((int)m_outputOperator);

  UpdateData(FALSE);
}

void
WINLoggingDlg::StoreVariables()
{
  m_validate->SetCheckLoggingValue(m_useOutput);
  m_validate->SetLoggingOperator(m_outputOperator);
  m_validate->SetExpectedLogging(m_expectedOutput);
}

// WINLoggingDlg message handlers

void 
WINLoggingDlg::OnBnClickedUseOutput()
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
WINLoggingDlg::OnCbnSelchangeOperator()
{
  int ind = m_comboOperator.GetCurSel();
  if (ind >= 0)
  {
    m_outputOperator = (BufferOperator)(ind);
    UpdateData(FALSE);
  }
}

void 
WINLoggingDlg::OnBnClickedSearchvar()
{
  SearchVarDlg dlg(this,m_parameters,true,true,true,true);
  if (dlg.DoModal() == IDOK || dlg.GetSaved())
  {
    m_expectedOutput += dlg.GetVariable();
    UpdateData(FALSE);

    OnEnKillfocusOutput();
  }
}

void 
WINLoggingDlg::OnEnChangeOutput()
{
  UpdateData();
}

void
WINLoggingDlg::OnEnKillfocusOutput()
{
  UpdateData();
  m_validate->SetExpectedLogging(m_expectedOutput);

  ValidateWindowsDlg* parent = (ValidateWindowsDlg*)GetParent()->GetParent();
  StoreVariables();
  parent->EffectiveParameters();
  LoadVariables();
}

void 
WINLoggingDlg::OnEnChangeEffective()
{
  UpdateData();
}
