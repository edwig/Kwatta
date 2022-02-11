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
// This program: Kwatta
// This File   : SystemDlg.cpp
// What it does: System wide hidden parameters
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SystemDlg.h"
#include <FileDialog.h>
#include "resource.h"

// SystemDlg dialog

IMPLEMENT_DYNAMIC(SystemDlg, StyleDialog)

SystemDlg::SystemDlg(CWnd* p_parent,Parameters* p_parameters)
	        :StyleDialog(IDD_SYSTEM,p_parent)
          ,m_parameters(p_parameters)
{

}

SystemDlg::~SystemDlg()
{
}

void SystemDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);

  DDX_CBIndex(pDX,IDC_LOGLEVEL,m_comboLoglevel,m_loglevel);
  DDX_Control(pDX,IDC_LOGFILE, m_editLogfile,  m_logfile);
  DDX_Control(pDX,IDC_BUT_LOG, m_buttonLogfile);
  DDX_Control(pDX,IDC_HIDE_PWD,m_checkHidePwd);
  DDX_Control(pDX,IDC_HIDDEN,  m_editPassword,m_password);
  DDX_Control(pDX,IDC_SHOW,    m_buttonShow);
  DDX_Control(pDX,IDC_HELPLINK,m_help);
  DDX_Control(pDX,IDOK,        m_buttonOK);
  DDX_Control(pDX,IDCANCEL,    m_buttonCancel);
}

BEGIN_MESSAGE_MAP(SystemDlg, StyleDialog)
  ON_WM_TIMER()
  ON_CBN_CLOSEUP (IDC_LOGLEVEL, OnCbnCloseupLoglevel)
  ON_EN_KILLFOCUS(IDC_LOGFILE,  OnEnKillfocusLogfile)
  ON_BN_CLICKED  (IDC_BUT_LOG,  OnBnClickedLogfile)
  ON_BN_CLICKED  (IDC_HIDE_PWD, OnBnClickedHidePassword)
  ON_BN_CLICKED  (IDC_SHOW,     OnBnClickedShow)
  ON_BN_CLICKED  (IDOK,         OnBnClickedOK)
  ON_BN_CLICKED  (IDCANCEL,     OnBnClickedCancel)
END_MESSAGE_MAP()

BOOL
SystemDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();
  SetWindowText("System parameters");

  InitButtons();
  InitParameters();

  return FALSE;
}

void
SystemDlg::InitButtons()
{
  m_buttonLogfile.SetStyle("dir");
  m_buttonOK     .SetStyle("ok");
  m_buttonCancel .SetStyle("can");

  m_editPassword.SetMutable(false);
  m_editPassword.SetBkColor(ThemeColor::_Color2);

  m_comboLoglevel.AddString("No logging");                                          // HLL_NOLOG      0       // No logging is ever done
  m_comboLoglevel.AddString("Error logging only");                                  // HLL_ERRORS     1       // Only errors are logged
  m_comboLoglevel.AddString("Logging of HTTP actions");                             // HLL_LOGGING    2       // 1 + Logging of actions
  m_comboLoglevel.AddString("HTTP actions and SOAP bodies");                        // HLL_LOGBODY    3       // 2 + Logging of actions and soap bodies
  m_comboLoglevel.AddString("HTTP actions, SOAP bodies and tracing");               // HLL_TRACE      4       // 3 + Tracing of settings
  m_comboLoglevel.AddString("HTTP actions, SOAP bodies, tracing and HEX dumping");  // HLL_TRACEDUMP  5       // 4 + Tracing and HEX dumping of objects

  m_help.SetTipText("Use to set the KWATTA_PASSWORD variable with the 'setx /m' command!");
  m_help.SetURL("https://docs.microsoft.com/en-us/windows-server/administration/windows-commands/setx");
  m_help.SetColours(ThemeColor::_Color1,RGB(0,0,255));
}

void
SystemDlg::InitParameters()
{
  m_loglevel     = atoi(m_parameters->FindSystemParameter("Loglevel"));
  m_logfile      =      m_parameters->FindSystemParameter("Logfile");
  m_hidePassword = atoi(m_parameters->FindSystemParameter("HidePassword"));

  UpdateData(FALSE);
  m_comboLoglevel.SetCurSel(m_loglevel);
  m_checkHidePwd.SetCheck(m_hidePassword);
}

void
SystemDlg::SaveParameters()
{
  CString level;
  CString hide;
  level.Format("%d",m_loglevel);
  hide .Format("%d",m_hidePassword);

  m_parameters->OverwriteSystemParameter("Loglevel",    level);
  m_parameters->OverwriteSystemParameter("Logfile",     m_logfile);
  m_parameters->OverwriteSystemParameter("HidePassword",hide);

  m_parameters->WriteToXML(false,true);
}

bool
SystemDlg::Check()
{
  if(m_loglevel > 0 && m_logfile.IsEmpty())
  {
    StyleMessageBox(this,"Provide a logfile if you choose a loglevel > 'No logging'",KWATTA,MB_OK|MB_ICONERROR);
    m_editLogfile.SetErrorState(true);
    return false;
  }
  m_editLogfile.SetErrorState(false);
  return true;
}

// SystemDlg message handlers

void
SystemDlg::OnTimer(UINT_PTR p_id)
{
  if(p_id == 1)
  {
    KillTimer(1);
    m_password.Empty();
    UpdateData(FALSE);
  }
}

void 
SystemDlg::OnCbnCloseupLoglevel()
{
  int ind = m_comboLoglevel.GetCurSel();
  if(ind >= 0)
  {
    m_loglevel = ind;
    m_changed  = true;
  }
}

void 
SystemDlg::OnEnKillfocusLogfile()
{
  CString logfile = m_logfile;
  UpdateData();
  if(logfile.CompareNoCase(m_logfile))
  {
    m_changed = true;
    Check();
  }
}

void 
SystemDlg::OnBnClickedLogfile()
{
  DocFileDialog dlg(false,"Choose a logfile for HTTP actions","txt","",0,"Logfiles *.txt|*.txt|All files *.*|*.*");
  if(dlg.DoModal())
  {
    m_logfile = dlg.GetChosenFile();
    UpdateData(FALSE);
    m_changed = false;
  }
}

void 
SystemDlg::OnBnClickedHidePassword()
{
  m_hidePassword = m_checkHidePwd.GetCheck() ? 1 : 0;
  m_changed = true;
}

void
SystemDlg::OnBnClickedShow()
{
  m_password.GetEnvironmentVariable("KWATTA_PASSWORD");
  UpdateData(FALSE);
  SetTimer(1,3000,nullptr);
}

void 
SystemDlg::OnBnClickedOK()
{
  if(!Check())
  {
    return;
  }
  if(m_changed)
  {
    SaveParameters();
  }
  OnOK();
}

void 
SystemDlg::OnBnClickedCancel()
{
  if(m_changed)
  {
    if(StyleMessageBox(this,"You have made changes. Save the changes?",KWATTA,MB_YESNO|MB_DEFBUTTON1|MB_ICONQUESTION) == IDYES)
    {
      SaveParameters();
    }
  }
  OnCancel();
}
