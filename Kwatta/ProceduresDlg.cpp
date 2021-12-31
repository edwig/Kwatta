// ProceduresDlg.cpp : implementation file
//
#include "StdAfx.h"
#include "afxdialogex.h"
#include "ProceduresDlg.h"
#include <ResetAll.h>
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
  DDX_Control(pDX,IDOK,               m_buttonOK);
  DDX_Control(pDX,IDCANCEL,           m_buttonCancel);
}

BEGIN_MESSAGE_MAP(ProceduresDlg, StyleDialog)
  ON_BN_CLICKED(IDC_RESET_RESULTS,   OnBnClickedReset)
  ON_BN_CLICKED(IDC_GENERATE_REPORT, OnBnClickedReport)
END_MESSAGE_MAP()

BOOL
ProceduresDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();
  SetWindowText("Suite procedures");

  m_buttonOK.SetStyle("ok");
  m_buttonCancel.SetStyle("can");
  m_editReportOutcome.SetMutable(false);
  m_editResetOutcome.SetMutable(false);

  return FALSE;
}

// ProceduresDlg message handlers

void 
ProceduresDlg::OnBnClickedReset()
{
  if(StyleMessageBox(this,"Do you wish to reset all test results?",KWATTA,MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION) == IDNO)
  {
    return;
  }
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
  StyleMessageBox(this,"Not implemented yet!",KWATTA,MB_OK);
}
