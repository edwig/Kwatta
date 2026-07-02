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
// This File   : InputDlg.cpp
// What it does: Standard input of a command-line test step
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StepEditor.h"
#include "InputDlg.h"
#include "StepCommandDlg.h"
#include <SearchVarDlg.h>
#include <TestStep.h>
#include "afxdialogex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// InputDlg dialog

IMPLEMENT_DYNAMIC(InputDlg,StyleTab)

InputDlg::InputDlg(CWnd* pParent /*=nullptr*/)
         :StyleTab(IDD_INPUT, pParent)
         ,m_step(nullptr)
{
}

InputDlg::~InputDlg()
{
}

void InputDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleTab::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_INPUT,     m_editInput,    m_input);
  DDX_Control(pDX,IDC_EFFECTIVE, m_editEffective,m_effective);
  DDX_Control(pDX,IDC_INPUT_PARM,m_buttonInputParm);
}

BEGIN_MESSAGE_MAP(InputDlg, StyleTab)
  ON_EN_KILLFOCUS(IDC_INPUT,     &InputDlg::OnEnChangeInput)
  ON_BN_CLICKED  (IDC_INPUT_PARM,&InputDlg::OnBnClickedInputParm)
END_MESSAGE_MAP()

BOOL
InputDlg::OnInitDialog()
{
  StyleTab::OnInitDialog();

  EnableToolTips();
  m_buttonInputParm.SetIconImage(IDI_LIST);
  RegisterTooltip(m_buttonInputParm,_T("Choose global/test parameter(s) for the standard input."));

  SetCanResize();
  UpdateData(FALSE);
  return TRUE;
}

void
InputDlg::SetupDynamicLayout()
{
  StyleTab::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif

  manager.AddItem(IDC_GRP_INPUT, CMFCDynamicLayout::MoveNone(),         CMFCDynamicLayout::SizeHorizontalAndVertical(100,100));
  manager.AddItem(IDC_INPUT,     CMFCDynamicLayout::MoveNone(),         CMFCDynamicLayout::SizeHorizontalAndVertical(100, 50));
  manager.AddItem(IDC_EFFECTIVE, CMFCDynamicLayout::MoveVertical(50),   CMFCDynamicLayout::SizeHorizontalAndVertical(100, 50));
  manager.AddItem(IDC_INPUT_PARM,CMFCDynamicLayout::MoveHorizontal(100),CMFCDynamicLayout::SizeNone());
}

void
InputDlg::InitTab(TestStepCMD* p_step,Parameters* p_parameters)
{
  m_step  = p_step;
  m_input = p_step->GetStandardInput();
  m_parameters = p_parameters;

  UpdateData(FALSE);
}

void 
InputDlg::StoreVariables()
{
  if(m_step)
  {
    m_step->SetStandardInput(m_input);
  }
}

void
InputDlg::Effectuate()
{
  if(m_step)
  {
    m_step->SetStandardInput(m_input);
    StepCommandDlg* parent = (StepCommandDlg*)GetParent()->GetParent();
    parent->EffectiveParameters();
    m_effective = m_step->GetEffectiveInput();

    UpdateData(FALSE);
  }
}

// InputDlg message handlers

void 
InputDlg::OnEnChangeInput()
{
  UpdateData();
  Effectuate();
}

void
InputDlg::OnBnClickedInputParm()
{
  AutoFocus focus;
  SearchVarDlg dlg(this,m_parameters,true,true,true,true);
  if(dlg.DoModal() == IDOK || dlg.GetSaved())
  {
    XString variable = dlg.GetVariable();
    m_editInput.InsertAtCurPos(variable,0);
    UpdateData();
  }
}
