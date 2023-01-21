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
// This File   : HeaderDlg.cpp
// What it does: Defining the validation of response headers of a HTTP internet message call
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ValidateEditor.h"
#include "ValidateInternetDlg.h"
#include "HeaderDlg.h"
#include "SearchVarDlg.h"
#include "NewVariableDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// HeaderDlg dialog

IMPLEMENT_DYNAMIC(HeaderDlg,StyleTab)

HeaderDlg::HeaderDlg(CWnd* pParent /*=nullptr*/)
          :StyleTab(IDD_HEADER, pParent)
{
}

HeaderDlg::~HeaderDlg()
{
}

void 
HeaderDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleTab::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_USE_HEADER, m_buttonCheck);
  DDX_Control(pDX, IDC_OPERATOR,   m_comboOperator);
  DDX_Control(pDX, IDC_NAME,       m_editHeaderName,m_headerName);
  DDX_Control(pDX, IDC_HEADER,     m_editExpected,  m_expectedHeader);
  DDX_Control(pDX, IDC_EFF_HEADER, m_editEffective, m_effectiveHeader);
  DDX_Control(pDX, IDC_NEWVAR,     m_buttonVariable);
  DDX_Control(pDX, IDC_VAR_HEADER, m_comboHeaderVariable);
  DDX_Control(pDX, IDC_HEAD_PARM,  m_buttonHeaderParm);

  if(pDX->m_bSaveAndValidate == false)
  {
    m_comboOperator      .EnableWindow(m_useHeaderValue);
    m_editHeaderName     .EnableWindow(m_useHeaderValue);
    m_editExpected       .EnableWindow(m_useHeaderValue);
    m_editEffective      .EnableWindow(m_useHeaderValue);
    m_comboHeaderVariable.EnableWindow(m_useHeaderValue);
    m_buttonVariable     .EnableWindow(m_useHeaderValue);
    m_buttonHeaderParm   .EnableWindow(m_useHeaderValue);
  }
}

BEGIN_MESSAGE_MAP(HeaderDlg, StyleTab)
  ON_BN_CLICKED   (IDC_USE_HEADER,  &HeaderDlg::OnBnClickedUseHeader)
  ON_CBN_SELCHANGE(IDC_OPERATOR,    &HeaderDlg::OnCbnSelchangeOperator)
  ON_EN_KILLFOCUS (IDC_NAME,        &HeaderDlg::OnEnChangeHeaderName)
  ON_EN_CHANGE    (IDC_HEADER,      &HeaderDlg::OnEnChangeHeader)
  ON_EN_KILLFOCUS (IDC_HEADER,      &HeaderDlg::OnEnKillfocusHeader)
  ON_BN_CLICKED   (IDC_NEWVAR,      &HeaderDlg::OnBnClickedNewvar)
  ON_EN_CHANGE    (IDC_EFF_RETURN,  &HeaderDlg::OnEnChangeEffStatus)
  ON_CBN_SELCHANGE(IDC_VAR_HEADER,  &HeaderDlg::OnCbnSelHeaderVariable)
  ON_BN_CLICKED   (IDC_HEAD_PARM,   &HeaderDlg::OnBnClickedHeaderParm)
END_MESSAGE_MAP()

BOOL
HeaderDlg::OnInitDialog()
{
  StyleTab::OnInitDialog();

  m_buttonVariable.SetIconImage(IDI_LIST);
  m_buttonHeaderParm.SetIconImage(IDI_RETURN);

  SetCanResize();
  return TRUE;
}

void
HeaderDlg::SetupDynamicLayout()
{
  StyleTab::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif

  manager.AddItem(IDC_NAME,       CMFCDynamicLayout::MoveNone(),          CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_HEADER,     CMFCDynamicLayout::MoveNone(),          CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_EFF_HEADER, CMFCDynamicLayout::MoveNone(),          CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_NEWVAR,     CMFCDynamicLayout::MoveHorizontal(100), CMFCDynamicLayout::SizeNone());
}

void
HeaderDlg::InitTab(ValidateNET* p_validate,Parameters* p_parameters)
{
  m_validate   = p_validate;
  m_parameters = p_parameters;

  LoadVariables();
  FillCombos();
  SetCombos();
}

void 
HeaderDlg::FillCombos()
{
  m_comboOperator.ResetContent();
  m_comboHeaderVariable.ResetContent();

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

  m_comboHeaderVariable.AddString("");
  for(auto& ret : m_parameters->GetReturns())
  {
    m_comboHeaderVariable.AddString(ret.first);
  }
}

void 
HeaderDlg::SetCombos()
{
  m_comboOperator.SetCurSel((int)m_headerOperator);

  int ind = m_comboHeaderVariable.FindStringExact(0,m_headerVariable);
  if (ind >= 0)
  {
    m_comboHeaderVariable.SetCurSel(ind);
  }
}

void
HeaderDlg::LoadVariables()
{
  m_useHeaderValue  = m_validate->GetCheckHeader();
  m_headerOperator  = m_validate->GetHeaderOperator();
  m_headerName      = m_validate->GetVerifyHeader();
  m_expectedHeader  = m_validate->GetExpectedHeaderValue();
  m_effectiveHeader = m_validate->GetEffectiveHeaderValue();
  m_headerVariable  = m_validate->GetHeaderVariable();

  m_buttonCheck.SetCheck(m_useHeaderValue);
  m_comboOperator.SetCurSel((int)m_headerOperator);

  UpdateData(FALSE);
}

void
HeaderDlg::StoreVariables()
{
  m_validate->SetCheckHeader(m_useHeaderValue);
  m_validate->SetVerifyHeader(m_headerName);
  m_validate->SetHeaderOperator(m_headerOperator);
  m_validate->SetExpectedHeaderValue(m_expectedHeader);
  m_validate->SetHeaderVariable(m_headerVariable);
}

// HeaderDlg message handlers

void 
HeaderDlg::OnBnClickedUseHeader()
{
  m_useHeaderValue= m_buttonCheck.GetCheck() > 0;

  if(!m_useHeaderValue)
  {
    m_headerOperator = BufferOperator::BOP_NOP;
    m_comboOperator.SetCurSel(-1);
    m_headerName.Empty();
    m_expectedHeader.Empty();
    m_effectiveHeader.Empty();
    m_headerVariable.Empty();
    m_comboHeaderVariable.SetCurSel(-1);
  }
  UpdateData(FALSE);
}

void 
HeaderDlg::OnCbnSelchangeOperator()
{
  int ind = m_comboOperator.GetCurSel();
  if (ind >= 0)
  {
    m_headerOperator = (BufferOperator)ind;
  }
}

void
HeaderDlg::OnEnChangeHeaderName()
{
  UpdateData(); 
  m_validate->SetVerifyHeader(m_headerName);

  ValidateInternetDlg* parent = (ValidateInternetDlg*)GetParent()->GetParent();
  StoreVariables();
  parent->EffectiveParameters();
  LoadVariables();
}

void 
HeaderDlg::OnEnChangeHeader()
{
  UpdateData();
}

void
HeaderDlg::OnEnKillfocusHeader()
{
  UpdateData();
  m_validate->SetExpectedHeaderValue(m_expectedHeader);

  ValidateInternetDlg* parent = (ValidateInternetDlg*)GetParent()->GetParent();
  StoreVariables();
  parent->EffectiveParameters();
  LoadVariables();
}

void 
HeaderDlg::OnBnClickedNewvar()
{
  SearchVarDlg dlg(this,m_parameters,true,true,true,true);
  if (dlg.DoModal() == IDOK || dlg.GetSaved())
  {
    m_expectedHeader += dlg.GetVariable();
    UpdateData(FALSE);

    OnEnKillfocusHeader();
  }
}

void 
HeaderDlg::OnEnChangeEffStatus()
{
  UpdateData();
}

void 
HeaderDlg::OnCbnSelHeaderVariable()
{
  int ind = m_comboHeaderVariable.GetCurSel();
  if(ind >= 0)
  {
    CString var;
    m_comboHeaderVariable.GetLBText(ind,var);
    m_headerVariable = var;
  }
}

void 
HeaderDlg::OnBnClickedHeaderParm()
{
  NewVariableDlg dlg(ParType::PAR_RETURN,m_parameters->GetReturns());
  if(dlg.DoModal() == IDOK)
  {
    CString newvar = dlg.GetNewName();
    if(!newvar.IsEmpty())
    {
      m_headerVariable = newvar;
      m_parameters->AddReturnParameter(m_headerVariable,"");
      FillCombos();
      SetCombos();
    }
  }
}
