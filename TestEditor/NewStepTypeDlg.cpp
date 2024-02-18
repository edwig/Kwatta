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
// This File   : NewStepTypeDlg.cpp
// What it does: Defining the first step of the new teststep wizard
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TestEditor.h"
#include "afxdialogex.h"
#include "NewStepTypeDlg.h"

// NewStepTypeDlg dialog

IMPLEMENT_DYNAMIC(NewStepTypeDlg, StyleTab)

NewStepTypeDlg::NewStepTypeDlg(CWnd* pParent /*=nullptr*/)
	             :StyleTab(IDD_NEWSTEP_TYPE,pParent)
               ,m_stepType(StepType::Step_command)
{
}

NewStepTypeDlg::~NewStepTypeDlg()
{
}

void NewStepTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleTab::DoDataExchange(pDX);

  DDX_Control(pDX,IDC_TESTTYPE,m_comboType);

}

BEGIN_MESSAGE_MAP(NewStepTypeDlg, StyleTab)
  ON_CBN_SELCHANGE(IDC_TESTTYPE,&NewStepTypeDlg::OnCbnSelchangeTestType)
END_MESSAGE_MAP()

bool
NewStepTypeDlg::InitStyleTab(void* p_data)
{
  m_comboType.AddString(_T("Command (CMD)"));
  m_comboType.AddString(_T("Internet (HTTP)"));
  m_comboType.AddString(_T("Database (SQL)"));
  m_comboType.AddString(_T("Windows (WIN)"));
  m_comboType.SetCurSel(0);

  StepType type = *((StepType*)p_data);
  if(m_stepType != type)
  {
    m_stepType = type;
    m_comboType.SetCurSel((int)type);
  }
  return true;
}

bool
NewStepTypeDlg::CheckStyleTab(void* p_data)
{
  if(m_stepType < StepType::Step_command &&
     m_stepType > StepType::Step_win)
  {
    return false;
  }
  SaveStyleTab(p_data);
  return true;
}

bool
NewStepTypeDlg::SaveStyleTab(void* p_data)
{
  if(m_stepType >= StepType::Step_command &&
     m_stepType <= StepType::Step_win)
  {
    *((StepType*)p_data) = m_stepType;
  }
  return true;
}

// NewStepTypeDlg message handlers

void
NewStepTypeDlg::OnCbnSelchangeTestType()
{
  int ind = m_comboType.GetCurSel();
  if(ind >= 0)
  {
    m_stepType = static_cast<StepType>(ind);
  }
  UpdateData();
}

