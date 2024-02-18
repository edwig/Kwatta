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
#include "resource.h"
#include <FileDialog.h>
#include <ExecuteProcess.h>
#include <RegistryManager.h>
#include <Crypto.h>

using namespace ThemeColor;

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
  DDX_Control(pDX,IDC_PWD,     m_buttonNew);
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
  ON_EN_KILLFOCUS(IDC_HIDDEN,   OnEnChangeHidden)
  ON_BN_CLICKED  (IDC_SHOW,     OnBnClickedShow)
  ON_BN_CLICKED  (IDC_PWD,      OnBnClickedNew)
  ON_BN_CLICKED  (IDOK,         OnBnClickedOK)
  ON_BN_CLICKED  (IDCANCEL,     OnBnClickedCancel)
END_MESSAGE_MAP()

BOOL
SystemDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();
  SetWindowText(_T("System parameters"));

  InitButtons();
  InitParameters();

  return FALSE;
}

void
SystemDlg::InitButtons()
{
  m_buttonLogfile.SetStyle(_T("dir"));
  m_buttonOK     .SetStyle(_T("ok"));
  m_buttonCancel .SetStyle(_T("can"));

  m_editPassword.SetMutable(false);
  m_editPassword.SetBkColor(ThemeColor::GetColor(Colors::AccentColor2));

  m_comboLoglevel.AddString(_T("No logging"));                                          // HLL_NOLOG      0       // No logging is ever done
  m_comboLoglevel.AddString(_T("Error logging only"));                                  // HLL_ERRORS     1       // Only errors are logged
  m_comboLoglevel.AddString(_T("Logging of HTTP actions"));                             // HLL_LOGGING    2       // 1 + Logging of actions
  m_comboLoglevel.AddString(_T("HTTP actions and SOAP bodies"));                        // HLL_LOGBODY    3       // 2 + Logging of actions and soap bodies
  m_comboLoglevel.AddString(_T("HTTP actions, SOAP bodies and tracing"));               // HLL_TRACE      4       // 3 + Tracing of settings
  m_comboLoglevel.AddString(_T("HTTP actions, SOAP bodies, tracing and HEX dumping"));  // HLL_TRACEDUMP  5       // 4 + Tracing and HEX dumping of objects

  m_help.SetURL(_T("https://docs.microsoft.com/en-us/windows-server/administration/windows-commands/setx"));
  m_help.SetColours(ThemeColor::GetColor(Colors::AccentColor1),RGB(0,0,255));
  m_help.SetTipText(_T("Use to set the KWATTA_PASSWORD variable with the 'setx /m' command!"));
}

void
SystemDlg::InitParameters()
{
  m_loglevel     = _ttoi(m_parameters->FindSystemParameter(_T("Loglevel")));
  m_logfile      =      m_parameters->FindSystemParameter(_T("Logfile"));
  m_hidePassword = _ttoi(m_parameters->FindSystemParameter(_T("HidePassword")));

  UpdateData(FALSE);
  m_comboLoglevel.SetCurSel(m_loglevel);
  m_checkHidePwd.SetCheck(m_hidePassword);
}

void
SystemDlg::SaveParameters()
{
  CString level;
  CString hide;
  level.Format(_T("%d"),m_loglevel);
  hide .Format(_T("%d"),m_hidePassword);

  m_parameters->OverwriteSystemParameter(_T("Loglevel"),    level);
  m_parameters->OverwriteSystemParameter(_T("Logfile"),     m_logfile);
  m_parameters->OverwriteSystemParameter(_T("HidePassword"),hide);

  m_parameters->WriteToXML(false,true);
}

bool
SystemDlg::Check()
{
  if(m_loglevel > 0 && m_logfile.IsEmpty())
  {
    StyleMessageBox(this,_T("Provide a logfile if you choose a loglevel > 'No logging'"),_T(KWATTA),MB_OK|MB_ICONERROR);
    m_editLogfile.SetErrorState(true);
    return false;
  }
  m_editLogfile.SetErrorState(false);
  return true;
}

void
SystemDlg::ChangePassword()
{
  bool localChange(false);
  bool globalChange(false);
  CString variable(KWATTA_PASSWORD);

  Crypto crypt;
  CString password = crypt.Encryption(m_password,_T(KWATTA_ENCRYPT));

  // Try to change
  localChange  = SetEnvironmentVariable      (variable,password);
  globalChange = SetGlobalEnvironmentVariable(variable,password);

  if(localChange && globalChange)
  {
    StyleMessageBox(this,_T("Password has been changed!"),_T(KWATTA),MB_OK|MB_ICONINFORMATION);
  }
  else if(localChange)
  {
    StyleMessageBox(this,_T("BEWARE: Password could only be changed for this program!\n")
                         _T("Try running this program as local administrator!")
                        ,_T(KWATTA),MB_OK|MB_ICONERROR);
  }
  else
  {
    StyleMessageBox(this,_T("Password could NOT be changed!"),_T(KWATTA),MB_OK|MB_ICONERROR);
  }
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
  AutoFocus focus;
  DocFileDialog dlg(GetSafeHwnd(),false,_T("Choose a logfile for HTTP actions"),_T("txt"),_T(""),0,_T("Logfiles *.txt|*.txt|All files *.*|*.*"));
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
SystemDlg::OnEnChangeHidden()
{
  UpdateData();
  if(m_editPassword.GetMutable())
  {
    ChangePassword();
  }
  m_editPassword.SetMutable(false);
  m_editPassword.SetPassword(false);
  m_password.Empty();
  UpdateData(FALSE);
}

void
SystemDlg::OnBnClickedShow()
{
  if(!m_password.GetEnvironmentVariable(_T(KWATTA_PASSWORD)))
  {
    m_password = GetGlobalEnvironmentVariable(_T(KWATTA_PASSWORD));
  }
  if(!m_password.IsEmpty())
  {
    Crypto crypt;
    m_password = crypt.Decryption(m_password,_T(KWATTA_ENCRYPT));
  }
  UpdateData(FALSE);
  SetTimer(1,3000,nullptr);
}

void
SystemDlg::OnBnClickedNew()
{
  m_password.Empty();
  m_editPassword.SetMutable(true);
  m_editPassword.SetPassword(true);
  UpdateData(FALSE);
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
    if(StyleMessageBox(this,_T("You have made changes. Save the changes?"),_T(KWATTA),MB_YESNO|MB_DEFBUTTON1|MB_ICONQUESTION) == IDYES)
    {
      SaveParameters();
    }
  }
  OnCancel();
}
