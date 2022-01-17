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
// This File   : NewStepDlg.cpp
// What it does: Defining a new test step in the set
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TestEditor.h"
#include "NewStepDlg.h"
#include <ExtraExtensions.h>
#include "afxdialogex.h"
#include <io.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// NewStepDlg dialog

IMPLEMENT_DYNAMIC(NewStepDlg, StyleDialog)

NewStepDlg::NewStepDlg(CWnd* p_parent /*=nullptr*/,bool p_valiOnly /*=false*/)
           :StyleDialog(IDD_NEW_STEP,p_parent)
           ,m_valiOnly(p_valiOnly)
{
}

NewStepDlg::~NewStepDlg()
{
}

void NewStepDlg::DoDataExchange(CDataExchange* pDX)
{
  StyleDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_TESTTYPE, m_comboType);
  DDX_Control(pDX,IDC_NAME,     m_editName,     m_name);
  DDX_Control(pDX,IDC_FILENAME, m_editFilename, m_filename);
  DDX_Control(pDX,IDC_VALITYPE, m_comboVali);
  DDX_Control(pDX,IDC_VALIDAT,  m_editVali,     m_valiName);
  DDX_Control(pDX,IDC_VALI_FILE,m_editValiFile, m_valiFile);
  DDX_Control(pDX,IDOK,         m_buttonOK);
  DDX_Control(pDX,IDCANCEL,     m_buttonCancel);

  m_comboType   .EnableWindow(!m_valiOnly);
  m_editName    .EnableWindow(!m_valiOnly);
  m_editFilename.EnableWindow(!m_valiOnly);
  m_comboVali   .EnableWindow(m_valiOnly);
}

BEGIN_MESSAGE_MAP(NewStepDlg, StyleDialog)
  ON_CBN_SELCHANGE(IDC_TESTTYPE,	&NewStepDlg::OnCbnSelchangeTestType)
  ON_EN_KILLFOCUS (IDC_NAME,		  &NewStepDlg::OnEnChangeName)
  ON_EN_KILLFOCUS (IDC_FILENAME,	&NewStepDlg::OnEnChangeFilename)
  ON_CBN_SELCHANGE(IDC_VALITYPE,	&NewStepDlg::OnCbnSelchangeValiType)
  ON_EN_KILLFOCUS (IDC_VALIDAT,	  &NewStepDlg::OnEnChangeValiName)
  ON_EN_KILLFOCUS (IDC_VALI_FILE,	&NewStepDlg::OnEnChangeValiFile)
  ON_BN_CLICKED	  (IDOK,          &NewStepDlg::OnBnClickedOK)
END_MESSAGE_MAP()

BOOL
NewStepDlg::OnInitDialog()
{
	StyleDialog::OnInitDialog();
	SetWindowText("New test step");

	m_comboType.AddString("Command-line");
	m_comboType.AddString("Internet (HTTP)");
	m_comboType.SetCurSel(0);
  m_type = StepType::Step_command;

	m_comboVali.AddString("Command-line");
	m_comboVali.AddString("Internet (HTTP)");
	m_comboVali.SetCurSel(0);

	UpdateData(FALSE);
	return TRUE;
}

void
NewStepDlg::CheckFilename()
{
	CString path = theApp.GetTestDirectory();
	CString file(m_filename);
	if(file.Find('.') < 0)
	{
    switch (m_type)
    {
      case StepType::Step_command: file += EXTENSION_TESTSTEP_CL; break;
      case StepType::Step_http:    file += EXTENSION_TESTSTEP_IN; break;
    }
	}
	path += file;

	if(_access(path,0) == 0)
	{
		m_editFilename.SetErrorState(true,"This file already exists!. Choose another name!");
	}
	else
	{
    m_filename = file;
		m_editFilename.SetErrorState(false);
	}
}

void
NewStepDlg::CheckValiFile()
{
  CString path = theApp.GetTestDirectory();
  CString file(m_valiFile);
  if(file.Find('.') < 0)
  {
    file += m_valiType ? EXTENSION_VALIDATE_IN : EXTENSION_VALIDATE_CL;
  }
  path += file;

  if(_access(path,0) == 0)
  {
    m_editValiFile.SetErrorState(true,"This file already exists!. Choose another name!");
  }
  else
  {
    m_valiFile = file;
    m_editValiFile.SetErrorState(false);
  }
  UpdateData(FALSE);
}

// NewStepDlg message handlers

void 
NewStepDlg::OnCbnSelchangeTestType()
{
	int ind = m_comboType.GetCurSel();
	if(ind >= 0)
	{
		m_type = static_cast<StepType>(ind);
    m_valiType = ind;
    m_comboVali.SetCurSel(ind);
	}
	UpdateData();
}

void 
NewStepDlg::OnEnChangeName()
{
  UpdateData();
	if(m_filename.IsEmpty())
	{
		m_filename = m_name;
    CheckFilename();
    UpdateData(FALSE);
	}
}

void 
NewStepDlg::OnEnChangeFilename()
{
  UpdateData();
	CheckFilename();
}

void 
NewStepDlg::OnCbnSelchangeValiType()
{
  int ind = m_comboVali.GetCurSel();
  if(ind >= 0)
  {
    m_valiType = ind;
  }
  UpdateData();
}

void 
NewStepDlg::OnEnChangeValiName()
{
  UpdateData();
  if(m_valiFile.IsEmpty())
  {
		m_valiFile = m_valiName;
    CheckValiFile();
    UpdateData(FALSE);
  }
}

void 
NewStepDlg::OnEnChangeValiFile()
{
  UpdateData();
  CheckValiFile();
}

void
NewStepDlg::OnBnClickedOK()
{
  if(m_valiOnly)
  {
	  if(!m_valiName.IsEmpty() && !m_valiFile.IsEmpty())
	  {
		  OnOK();
      return;
	  }
  }
	if(!m_name    .IsEmpty() && !m_filename.IsEmpty() &&
		 !m_valiName.IsEmpty() && !m_valiFile.IsEmpty())
	{
		OnOK();
    return;
	}
	StyleMessageBox(this,"Provide a testname and a validation name!","ERROR",MB_OK|MB_ICONERROR);
}
