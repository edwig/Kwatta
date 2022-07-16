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
// This File   : MutateDlg.cpp
// What it does: Change the underlying filename of a teststep or a validation
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TestEditor.h"
#include "MutateDlg.h"
#include <ExtraExtensions.h>
#include <EnsureFile.h>
#include <filesystem>

using namespace ThemeColor;

// MutateDlg dialog

IMPLEMENT_DYNAMIC(MutateDlg, StyleDialog)

MutateDlg::MutateDlg(CWnd* p_parent,CString p_object,CString p_filename)
	        :StyleDialog(IDD_MUTATE,p_parent)
          ,m_object(p_object)
          ,m_orgFilename(p_filename)
{
  m_newFilename = m_orgFilename;
}

MutateDlg::~MutateDlg()
{
}

void MutateDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);

  DDX_Control(pDX,IDC_ORG_FILENAME,m_editOrgFilename,m_orgFilename);
  DDX_Control(pDX,IDC_NEW_FILENAME,m_editNewFilename,m_newFilename);
  DDX_Control(pDX,IDC_CHANGE,      m_buttonChange);
  DDX_Control(pDX,IDOK,            m_buttonOK);
}

BEGIN_MESSAGE_MAP(MutateDlg, StyleDialog)
  ON_EN_KILLFOCUS(IDC_ORG_FILENAME, OnEnKillfocusOrgFilename)
  ON_EN_KILLFOCUS(IDC_NEW_FILENAME, OnEnKillfocusNewFilename)
  ON_BN_CLICKED  (IDC_CHANGE,       OnBnClickedChange)
  ON_BN_CLICKED  (IDOK,             OnBnClickedOK)
END_MESSAGE_MAP()

BOOL
MutateDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();
  SetWindowText("Change filename for: " + m_object);

  m_editOrgFilename.SetMutable(false);
  m_editOrgFilename.SetBkColor(ThemeColor::GetColor(Colors::AccentColor2));
  m_editNewFilename.SetFocus();
  return FALSE;
}

bool
MutateDlg::CheckFilenames()
{
  if(m_newFilename.IsEmpty())
  {
    m_editNewFilename.SetErrorState(true);
    m_editNewFilename.SetEmpty(true,"Fill in a NEW (not existing) filename!");
    return false;
  }
  EnsureFile ensure;
  CString orgExtension = ensure.ExtensionPart(m_orgFilename);
  CString newExtension = ensure.ExtensionPart(m_newFilename);
  if(orgExtension.CompareNoCase(newExtension))
  {
    m_editNewFilename.SetErrorState(true);
    m_editNewFilename.SetEmpty(true,"You cannot change the extension of the new filename!");
    return false;
  }
  m_editNewFilename.SetErrorState(false);
  return true;
}

CString 
MutateDlg::GetFilename()
{
  return m_orgFilename;
}

// MutateDlg message handlers

void
MutateDlg::OnEnKillfocusOrgFilename()
{
  UpdateData();
}

void 
MutateDlg::OnEnKillfocusNewFilename()
{
  CString filename = m_newFilename;
  UpdateData();
  if(filename.Compare(m_newFilename))
  {
    CheckFilenames();
  }
}

void 
MutateDlg::OnBnClickedChange()
{
  // Basic tests for the new filename
  if(!CheckFilenames())
  {
    return;
  }
  if(m_orgFilename.CompareNoCase(m_newFilename) == 0)
  {
    StyleMessageBox(this,"Specify a DIFFERENT filename!",KWATTA,MB_OK|MB_ICONERROR);
    return;
  }
  CString ask;
  ask.Format("Do you want to rename %s [%s] to [%s] ?",m_object.GetString(),m_orgFilename.GetString(),m_newFilename.GetString());
  if(StyleMessageBox(this,ask,KWATTA,MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION) == IDYES)
  {
    CString oldfile = theApp.GetBaseDirectory() + theApp.GetTestDirectory() + m_orgFilename;
    CString newfile = theApp.GetBaseDirectory() + theApp.GetTestDirectory() + m_newFilename;

    std::error_code error;
    std::filesystem::rename(oldfile.GetString(),newfile.GetString(),error);
    if(error.value() == 0)
    {
      if(m_object.CompareNoCase("testset") == 0)
      {
        oldfile.Replace(EXTENSION_TESTSTEP_CMD,EXTENSION_RESULT_CMD);
        oldfile.Replace(EXTENSION_TESTSTEP_NET,EXTENSION_RESULT_NET);
        oldfile.Replace(EXTENSION_TESTSTEP_SQL,EXTENSION_RESULT_SQL);
        newfile.Replace(EXTENSION_TESTSTEP_CMD,EXTENSION_RESULT_CMD);
        newfile.Replace(EXTENSION_TESTSTEP_NET,EXTENSION_RESULT_NET);
        newfile.Replace(EXTENSION_TESTSTEP_SQL,EXTENSION_RESULT_SQL);
        std::filesystem::rename(oldfile.GetString(),newfile.GetString(),error);
      }
      // Change
      m_orgFilename = m_newFilename;
      UpdateData(FALSE);
    }
  }
}

void 
MutateDlg::OnBnClickedOK()
{
  OnOK();
}
