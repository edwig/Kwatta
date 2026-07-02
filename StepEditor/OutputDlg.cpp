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
// This File   : OutputDlg.cpp
// What it does: Standard output definition for a test step
// Author      : ir. W.E. Huisman
// License     : See license.md file in the root directory
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StepEditor.h"
#include "OutputDlg.h"
#include "NewVariableDlg.h"
#include <TestStepCMD.h>
#include <Parameters.h>
#include "afxdialogex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// OutputDlg dialog

IMPLEMENT_DYNAMIC(OutputDlg,StyleTab)

OutputDlg::OutputDlg(CWnd* pParent /*=nullptr*/)
          :StyleTab(IDD_OUTPUT, pParent)
{

}

OutputDlg::~OutputDlg()
{
}

void OutputDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleTab::DoDataExchange(pDX);

  DDX_Control(pDX,IDC_USE_RETURN, m_buttonUseReturn);
  DDX_Control(pDX,IDC_USE_OUTPUT, m_buttonUseOutput);
  DDX_Control(pDX,IDC_USE_ERROR,  m_buttonUseError);

  DDX_CBString(pDX,IDC_VAR_RETURN, m_comboReturnVars,m_varReturn);
  DDX_CBString(pDX,IDC_VAR_OUTPUT, m_comboOutputVars,m_varOutput);
  DDX_CBString(pDX,IDC_VAR_ERROR,  m_comboErrorVars, m_varError);

  DDX_Control(pDX,IDC_NEW_RETURN, m_buttonNewReturnVar);
  DDX_Control(pDX,IDC_NEW_OUTPUT, m_buttonNewOutputVar);
  DDX_Control(pDX,IDC_NEW_ERROR,  m_buttonNewErrorVar);

  if(pDX->m_bSaveAndValidate == FALSE)
  {
    m_comboReturnVars.EnableWindow(m_useReturn);
    m_comboOutputVars.EnableWindow(m_useOutput);
    m_comboErrorVars .EnableWindow(m_useError);

    m_buttonNewReturnVar.EnableWindow(m_useReturn);
    m_buttonNewOutputVar.EnableWindow(m_useOutput);
    m_buttonNewErrorVar .EnableWindow(m_useError);
  }
}


BEGIN_MESSAGE_MAP(OutputDlg, StyleTab)
  ON_BN_CLICKED   (IDC_USE_RETURN, &OutputDlg::OnBnClickedUseReturn)
  ON_CBN_SELCHANGE(IDC_VAR_RETURN, &OutputDlg::OnCbnSelchangeVarReturn)
  ON_BN_CLICKED   (IDC_NEW_RETURN, &OutputDlg::OnBnClickedNewReturn)
  ON_BN_CLICKED   (IDC_USE_OUTPUT, &OutputDlg::OnBnClickedUseOutput)
  ON_CBN_SELCHANGE(IDC_VAR_OUTPUT, &OutputDlg::OnCbnSelchangeVarOutput)
  ON_BN_CLICKED   (IDC_NEW_OUTPUT, &OutputDlg::OnBnClickedNewOutput)
  ON_BN_CLICKED   (IDC_USE_ERROR,  &OutputDlg::OnBnClickedUseError)
  ON_CBN_SELCHANGE(IDC_VAR_ERROR,  &OutputDlg::OnCbnSelchangeVarError)
  ON_BN_CLICKED   (IDC_NEW_ERROR,  &OutputDlg::OnBnClickedNewError)
END_MESSAGE_MAP()

BOOL
OutputDlg::OnInitDialog()
{
  StyleTab::OnInitDialog();

  InitButtons();
  return TRUE;
}

void
OutputDlg::InitTab(TestStepCMD* p_step,Parameters* p_param)
{
  // Remember the objects
  m_step       = p_step;
  m_parameters = p_param;

  // Extract our values
  m_useReturn = m_step->GetUseReturnValue();
  m_useOutput = m_step->GetUseOutputValue();
  m_useError  = m_step->GetUseErrorValue();

  m_varReturn = m_step->GetReturnVariable();
  m_varOutput = m_step->GetOutputVariable();
  m_varError  = m_step->GetErrorVariable();

  m_buttonUseReturn.SetCheck(m_useReturn);
  m_buttonUseOutput.SetCheck(m_useOutput);
  m_buttonUseError .SetCheck(m_useError);

  FillCombos();
  SetCombos();

  UpdateData(FALSE);
}

void
OutputDlg::InitButtons()
{
  EnableToolTips();

  m_buttonNewReturnVar.SetIconImage(IDI_RETURN);
  m_buttonNewOutputVar.SetIconImage(IDI_RETURN);
  m_buttonNewErrorVar .SetIconImage(IDI_RETURN);

  RegisterTooltip(m_buttonNewReturnVar,_T("Choose a return parameter for the return value."));
  RegisterTooltip(m_buttonNewOutputVar,_T("Choose a buffer parameter for the output stream."));
  RegisterTooltip(m_buttonNewErrorVar, _T("Choose a buffer parameter for the error stream."));
}

void
OutputDlg::StoreVariables()
{
  m_step->SetUseReturnValue(m_useReturn);
  m_step->SetUseOutputValue(m_useOutput);
  m_step->SetUseErrorValue (m_useError);

  m_step->SetReturnVariable(m_varReturn.GetString());
  m_step->SetOutputVariable(m_varOutput.GetString());
  m_step->SetErrorVariable (m_varError .GetString());
}

void
OutputDlg::FillCombos()
{
  m_comboReturnVars.ResetContent();
  m_comboOutputVars.ResetContent();
  m_comboErrorVars .ResetContent();

  m_comboReturnVars.AddString(_T(""));
  m_comboOutputVars.AddString(_T(""));
  m_comboErrorVars .AddString(_T(""));

  for(auto& ret : m_parameters->GetReturns())
  {
    m_comboReturnVars.AddString(ret.first);
  }

  for(auto& buf : m_parameters->GetBuffers())
  {
    m_comboOutputVars.AddString(buf.first);
    m_comboErrorVars .AddString(buf.first);
  }
}

void
OutputDlg::SetCombos()
{
  // Return variable
  int ind = m_comboReturnVars.FindStringExact(0,m_varReturn);
  if (ind >= 0)
  {
    m_comboReturnVars.SetCurSel(ind);
  }

  // Output buffer variable
  ind = m_comboOutputVars.FindStringExact(0,m_varOutput);
  if (ind >= 0)
  {
    m_comboOutputVars.SetCurSel(ind);
  }

  // Error buffer variable
  ind = m_comboErrorVars.FindStringExact(0, m_varError);
  if (ind >= 0)
  {
    m_comboErrorVars.SetCurSel(ind);
  }
}

// OutputDlg message handlers

void 
OutputDlg::OnBnClickedUseReturn()
{
  m_useReturn = m_buttonUseReturn.GetCheck() > 0;
  UpdateData(FALSE);
}

void 
OutputDlg::OnCbnSelchangeVarReturn()
{
  int ind = m_comboReturnVars.GetCurSel();
  if (ind >= 0)
  {
    XString var;
    m_comboReturnVars.GetLBText(ind,var);
    m_varReturn = var;
  }
}

void 
OutputDlg::OnBnClickedNewReturn()
{
  AutoFocus focus;
  NewVariableDlg dlg(ParType::PAR_RETURN,m_parameters->GetReturns());
  if(dlg.DoModal() == IDOK)
  {
    XString newvar = dlg.GetNewName();
    if(!newvar.IsEmpty())
    {
      m_varReturn = newvar;
      FillCombos();
      SetCombos();
    }
  }
}

void 
OutputDlg::OnBnClickedUseOutput()
{
  m_useOutput = m_buttonUseOutput.GetCheck() > 0;
  UpdateData(FALSE);
}

void 
OutputDlg::OnCbnSelchangeVarOutput()
{
  int ind = m_comboOutputVars.GetCurSel();
  if (ind >= 0)
  {
    XString var;
    m_comboOutputVars.GetLBText(ind,var);
    m_varOutput = var;
  }
}

void 
OutputDlg::OnBnClickedNewOutput()
{
  AutoFocus focus;
  NewVariableDlg dlg(ParType::PAR_BUFFER,m_parameters->GetBuffers());
  if(dlg.DoModal() == IDOK)
  {
    XString newvar = dlg.GetNewName();
    if(!newvar.IsEmpty())
    {
      m_varOutput = newvar;
      FillCombos();
      SetCombos();
    }
  }
}

void 
OutputDlg::OnBnClickedUseError()
{
  m_useError = m_buttonUseError.GetCheck() > 0;
  UpdateData(FALSE);
}

void 
OutputDlg::OnCbnSelchangeVarError()
{
  int ind = m_comboErrorVars.GetCurSel();
  if (ind >= 0)
  {
    XString var;
    m_comboErrorVars.GetLBText(ind,var);
    m_varError = var;
  }
}

void 
OutputDlg::OnBnClickedNewError()
{
  AutoFocus focus;
  NewVariableDlg dlg(ParType::PAR_BUFFER,m_parameters->GetBuffers());
  if(dlg.DoModal() == IDOK)
  {
    XString newvar = dlg.GetNewName();
    if(!newvar.IsEmpty())
    {
      m_varError = newvar;
      FillCombos();
      SetCombos();
    }
  }
}
