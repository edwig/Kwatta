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
// This program: StepEditor
// This File   : NewVariableDlg.cpp
// What it does: Define a new local variable for a test step
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NewVariableDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#define KWATTA_NOAUTOLINK
#include "SuiteLibrary.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// NewVariableDlg dialog

IMPLEMENT_DYNAMIC(NewVariableDlg, StyleDialog)

NewVariableDlg::NewVariableDlg(ParType p_type,ParMap& p_map,CWnd* pParent /*=nullptr*/)
               :StyleDialog(IDD_NEWVAR, pParent)
               ,m_type(p_type)
               ,m_map(p_map)
{
}

NewVariableDlg::~NewVariableDlg()
{
}

void NewVariableDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_EXPLAIN, m_editExplain,m_explain);
  DDX_Control(pDX,IDC_NAME,    m_editNewName,m_newName);
  DDX_Control(pDX,IDC_EXISTING,m_comboExisting);
  DDX_Control(pDX,IDOK,        m_buttonOK);
  DDX_Control(pDX,IDCANCEL,    m_buttonCancel);
}

BEGIN_MESSAGE_MAP(NewVariableDlg, StyleDialog)
  ON_EN_KILLFOCUS(IDC_EXPLAIN, &NewVariableDlg::OnEnChangeExplain)
  ON_EN_KILLFOCUS(IDC_NAME,    &NewVariableDlg::OnEnChangeName)
  ON_BN_CLICKED  (IDOK,        &NewVariableDlg::OnBnClickedOk)
  ON_BN_CLICKED  (IDCANCEL,    &NewVariableDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

BOOL
NewVariableDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();
  SetWindowText(_T("New Variable"));

  switch(m_type)
  {
    case ParType::PAR_GLOBAL:  m_explain = _T("Define a new GLOBAL test suite parameter.\r\n")
                                           _T("Variable is visible throughout all tests."); break;
    case ParType::PAR_RETURN:  m_explain = _T("Define a new RETURN value parameter.\r\n")
                                           _T("Variable only visible in this test.");       break;
    case ParType::PAR_BUFFER:  m_explain = _T("Define a new BUFFER stream parameter.\r\n")
                                           _T("Variable only visible in this test.");       break;
  }

  m_buttonOK.SetStyle(_T("ok"));
  m_buttonCancel.SetStyle(_T("can"));

  FillCombo();
  UpdateData(FALSE);

  return TRUE;
}

void
NewVariableDlg::FillCombo()
{
  for(auto& var : m_map)
  {
    m_comboExisting.AddString(var.first);
  }
}

// NewVariableDlg message handlers

void 
NewVariableDlg::OnEnChangeExplain()
{
  // Do Nothing!
}

void 
NewVariableDlg::OnEnChangeName()
{
  UpdateData();

  CheckName();
}

bool
NewVariableDlg::CheckName()
{
  if(m_map.find(m_newName) != m_map.end())
  {
    XString error;
    error.Format(_T("The newly entered variable [%s] already exists"),m_newName.GetString());
    StyleMessageBox(this,error,_T(KWATTA),MB_OK|MB_ICONERROR);
    return false;
  }
  return true;
}

void
NewVariableDlg::SaveName()
{
  XString empty;
  m_map.insert(std::make_pair(m_newName,empty));
}

void 
NewVariableDlg::OnBnClickedOk()
{
  if(CheckName())
  {
    SaveName();
    StyleDialog::OnOK();
  }
}

void 
NewVariableDlg::OnBnClickedCancel()
{
  m_newName.Empty();
  StyleDialog::OnCancel();
}
