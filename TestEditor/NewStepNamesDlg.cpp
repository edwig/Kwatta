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
// What it does: Defining the second step of the new teststep wizard
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TestEditor.h"
#include "TestEditorDlg.h"
#include "afxdialogex.h"
#include "NewStepNamesDlg.h"
#include <ExtraExtensions.h>
#include <GlobalFileDlg.h>
#include <io.h>

// NewStepNamesDlg dialog

IMPLEMENT_DYNAMIC(NewStepNamesDlg,StyleTab)

NewStepNamesDlg::NewStepNamesDlg(CWnd* pParent /*=nullptr*/)
                :StyleTab(IDD_NEWSTEP_NAMES,pParent)
{

}

NewStepNamesDlg::~NewStepNamesDlg()
{
}

void NewStepNamesDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleTab::DoDataExchange(pDX);

  DDX_Control(pDX,IDC_STEP_GLOBAL,m_buttonStepGlobal);
  DDX_Control(pDX,IDC_NAME,       m_editName,    m_stepName);
  DDX_Control(pDX,IDC_FILENAME,   m_editFile,m_stepFile);

}

BEGIN_MESSAGE_MAP(NewStepNamesDlg, StyleTab)
  ON_EN_KILLFOCUS (IDC_NAME,		    &NewStepNamesDlg::OnEnChangeName)
  ON_BN_CLICKED   (IDC_STEP_GLOBAL, &NewStepNamesDlg::OnBnClickedStepGlobal)
  ON_EN_KILLFOCUS (IDC_FILENAME,	  &NewStepNamesDlg::OnEnChangeFilename)
END_MESSAGE_MAP()

void
NewStepNamesDlg::SetupDynamicLayout()
{
  StyleTab::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif

  manager.AddItem(IDC_NAME,       manager.MoveNone(),manager.SizeHorizontal(100));
  manager.AddItem(IDC_STEP_GLOBAL,manager.MoveHorizontal(100),manager.SizeNone());
  manager.AddItem(IDC_FILENAME,   manager.MoveNone(),manager.SizeHorizontal(100));
  manager.AddItem(IDC_GRP,        manager.MoveNone(),manager.SizeHorizontal(100));

  manager.Adjust();
}

bool
NewStepNamesDlg::InitStyleTab(void* p_data)
{
  NewStepInfo* info= static_cast<NewStepInfo*>(p_data);
  if(m_stepType != info->m_type)
  {
    m_stepType = info->m_type;
    ResetPage();
    UpdateData(FALSE);
  }
  return true;
}

bool
NewStepNamesDlg::CheckStyleTab(void* p_data)
{
  if(m_stepName.IsEmpty() || m_stepFile.IsEmpty())
  {
    CString error(_T("Fill in both a step name and a valid test step file name!"));
    if(m_stepName.IsEmpty())
    {
      m_editName.SetErrorState(true,error);
    }
    else if(m_stepFile.IsEmpty())
    {
      m_editFile.SetErrorState(true,error);
    }
    return false;
  }
  m_editName.SetErrorState(false);
  m_editFile.SetErrorState(false);
  
  // Save the state
  (static_cast<NewStepInfo*>(p_data))->m_type = m_stepType;
  CString defaultName = StripExtension(m_stepFile);
  (static_cast<NewStepInfo*>(p_data))->m_default = defaultName;

  return true;
}

bool
NewStepNamesDlg::SaveStyleTab(void* p_data)
{
  // Nothing to be done
  return true;
}

void
NewStepNamesDlg::PresetFilenames()
{
  if(m_stepFile.IsEmpty())
  {
    m_stepFile = m_stepName;
    CheckFilename();
    UpdateData(FALSE);
  }
}

void
NewStepNamesDlg::CheckFilename()
{
  CString path = theApp.GetTestDirectory();
  CString file(StripExtension(m_stepFile));
  if(file.IsEmpty())
  {
    return;
  }
  switch(m_stepType)
  {
    case StepType::Step_command: file += EXTENSION_TESTSTEP_CMD; break;
    case StepType::Step_http:    file += EXTENSION_TESTSTEP_NET; break;
    case StepType::Step_sql:     file += EXTENSION_TESTSTEP_SQL; break;
    case StepType::Step_win:     file += EXTENSION_TESTSTEP_WIN; break;
  }
  path += file;

  if(_taccess(path,0) == 0)
  {
    m_editFile.SetErrorState(true,_T("This file already exists!. Choose another name!"));
  }
  else
  {
    m_stepFile = file;
    m_editFile.SetErrorState(false);
  }
  UpdateData(FALSE);
}

CString
NewStepNamesDlg::StripExtension(CString p_filename)
{
  int pos = p_filename.ReverseFind('.');
  if(pos > 0)
  {
    return p_filename.Left(pos);
  }
  return p_filename;
}

void
NewStepNamesDlg::ResetPage()
{
  // Reset the global state
  m_stepGlobal = false;
  m_stepName.Empty();
  m_stepFile.Empty();
  m_editName.SetBkColor(FRAME_DEFAULT_COLOR);
  m_editFile.SetBkColor(FRAME_DEFAULT_COLOR);
}

// NewStepNamesDlg message handlers

void
NewStepNamesDlg::OnBnClickedStepGlobal()
{
  AutoFocus focus;
  GlobalFileDlg dlg(this,true,m_stepType,theApp.GetBaseDirectory());
  if(dlg.DoModal() == IDOK)
  {
    m_stepGlobal = true;
    m_stepName   = StripExtension(dlg.GetChosenFile());
    m_stepFile   = dlg.GetChosenFile();
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
NewStepNamesDlg::OnEnChangeName()
{
  UpdateData();
  if(!m_stepName.IsEmpty())
  {
    m_editName.SetErrorState(false);
  }
  PresetFilenames();
}

void
NewStepNamesDlg::OnEnChangeFilename()
{
  UpdateData();
  CheckFilename();
}
