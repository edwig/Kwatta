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
// This File   : ParametersDlg.cpp
// What it does: Parameters for running a test step
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StepEditor.h"
#include "ParametersDlg.h"
#include "StepEditorDlg.h"
#include <TestStepCMD.h>
#include <SearchVarDlg.h>
#include <Parameters.h>
#include "afxdialogex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// ParametersDlg dialog

IMPLEMENT_DYNAMIC(ParametersDlg, StyleDialog)

ParametersDlg::ParametersDlg(CWnd* pParent /*=nullptr*/)
	            :StyleDialog(IDD_PARAMETERS, pParent)
{
}

ParametersDlg::~ParametersDlg()
{
}

void ParametersDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);

  DDX_Control(pDX, IDC_EFF_RUNTIMER, m_editEffRuntimer,m_effectiveRuntimer);
  DDX_Control(pDX, IDC_EFF_COMLINE,  m_editEffCommand, m_effectiveCommandline);
  DDX_Control(pDX, IDC_STARTWINDOW,  m_comboStartWindow);
  DDX_Control(pDX, IDC_WAITFORIDLE,  m_buttonWaitForIdle);
  DDX_Control(pDX, IDC_HANDLEENV,    m_comboEnvironment);
  DDX_Control(pDX, IDC_WAITFORRUN,   m_editWaitBeforeRun,m_waitBeforeRun);
  DDX_Control(pDX, IDC_WAITAFTERRUN, m_editWaitAfterRun, m_waitAfterRun);
  DDX_Control(pDX, IDC_KILLONTIMEOUT,m_buttonKillOnTimeout);
  DDX_Control(pDX, IDC_TIMEOUT,      m_editMaxExecution, m_maxExecution);
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

BEGIN_MESSAGE_MAP(ParametersDlg, StyleDialog)
  ON_EN_KILLFOCUS (IDC_EFF_RUNTIMER,  &ParametersDlg::OnEnChangeEffRuntimer)
  ON_EN_KILLFOCUS (IDC_EFF_COMLINE,   &ParametersDlg::OnEnChangeEffComline)
  ON_CBN_SELCHANGE(IDC_STARTWINDOW,   &ParametersDlg::OnCbnSelchangeStartwindow)
  ON_CBN_SELCHANGE(IDC_HANDLEENV,     &ParametersDlg::OnCbnSelchangeHandleenv)
  ON_EN_KILLFOCUS (IDC_WAITFORRUN,    &ParametersDlg::OnEnChangeWaitBeforerun)
  ON_BN_CLICKED   (IDC_WAITFORIDLE,   &ParametersDlg::OnBnClickedWaitforidle)
  ON_EN_KILLFOCUS (IDC_WAITAFTERRUN,  &ParametersDlg::OnEnChangeWaitafterrun)
  ON_BN_CLICKED   (IDC_KILLONTIMEOUT, &ParametersDlg::OnBnClickedKillontimeout)
  ON_EN_KILLFOCUS (IDC_TIMEOUT,       &ParametersDlg::OnEnChangeTimeout)
  ON_BN_CLICKED   (IDC_BUT_BEFORE,    &ParametersDlg::OnBnClickedBefore)
  ON_BN_CLICKED   (IDC_BUT_EXEC,      &ParametersDlg::OnBnClickedExec)
  ON_BN_CLICKED   (IDC_BUT_AFTER,     &ParametersDlg::OnBnClickedAfter)
END_MESSAGE_MAP()

BOOL
ParametersDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();

  // SW_HIDE etc
  m_comboStartWindow.AddString("Hidden window");
  m_comboStartWindow.AddString("Normal default Window");
  m_comboStartWindow.AddString("Minimized window");
  m_comboStartWindow.AddString("Maximized window");
  m_comboStartWindow.AddString("Show not-activated");

  // Environment
  m_comboEnvironment.AddString("Default");
  m_comboEnvironment.AddString("Add to environment");
  m_comboEnvironment.AddString("Replace environment");

  m_buttonBefore.SetIconImage(IDI_LIST);
  m_buttonAfter .SetIconImage(IDI_LIST);
  m_buttonExec  .SetIconImage(IDI_LIST);

  m_editEffWaitBefore.SetMutable(false);
  m_editEffWaitAfter .SetMutable(false);
  m_editEffWaitExec  .SetMutable(false);

  SetCanResize();
  return TRUE;
}

void
ParametersDlg::SetupDynamicLayout()
{
  StyleDialog::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif

  manager.AddItem(IDC_GRP_EFFECTIVE,CMFCDynamicLayout::MoveNone(),CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_EFF_RUNTIMER, CMFCDynamicLayout::MoveNone(),CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_EFF_COMLINE,  CMFCDynamicLayout::MoveNone(),CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_GRP_RUNNING,  CMFCDynamicLayout::MoveNone(),CMFCDynamicLayout::SizeHorizontalAndVertical(100,100));
}

void
ParametersDlg::InitTab(TestStepCMD* p_step,Parameters* p_param)
{
  // Remember the step
  m_step       = p_step;
  m_parameters = p_param;

  // Get all values
  m_startWindow       = m_step->GetStartWindow();
  m_waitForIdle       = m_step->GetWaitForIdle();
  m_handleEnvironment = m_step->GetHandleEnvironment();
  m_waitBeforeRun     = m_step->GetWaitBeforeRun();
  m_waitAfterRun      = m_step->GetWaitAfterRun();
  m_killOnTimeout     = m_step->GetKillOnTimeout();
  m_maxExecution      = m_step->GetMaxExecution();

  // Get compound values
  m_effectiveRuntimer    = m_step->GetEffectiveDirectory();
  m_effectiveCommandline = m_step->GetEffectiveCommandLine();
  if(m_effectiveRuntimer.Right(1) != '\\')
  {
    m_effectiveRuntimer.Append("\\");
  }
  m_effectiveRuntimer.Append(m_step->GetEffectiveRuntimer());

  m_comboStartWindow   .SetCurSel(m_startWindow);
  m_comboEnvironment   .SetCurSel(m_handleEnvironment);
  m_buttonWaitForIdle  .SetCheck(m_waitForIdle);
  m_buttonKillOnTimeout.SetCheck(m_killOnTimeout);

  Effectuate();
}

void
ParametersDlg::StoreVariables()
{
  m_step->SetStartWindow(m_startWindow);
  m_step->SetWaitForIdle(m_waitForIdle);
  m_step->SetHandleEnvironment(m_handleEnvironment);
  m_step->SetWaitBeforeRun(m_waitBeforeRun);
  m_step->SetWaitAfterRun(m_waitAfterRun);
  m_step->SetKillOnTimeout(m_killOnTimeout);
  m_step->SetMaxExecution(m_maxExecution);
}

void
ParametersDlg::Effectuate()
{
  if(m_step)
  {
    m_step->SetMaxExecution (m_maxExecution);
    m_step->SetWaitBeforeRun(m_waitBeforeRun);
    m_step->SetWaitAfterRun (m_waitAfterRun);

    StepEditorDlg* parent = (StepEditorDlg*)GetParent()->GetParent();
    parent->EffectiveParameters();

    m_effectiveExec   = m_step->GetEffectiveMaxExecution();
    m_effectiveBefore = m_step->GetEffectiveWaitBeforeRun();
    m_effectiveAfter  = m_step->GetEffectiveWaitAfterRun();

    UpdateData(FALSE);
  }
}

// ParametersDlg message handlers

void 
ParametersDlg::OnEnChangeEffRuntimer()
{
  UpdateData();
}

void 
ParametersDlg::OnEnChangeEffComline()
{
  UpdateData();
}

void 
ParametersDlg::OnCbnSelchangeStartwindow()
{
  int ind = m_comboStartWindow.GetCurSel();
  if (ind >= 0)
  {
    m_startWindow = ind;
  }
}

void 
ParametersDlg::OnCbnSelchangeHandleenv()
{
  int ind = m_comboEnvironment.GetCurSel();
  if (ind >= 0)
  {
    m_handleEnvironment = ind;
  }
}

void 
ParametersDlg::OnEnChangeWaitBeforerun()
{
  UpdateData();
  Effectuate();
}

void 
ParametersDlg::OnBnClickedWaitforidle()
{
  m_waitForIdle = m_buttonWaitForIdle.GetCheck() > 0;
  UpdateData(FALSE);
}

void 
ParametersDlg::OnEnChangeWaitafterrun()
{
  UpdateData();
  Effectuate();
}

void 
ParametersDlg::OnBnClickedKillontimeout()
{
  m_killOnTimeout = m_buttonKillOnTimeout.GetCheck() > 0;
  UpdateData(FALSE);
}

void 
ParametersDlg::OnEnChangeTimeout()
{
  UpdateData();
  Effectuate();
}

void
ParametersDlg::OnBnClickedBefore()
{
  SearchVarDlg dlg(this,m_parameters,true,true,true,true);
  if (dlg.DoModal() == IDOK || dlg.GetSaved())
  {
    CString variable = dlg.GetVariable();
    m_editWaitBeforeRun.InsertAtCurPos(variable, 0);
    UpdateData();
    Effectuate();
  }
}

void
ParametersDlg::OnBnClickedExec()
{
  SearchVarDlg dlg(this,m_parameters,true,true,true,true);
  if (dlg.DoModal() == IDOK || dlg.GetSaved())
  {
    CString variable = dlg.GetVariable();
    m_editMaxExecution.InsertAtCurPos(variable, 0);
    UpdateData();
    Effectuate();
  }
}

void
ParametersDlg::OnBnClickedAfter()
{
  SearchVarDlg dlg(this,m_parameters,true,true,true,true);
  if (dlg.DoModal() == IDOK || dlg.GetSaved())
  {
    CString variable = dlg.GetVariable();
    m_editWaitAfterRun.InsertAtCurPos(variable, 0);
    UpdateData();
    Effectuate();
  }
}
