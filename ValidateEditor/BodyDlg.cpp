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
// This File   : BodyDlg.cpp
// What it does: Defining the validation of a HTTP message body
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ValidateEditor.h"
#include "ValidateInternetDlg.h"
#include "BodyDlg.h"
#include <SearchVarDlg.h>
#include <NewVariableDlg.h>
#include <afxdialogex.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// BodyDlg dialog

IMPLEMENT_DYNAMIC(BodyDlg, StyleDialog)

BodyDlg::BodyDlg(CWnd* pParent /*=nullptr*/)
        :StyleDialog(IDD_BODY, pParent)
{
}

BodyDlg::~BodyDlg()
{
}

void 
BodyDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_USE_BODY,   m_buttonUseBody);
  DDX_Control(pDX, IDC_OPERATOR,   m_comboOperator);
  DDX_Control(pDX, IDC_SEARCHVAR,  m_buttonSearchvar);
  DDX_Control(pDX, IDC_BODY,       m_editExpected, m_expectedBody);
  DDX_Control(pDX, IDC_EFFECTIVE,  m_editEffective,m_effectiveBody);
  DDX_Control(pDX, IDC_BODY_VAR,   m_comboBodyVariable);
  DDX_Control(pDX, IDC_BODY_PARM,  m_buttonBodyParm);

  if(pDX->m_bSaveAndValidate == false)
  {
    m_comboOperator    .EnableWindow(m_useBody);
    m_buttonSearchvar  .EnableWindow(m_useBody);
    m_editExpected     .EnableWindow(m_useBody);
    m_editEffective    .EnableWindow(m_useBody);
    m_comboBodyVariable.EnableWindow(m_useBody);
    m_buttonBodyParm   .EnableWindow(m_useBody);
  }
}

BEGIN_MESSAGE_MAP(BodyDlg, StyleDialog)
  ON_BN_CLICKED   (IDC_USE_BODY , &BodyDlg::OnBnClickedUseBody)
  ON_CBN_SELCHANGE(IDC_OPERATOR,  &BodyDlg::OnCbnSelchangeOperator)
  ON_BN_CLICKED   (IDC_SEARCHVAR, &BodyDlg::OnBnClickedSearchvar)
  ON_EN_CHANGE    (IDC_BODY,      &BodyDlg::OnEnChangeBody)
  ON_EN_KILLFOCUS (IDC_BODY,      &BodyDlg::OnEnKillfocusBody)
  ON_EN_CHANGE    (IDC_EFFECTIVE, &BodyDlg::OnEnChangeEffective)
  ON_CBN_SELCHANGE(IDC_BODY_VAR,  &BodyDlg::OnCbnSelchangeBodyVariable)
  ON_BN_CLICKED   (IDC_BODY_PARM, &BodyDlg::OnBnClickedBodyParm)
END_MESSAGE_MAP()

BOOL
BodyDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_buttonSearchvar.SetIconImage(IDI_LIST);
  m_buttonBodyParm.SetIconImage(IDI_RETURN);

  SetCanResize();
  return TRUE;
}

void
BodyDlg::SetupDynamicLayout()
{
  StyleDialog::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif
  manager.AddItem(IDC_SEARCHVAR,  CMFCDynamicLayout::MoveHorizontal(100), CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_BODY,       CMFCDynamicLayout::MoveNone(),          CMFCDynamicLayout::SizeHorizontalAndVertical(100, 50));
  manager.AddItem(IDC_ST_EFF,     CMFCDynamicLayout::MoveVertical(50),    CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_EFFECTIVE,  CMFCDynamicLayout::MoveVertical(50),    CMFCDynamicLayout::SizeHorizontalAndVertical(100, 50));
  manager.AddItem(IDC_ST_VAR,     CMFCDynamicLayout::MoveVertical(100),   CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_BODY_VAR,   CMFCDynamicLayout::MoveVertical(100),   CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_BODY_PARM,  CMFCDynamicLayout::MoveVertical(100),   CMFCDynamicLayout::SizeNone());
}

void
BodyDlg::InitTab(ValidateIN* p_validate, Parameters* p_param)
{
  m_validate   = p_validate;
  m_parameters = p_param;

  LoadVariables();
  FillCombos();
  SetCombos();
}

void 
BodyDlg::FillCombos()
{
  m_comboOperator.ResetContent();
  m_comboBodyVariable.ResetContent();

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

  m_comboBodyVariable.AddString("");
  for(auto& ret : m_parameters->GetReturns())
  {
    m_comboBodyVariable.AddString(ret.first);
  }
}

void 
BodyDlg::SetCombos()
{
  m_comboOperator.SetCurSel((int)m_bodyOperator);

  int ind = m_comboBodyVariable.FindStringExact(0,m_bodyVariable);
  if (ind >= 0)
  {
    m_comboBodyVariable.SetCurSel(ind);
  }
}

void
BodyDlg::LoadVariables()
{
  m_useBody       = m_validate->GetCheckBody();
  m_bodyOperator  = m_validate->GetBodyOperator();
  m_expectedBody  = m_validate->GetExpectedBody();
  m_effectiveBody = m_validate->GetEffectiveBody();
  m_bodyVariable  = m_validate->GetBodyVariable();

  m_buttonUseBody.SetCheck(m_useBody);
  m_comboOperator.SetCurSel((int)m_bodyOperator - 1);

  UpdateData(FALSE);
}

void
BodyDlg::StoreVariables()
{
  m_validate->SetCheckBody(m_useBody);
  m_validate->SetBodyOperator(m_bodyOperator);
  m_validate->SetExpectedBody(m_expectedBody);
  m_validate->SetBodyVariable(m_bodyVariable);
}

// BodyDlg message handlers

void 
BodyDlg::OnBnClickedUseBody()
{
  m_useBody = m_buttonUseBody.GetCheck() > 0;

  if(!m_useBody)
  {
    m_bodyOperator = BufferOperator::BOP_NOP;
    m_comboOperator.SetCurSel(-1);
    m_expectedBody.Empty();
    m_effectiveBody.Empty();
    m_bodyVariable.Empty();
    m_comboBodyVariable.SetCurSel(-1);
  }
  UpdateData(FALSE);
}

void 
BodyDlg::OnCbnSelchangeOperator()
{
  int ind = m_comboOperator.GetCurSel();
  if (ind >= 0)
  {
    m_bodyOperator = (BufferOperator)ind;
  }
}

void 
BodyDlg::OnBnClickedSearchvar()
{
  SearchVarDlg dlg(m_parameters,true,true,true);
  if (dlg.DoModal() == IDOK)
  {
    CString var = dlg.GetChosenVariable();
    CString variable;
    switch (dlg.GetResultType())
    {
      case ParType::PAR_GLOBAL: variable.Format("$%s$", var); break;
      case ParType::PAR_RETURN: variable.Format("[%s]", var); break;
      case ParType::PAR_BUFFER: variable.Format("<%s>", var); break;
    }

    m_expectedBody += variable;
    UpdateData(FALSE);

    OnEnKillfocusBody();
  }
}

void 
BodyDlg::OnEnChangeBody()
{
  UpdateData();
}

void
BodyDlg::OnEnKillfocusBody()
{
  UpdateData();
  m_validate->SetExpectedBody(m_expectedBody);

  ValidateInternetDlg* parent = (ValidateInternetDlg*)GetParent()->GetParent();
  StoreVariables();
  parent->EffectiveParameters();
  LoadVariables();
}

void 
BodyDlg::OnEnChangeEffective()
{
  UpdateData();
}

void 
BodyDlg::OnCbnSelchangeBodyVariable()
{
  int ind = m_comboBodyVariable.GetCurSel();
  if(ind >= 0)
  {
    CString var;
    m_comboBodyVariable.GetLBText(ind,var);
    m_bodyVariable = var;
  }
}

void 
BodyDlg::OnBnClickedBodyParm()
{
  NewVariableDlg dlg(ParType::PAR_RETURN, m_parameters->GetReturns());
  if (dlg.DoModal() == IDOK)
  {
    CString newvar = dlg.GetNewName();
    if (!newvar.IsEmpty())
    {
      m_bodyVariable = newvar;
      FillCombos();
      SetCombos();
    }
  }
}
