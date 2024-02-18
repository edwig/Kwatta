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
// This File   : ScriptDlg.cpp
// What it does: Resulting response only of a HTTP internet test call
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StepEditor.h"
#include "ScriptDlg.h"
#include "StepInternetDlg.h"
#include "StepDatabaseDlg.h"
#include "StepCommandDlg.h"

// ScriptDlg dialog

IMPLEMENT_DYNAMIC(ScriptDlg,StyleTab)

ScriptDlg::ScriptDlg(CWnd* p_parent /*=nullptr*/)
	        :StyleTab(IDD_SCRIPT,p_parent)
{
}

ScriptDlg::~ScriptDlg()
{
}

void ScriptDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleTab::DoDataExchange(pDX);

  DDX_CBIndex(pDX,IDC_STATUS,   m_comboStatus, (int&)m_status);
  DDX_Control(pDX,IDC_STATUSOK, m_editStatusOK,m_statusOK);
  DDX_Control(pDX,IDC_SCRIPT,   m_editScript,  m_script);

  if(pDX->m_bSaveAndValidate == FALSE)
  {
    m_editScript  .EnableWindow(m_status != ScriptStatus::NoScript);
    m_editStatusOK.EnableWindow(m_status >= ScriptStatus::SuccessIsZero);
  }
}


BEGIN_MESSAGE_MAP(ScriptDlg, StyleTab)
  ON_CBN_SELCHANGE(IDC_STATUS,    &ScriptDlg::OnCbnSelchangeStatus)
  ON_EN_KILLFOCUS (IDC_STATUSOK,  &ScriptDlg::OnEnChangeStatusok)
  ON_EN_KILLFOCUS (IDC_SCRIPT,    &ScriptDlg::OnEnChangeScript)
END_MESSAGE_MAP()

BOOL
ScriptDlg::OnInitDialog()
{
  StyleTab::OnInitDialog();

  InitCombo();

  SetCanResize();
  return TRUE;
}

void
ScriptDlg::SetupDynamicLayout()
{
  StyleTab::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif
  manager.AddItem(IDC_SCRIPT,CMFCDynamicLayout::MoveNone(),CMFCDynamicLayout::SizeHorizontalAndVertical(100,100));
}

void
ScriptDlg::InitCombo()
{
  m_comboStatus.AddString(_T("No QL Script"));
  m_comboStatus.AddString(_T("Success is ZERO"));
  m_comboStatus.AddString(_T("Success is Positive"));
  m_comboStatus.AddString(_T("Success is Negative"));
}

void
ScriptDlg::InitTab(TestStep* p_step,Parameters* p_param)
{
  // Retain these
  m_step = p_step;
  m_parameters = p_param;
  // Fill tab
  m_status   = p_step->GetScriptStatus();
  m_script   = p_step->GetScriptToRun();
  m_statusOK = p_step->GetStatusOK();

  UpdateData(FALSE);
}

void
ScriptDlg::StoreVariables()
{
  if(m_step)
  {
    m_step->SetScriptToRun(m_script);
    m_step->SetScriptStatus(m_status);
    m_step->SetStatusOK(m_statusOK);
  }
}

bool
ScriptDlg::IsFilled()
{
  return !m_script.IsEmpty();
}

void
ScriptDlg::EffectiveParameters()
{
  if(m_step)
  {
    // Check parameters
    StepInternetDlg* istep = dynamic_cast<StepInternetDlg*>(GetParent()->GetParent());
    if(istep)
    {
      istep->EffectiveParameters();
    }
    StepDatabaseDlg* dstep = dynamic_cast<StepDatabaseDlg*>(GetParent()->GetParent());
    if(dstep)
    {
      dstep->EffectiveParameters();
    }
    StepCommandDlg* estep = dynamic_cast<StepCommandDlg*>(GetParent()->GetParent());
    if(estep)
    {
      estep->EffectiveParameters();
    }
  }
}

// ScriptDlg message handlers

void 
ScriptDlg::OnCbnSelchangeStatus()
{
  int ind = m_comboStatus.GetCurSel();
  if(ind >= 0)
  {
    m_status = (ScriptStatus) ind;
    if(m_status == ScriptStatus::NoScript)
    {
      m_script.Empty();
      m_statusOK.Empty();
    }
    UpdateData(FALSE);
  }
}

void 
ScriptDlg::OnEnChangeStatusok()
{
  UpdateData();
  EffectiveParameters();
}

void 
ScriptDlg::OnEnChangeScript()
{
  UpdateData();
  EffectiveParameters();
}
