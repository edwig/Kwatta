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
#include <GlobalFileDlg.h>
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
  DDX_Control(pDX,IDC_TESTTYPE,   m_comboType);
  DDX_Control(pDX,IDC_STEP_GLOBAL,m_buttonStepGlobal);
  DDX_Control(pDX,IDC_NAME,       m_editName,     m_stepName);
  DDX_Control(pDX,IDC_FILENAME,   m_editFilename, m_stepFile);
  DDX_Control(pDX,IDC_VALITYPE,   m_comboVali);
  DDX_Control(pDX,IDC_VALI_GLOBAL,m_buttonValiGlobal);
  DDX_Control(pDX,IDC_VALIDAT,    m_editVali,     m_valiName);
  DDX_Control(pDX,IDC_VALI_FILE,  m_editValiFile, m_valiFile);
  DDX_Control(pDX,IDOK,           m_buttonOK);
  DDX_Control(pDX,IDCANCEL,       m_buttonCancel);

  m_comboType   .EnableWindow(!m_valiOnly);
  m_editName    .EnableWindow(!m_valiOnly);
  m_editFilename.EnableWindow(!m_valiOnly);
  m_comboVali   .EnableWindow(m_valiOnly);
}

BEGIN_MESSAGE_MAP(NewStepDlg, StyleDialog)
  ON_CBN_SELCHANGE(IDC_TESTTYPE,	  &NewStepDlg::OnCbnSelchangeTestType)
  ON_EN_KILLFOCUS (IDC_NAME,		    &NewStepDlg::OnEnChangeName)
  ON_BN_CLICKED   (IDC_STEP_GLOBAL, &NewStepDlg::OnBnClickedStepGlobal)
  ON_EN_KILLFOCUS (IDC_FILENAME,	  &NewStepDlg::OnEnChangeFilename)
  ON_CBN_SELCHANGE(IDC_VALITYPE,	  &NewStepDlg::OnCbnSelchangeValiType)
  ON_BN_CLICKED   (IDC_VALI_GLOBAL, &NewStepDlg::OnBnClickedValiGlobal)
  ON_EN_KILLFOCUS (IDC_VALIDAT,	    &NewStepDlg::OnEnChangeValiName)
  ON_EN_KILLFOCUS (IDC_VALI_FILE,	  &NewStepDlg::OnEnChangeValiFile)
  ON_BN_CLICKED	  (IDOK,            &NewStepDlg::OnBnClickedOK)
END_MESSAGE_MAP()

BOOL
NewStepDlg::OnInitDialog()
{
	StyleDialog::OnInitDialog();
	SetWindowText("New test step");

	m_comboType.AddString("Command-line");
	m_comboType.AddString("Internet (HTTP)");
	m_comboType.SetCurSel(0);
  m_stepType = StepType::Step_command;

	m_comboVali.AddString("Command-line");
	m_comboVali.AddString("Internet (HTTP)");
	m_comboVali.SetCurSel(0);

  m_buttonStepGlobal.SetIconImage(IDI_EARTH);
  m_buttonValiGlobal.SetIconImage(IDI_EARTH);

	UpdateData(FALSE);
	return TRUE;
}

void
NewStepDlg::CheckFilename()
{
	CString path = theApp.GetTestDirectory();
	CString file(m_stepFile);
	if(file.Find('.') < 0)
	{
    switch (m_stepType)
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
    m_stepFile = file;
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
    file += m_valiType == StepType::Step_http ? EXTENSION_VALIDATE_IN : EXTENSION_VALIDATE_CL;
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

CString
NewStepDlg::StripExtension(CString p_filename)
{
  int pos = p_filename.ReverseFind('.');
  if (pos > 0)
  {
    return p_filename.Left(pos);
  }
  return p_filename;
}

// NewStepDlg message handlers

void 
NewStepDlg::OnCbnSelchangeTestType()
{
	int ind = m_comboType.GetCurSel();
	if(ind >= 0)
	{
		m_stepType = static_cast<StepType>(ind);
    m_valiType = (StepType) ind;
    m_comboVali.SetCurSel(ind);
	}
	UpdateData();
}

void
NewStepDlg::OnBnClickedStepGlobal()
{
  GlobalFileDlg dlg(this,true,m_stepType,theApp.GetBaseDirectory());
  if(dlg.DoModal() == IDOK)
  {
    m_stepName   = StripExtension(dlg.GetChosenFile());
    m_stepFile   = dlg.GetChosenFile();
    m_stepGlobal = true;
    UpdateData(FALSE);

    m_editName    .SetBkColor(GLOBAL_COLOR);
    m_editFilename.SetBkColor(GLOBAL_COLOR);
  }
}

void 
NewStepDlg::OnEnChangeName()
{
  UpdateData();
	if(m_stepFile.IsEmpty())
	{
		m_stepFile = m_stepName;
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
    m_valiType = (StepType)ind;
    m_stepType = (StepType)ind;
    m_comboType.SetCurSel(ind);
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
NewStepDlg::OnBnClickedValiGlobal()
{
  GlobalFileDlg dlg(this,false,m_stepType,theApp.GetBaseDirectory());
  if(dlg.DoModal() == IDOK)
  {
    m_valiName   = StripExtension(dlg.GetChosenFile());
    m_valiFile   = dlg.GetChosenFile();
    m_valiGlobal = true;
    UpdateData(FALSE);

    m_editVali    .SetBkColor(GLOBAL_COLOR);
    m_editValiFile.SetBkColor(GLOBAL_COLOR);
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
  	StyleMessageBox(this,"Provide a validation name!","ERROR",MB_OK|MB_ICONERROR);
    return;
  }
	if(!m_stepName.IsEmpty() && !m_stepFile.IsEmpty() &&
		 !m_valiName.IsEmpty() && !m_valiFile.IsEmpty())
	{
		OnOK();
    return;
	}
	StyleMessageBox(this,"Provide a teststep name and a validation name!","ERROR",MB_OK|MB_ICONERROR);
}
