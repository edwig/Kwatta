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
// This File   : MutateNames.dlg
// What it does: Mutates the name of the directory or the filename of the testset in the suite.
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MutateNamesDlg.h"
#include "KwattaApp.h"
#include "resource.h"
#include "ExtraExtensions.h"
#include <filesystem>

using namespace ThemeColor;

// MutateNamesDlg dialog

IMPLEMENT_DYNAMIC(MutateNamesDlg, StyleDialog)

MutateNamesDlg::MutateNamesDlg(CWnd* p_parent,CString p_directory,CString p_filename)
               :StyleDialog(IDD_MUTATENAMES,p_parent)
               ,m_origDirectory(p_directory)
               ,m_origFilename(p_filename)
{
}

MutateNamesDlg::~MutateNamesDlg()
{
}

void MutateNamesDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);

  DDX_Control(pDX,IDC_ORIG_DIR,  m_editOrigDirectory,m_origDirectory);
  DDX_Control(pDX,IDC_NEW_DIR,   m_editNewDirectory, m_newDirectory);
  DDX_Control(pDX,IDC_ORIG_FILE, m_editOrigFilename, m_origFilename);
  DDX_Control(pDX,IDC_NEW_FILE,  m_editNewFilename,  m_newFilename);

  DDX_Control(pDX,IDC_DO_DIR,    m_buttonDoDirectory);
  DDX_Control(pDX,IDC_DO_FILE,   m_buttonDoFilename);
  DDX_Control(pDX,IDOK,          m_buttonOK);
}


BEGIN_MESSAGE_MAP(MutateNamesDlg, StyleDialog)
  ON_EN_KILLFOCUS(IDC_ORIG_DIR,  OnEnKillFocusOrigDirectory)
  ON_EN_KILLFOCUS(IDC_NEW_DIR,   OnEnKillFocusNewDirectory)
  ON_EN_KILLFOCUS(IDC_ORIG_FILE, OnEnKillFocusOrigFilename)
  ON_EN_KILLFOCUS(IDC_NEW_FILE,  OnEnKillFocusNewFilename)
  ON_BN_CLICKED  (IDC_DO_DIR,    OnBnClickedDoDirectory)
  ON_BN_CLICKED  (IDC_DO_FILE,   OnBnClickedDoFilename)
  ON_BN_CLICKED  (IDOK,          OnBnClickedOK)
END_MESSAGE_MAP()

BOOL
MutateNamesDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();
  SetWindowText("Change directory or filename");

  m_editOrigDirectory.SetMutable(false);
  m_editOrigDirectory.SetBkColor(ThemeColor::GetColor(Colors::AccentColor2));
  m_editOrigFilename .SetMutable(false);
  m_editOrigFilename .SetBkColor(ThemeColor::GetColor(Colors::AccentColor2));

  m_newDirectory = m_origDirectory;
  m_newFilename  = m_origFilename;
  SetTestName();

  UpdateData(FALSE);
  m_editNewDirectory.SetFocus();
  return FALSE;
}

bool
MutateNamesDlg::CheckDirectory()
{
  if(m_newDirectory.IsEmpty())
  {
    m_editNewDirectory.SetErrorState(true);
    m_editNewDirectory.SetEmpty(true,"Fill in a test sub-directory!");
    return false;
  }
  m_editNewDirectory.SetErrorState(false);
  return true;
}

bool
MutateNamesDlg::CheckFilename()
{
  if(m_newFilename.IsEmpty())
  {
    m_editNewFilename.SetErrorState(true);
    m_editNewFilename.SetEmpty(true,"File in a testset name.");
    return false;
  }
  m_editNewFilename.SetErrorState(false);
  return true;
}

void 
MutateNamesDlg::SetTestName()
{
  int pos = m_origFilename.ReverseFind('.');
  if (pos > 0)
  {
    m_testName = m_origFilename.Left(pos);
  }
}

// MutateNamesDlg message handlers

void 
MutateNamesDlg::OnEnKillFocusOrigDirectory()
{
  UpdateData();
}

void 
MutateNamesDlg::OnEnKillFocusNewDirectory()
{
  UpdateData();
}

void 
MutateNamesDlg::OnEnKillFocusOrigFilename()
{
  UpdateData();
}

void 
MutateNamesDlg::OnEnKillFocusNewFilename()
{
  UpdateData();
}

void 
MutateNamesDlg::OnBnClickedDoDirectory()
{
  if(CheckDirectory())
  {
    CString ask;
    ask.Format("Do you want to change the directory for the testset from [%s] to [%s] ?",m_origDirectory.GetString(),m_newDirectory.GetString());
    if(StyleMessageBox(this,ask,KWATTA,MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION) == IDNO)
    {
      return;
    }

    CString oldname = theApp.GetBaseDirectory() + m_origDirectory;
    CString newname = theApp.GetBaseDirectory() + m_newDirectory;

    std::error_code error;
    std::filesystem::rename(oldname.GetString(),newname.GetString(),error);

    if(error.value() == 0)
    {
      m_origDirectory = m_newDirectory;
      UpdateData(FALSE);

      CString msg;
      msg.Format("The directory has been changed to: %s",m_origDirectory.GetString());
      StyleMessageBox(this,msg,KWATTA,MB_OK|MB_ICONEXCLAMATION);
    }
  }
}

void 
MutateNamesDlg::OnBnClickedDoFilename()
{
  if(CheckFilename())
  {
    CString ask;
    ask.Format("Do you want to change the file for the testset from [%s] to [%s] ?",m_origFilename.GetString(),m_newFilename.GetString());
    if(StyleMessageBox(this,ask,KWATTA,MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION) == IDNO)
    {
      return;
    }

    CString oldname = theApp.GetBaseDirectory() + m_origDirectory + "\\" + m_origFilename;
    CString newname = theApp.GetBaseDirectory() + m_origDirectory + "\\" + m_newFilename;

    std::error_code error;
    std::filesystem::rename(oldname.GetString(),newname.GetString(),error);

    if(error.value() == 0)
    {
      // Try to rename the parameters file as well!
      oldname.Replace(EXTENSION_SET,EXTENSION_PARAMETERS);
      newname.Replace(EXTENSION_SET,EXTENSION_PARAMETERS);
      std::filesystem::rename(oldname.GetString(),newname.GetString(),error);

      // Record the fact that we did the rename
      m_origFilename = m_newFilename;
      SetTestName();
      UpdateData(FALSE);

      CString msg;
      msg.Format("The filename has been changed to: %s",m_origFilename.GetString());
      StyleMessageBox(this,msg,KWATTA,MB_OK|MB_ICONEXCLAMATION);
    }
  }
}

void 
MutateNamesDlg::OnBnClickedOK()
{
  StyleDialog::OnOK();
}
