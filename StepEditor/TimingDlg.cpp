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
// This File   : TimingDlg.cpp
// What it does: Parameters for running a internet test step
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StepEditor.h"
#include "StepInternetDlg.h"
#include "TimingDlg.h"
#include <TestStepIN.h>
#include <SearchVarDlg.h>
#include <Parameters.h>
#include <afxdialogex.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// TimingDlg dialog

IMPLEMENT_DYNAMIC(TimingDlg,StyleDialog)

TimingDlg::TimingDlg(CWnd* pParent /*=nullptr*/)
	        :StyleDialog(IDD_TIMING, pParent)
{
}

TimingDlg::~TimingDlg()
{
}

void TimingDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);

  DDX_Control(pDX, IDC_KILLONTIMEOUT,m_buttonKillOnTimeout);
  DDX_Control(pDX, IDC_WAITFORRUN,   m_editWaitBeforeRun,m_waitBeforeRun);
  DDX_Control(pDX, IDC_TIMEOUT,      m_editMaxExecution, m_maxExecution);
  DDX_Control(pDX, IDC_WAITAFTERRUN, m_editWaitAfterRun, m_waitAfterRun);
  DDX_Control(pDX, IDC_BUT_BEFORE,   m_buttonBefore);
  DDX_Control(pDX, IDC_BUT_EXEC,     m_buttonExec);
  DDX_Control(pDX, IDC_BUT_AFTER,    m_buttonAfter);
  DDX_Control(pDX, IDC_EFF_BEFORE,   m_editEffWaitBefore,m_effectiveBefore);
  DDX_Control(pDX, IDC_EFF_EXEC,     m_editEffWaitExec,  m_effectiveExec);
  DDX_Control(pDX, IDC_EFF_AFTER,    m_editEffWaitAfter, m_effectiveAfter);

  if(pDX->m_bSaveAndValidate == FALSE)
  {
    m_editMaxExecution.EnableWindow(m_killOnTimeout);
    m_buttonExec      .EnableWindow(m_killOnTimeout);
    m_editEffWaitExec .EnableWindow(m_killOnTimeout);
  }
}

BEGIN_MESSAGE_MAP(TimingDlg, StyleDialog)
  ON_EN_KILLFOCUS (IDC_WAITFORRUN,    &TimingDlg::OnEnChangeWaitBeforerun)
  ON_EN_KILLFOCUS (IDC_WAITAFTERRUN,  &TimingDlg::OnEnChangeWaitafterrun)
  ON_BN_CLICKED   (IDC_KILLONTIMEOUT, &TimingDlg::OnBnClickedKillontimeout)
  ON_EN_KILLFOCUS (IDC_TIMEOUT,       &TimingDlg::OnEnChangeTimeout)
  ON_BN_CLICKED   (IDC_BUT_BEFORE,    &TimingDlg::OnBnClickedBefore)
  ON_BN_CLICKED   (IDC_BUT_EXEC,      &TimingDlg::OnBnClickedExec)
  ON_BN_CLICKED   (IDC_BUT_AFTER,     &TimingDlg::OnBnClickedAfter)
END_MESSAGE_MAP()

BOOL
TimingDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();
  
  m_buttonBefore.SetIconImage(IDI_LIST);
  m_buttonAfter .SetIconImage(IDI_LIST);
  m_buttonExec  .SetIconImage(IDI_LIST);

  m_editEffWaitBefore.SetMutable(false);
  m_editEffWaitAfter .SetMutable(false);
  m_editEffWaitExec  .SetMutable(false);

  return TRUE;
}

void
TimingDlg::InitTab(TestStepIN* p_step,Parameters* p_param)
{
  // Remember the step
  m_step = p_step;
  m_parameters = p_param;

  // Get all values
  m_waitBeforeRun     = m_step->GetWaitBeforeRun();
  m_waitAfterRun      = m_step->GetWaitAfterRun();
  m_killOnTimeout     = m_step->GetKillOnTimeout();
  m_maxExecution      = m_step->GetMaxExecution();

  m_buttonKillOnTimeout.SetCheck(m_killOnTimeout);
  Effectuate();
}

void
TimingDlg::StoreVariables()
{
  m_step->SetWaitBeforeRun(m_waitBeforeRun);
  m_step->SetWaitAfterRun(m_waitAfterRun);
  m_step->SetKillOnTimeout(m_killOnTimeout);
  m_step->SetMaxExecution(m_maxExecution);
}

void
TimingDlg::Effectuate()
{
  if(m_step)
  {
    m_step->SetMaxExecution (m_maxExecution);
    m_step->SetWaitBeforeRun(m_waitBeforeRun);
    m_step->SetWaitAfterRun (m_waitAfterRun);

    StepInternetDlg* parent = (StepInternetDlg*)GetParent()->GetParent();
    parent->EffectiveParameters();

    m_effectiveExec   = m_step->GetEffectiveMaxExecution();
    m_effectiveBefore = m_step->GetEffectiveWaitBeforeRun();
    m_effectiveAfter  = m_step->GetEffectiveWaitAfterRun();

    UpdateData(FALSE);
  }
}

// TimingDlg message handlers

void 
TimingDlg::OnEnChangeWaitBeforerun()
{
  UpdateData();
  Effectuate();
}

void 
TimingDlg::OnEnChangeWaitafterrun()
{
  UpdateData();
  Effectuate();
}

void 
TimingDlg::OnBnClickedKillontimeout()
{
  m_killOnTimeout = m_buttonKillOnTimeout.GetCheck() > 0;
  UpdateData(FALSE);
}

void 
TimingDlg::OnEnChangeTimeout()
{
  UpdateData();
  Effectuate();
}

void 
TimingDlg::OnBnClickedBefore()
{
  SearchVarDlg dlg(m_parameters,true,true,true);
  if (dlg.DoModal() == IDOK || dlg.GetSaved())
  {
    CString var = dlg.GetChosenVariable();
    CString variable;
    switch (dlg.GetResultType())
    {
      case ParType::PAR_GLOBAL: variable.Format("$%s$", var.GetString()); break;
      case ParType::PAR_RETURN: variable.Format("[%s]", var.GetString()); break;
      case ParType::PAR_BUFFER: variable.Format("<%s>", var.GetString()); break;
    }
    m_editWaitBeforeRun.InsertAtCurPos(variable, 0);
    UpdateData();
    Effectuate();
  }
}

void 
TimingDlg::OnBnClickedExec()
{
  SearchVarDlg dlg(m_parameters,true,true,true);
  if (dlg.DoModal() == IDOK || dlg.GetSaved())
  {
    CString var = dlg.GetChosenVariable();
    CString variable;
    switch (dlg.GetResultType())
    {
      case ParType::PAR_GLOBAL: variable.Format("$%s$", var.GetString()); break;
      case ParType::PAR_RETURN: variable.Format("[%s]", var.GetString()); break;
      case ParType::PAR_BUFFER: variable.Format("<%s>", var.GetString()); break;
    }
    m_editMaxExecution.InsertAtCurPos(variable, 0);
    UpdateData();
    Effectuate();
  }
}

void 
TimingDlg::OnBnClickedAfter()
{
  SearchVarDlg dlg(m_parameters,true,true,true);
  if (dlg.DoModal() == IDOK || dlg.GetSaved())
  {
    CString var = dlg.GetChosenVariable();
    CString variable;
    switch (dlg.GetResultType())
    {
      case ParType::PAR_GLOBAL: variable.Format("$%s$", var.GetString()); break;
      case ParType::PAR_RETURN: variable.Format("[%s]", var.GetString()); break;
      case ParType::PAR_BUFFER: variable.Format("<%s>", var.GetString()); break;
    }
    m_editWaitAfterRun.InsertAtCurPos(variable, 0);
    UpdateData();
    Effectuate();
  }
}
