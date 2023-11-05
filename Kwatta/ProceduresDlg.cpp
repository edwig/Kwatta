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
// This program: TestSuite
// This File   : ProceduresDlg.cpp
// What it does: Start the suite global procedures (reset / reporting)
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "ProceduresDlg.h"
#include <ResetAll.h>
#include <ExecuteShell.h>
#include "resource.h"

// ProceduresDlg dialog

IMPLEMENT_DYNAMIC(ProceduresDlg, StyleDialog)

ProceduresDlg::ProceduresDlg(CWnd* p_parent,CString p_baseDirectory)
	            :StyleDialog(IDD_PROCEDURES,p_parent)
              ,m_baseDirectory(p_baseDirectory)
{
}

ProceduresDlg::~ProceduresDlg()
{
}

void ProceduresDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);

  DDX_Control(pDX,IDC_RESET_RESULTS,  m_buttonReset);
  DDX_Control(pDX,IDC_GENERATE_REPORT,m_buttonReport);
  DDX_Control(pDX,IDC_RESET_OUTCOME,  m_editResetOutcome,m_resetOutcome);
  DDX_Control(pDX,IDC_REPORT_OUTCOME, m_editReportOutcome,m_reportOutcome);
  DDX_CBIndex(pDX,IDC_DEPTH,          m_comboDepth,(int&)m_depth);
  DDX_Control(pDX,IDOK,               m_buttonOK);
  DDX_Control(pDX,IDCANCEL,           m_buttonCancel);
}

BEGIN_MESSAGE_MAP(ProceduresDlg, StyleDialog)
  ON_BN_CLICKED (IDC_RESET_RESULTS,  OnBnClickedReset)
  ON_BN_CLICKED (IDC_GENERATE_REPORT,OnBnClickedReport)
  ON_CBN_CLOSEUP(IDC_DEPTH,          OnCbnCloseupDepth)
END_MESSAGE_MAP()

BOOL
ProceduresDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();
  SetWindowText("Suite procedures");

  InitButtons();

  return FALSE;
}

void
ProceduresDlg::InitButtons()
{
  m_buttonOK.SetStyle("ok");
  m_buttonCancel.SetStyle("can");
  m_editReportOutcome.SetMutable(false);
  m_editResetOutcome.SetMutable(false);

  m_comboDepth.AddString("Only test sets");
  m_comboDepth.AddString("Include all test steps");
  m_comboDepth.AddString("Include all validations");
  m_comboDepth.AddString("Full info including output");

  m_comboDepth.SetCurSel(0);
}

// ProceduresDlg message handlers

void 
ProceduresDlg::OnBnClickedReset()
{
  if(StyleMessageBox(this,"Do you wish to reset all test results?",KWATTA,MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION) == IDNO)
  {
    return;
  }
  CWaitCursor takeAsigh;
  ResetAll all(m_baseDirectory,"","");
  all.Reset(true);

  m_resetOutcome = all.LastError();
  if(m_resetOutcome.IsEmpty())
  {
    m_resetOutcome = "OK";
  }
  else
  {
    if(m_resetOutcome.GetLength() > 30)
    {
      StyleMessageBox(this,m_resetOutcome,KWATTA,MB_OK|MB_ICONERROR);
    }
  }
  UpdateData(FALSE);
  m_purge = true;
}

void 
ProceduresDlg::OnBnClickedReport()
{
  CString reportName("Report.txt");

  CWaitCursor takeAsigh;
  TestReport report(m_baseDirectory,reportName,"KWATTA Test Report",m_depth);
  report.StartReport();

  ExecuteShell("open",m_baseDirectory + reportName,"",nullptr,true);
}

void 
ProceduresDlg::OnCbnCloseupDepth()
{
  int ind = m_comboDepth.GetCurSel();
  if (ind >= 0)
  {
    m_depth = (ReportDepth)ind;
  }
}
