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
// This File   : JsonPairDlg.cpp
// What it does: Defining the validation of JSON pair in the HTTP response body
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ValidateEditor.h"
#include "ValidateInternetDlg.h"
#include "JSONPairDlg.h"
#include <SearchVarDlg.h>
#include <NewVariableDlg.h>
#include <afxdialogex.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// JSONPairDlg dialog

IMPLEMENT_DYNAMIC(JSONPairDlg,StyleTab)

JSONPairDlg::JSONPairDlg(CWnd* pParent /*=nullptr*/)
            :StyleTab(IDD_JSON_PAIR, pParent)
{
}

JSONPairDlg::~JSONPairDlg()
{
}

void 
JSONPairDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleTab::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_USE_JSON, m_buttonCheck);
  DDX_Control(pDX, IDC_OPERATOR, m_comboOperator);
  DDX_Control(pDX, IDC_JSONPATH, m_editJsonPath,  m_jsonPath);
  DDX_Control(pDX, IDC_JSON,     m_editExpected,  m_expectedJSON);
  DDX_Control(pDX, IDC_EFF_JSON, m_editEffective, m_effectiveJSON);
  DDX_Control(pDX, IDC_NEWVAR,   m_buttonVariable);
  DDX_Control(pDX, IDC_JSON_VAR, m_comboJsonVariable);
  DDX_Control(pDX, IDC_JSON_PARM,m_buttonJsonParm);

  if(pDX->m_bSaveAndValidate == false)
  {
    m_comboOperator    .EnableWindow(m_useJSON);
    m_editJsonPath     .EnableWindow(m_useJSON);
    m_editExpected     .EnableWindow(m_useJSON);
    m_editEffective    .EnableWindow(m_useJSON);
    m_comboJsonVariable.EnableWindow(m_useJSON);
    m_buttonVariable   .EnableWindow(m_useJSON);
    m_buttonJsonParm   .EnableWindow(m_useJSON);
  }
}

BEGIN_MESSAGE_MAP(JSONPairDlg, StyleTab)
  ON_BN_CLICKED   (IDC_USE_JSON,  &JSONPairDlg::OnBnClickedUseJSON)
  ON_CBN_SELCHANGE(IDC_OPERATOR,  &JSONPairDlg::OnCbnSelchangeOperator)
  ON_EN_KILLFOCUS (IDC_JSONPATH,  &JSONPairDlg::OnEnChangeJsonPath)
  ON_EN_CHANGE    (IDC_JSON,      &JSONPairDlg::OnEnChangeJSON)
  ON_EN_KILLFOCUS (IDC_JSON,      &JSONPairDlg::OnEnKillfocusJSON)
  ON_BN_CLICKED   (IDC_NEWVAR,    &JSONPairDlg::OnBnClickedNewvar)
  ON_EN_CHANGE    (IDC_EFF_JSON,  &JSONPairDlg::OnEnChangeEffStatus)
  ON_CBN_SELCHANGE(IDC_JSON_VAR,  &JSONPairDlg::OnCbnSelchangeJsonVariable)
  ON_BN_CLICKED   (IDC_JSON_PARM, &JSONPairDlg::OnBnClickedJsonParm)
END_MESSAGE_MAP()

BOOL
JSONPairDlg::OnInitDialog()
{
  StyleTab::OnInitDialog();

  m_buttonVariable.SetIconImage(IDI_LIST);
  m_buttonJsonParm.SetIconImage(IDI_RETURN);

  SetCanResize();
  return TRUE;
}

void
JSONPairDlg::SetupDynamicLayout()
{
  StyleTab::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif

  manager.AddItem(IDC_JSONPATH,CMFCDynamicLayout::MoveNone(),          CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_JSON,    CMFCDynamicLayout::MoveNone(),          CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_EFF_JSON,CMFCDynamicLayout::MoveNone(),          CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_NEWVAR,  CMFCDynamicLayout::MoveHorizontal(100), CMFCDynamicLayout::SizeNone());
}

void
JSONPairDlg::InitTab(ValidateNET* p_validate,Parameters* p_parameters)
{
  m_validate   = p_validate;
  m_parameters = p_parameters;

  LoadVariables();
  FillCombos();
  SetCombos();
}

void 
JSONPairDlg::FillCombos()
{
  m_comboOperator.ResetContent();
  m_comboJsonVariable.ResetContent();

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

  m_comboJsonVariable.AddString("");
  for(auto& ret : m_parameters->GetReturns())
  {
    m_comboJsonVariable.AddString(ret.first);
  }
}

void 
JSONPairDlg::SetCombos()
{
  m_comboOperator.SetCurSel((int)m_jsonOperator);

  int ind = m_comboJsonVariable.FindStringExact(0,m_jsonVariable);
  if (ind >= 0)
  {
    m_comboJsonVariable.SetCurSel(ind);
  }
}

void
JSONPairDlg::LoadVariables()
{
  m_useJSON       = m_validate->GetCheckJSON();
  m_jsonOperator  = m_validate->GetJSONOperator();
  m_jsonPath      = m_validate->GetVerifyJSONPath();
  m_expectedJSON  = m_validate->GetExpectedJSONValue();
  m_effectiveJSON = m_validate->GetEffectiveJSONValue();
  m_jsonVariable  = m_validate->GetJsonVariable();

  m_buttonCheck.SetCheck(m_useJSON);
  m_comboOperator.SetCurSel((int)m_jsonOperator - 1);

  UpdateData(FALSE);
}

void
JSONPairDlg::StoreVariables()
{
  m_validate->SetCheckJSON(m_useJSON);
  m_validate->SetVerifyJSONPath(m_jsonPath);
  m_validate->SetJSONOperator(m_jsonOperator);
  m_validate->SetExpectedJSONValue(m_expectedJSON);
  m_validate->SetJsonVariable(m_jsonVariable);
}

// JSONPairDlg message handlers

void 
JSONPairDlg::OnBnClickedUseJSON()
{
  m_useJSON = m_buttonCheck.GetCheck() > 0;

  if(!m_useJSON)
  {
    m_jsonOperator = BufferOperator::BOP_NOP;
    m_comboOperator.SetCurSel(-1);
    m_jsonPath.Empty();
    m_expectedJSON.Empty();
    m_effectiveJSON.Empty();
    m_jsonVariable.Empty();
    m_comboJsonVariable.SetCurSel(-1);
  }
  UpdateData(FALSE);
}

void 
JSONPairDlg::OnCbnSelchangeOperator()
{
  int ind = m_comboOperator.GetCurSel();
  if (ind >= 0)
  {
    m_jsonOperator = (BufferOperator)ind;
  }
}

void
JSONPairDlg::OnEnChangeJsonPath()
{
  UpdateData(); 
  m_validate->SetVerifyJSONPath(m_jsonPath);

  ValidateInternetDlg* parent = (ValidateInternetDlg*)GetParent()->GetParent();
  StoreVariables();
  parent->EffectiveParameters();
  LoadVariables();
}

void 
JSONPairDlg::OnEnChangeJSON()
{
  UpdateData();
}

void
JSONPairDlg::OnEnKillfocusJSON()
{
  UpdateData();
  m_validate->SetExpectedJSONValue(m_expectedJSON);

  ValidateInternetDlg* parent = (ValidateInternetDlg*)GetParent()->GetParent();
  StoreVariables();
  parent->EffectiveParameters();
  LoadVariables();
}

void 
JSONPairDlg::OnBnClickedNewvar()
{
  AutoFocus focus;
  SearchVarDlg dlg(this,m_parameters,true,true,true,true);
  if (dlg.DoModal() == IDOK || dlg.GetSaved())
  {
    m_expectedJSON += dlg.GetVariable();
    UpdateData(FALSE);

    OnEnKillfocusJSON();
  }
}

void 
JSONPairDlg::OnEnChangeEffStatus()
{
  UpdateData();
}

void 
JSONPairDlg::OnCbnSelchangeJsonVariable()
{
  int ind = m_comboJsonVariable.GetCurSel();
  if (ind >= 0)
  {
    CString var;
    m_comboJsonVariable.GetLBText(ind, var);
    m_jsonVariable = var;
  }
}

void 
JSONPairDlg::OnBnClickedJsonParm()
{
  AutoFocus focus;
  NewVariableDlg dlg(ParType::PAR_RETURN, m_parameters->GetReturns());
  if (dlg.DoModal() == IDOK)
  {
    CString newvar = dlg.GetNewName();
    if (!newvar.IsEmpty())
    {
      m_jsonVariable = newvar;
      FillCombos();
      SetCombos();
    }
  }
}
