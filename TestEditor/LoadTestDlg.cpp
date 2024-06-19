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
// This program: TestEditor
// This File   : LoadTestDlg.cpp
// What it does: Enter and run a loadtest for the current teststep
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LoadTestDlg.h"
#include "TestEditor.h"

IMPLEMENT_DYNAMIC(LoadTestDlg, StyleDialog);

LoadTestDlg::LoadTestDlg(CWnd*     p_parent
                        ,CString   p_name
                        ,CString   p_filename
                        ,bool      p_global
                        ,TSValSet* p_valset
                        ,HWND      p_report
                        ,int       p_row)
            :StyleDialog(IDD_LOADTEST,p_parent)
            ,m_name(p_name)
            ,m_testfilename(p_filename)
            ,m_global(p_global)
            ,m_validationSet(p_valset)
            ,m_reportTo(p_report)
            ,m_row(p_row)
{
}

LoadTestDlg::~LoadTestDlg()
{
}

void 
LoadTestDlg::DoDataExchange(CDataExchange* pDX)
{
  DDX_Control(pDX, IDC_NAME,     m_editName,      m_name);
  DDX_Control(pDX,IDC_PARALLEL,  m_editParallel,  m_parallel);
  DDX_Control(pDX,IDC_PERSECOND, m_editPerSecond, m_persecond);
  DDX_Control(pDX,IDC_RUNTIME,   m_editSeconds,   m_seconds);
  DDX_Control(pDX,IDC_TOTAL,     m_editTotal,     m_totalcalls);
  DDX_Control(pDX,IDOK,          m_buttonOK);
  DDX_Control(pDX,IDCANCEL,      m_buttonCancel);
  DDX_Control(pDX,IDC_PROGRESS,  m_progress);
}

BEGIN_MESSAGE_MAP(LoadTestDlg, StyleDialog)
  ON_WM_TIMER()
  ON_EN_KILLFOCUS(IDC_PARALLEL, OnEditParallel)
  ON_EN_KILLFOCUS(IDC_PERSECOND,OnEditPerSecond)
  ON_EN_KILLFOCUS(IDC_RUNTIME,  OnEditSeconds)
  ON_EN_KILLFOCUS(IDC_TOTAL,    OnEditTotal)
  ON_BN_CLICKED  (IDOK,         OnButtonOK)
  ON_BN_CLICKED  (IDCANCEL,     OnButtonCancel)
END_MESSAGE_MAP()

BOOL
LoadTestDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();
  SetWindowText(_T("Load testing"));

  m_editTotal.SetMutable(false);
  m_buttonOK.SetStyle(_T("ok"));
  m_buttonCancel.SetStyle(_T("can"));

  SetTotal();
  return TRUE;
}

void
LoadTestDlg::SetTotal()
{
  // Test for illegal ranges
  if(m_parallel  <= 1) m_parallel  = 2;
  if(m_persecond <= 1) m_persecond = 5;
  if(m_seconds   <= 1) m_seconds   = 10;

  // Pre-calculate the total number of calls
  m_totalcalls = m_parallel * m_persecond * m_seconds;
  UpdateData(FALSE);
}

// Handlers

void
LoadTestDlg::OnTimer(UINT_PTR nID)
{
  if(nID == 1)
  {
    for(int index = 0;index < m_parallel; ++index)
    {
      int res = theApp.StartTestRunner(m_testfilename,m_global,m_validationSet,m_reportTo,m_row,false);
      --m_totalcalls;
    }
    m_progress.StepIt();
    if(m_totalcalls <= 0)
    {
      KillTimer(1);
      SetTotal();
    }
  }
  UpdateData(FALSE);
}

void
LoadTestDlg::OnEditParallel()
{
  UpdateData();
  SetTotal();
}

void
LoadTestDlg::OnEditPerSecond()
{
  UpdateData();
  SetTotal();
}

void
LoadTestDlg::OnEditSeconds()
{
  UpdateData();
  SetTotal();
}

void
LoadTestDlg::OnEditTotal()
{
  UpdateData();
}

void 
LoadTestDlg::OnButtonOK()
{
  // Check double OK pressed
  SetTotal();

  // Preset the progress bar
  m_progress.SetRange(0,m_persecond * m_seconds);
  m_progress.SetPos(0);
  m_progress.SetStep(1);

  // Start the test by setting the timer
  unsigned milli = CLOCKS_PER_SEC / m_persecond;
  SetTimer(1,milli,nullptr);
}

void 
LoadTestDlg::OnButtonCancel()
{
  OnCancel();
}
