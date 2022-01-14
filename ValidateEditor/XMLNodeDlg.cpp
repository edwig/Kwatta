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
// This File   : XMLNodeDlg.cpp
// What it does: Validation of an XML node in the HTTPMessage response body
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ValidateEditor.h"
#include "ValidateInternetDlg.h"
#include "XMLNodeDlg.h"
#include <SearchVarDlg.h>
#include <NewVariableDlg.h>
#include <afxdialogex.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// XMLNodeDlg dialog

IMPLEMENT_DYNAMIC(XMLNodeDlg, StyleDialog)

XMLNodeDlg::XMLNodeDlg(CWnd* pParent /*=nullptr*/)
           :StyleDialog(IDD_XML_NODE, pParent)
{
}

XMLNodeDlg::~XMLNodeDlg()
{
}

void 
XMLNodeDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_USE_XML,  m_buttonCheck);
  DDX_Control(pDX, IDC_OPERATOR, m_comboOperator);
  DDX_Control(pDX, IDC_XMLPATH,  m_editXmlPath,   m_xmlPath);
  DDX_Control(pDX, IDC_XML,      m_editExpected,  m_expectedXML);
  DDX_Control(pDX, IDC_EFF_XML,  m_editEffective, m_effectiveXML);
  DDX_Control(pDX, IDC_NEWVAR,   m_buttonVariable);
  DDX_Control(pDX, IDC_XML_VAR,  m_comboXmlVariable);
  DDX_Control(pDX, IDC_XML_PARM, m_buttonXmlParm);

  if(pDX->m_bSaveAndValidate == false)
  {
    m_comboOperator   .EnableWindow(m_useXML);
    m_editXmlPath     .EnableWindow(m_useXML);
    m_editExpected    .EnableWindow(m_useXML);
    m_editEffective   .EnableWindow(m_useXML);
    m_comboXmlVariable.EnableWindow(m_useXML);
    m_buttonVariable  .EnableWindow(m_useXML);
    m_buttonXmlParm   .EnableWindow(m_useXML);
  }
}

BEGIN_MESSAGE_MAP(XMLNodeDlg, StyleDialog)
  ON_BN_CLICKED   (IDC_USE_XML, &XMLNodeDlg::OnBnClickedUseXML)
  ON_CBN_SELCHANGE(IDC_OPERATOR,&XMLNodeDlg::OnCbnSelchangeOperator)
  ON_EN_KILLFOCUS (IDC_XMLPATH, &XMLNodeDlg::OnEnChangeXmlPath)
  ON_EN_CHANGE    (IDC_XML,     &XMLNodeDlg::OnEnChangeXML)
  ON_EN_KILLFOCUS (IDC_XML,     &XMLNodeDlg::OnEnKillfocusXML)
  ON_BN_CLICKED   (IDC_NEWVAR,  &XMLNodeDlg::OnBnClickedNewvar)
  ON_EN_CHANGE    (IDC_EFF_XML, &XMLNodeDlg::OnEnChangeEffStatus)
  ON_CBN_SELCHANGE(IDC_XML_VAR, &XMLNodeDlg::OnCbnSelchangeXmlVariable)
  ON_BN_CLICKED   (IDC_XML_PARM,&XMLNodeDlg::OnBnClickedXmlParm)
END_MESSAGE_MAP()

BOOL
XMLNodeDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();

  m_buttonVariable.SetIconImage(IDI_LIST);
  m_buttonXmlParm .SetIconImage(IDI_RETURN);

  SetCanResize();
  return TRUE;
}

void
XMLNodeDlg::SetupDynamicLayout()
{
  StyleDialog::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif

  manager.AddItem(IDC_XMLPATH, CMFCDynamicLayout::MoveNone(),          CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_XML,     CMFCDynamicLayout::MoveNone(),          CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_EFF_XML, CMFCDynamicLayout::MoveNone(),          CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_NEWVAR,  CMFCDynamicLayout::MoveHorizontal(100), CMFCDynamicLayout::SizeNone());
}

void
XMLNodeDlg::InitTab(ValidateIN* p_validate,Parameters* p_parameters)
{
  m_validate   = p_validate;
  m_parameters = p_parameters;

  LoadVariables();
  FillCombos();
  SetCombos();
}

void 
XMLNodeDlg::FillCombos()
{
  m_comboOperator.ResetContent();
  m_comboXmlVariable.ResetContent();

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

  m_comboXmlVariable.AddString("");
  for (auto& ret : m_parameters->GetReturns())
  {
    m_comboXmlVariable.AddString(ret.first);
  }
}

void 
XMLNodeDlg::SetCombos()
{
  m_comboOperator.SetCurSel((int)m_xmlOperator);

  int ind = m_comboXmlVariable.FindStringExact(0, m_xmlVariable);
  if (ind >= 0)
  {
    m_comboXmlVariable.SetCurSel(ind);
  }
}

void
XMLNodeDlg::LoadVariables()
{
  m_useXML       = m_validate->GetCheckXML();
  m_xmlOperator  = m_validate->GetXMLOperator();
  m_xmlPath      = m_validate->GetVerifyXmlPath();
  m_expectedXML  = m_validate->GetExpectedXMLValue();
  m_effectiveXML = m_validate->GetEffectiveXMLValue();
  m_xmlVariable  = m_validate->GetXmlVariable();

  m_buttonCheck.SetCheck(m_useXML);
  m_comboOperator.SetCurSel((int)m_xmlOperator - 1);

  UpdateData(FALSE);
}

void
XMLNodeDlg::StoreVariables()
{
  m_validate->SetCheckXML(m_useXML);
  m_validate->SetVerifyXmlPath(m_xmlPath);
  m_validate->SetXMLOperator(m_xmlOperator);
  m_validate->SetExpectedXMLValue(m_expectedXML);
  m_validate->SetXmlVariable(m_xmlVariable);
}

// XMLNodeDlg message handlers

void 
XMLNodeDlg::OnBnClickedUseXML()
{
  m_useXML= m_buttonCheck.GetCheck() > 0;

  if(!m_useXML)
  {
    m_xmlOperator = BufferOperator::BOP_NOP;
    m_comboOperator.SetCurSel(-1);
    m_expectedXML.Empty();
    m_effectiveXML.Empty();
  }
  UpdateData(FALSE);
}

void 
XMLNodeDlg::OnCbnSelchangeOperator()
{
  int ind = m_comboOperator.GetCurSel();
  if (ind >= 0)
  {
    m_xmlOperator = (BufferOperator)ind;
  }
}

void
XMLNodeDlg::OnEnChangeXmlPath()
{
  UpdateData(); 
  m_validate->SetVerifyXmlPath(m_xmlPath);

  ValidateInternetDlg* parent = (ValidateInternetDlg*)GetParent()->GetParent();
  StoreVariables();
  parent->EffectiveParameters();
  LoadVariables();
}

void 
XMLNodeDlg::OnEnChangeXML()
{
  UpdateData();
}

void
XMLNodeDlg::OnEnKillfocusXML()
{
  UpdateData();
  m_validate->SetExpectedXMLValue(m_expectedXML);

  ValidateInternetDlg* parent = (ValidateInternetDlg*)GetParent()->GetParent();
  StoreVariables();
  parent->EffectiveParameters();
  LoadVariables();
}

void 
XMLNodeDlg::OnBnClickedNewvar()
{
  SearchVarDlg dlg(m_parameters,true,true,true);
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

    m_expectedXML += variable;
    UpdateData(FALSE);

    OnEnKillfocusXML();
  }
}

void 
XMLNodeDlg::OnEnChangeEffStatus()
{
  UpdateData();
}

void 
XMLNodeDlg::OnCbnSelchangeXmlVariable()
{
  int ind = m_comboXmlVariable.GetCurSel();
  if(ind >= 0)
  {
    CString var;
    m_comboXmlVariable.GetLBText(ind,var);
    m_xmlVariable = var;
  }
}

void 
XMLNodeDlg::OnBnClickedXmlParm()
{
  NewVariableDlg dlg(ParType::PAR_RETURN, m_parameters->GetReturns());
  if(dlg.DoModal() == IDOK)
  {
    CString newvar = dlg.GetNewName();
    if(!newvar.IsEmpty())
    {
      m_xmlVariable = newvar;
      FillCombos();
      SetCombos();
    }
  }
}
