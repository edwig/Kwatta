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
// This program: StepEditor
// This File   : SQLDlg.cpp
// What it does: Tab in the test editor for the SQL of a database test call
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SQLDlg.h"
#include "StepEditor.h"
#include "TestStepSQL.h"
#include "StepDatabaseDlg.h"
#include <SearchVarDlg.h>
#include "afxdialogex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// PayloadDlg dialog

IMPLEMENT_DYNAMIC(SQLDlg,StyleTab)

SQLDlg::SQLDlg(CWnd* p_parent)
       :StyleTab(IDD_SQL,p_parent)
{
}

SQLDlg::~SQLDlg()
{
}

void
SQLDlg::DoDataExchange(CDataExchange* pDX)
{
  StyleTab::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_SQL,  m_editPayload,m_payload);
  DDX_Control(pDX,IDC_PARAM,m_buttonParam);
}

BEGIN_MESSAGE_MAP(SQLDlg, StyleTab)
  ON_EN_CHANGE (IDC_SQL,  &SQLDlg::OnEnKillfocusPayload)
  ON_BN_CLICKED(IDC_PARAM,&SQLDlg::OnBnClickedParam)
END_MESSAGE_MAP()

BOOL
SQLDlg::OnInitDialog()
{
  StyleTab::OnInitDialog();

  m_buttonParam.SetIconImage(IDI_LIST);
  EnableToolTips();
  RegisterTooltip(m_buttonParam, "Choose global/test parameter.");
  InitPayload();
  SetCanResize();
  return TRUE;
}

void
SQLDlg::SetupDynamicLayout()
{
  StyleTab::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif

  manager.AddItem(IDC_SQL,  CMFCDynamicLayout::MoveNone(), CMFCDynamicLayout::SizeHorizontalAndVertical(100,100));
  manager.AddItem(IDC_PARAM,CMFCDynamicLayout::MoveHorizontalAndVertical(100,100), CMFCDynamicLayout::SizeNone());
}

void
SQLDlg::InitPayload()
{
  m_editPayload.SetFontName("Courier new", 100);
}

void
SQLDlg::InitTab(TestStepSQL* p_testStep,Parameters* p_parameters)
{
  // Register
  m_testStep   = p_testStep;
  m_parameters = p_parameters;
  // Setting the tab
  m_payload = m_testStep->GetSQL();
  UpdateData(FALSE);
}

bool
SQLDlg::IsFilled()
{
  return !m_payload.IsEmpty();
}


void
SQLDlg::EffectiveParameters()
{
  if(m_testStep)
  {
    // Check parameters
    StepDatabaseDlg* step = reinterpret_cast<StepDatabaseDlg*>(GetParent()->GetParent());
    step->EffectiveParameters();
  }
}

void
SQLDlg::StoreVariables()
{
  UpdateData();

  m_payload.Replace("\r","");
  m_testStep->SetSQL(m_payload);
}

// PayloadDlg message handlers

void
SQLDlg::OnBnClickedParam()
{
  AutoFocus focus;
  SearchVarDlg dlg(this,m_parameters,true,true,true,true);
  if (dlg.DoModal() == IDOK || dlg.GetSaved())
  {
    CString variable = dlg.GetVariable();
    m_editPayload.InsertAtCurPos(variable,0);
    OnEnKillfocusPayload();
  }
}

void
SQLDlg::OnEnKillfocusPayload()
{
  // Getting the payload as body for the message
  UpdateData();
  EffectiveParameters();
}
