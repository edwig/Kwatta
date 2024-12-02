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
// This File   : NewSuiteDlg.cpp
// What it does: Creates a new test suite in a new and empty directory
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "NewSuiteDlg.h"
#include "Resource.h"
#include <FileDialog.h>
#include <MapDialog.h>
#include <TestSuite.h>
#include <ExtraExtensions.h>
#include <filesystem>

// NewSuiteDlg dialog

IMPLEMENT_DYNAMIC(NewSuiteDlg,StyleDialog)

NewSuiteDlg::NewSuiteDlg(CWnd* p_parent)
	          :StyleDialog(IDD_NEWSUITE,p_parent)
            ,m_init(false)
{
}

NewSuiteDlg::~NewSuiteDlg()
{
}

void NewSuiteDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);

  DDX_Control(pDX,IDC_EXPLAIN,   m_editExplain,  m_explain);
  DDX_Control(pDX,IDC_DIRECTORY, m_editDirectory,m_directory);
  DDX_Control(pDX,IDC_SUITENAME, m_editSuiteName,m_suiteName);
  DDX_Control(pDX,IDC_SEARCHDIR, m_buttonChooseDir);
  DDX_Control(pDX,IDC_SEARCHFILE,m_buttonChooseFile);
  DDX_Control(pDX,IDOK,          m_buttonOK);
  DDX_Control(pDX,IDCANCEL,      m_buttonCancel);
}

BEGIN_MESSAGE_MAP(NewSuiteDlg,StyleDialog)
  ON_EN_KILLFOCUS(IDC_DIRECTORY,  OnEnKillfocusDirectory)
  ON_EN_KILLFOCUS(IDC_SUITENAME,  OnEnKillfocusSuiteName)
  ON_BN_CLICKED  (IDC_SEARCHDIR,  OnBnClickedChooseDir)
  ON_BN_CLICKED  (IDC_SEARCHFILE, OnBnClickedChooseFile)
  ON_BN_CLICKED  (IDOK,           OnBnClickedOK)
  ON_BN_CLICKED  (IDCANCEL,       OnBnClickedCancel)
END_MESSAGE_MAP()

BOOL
NewSuiteDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();
  SetWindowText(_T("Create a new test suite"));

  m_buttonChooseDir.SetStyle(_T("dir"));
  m_buttonChooseFile.SetIconImage(IDI_NEW);
  m_buttonOK.SetStyle(_T("ok"));
  m_buttonCancel.SetStyle(_T("can"));
  m_editDirectory.SetEmpty(true,_T("Choose a new folder"));
  m_editSuiteName.SetEmpty(true,_T("Choose a new test suite name"));

  m_explain = _T("Create a new test suite by creating a new empty directory.\r\n")
              _T("The directory will only contain your <testsuite name>.xtest file.");
  UpdateData(FALSE);
  m_editDirectory.SetFocus();
  m_init = true;
  return FALSE;
}

CString 
NewSuiteDlg::GetNewTestsuiteFile()
{
  return m_directory + _T("\\") + m_suiteName;
}

bool 
NewSuiteDlg::CheckDirectory()
{
  if(m_directory.IsEmpty())
  {
    m_editDirectory.SetErrorState(true,_T("Choose a new directory!"));
    return false;
  }
  m_editDirectory.SetErrorState(false);

  if(std::filesystem::exists(m_directory.GetString()))
  {
    if(std::filesystem::is_empty(m_directory.GetString()))
    {
      return true;
    }
    m_editDirectory.SetErrorState(true,_T("You must choose/create an empty directory!"));
    return false;
  }
  else
  {
    std::filesystem::create_directory(m_directory.GetString());
  }
  return true;
}


bool
NewSuiteDlg::CheckSuiteName()
{
  if(m_suiteName.IsEmpty())
  {
    m_editSuiteName.SetErrorState(true);
    return false;
  }
  m_editSuiteName.SetErrorState(false);

  WinFile file(m_suiteName);
  CString extension = file.GetFilenamePartExtension();
  if(extension.IsEmpty())
  {
    m_suiteName += EXTENSION_SUITE;
    UpdateData(FALSE);
    return true;
  }

  if(extension.CompareNoCase(EXTENSION_SUITE) == 0)
  {
    return true;
  }

  m_editSuiteName.SetErrorState(true);
  return false;
}

bool
NewSuiteDlg::CreateEmptySuite()
{
  CString path = GetNewTestsuiteFile();
  TestSuite suite(m_directory);
  suite.SetFilename(path);
  return suite.WriteToXML(true);
}

// NewSuiteDlg message handlers

void
NewSuiteDlg::OnEnKillfocusExplain()
{
  UpdateData();
}

void 
NewSuiteDlg::OnEnKillfocusDirectory()
{
  UpdateData();
}

void 
NewSuiteDlg::OnEnKillfocusSuiteName()
{
  UpdateData();
  if(m_init)
  {
    CheckSuiteName();
  }
}

void 
NewSuiteDlg::OnBnClickedChooseDir()
{
  MapDialog dlg;
  if(dlg.Browse(GetSafeHwnd(),_T("Choose a new (empty) test suite directory"),_T(""),_T(""),false,true))
  {
    m_directory = dlg.GetPath();
    UpdateData(FALSE);
    CheckDirectory();
  }
}

void 
NewSuiteDlg::OnBnClickedChooseFile()
{
  AutoFocus focus;
  DocFileDialog dlg(GetSafeHwnd(),false,_T("Choose a new test suite filename"),EXTENSION_SUITE,_T(""),0,_T("Kwatta test suite *.xtest|*.xtest"));
  if(dlg.DoModal())
  {
    CString filename = dlg.GetChosenFile();
    WinFile file(filename);
    m_directory = file.GetFilenamePartDirectory();
    m_suiteName = file.GetFilenamePartFilename();
    m_directory = m_directory.TrimRight('\\');
    UpdateData(FALSE);
    CheckDirectory();
    CheckSuiteName();
  }
}

void 
NewSuiteDlg::OnBnClickedOK()
{
  UpdateData();
  if(!m_directory.IsEmpty() && !m_suiteName.IsEmpty())
  {
    if(CheckDirectory() && CheckSuiteName())
    {
      CreateEmptySuite();
      StyleDialog::OnOK();
      return;
    }
  }
  StyleMessageBox(this,_T("You must supply both a directory name and a testsuite name!"),_T(KWATTA),MB_OK|MB_ICONERROR);
}

void 
NewSuiteDlg::OnBnClickedCancel()
{
  m_directory.Empty();
  m_suiteName.Empty();
  StyleDialog::OnCancel();
}
