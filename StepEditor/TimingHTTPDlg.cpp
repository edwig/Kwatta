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
// This File   : TimingHTTPDlg.cpp
// What it does: Parameters for running a internet test step
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StepEditor.h"
#include "StepInternetDlg.h"
#include "StepDatabaseDlg.h"
#include "TimingHTTPDlg.h"
#include <TestStepNET.h>
#include <SearchVarDlg.h>
#include <Parameters.h>
#include <afxdialogex.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// TimingHTTPDlg dialog

IMPLEMENT_DYNAMIC(TimingHTTPDlg,StyleTab)

TimingHTTPDlg::TimingHTTPDlg(CWnd* pParent /*=nullptr*/)
	            :StyleTab(IDD_TIMING_HTTP, pParent)
{
}

TimingHTTPDlg::~TimingHTTPDlg()
{
}

void TimingHTTPDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleTab::DoDataExchange(pDX);

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
  DDX_Control(pDX, IDC_TO_RESOLVE,   m_editTOResolve,    m_timeoutResolve);
  DDX_Control(pDX, IDC_TO_CONNECT,   m_editTOConnect,    m_timeoutConnect);
  DDX_Control(pDX, IDC_TO_SEND,      m_editTOSend,       m_timeoutSend);
  DDX_Control(pDX, IDC_TO_RECEIVE,   m_editTOReceive,    m_timeoutReceive);

  if(pDX->m_bSaveAndValidate == FALSE)
  {
    m_editMaxExecution.EnableWindow(m_killOnTimeout);
    m_buttonExec      .EnableWindow(m_killOnTimeout);
    m_editEffWaitExec .EnableWindow(m_killOnTimeout);
  }
}

BEGIN_MESSAGE_MAP(TimingHTTPDlg, StyleTab)
  ON_EN_KILLFOCUS (IDC_WAITFORRUN,    &TimingHTTPDlg::OnEnChangeWaitBeforerun)
  ON_EN_KILLFOCUS (IDC_WAITAFTERRUN,  &TimingHTTPDlg::OnEnChangeWaitafterrun)
  ON_BN_CLICKED   (IDC_KILLONTIMEOUT, &TimingHTTPDlg::OnBnClickedKillontimeout)
  ON_EN_KILLFOCUS (IDC_TIMEOUT,       &TimingHTTPDlg::OnEnChangeTimeout)
  ON_BN_CLICKED   (IDC_BUT_BEFORE,    &TimingHTTPDlg::OnBnClickedBefore)
  ON_BN_CLICKED   (IDC_BUT_EXEC,      &TimingHTTPDlg::OnBnClickedExec)
  ON_BN_CLICKED   (IDC_BUT_AFTER,     &TimingHTTPDlg::OnBnClickedAfter)
  ON_EN_KILLFOCUS (IDC_TO_RESOLVE,    &TimingHTTPDlg::OnEnChangeTOResolve)
  ON_EN_KILLFOCUS (IDC_TO_CONNECT,    &TimingHTTPDlg::OnEnChangeTOConnect)
  ON_EN_KILLFOCUS (IDC_TO_SEND,       &TimingHTTPDlg::OnEnChangeTOSend)
  ON_EN_KILLFOCUS (IDC_TO_RECEIVE,    &TimingHTTPDlg::OnEnChangeTOReceive)
END_MESSAGE_MAP()

BOOL
TimingHTTPDlg::OnInitDialog()
{
  StyleTab::OnInitDialog();
  
  m_buttonBefore.SetIconImage(IDI_LIST);
  m_buttonAfter .SetIconImage(IDI_LIST);
  m_buttonExec  .SetIconImage(IDI_LIST);

  m_editEffWaitBefore.SetMutable(false);
  m_editEffWaitAfter .SetMutable(false);
  m_editEffWaitExec  .SetMutable(false);

  return TRUE;
}

void
TimingHTTPDlg::InitTab(TestStep* p_step,Parameters* p_param)
{
  // Remember the step
  m_step       = p_step;
  m_parameters = p_param;

  // Get all values
  m_waitBeforeRun     = m_step->GetWaitBeforeRun();
  m_waitAfterRun      = m_step->GetWaitAfterRun();
  m_killOnTimeout     = m_step->GetKillOnTimeout();
  m_maxExecution      = m_step->GetMaxExecution();

  TestStepNET* net = reinterpret_cast<TestStepNET*>(p_step);
  if(net)
  {
    m_timeoutResolve = net->GetTimeoutResolve();
    m_timeoutConnect = net->GetTimeoutConnect();
    m_timeoutSend    = net->GetTimeoutSend();
    m_timeoutReceive = net->GetTimeoutReceive();
  }
  m_buttonKillOnTimeout.SetCheck(m_killOnTimeout);
  Effectuate();
}

bool
TimingHTTPDlg::IsFilled()
{
  return m_killOnTimeout;
}

void
TimingHTTPDlg::StoreVariables()
{
  if(m_step)
  {
    m_step->SetWaitBeforeRun(m_waitBeforeRun);
    m_step->SetWaitAfterRun(m_waitAfterRun);
    m_step->SetKillOnTimeout(m_killOnTimeout);
    m_step->SetMaxExecution(m_maxExecution);

    TestStepNET* net = reinterpret_cast<TestStepNET*>(m_step);
    if(net)
    {
      net->SetTimeoutResolve(m_timeoutResolve);
      net->SetTimeoutConnect(m_timeoutConnect);
      net->SetTimeoutSend   (m_timeoutSend);
      net->SetTimeoutReceive(m_timeoutReceive);
    }
  }
}

void
TimingHTTPDlg::Effectuate()
{
  if(m_step)
  {
    m_step->SetMaxExecution (m_maxExecution);
    m_step->SetWaitBeforeRun(m_waitBeforeRun);
    m_step->SetWaitAfterRun (m_waitAfterRun);

    StepInternetDlg* parent = dynamic_cast<StepInternetDlg*>(GetParent()->GetParent());
    if(parent)
    {
      parent->EffectiveParameters();
    }
    else
    {
      StepDatabaseDlg* parent = dynamic_cast<StepDatabaseDlg*>(GetParent()->GetParent());
      if(parent)
      {
        parent->EffectiveParameters();
      }
    }

    m_effectiveExec   = m_step->GetEffectiveMaxExecution();
    m_effectiveBefore = m_step->GetEffectiveWaitBeforeRun();
    m_effectiveAfter  = m_step->GetEffectiveWaitAfterRun();

    UpdateData(FALSE);
  }
}

// TimingHTTPDlg message handlers

void 
TimingHTTPDlg::OnEnChangeWaitBeforerun()
{
  UpdateData();
  Effectuate();
}

void 
TimingHTTPDlg::OnEnChangeWaitafterrun()
{
  UpdateData();
  Effectuate();
}

void 
TimingHTTPDlg::OnBnClickedKillontimeout()
{
  m_killOnTimeout = m_buttonKillOnTimeout.GetCheck() > 0;
  UpdateData(FALSE);
}

void 
TimingHTTPDlg::OnEnChangeTimeout()
{
  UpdateData();
  Effectuate();
}

void 
TimingHTTPDlg::OnBnClickedBefore()
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
TimingHTTPDlg::OnBnClickedExec()
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
TimingHTTPDlg::OnBnClickedAfter()
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

void
TimingHTTPDlg::OnEnChangeTOResolve()
{
  UpdateData();
  m_editTOConnect.SetFocus();
}

void
TimingHTTPDlg::OnEnChangeTOConnect()
{
  UpdateData();
  m_editTOSend.SetFocus();
}

void
TimingHTTPDlg::OnEnChangeTOSend()
{
  UpdateData();
  m_editTOReceive.SetFocus();
}

void
TimingHTTPDlg::OnEnChangeTOReceive()
{
  UpdateData();
}
