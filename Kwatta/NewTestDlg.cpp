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
// This File   : NewTestDlg.cpp
// What it does: Create a new test set for a test suite
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "NewTestDlg.h"
#include "KwattaApp.h"
#include "afxdialogex.h"
#include <io.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// NewTestDlg dialog

IMPLEMENT_DYNAMIC(NewTestDlg, StyleDialog)

NewTestDlg::NewTestDlg(CWnd* pParent /*=nullptr*/)
           :StyleDialog(IDD_NEWTEST, pParent)
{
}

NewTestDlg::~NewTestDlg()
{
}

void NewTestDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_DIRECTORY,m_editDirectory,m_directory);
  DDX_Control(pDX,IDC_TESTNAME, m_editTestName, m_testName);
  DDX_Control(pDX,IDC_ACTIVE,   m_checkActive);
  DDX_Control(pDX,IDOK,         m_buttonOK);
  DDX_Control(pDX,IDCANCEL,     m_buttonCancel);

}

BEGIN_MESSAGE_MAP(NewTestDlg, StyleDialog)
  ON_EN_KILLFOCUS(IDC_DIRECTORY,OnKillfocusDirectory)
  ON_EN_KILLFOCUS(IDC_TESTNAME, OnKillfocusTestname)
  ON_BN_CLICKED  (IDC_ACTIVE,   OnBnClickedActive)
  ON_BN_CLICKED  (IDOK,         OnBnClickedOK)
END_MESSAGE_MAP()

BOOL
NewTestDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();
  SetWindowText(_T("Define a new test-set"));
  
  m_editDirectory.SetEmpty(true,_T("Fill in a directory name"));
  m_editTestName .SetEmpty(true,_T("Fill in a test set name"));
  m_checkActive  .SetCheck(1);

  UpdateData(FALSE);
  return TRUE;
}

bool
NewTestDlg::CheckInput()
{
  if(m_directory.IsEmpty() || m_testName.IsEmpty())
  {
    StyleMessageBox(this,_T("Fill in both the directory name and the name of the test."),_T("Kwatta"),MB_OK|MB_ICONERROR);
    return false;
  }

  CString path = theApp.GetBaseDirectoryClean();
  path += _T("\\");
  path += m_directory;
  if(_taccess(path,0) == 0)
  {
    StyleMessageBox(this,_T("The name of this directory already exists.\n")
                         _T("Please choose another directory name."),_T("Kwatta"),MB_OK|MB_ICONERROR);
    return false;
  }

  return true;
}

// NewTestDlg message handlers

void 
NewTestDlg::OnKillfocusDirectory()
{
  UpdateData();
}

void 
NewTestDlg::OnKillfocusTestname()
{
  UpdateData();
}

void 
NewTestDlg::OnBnClickedActive()
{
  m_active = m_checkActive.GetCheck() > 0;
}

void 
NewTestDlg::OnBnClickedOK()
{
  if(CheckInput())
  {
    OnOK();
  }
}
