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
// This File   : NewStepNamesDlg.cpp
// What it does: Defining the third step of the new teststep wizard
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TestEditor.h"
#include "afxdialogex.h"
#include "NewStepValiDlg.h"
#include <GlobalFileDlg.h>
#include <ExtraExtensions.h>
#include <io.h>

// NewStepValiDlg dialog

IMPLEMENT_DYNAMIC(NewStepValiDlg, StyleTab)

NewStepValiDlg::NewStepValiDlg(CWnd* pParent /*=nullptr*/)
               :StyleTab(IDD_NEWSTEP_VALI,pParent)
{
}

NewStepValiDlg::~NewStepValiDlg()
{
}

void 
NewStepValiDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleTab::DoDataExchange(pDX);

  DDX_Control(pDX,IDC_VALI_GLOBAL,m_buttonValiGlobal);
  DDX_Control(pDX,IDC_VALIDAT,    m_editName,     m_valiName);
  DDX_Control(pDX,IDC_VALI_FILE,  m_editFile, m_valiFile);
}


BEGIN_MESSAGE_MAP(NewStepValiDlg, StyleTab)
  ON_BN_CLICKED   (IDC_VALI_GLOBAL, &NewStepValiDlg::OnBnClickedValiGlobal)
  ON_EN_KILLFOCUS (IDC_VALIDAT,	    &NewStepValiDlg::OnEnChangeValiName)
  ON_EN_KILLFOCUS (IDC_VALI_FILE,	  &NewStepValiDlg::OnEnChangeValiFile)
END_MESSAGE_MAP()

bool
NewStepValiDlg::InitStyleTab(void* p_data)
{
  StepType stepType = *(static_cast<StepType*>(p_data));
  if(m_stepType != stepType)
  {
    m_stepType = stepType;
    ResetPage();
    UpdateData(FALSE);
  }
  return true;
}

void
NewStepValiDlg::SetupDynamicLayout()
{
  StyleTab::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif

  manager.AddItem(IDC_VALIDAT,    manager.MoveNone(),manager.SizeHorizontal(100));
  manager.AddItem(IDC_VALI_GLOBAL,manager.MoveHorizontal(100),manager.SizeNone());
  manager.AddItem(IDC_VALI_FILE,  manager.MoveNone(),manager.SizeHorizontal(100));
  manager.AddItem(IDC_GRP_VALI,   manager.MoveNone(),manager.SizeHorizontal(100));

  manager.Adjust();
}

bool
NewStepValiDlg::CheckStyleTab(void* p_data)
{
  if(m_valiName.IsEmpty() || m_valiFile.IsEmpty())
  {
    CString error(_T("Fill in both a validation name and a valid file name for the validation!"));
    if(m_valiName.IsEmpty())
    {
      m_editName.SetErrorState(true,error);
    }
    else if(m_valiFile.IsEmpty())
    {
      m_editFile.SetErrorState(true,error);
    }
    return false;
  }
  return true;
}

bool
NewStepValiDlg::SaveStyleTab(void* p_data)
{
  CheckValiFile();
  return true;
}

void
NewStepValiDlg::PresetFilenames()
{
  if(m_valiFile.IsEmpty() && !m_valiName.IsEmpty())
  {
    m_valiFile = m_valiName;
    CheckValiFile();
    UpdateData(FALSE);
  }
}

void
NewStepValiDlg::CheckValiFile()
{
  CString path = theApp.GetTestDirectory();
  CString file(StripExtension(m_valiFile));
  if(file.IsEmpty())
  {
    return;
  }
  switch(m_stepType)
  {
    case StepType::Step_command: file += EXTENSION_VALIDATE_CMD; break;
    case StepType::Step_http:    file += EXTENSION_VALIDATE_NET; break;
    case StepType::Step_sql:     file += EXTENSION_VALIDATE_SQL; break;
    case StepType::Step_win:     file += EXTENSION_VALIDATE_WIN; break;
  }
  path += file;

  if(_taccess(path,0) == 0)
  {
    m_editFile.SetErrorState(true,_T("This file already exists!. Choose another name!"));
  }
  else
  {
    m_valiFile = file;
    m_editFile.SetErrorState(false);
  }
  UpdateData(FALSE);
}

CString
NewStepValiDlg::StripExtension(CString p_filename)
{
  int pos = p_filename.ReverseFind('.');
  if(pos > 0)
  {
    return p_filename.Left(pos);
  }
  return p_filename;
}

void
NewStepValiDlg::ResetPage()
{
  // Reset the global state
  m_valiGlobal = false;
  m_valiName.Empty();
  m_valiFile.Empty();
  m_editName.SetBkColor(FRAME_DEFAULT_COLOR);
  m_editFile.SetBkColor(FRAME_DEFAULT_COLOR);
}

// NewStepValidationDlg message handlers

void
NewStepValiDlg::OnBnClickedValiGlobal()
{
  AutoFocus focus;
  GlobalFileDlg dlg(this,false,m_stepType,theApp.GetBaseDirectory());
  if(dlg.DoModal() == IDOK)
  {
    m_valiName   = StripExtension(dlg.GetChosenFile());
    m_valiFile   = dlg.GetChosenFile();
    m_valiGlobal = true;
    m_editName.SetBkColor(GLOBAL_COLOR);
    m_editFile.SetBkColor(GLOBAL_COLOR);
  }
  else
  {
    ResetPage();
  }
  UpdateData(FALSE);

}

void
NewStepValiDlg::OnEnChangeValiName()
{
  UpdateData();
  if(!m_valiName.IsEmpty())
  {
    m_editName.SetErrorState(false);
  }
  PresetFilenames();
}

void
NewStepValiDlg::OnEnChangeValiFile()
{
  UpdateData();
  CheckValiFile();
}
